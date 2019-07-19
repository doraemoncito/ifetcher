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

#ifndef CONFIRMDOWNLOADDIALOG_H
#define CONFIRMDOWNLOADDIALOG_H

#include <QDialog>

class Download;
class Metadata;
class ProgrammeInfo;

namespace Ui {
    class ConfirmDownloadDialog;
}

class ConfirmDownloadDialog : public QDialog
{
    Q_OBJECT

public:
    explicit ConfirmDownloadDialog(QWidget *parent, const QString &programmeId);
    ~ConfirmDownloadDialog();
    ProgrammeInfo *getProgrammeInfo() const;

public slots:
    virtual void accept();
    void metadataFinished();
    void downloadFinished();
    void downloadProgress(int value);

private:
    Ui::ConfirmDownloadDialog *ui;
    QString m_programmeId;
    Metadata *m_pMetadata;    
    Download *m_pDownload;
};

#endif // CONFIRMDOWNLOADDIALOG_H
