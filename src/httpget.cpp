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
#include <QtNetwork>
#include <QtGui>

#include "httpget.h"


// Used by Safari mobile
const QString HttpGet::UA_IPHONE(
        "Mozilla/5.0 (iPhone; U; CPU iPhone OS 3_1_3 like Mac OS X; en-us) AppleWebKit/528.18 (KHTML, like Gecko) Version/4.0 Mobile/7E18 Safari/528.16");

// Used by Quicktime
const QString HttpGet::UA_QT("Apple iPhone v1.1.4 CoreMedia v1.0.0.4A102");


HttpGet::HttpGet(QObject &listener, QNetworkAccessManager &manager, const QUrl &url, QMap<QString, QString> &options)
    : m_pNetworkReply(0), m_manager(manager) {

    QNetworkRequest request = QNetworkRequest(url);

    // Add the default headers
    options.insert("Accept", "*/*");
    options.insert("Accept-Language", "en-us");
    options.insert("Connection", "keep-alive");
    options.insert("Pragma", "no-cache");

    /* We can use a map for the header fields because the order of headers with different names is not significant
     * acording to the HTTP protocol.
     */
    QMapIterator<QString, QString> iter(options);
    while (iter.hasNext()) {
        iter.next();
        request.setRawHeader(iter.key().toUtf8(), iter.value().toUtf8());
    }

    QList<QNetworkCookie> cookieList = m_manager.cookieJar()->cookiesForUrl(url);

    connect(&m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
    connect(this, SIGNAL(finished(HttpGet *)), &listener, SLOT(finished(HttpGet *)));

    m_pNetworkReply = m_manager.get(request);
}

HttpGet *HttpGet::create(QObject &listener, QNetworkAccessManager &manager, const QUrl &url, QMap<QString, QString> &options) {
    return new HttpGet(listener, manager, url, options);
}

HttpGet::~HttpGet() {
    delete m_pNetworkReply;
}

void HttpGet::finished(QNetworkReply *pNetworkReply) {
    /* Disconnect the signal from the network manager so that we don't get
     * notifications for events that are not related to this request.
     */
    disconnect(&m_manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));
    m_pNetworkReply = (pNetworkReply->error() == QNetworkReply::NoError) ? pNetworkReply : 0;
    emit finished(this);
}

QImage HttpGet::image() {
    return ((0 != m_pNetworkReply) ? QImageReader(m_pNetworkReply).read() : QImage());
}

QString HttpGet::xml() {
    return QString((0 != m_pNetworkReply) ? m_pNetworkReply->readAll() : "");
}

QNetworkReply *HttpGet::networkReply() const {
    return m_pNetworkReply;
}
