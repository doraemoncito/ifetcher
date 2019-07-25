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
 * Copyright (C) 2010-2019 Jose Hernandez.
 */

#include <QPushButton>
#include <QFileDialog>
#include <QMessageBox>
#include "confirmdownloaddialog.h"
#include "ui_confirmdownloaddialog.h"
#include "metadata.h"
#include "programmeinfo.h"
#include "download.h"

ConfirmDownloadDialog::ConfirmDownloadDialog(QWidget *parent, const QString &programmeId) :
    QDialog(parent),
    ui(new Ui::ConfirmDownloadDialog),
    m_programmeId(programmeId),
    m_pMetadata(nullptr),
    m_pDownload(nullptr)
{
    ui->setupUi(this);

    // Initially the progress bar will be hidden and the "yes" button will be disabled
    ui->m_pProgressBar->hide();
    ui->buttonBox->button(QDialogButtonBox::Yes)->setEnabled(false);

    setWindowTitle(QString("iFetcher - Programme Information [%1]").arg(m_programmeId));
    m_pMetadata = new Metadata(m_programmeId);
    connect(m_pMetadata, SIGNAL(finished()), this, SLOT(metadataFinished()));
}

/**
 * Updates the UI with the programme image, name and description once the programme information is retrieved.
 */
void ConfirmDownloadDialog::metadataFinished() {
    ProgrammeInfo *programmeInfo = m_pMetadata->getProgrammeInfo();
    ui->m_pProgrammeInfo->setText(QString("<h2>%1</h2><p><b>%2</b>").arg(programmeInfo->fullTitle()).arg(programmeInfo->summary()));
    ui->m_pThumbnail->setPixmap(QPixmap::fromImage(programmeInfo->thumbnail()));
    ui->buttonBox->button(QDialogButtonBox::Yes)->setEnabled(true);
    adjustSize();
}

/**
 * Updates the UI to change the text of the "No" button to read "Close" and show a text message indicating
 * the download has been completed.
 */
void ConfirmDownloadDialog::downloadFinished() {
    ui->m_pMessage->setText(tr("Download complete; press 'Close' to dismiss this window"));
    ui->m_pProgressBar->hide();
    ui->m_pMessage->show();
    ui->buttonBox->button(QDialogButtonBox::No)->setText(tr("Close"));;
}

ConfirmDownloadDialog::~ConfirmDownloadDialog() {
    delete ui;
    delete m_pMetadata;
    delete m_pDownload;
}

ProgrammeInfo *ConfirmDownloadDialog::getProgrammeInfo() const {
    return m_pMetadata->getProgrammeInfo();
}

void ConfirmDownloadDialog::accept() {
    // Ask the user to choose a destination file name suggesting the name of the programme as the
    // possible file name.  The suggested file name is the programme title with all non filesystem
    // friendly characters removed.
    QString fileName = getProgrammeInfo()->title();
    fileName = fileName.replace(QRegExp("[^\\w]"), "_").replace(QRegExp("_{2,}"), "_").toLower();
    fileName = QFileDialog::getSaveFileName(this, tr("Save File"), fileName, tr("Video (*.mp4)"), nullptr, QFileDialog::DontConfirmOverwrite);

    // When the user cancels the operation the filename is empty
    if (!fileName.isEmpty()) {
        QFile *file = new QFile(fileName);
        if (!file->open(QIODevice::WriteOnly | QIODevice::Append)) {
            QMessageBox::information(this, tr("iFetcher"), tr("Unable to save the file %1: %2.").arg(fileName).arg(file->errorString()));
            return;
        }

        ui->m_pProgressBar->show();
        ui->m_pMessage->hide();
        ui->buttonBox->button(QDialogButtonBox::Yes)->hide();
        ui->buttonBox->button(QDialogButtonBox::No)->setText(tr("Cancel"));;

        // now dowload the programme
        m_pDownload = new Download(getProgrammeInfo()->programmeId(), file);
        connect(m_pDownload, SIGNAL(downloadProgress(int)), this, SLOT(downloadProgress(int)));
        connect(m_pDownload, SIGNAL(downloadFinished()), this, SLOT(downloadFinished()));
    }
}

void ConfirmDownloadDialog::downloadProgress(int value) {
    ui->m_pProgressBar->setValue(value);
}
