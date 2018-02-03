#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include "CubemapQuickRender.h"
#include "MapsImageProvider.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);

    qRegisterMetaType<CubemapQuickRender::Scheme>("CubemapQuickRender::Scheme");
    qRegisterMetaType<CubemapQuickRender::Scheme>("Renderer::Scheme");
    qmlRegisterType<CubemapQuickRender>("SceneGraphRendering", 1, 0, "Renderer");

    QQmlApplicationEngine engine;
    engine.addImageProvider("maps", MapsImageProvider::instance());
    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
