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

#ifndef HTTPGET_H
#define HTTPGET_H

#include <QtCore>

class QNetworkReply;
class QNetworkAccessManager;


class HttpGet : public QObject {

	Q_OBJECT

public:
	static const QString UA_IPHONE;
	static const QString UA_QT;

	~HttpGet();

	QImage image();
	QString xml();
        QNetworkReply *networkReply() const;

	static HttpGet *create(QObject &, QNetworkAccessManager &, const QUrl &, QMap<QString, QString> &);

signals:
	void finished(HttpGet *);

public slots:
	void finished(QNetworkReply *);

private:
	QNetworkReply *m_pNetworkReply;
	QNetworkAccessManager &m_manager;

	// Only the factory method can create HttpGet objects.
	HttpGet(QObject &, QNetworkAccessManager &, const QUrl &, QMap<QString, QString> &);

};


#endif // HTTPGET_H
