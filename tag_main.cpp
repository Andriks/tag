#include <QtGui/QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QList>
#include <QObject>

#include "controller.h"


int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/tag_main.qml")));

    QObject* root = engine.rootObjects()[0];
    Controller controller(root);
    engine.rootContext()->setContextProperty("controller", &controller);

    return app.exec();
}
