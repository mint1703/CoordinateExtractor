#include <QCoreApplication>
#include <QCommandLineParser>
#include "httpserver.h"

int main(int argc, char *argv[]) {
    QCoreApplication app(argc, argv);
    QCoreApplication::setApplicationName("Coordinate Extractor");
    QCoreApplication::setApplicationVersion("1.0");

    QCommandLineParser parser;
    parser.setApplicationDescription("HTTP service for coordinate extraction from text");
    parser.addHelpOption();
    parser.addVersionOption();

    QCommandLineOption portOption("p", "Port number", "port", "8080");
    parser.addOption(portOption);

    parser.process(app);

    quint16 port = parser.value(portOption).toUShort();

    HTTPServer server;
    if (!server.start(port)) {
        qCritical() << "Failed to start server on port" << port;
        return 1;
    }

    qInfo() << "Coordinate Extractor server running on port" << port;

    return app.exec();
}
