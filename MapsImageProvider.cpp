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
    static auto ptr = new  MapsImageProvider; //it will be deleted by qml engine
    return ptr;
}

MapsImageProvider::MapsImageProvider() : QQuickImageProvider(QQuickImageProvider::Pixmap)
{

}

QPixmap MapsImageProvider::requestPixmap(const QString &id, QSize *size, const QSize &requestedSize)
{
    QMutexLocker locker(&m_imageMutex);
    auto lowId = id.toLower();
    if(lowId.indexOf("cube")>=0)
        return m_cubemap.isNull()? emptyPixmap() :QPixmap::fromImage(m_cubemap);
    else if(lowId.indexOf("equirect")>=0)
        return m_equirectMap.isNull()? emptyPixmap() : QPixmap::fromImage(m_equirectMap);
    return emptyPixmap();
}

void MapsImageProvider::setCubemap(QImage _img)
{
    QMutexLocker locker(&m_imageMutex);
    m_cubemap = _img;
}

void MapsImageProvider::setEquirectangleMap(QImage _img)
{
    QMutexLocker locker(&m_imageMutex);
    m_equirectMap = _img;
}
