#ifndef MAPSIMAGEPROVIDER_H
#define MAPSIMAGEPROVIDER_H

#include <QQuickImageProvider>
#include <QImage>
#include <QMutex>

class QPixmap;

class MapsImageProvider: public QQuickImageProvider
{
public:
    void setEquirectangleMap(QImage);

    QPixmap requestPixmap(const QString &id, QSize *size, const QSize &requestedSize);

    static MapsImageProvider * instance();

    MapsImageProvider(MapsImageProvider const&) = delete;
    void operator = (MapsImageProvider const&) = delete;
private:
    MapsImageProvider();
    static QPixmap emptyPixmap();
    QImage m_cubemap, m_equirectMap;
    QMutex m_imageMutex;
};

#endif // MAPSIMAGEPROVIDER_H
