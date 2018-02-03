#include "MapsImageProvider.h"
#include <QImage>
#include <QPixmap>
#include <QPainter>
#include <QFontMetrics>
#include <QMutexLocker>
#include <QDebug>

QPixmap genEmptyPixmap()
{
    QPixmap pixmap(QSize(200,200));
    pixmap.fill("gray");

    QPainter painter(&pixmap);
    painter.setFont(QFont("Arial", 16, QFont::Bold));

    QString str ="Empty Image";

    QFontMetrics fm(painter.font());
    QRect fontRect = fm.boundingRect(str);

    fontRect.moveCenter(pixmap.rect().center());
    painter.drawText(fontRect, Qt::AlignCenter, str);

    return pixmap;
}

QPixmap MapsImageProvider::emptyPixmap(){
    static QPixmap _emptyPixmap = genEmptyPixmap();
    return _emptyPixmap;
}

MapsImageProvider * MapsImageProvider::instance()
{
    static auto ptr = new  MapsImageProvider; //it will delete qml engine
    return ptr;
}

MapsImageProvider::MapsImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap)
{
}


QPixmap MapsImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    qDebug() << "req pixmap";
    return emptyPixmap();
    auto lowId = id.toLower();
    if(lowId.indexOf("cube")>=0)
        return QPixmap::fromImage(m_cubemap);
    else if(lowId.indexOf("equirect")>=0)
        return QPixmap::fromImage(m_equirectMap);
    else
        return emptyPixmap();
}
