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

#include <QFile>
#include "httpget.h"
#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "confirmdownloaddialog.h"
#include "download.h"
#include "programmeinfo.h"


const QString MainWindow::baseUrl("http://www.bbc.co.uk");
const QString MainWindow::basePathIPlayer("/mobile/iplayer");
const QString MainWindow::basePathEpisode("/mobile/iplayer/episode/");


class IPhoneWebPage : public QWebPage {

public:
    IPhoneWebPage(QObject *parent) : QWebPage(parent) {
    }

protected:
    virtual QString userAgentForUrl(const QUrl &) const {
        return HttpGet::UA_IPHONE;
    }
};


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), ui(new Ui::MainWindow) {
    ui->setupUi(this);

    // Create a web page that pretends to be an iPhone
    IPhoneWebPage *iPhonePage = new IPhoneWebPage(ui->webView);
    iPhonePage->setLinkDelegationPolicy(QWebPage::DelegateAllLinks);

    // Add the page to the web view
    ui->webView->setPage(iPhonePage);
    ui->webView->load(QUrl(baseUrl + basePathIPlayer));
    ui->webView->resize(320, 480);

    ui->mainToolBar->addAction(ui->webView->pageAction(QWebPage::Back));
    ui->mainToolBar->addAction(ui->webView->pageAction(QWebPage::Forward));
    ui->mainToolBar->addAction(ui->webView->pageAction(QWebPage::Reload));
    ui->mainToolBar->addAction(ui->webView->pageAction(QWebPage::Stop));

    m_pLocation = new QLabel();
    m_pLocation->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
    statusBar()->addWidget(m_pLocation);
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionAbout_triggered(bool) {
    QString aboutStr("<b>About iFetcher version 1.0.</b><p><p>" \
        "iFetcher is free software: you can redistribute it and/or modify" \
        "it under the terms of the GNU General Public License as published by" \
        "the Free Software Foundation, either version 3 of the License, or" \
        "(at your option) any later version.<p><p>" \
        "iFetcher is distributed in the hope that it will be useful," \
        "but WITHOUT ANY WARRANTY; without even the implied warranty of" \
        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the" \
        "GNU General Public License for more details<p><p>" \
        "You should have received a copy of the GNU General Public License" \
        "along with iFetcher.  If not, see http://www.gnu.org/licenses/.<p><p>" \
        "<b>Copyright (C) 2010 Jose Hernandez.</b>");

    QMessageBox::about(this, "About iFetcher", aboutStr);
//    QMessageBox::about(this, "About iFetcher", "<b>iFetcher version 1.0<p>Copyright &copy; 2010 Jose Hernandez</b>");
}

void MainWindow::on_webView_linkClicked(QUrl url) {
    /* Present the confirmation dialog if this URL correspomds to an episode.  If the URL is not that of an episode
     * we check that it is actually within the iPlayer domain so the the browser does not point to anything outside
     * that of the BBC iPlayer's site.  In the case of iPlayer URLs we instruct the WebKit view to load the page,
     * otherwise we just ignore the the hyperlink.
     */
    if (url.path().contains(MainWindow::basePathEpisode)) {
        QString programmeId(url.path().split("/").last());
        ConfirmDownloadDialog dlg(this, programmeId);

        // Ask the user to confirm download
        dlg.exec();
    } else if (url.path().contains(MainWindow::basePathIPlayer)) {
        ui->webView->load(url);
    }
}

void MainWindow::on_webView_loadFinished(bool) {
    m_pLocation->setText(ui->webView->url().toString());
}

void MainWindow::on_webView_loadProgress(int) {
}

void MainWindow::on_webView_titleChanged(QString title) {
    setWindowTitle(QString("iFetcher - %1").arg(title));
}
