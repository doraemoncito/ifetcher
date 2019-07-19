/*
 * This file is part of iFetcher, the visual iPlayer downloader.
 *
 * iFetcher is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * iFetcher is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with iFetcher.  If not, see <http://www.gnu.org/licenses/>.
 *
 * Copyright (C) 2010 Jose Hernandez.
 */

#include <QtCore>
#include <QtGui>
#include <QtNetwork>

#include "download.h"
#include "httpget.h"


/*
 * URL from which we can retrieve playlist information such as the pogramme
 * title and its description.
 */
const QString Download::EPISODE_URL("http://www.bbc.co.uk/mobile/iplayer/episode/%1");
const unsigned int Download::BUFFERSIZE = 4 * 1024 * 1024;


Download::Download(const QString &programmeId, QFile *file)
    : m_programmeId(programmeId), m_programmeSize(0), m_startPosition(0), m_pFile(file) {
    /* fetch progamme data using a the episode URL of the form:
     *		http://www.bbc.co.uk/mobile/iplayer/episode/b00p4rl4
     * where b00p4rl4 is the programme identifier.
     * The first response will have the mobile safari user agent but subsequence responses
     * will change this to core media.
     */
    m_pManager = new QNetworkAccessManager(this);
    QMap<QString, QString> options;
    options.insert("User-Agent", HttpGet::UA_IPHONE);
    m_pEpisodeInfo = HttpGet::create(*this, *m_pManager, QUrl(EPISODE_URL.arg(m_programmeId)), options);
}

Download::~Download() {
    delete m_pEpisodeInfo;
    delete m_pEpisodeData;
    delete m_pEpisodeSize;
    delete m_pManager;
    delete m_pFile;
}

void Download::finished(HttpGet *getter) {
    if (getter == m_pEpisodeInfo) {
        /*
         * TODO: we need to check that the programme has no age restrictions
         */
        QString xmlString(m_pEpisodeInfo->xml());
        QRegExp re("<embed.+href=\"(.*)\"");
        re.setMinimal(true);
        re.indexIn(xmlString, 0);

        // The URL in the previous reply will be encoded so we need to decode it before we can use in the next request.
        // This is the real video file location
        m_location = QUrl::fromEncoded(QByteArray().append(re.cap(1)));

        // The first request of CoreMedia is always for the first byte
        QMap<QString, QString> options;
        options.insert("User-Agent", HttpGet::UA_QT);
        options.insert("Range", "bytes=0-1");
        m_pEpisodeInfo->deleteLater();
        m_pEpisodeInfo = 0;
        m_pEpisodeSize = HttpGet::create(*this, *m_pManager, m_location, options);

    } else if (getter == m_pEpisodeSize) {
        // Extract the full length in byte of the programme we are trying to download.
        m_programmeSize = QString(m_pEpisodeSize->networkReply()->rawHeader("content-range")).split("/").at(1).toLong();
        m_startPosition = m_pFile->size();
        m_pEpisodeSize->deleteLater();
        m_pEpisodeSize = 0;
        m_pEpisodeData = downloadChunk(m_startPosition, m_startPosition + Download::BUFFERSIZE - 1);
    } else if (getter == m_pEpisodeData) {
        m_pFile->write(m_pEpisodeData->networkReply()->readAll());
        m_startPosition += Download::BUFFERSIZE;
        if (m_startPosition >= m_programmeSize) {
            m_pFile->close();
            emit downloadFinished();
        } else {
            disconnect(m_pEpisodeData->networkReply(), SIGNAL(readyRead()), this, SLOT(readyRead()));
            disconnect(m_pEpisodeData->networkReply(), SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64, qint64)));
            m_pEpisodeData->deleteLater();
            m_pEpisodeData = downloadChunk(m_startPosition, m_startPosition + Download::BUFFERSIZE - 1);
        }
    }
}

HttpGet *Download::downloadChunk(long int startPosition, long int endPosition) {
    // After the first request we can start downloading chunks of under 70MB
    QMap<QString, QString> options;
    options.insert("User-Agent", HttpGet::UA_QT);
    options.insert("Range", QString("bytes=%1-%2").arg(startPosition).arg(endPosition));

    HttpGet *getter = HttpGet::create(*this, *m_pManager, m_location, options);

    connect(getter->networkReply(), SIGNAL(readyRead()), this, SLOT(readyRead()));
    connect(getter->networkReply(), SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(downloadProgress(qint64, qint64)));

    return getter;
}

void Download::readyRead() {
}

/**
 * Recomputes download progress before rebroadcasting the progress signal with the absolute progress measure.
 */
void Download::downloadProgress(qint64 bytesReceived, qint64 /* bytesTotal */) {
    int percent = static_cast<int>((100.0 * (m_startPosition + bytesReceived)) / m_programmeSize);
    emit downloadProgress(percent);
}
