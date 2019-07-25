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

#include "programmeinfo.h"

// https://github.com/MasterSoft24/CloudCross/issues/58
// https://stackoverflow.com/questions/18313509/default-argument-gcc-vs-clang
//ProgrammeInfo::ProgrammeInfo(const QString &programmeId = nullptr) {
ProgrammeInfo::ProgrammeInfo(const QString &programmeId) {
    setProgrammeId(programmeId);
}

void ProgrammeInfo::setProgrammeId(const QString &programmeId) {
    m_programmeId = programmeId;
}

QString ProgrammeInfo::programmeId() const {
    return m_programmeId;
}

void ProgrammeInfo::setTitle(const QString &title) {
    m_title = title;
}

QString ProgrammeInfo::title() const {
    return m_title;
}

void ProgrammeInfo::setSummary(const QString &summary) {
    m_summary = summary;
}

QString ProgrammeInfo::summary() const {
    return m_summary;
}

void ProgrammeInfo::setThumbnail(const QImage &thumbnail) {
    m_thumbnail = thumbnail;
}

QImage ProgrammeInfo::thumbnail() const {
    return m_thumbnail;
}

QString ProgrammeInfo::fullTitle() const {
    return QString(m_title).replace("\\s*:\\s*", " - ");
}
