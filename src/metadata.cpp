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
#include <QtWidgets/QMessageBox>

#include "metadata.h"
#include "httpget.h"
#include "programmeinfo.h"


/*
 * URL from which we can retrieve playlist information such as the programme
 * title and its description.
 */
const QString Metadata::PLAYLIST_URL("http://www.bbc.co.uk/iplayer/playlist/%1");

/*
 * URL from which we can retrieve a programme thumbnail.
 */
const QString Metadata::THUMBNAIL_URL("http://node1.bbcimg.co.uk/iplayer/images/episode/%1_150_84.jpg");


Metadata::Metadata(const QString &programmeId) : fetchedItemCounter(0) {
    m_pManager = new QNetworkAccessManager(this);
    m_pProgrammeInfo = new ProgrammeInfo(programmeId);

    /* fetch progamme data using a playlisy URL of the form:
     *		http://www.bbc.co.uk/iplayer/playlist/b00p4rl4
     * where b00p4rl4 is the programme identifier.
     */
    QMap<QString, QString> options;
    options.insert("User-Agent", HttpGet::UA_IPHONE);
    m_pPlaylist = HttpGet::create(*this, *m_pManager, PLAYLIST_URL.arg(programmeId), options);
}


Metadata::~Metadata() {
    delete m_pPlaylist;
    delete m_pManager;
    delete m_pProgrammeInfo;
}


void Metadata::finished(HttpGet *getter) {
    if (getter == m_pPlaylist) {
        QString xmlString(m_pPlaylist->xml());
        QXmlStreamReader xml(xmlString);

        /* We'll parse the XML until we reach end of it.*/
        while (!xml.atEnd() && !xml.hasError()) {
            /* Read next element.*/
            QXmlStreamReader::TokenType token = xml.readNext();

            /* If token is just StartDocument, we'll go to next.*/
            if (token == QXmlStreamReader::StartDocument) {
                continue;
            }

            /* If token is StartElement, we'll see if we can read it.*/
            if (token == QXmlStreamReader::StartElement) {
                /* If it's named persons, we'll go to the next.*/
                if (xml.name() == "title") {
                    xml.readNext();
                    /* This elements needs to contain Characters so we know it's
                     * actually data, if it's not we'll leave.
                     */
                    if (xml.tokenType() == QXmlStreamReader::Characters) {
                        m_pProgrammeInfo->setTitle(xml.text().toString());
                    }
                } else if (xml.name() == "summary") {
                    xml.readNext();
                    if (xml.tokenType() == QXmlStreamReader::Characters) {
                        m_pProgrammeInfo->setSummary(xml.text().toString());
                        break;
                    }
                }
            }
        }

        /* Error handling. */
        if (xml.hasError()) {
            QMessageBox::critical(nullptr, "iPlayer metadata", xml.errorString(), QMessageBox::Ok);
        }

        QMap<QString, QString> options;
        options.insert("User-Agent", HttpGet::UA_IPHONE);
        m_pPlaylist->deleteLater();
        m_pPlaylist = nullptr;
        m_pThumbnail = HttpGet::create(*this, *m_pManager, THUMBNAIL_URL.arg(m_pProgrammeInfo->programmeId()), options);

    } else if (getter == m_pThumbnail) {
        m_pProgrammeInfo->setThumbnail(m_pThumbnail->image());
        m_pThumbnail->deleteLater();
        m_pThumbnail = nullptr;
        emit finished();
    }
}

ProgrammeInfo *Metadata::getProgrammeInfo() const {
    return m_pProgrammeInfo;
}
