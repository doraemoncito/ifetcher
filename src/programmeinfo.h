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

#ifndef PROGRAMMEINFO_H
#define PROGRAMMEINFO_H

#include <QImage>
#include <QString>
#include "metadata.h"

class ProgrammeInfo {

    QString m_programmeId;
    QString m_title;
    QString m_summary;
    QImage m_thumbnail;

public:
    ProgrammeInfo(const QString &programmeId);

    void setProgrammeId(const QString &programmeId);
    QString programmeId() const;

    void setTitle(const QString &title);
    QString title() const;

    void setSummary(const QString &summary);
    QString summary() const;

    void setThumbnail(const QImage &thumbnail);
    QImage thumbnail() const;

    QString fullTitle() const;
};

#endif // PROGRAMMEINFO_H
