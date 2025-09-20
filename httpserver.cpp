#include "httpserver.h"
#include "textprocessor.h"
#include "extractionresult.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QRegularExpression>

HTTPServer::HTTPServer(QObject *parent) : QTcpServer(parent) {
    connect(this, &QTcpServer::newConnection, this, &HTTPServer::onNewConnection);
}

bool HTTPServer::start(quint16 port) {
    return listen(QHostAddress::Any, port);
}

void HTTPServer::onNewConnection() {
    QTcpSocket *socket = nextPendingConnection();
    connect(socket, &QTcpSocket::readyRead, this, &HTTPServer::onReadyRead);
    connect(socket, &QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);
}

void HTTPServer::onReadyRead() {
    QTcpSocket *socket = qobject_cast<QTcpSocket*>(sender());
    if (!socket) return;

    QByteArray requestData = socket->readAll();
    processHttpRequest(socket, requestData);
}

void HTTPServer::processHttpRequest(QTcpSocket *socket, const QByteArray &request) {
    QString requestStr = QString::fromUtf8(request);

    if (requestStr.contains("POST /extract HTTP")) {
        int bodyStart = requestStr.indexOf("\r\n\r\n");
        if (bodyStart != -1) {
            QByteArray body = request.mid(bodyStart + 4);
            QByteArray response = handleExtractCoordinates(body);
            sendHttpResponse(socket, response);
        } else {
            sendHttpResponse(socket, "{\"error\":\"Invalid request format\"}");
        }
    } else {
        sendHttpResponse(socket, "{\"error\":\"Endpoint not found. Use POST /extract\"}");
    }
}

void HTTPServer::sendHttpResponse(QTcpSocket *socket, const QByteArray &response, const QString &contentType) {
    QByteArray httpResponse;
    httpResponse.append("HTTP/1.1 200 OK\r\n");
    httpResponse.append("Content-Type: " + contentType.toUtf8() + "\r\n");
    httpResponse.append("Content-Length: " + QByteArray::number(response.size()) + "\r\n");
    httpResponse.append("Connection: close\r\n");
    httpResponse.append("\r\n");
    httpResponse.append(response);

    socket->write(httpResponse);
    socket->disconnectFromHost();
}

QByteArray HTTPServer::handleExtractCoordinates(const QByteArray &requestData) {
    try {
        QJsonDocument doc = QJsonDocument::fromJson(requestData);
        if (doc.isNull() || !doc.isObject()) {
            return QByteArray("{\"error\":\"Invalid JSON\"}");
        }

        QJsonObject json = doc.object();
        if (!json.contains("text") || !json["text"].isString()) {
            return QByteArray("{\"error\":\"Missing text field\"}");
        }

        QString text = json["text"].toString();
        ExtractionResult result = TextProcessor::processText(text);

        QJsonDocument responseDoc(result.toJson());
        return responseDoc.toJson();

    } catch (const std::exception& e) {
        return QByteArray("{\"error\":\"Internal server error\"}");
    }
}
