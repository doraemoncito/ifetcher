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

#ifndef DOWNLOAD
#define DOWNLOAD

#include <QtCore>
#include <QtGui>

class HttpGet;
class QFile;
class QNetworkAccessManager;


class Download : public QObject {

    Q_OBJECT

public:
    Download(const QString &programmeId, QFile *file);
    ~Download();

signals:
    void downloadFinished();
    void downloadProgress(int value);

public slots:
    void finished(HttpGet *);

private slots:
    void downloadProgress(qint64 bytesReceived, qint64 bytesTotal);
    void readyRead();

private:
    HttpGet *downloadChunk(long int startPosition, long int chunkSize);

    const static QString EPISODE_URL;
    const static unsigned int BUFFERSIZE;

    QString m_programmeId;
    QNetworkAccessManager *m_pManager;
    QUrl m_location;

    HttpGet *m_pEpisodeInfo;
    HttpGet *m_pEpisodeSize;
    HttpGet *m_pEpisodeData;

    // Size and start position of the data stream
    long int m_programmeSize;
    long int m_startPosition;

    QFile *m_pFile;

};


#endif // DOWNLOAD
