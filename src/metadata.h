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

#ifndef METADATA
#define METADATA

#include <QtCore>
#include <QtGui>

class HttpGet;
class QNetworkAccessManager;
class ProgrammeInfo;

class Metadata : public QObject {

    Q_OBJECT

public:
    Metadata(const QString &);
    ~Metadata();
    ProgrammeInfo *getProgrammeInfo() const;

signals:
    void finished();

public slots:
    void finished(HttpGet *);

private:
    const static QString PLAYLIST_URL;
    const static QString THUMBNAIL_URL;

    HttpGet *m_pPlaylist;
    HttpGet *m_pThumbnail;
    QNetworkAccessManager *m_pManager;

    // Counts the number of remote items fetched so far
    int fetchedItemCounter;

    ProgrammeInfo *m_pProgrammeInfo;
};


#endif // METADATA
