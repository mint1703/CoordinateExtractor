#ifndef HTTPSERVER_H
#define HTTPSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>
#include <QJsonDocument>
#include <QJsonObject>

class HTTPServer : public QTcpServer {
    Q_OBJECT
public:
    explicit HTTPServer(QObject *parent = nullptr);
    bool start(quint16 port = 8080);

private slots:
    void onNewConnection();
    void onReadyRead();

private:
    void processHttpRequest(QTcpSocket *socket, const QByteArray &request);
    void sendHttpResponse(QTcpSocket *socket, const QByteArray &response,
                          const QString &contentType = "application/json");
    QByteArray handleExtractCoordinates(const QByteArray &requestData);
};

#endif // HTTPSERVER_H
