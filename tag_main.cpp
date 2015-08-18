#include <QtGui/QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickItem>
#include <QList>
#include <QObject>

#include <QtQml>

#include "controller.h"


int main(int argc, char *argv[])
{

    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;
    engine.load(QUrl(QStringLiteral("qrc:/tag_main.qml")));

    AbstractModel model(engine.rootContext());
    engine.rootContext()->setContextProperty("dataModel", &model);

    return app.exec();
}
