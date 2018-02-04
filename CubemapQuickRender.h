#ifndef CUBEMAPQUICKRENDER_H
#define CUBEMAPQUICKRENDER_H

#include <QtQuick/QQuickFramebufferObject>
#include <QImage>

class CubemapFboRender;

class CubemapQuickRender : public QQuickFramebufferObject
{
    Q_OBJECT
    Q_PROPERTY(Scheme scheme READ scheme WRITE setScheme NOTIFY schemeChanged)
    Q_PROPERTY(float yRotation READ yRotation WRITE setYRotation NOTIFY yRotationChanged)
public:
    enum class Scheme{
        VerticalCross,
        HorizontalCross,
        VerticalLine,
        HorizontalLine
    };
    Q_ENUMS(Scheme)

    QSize renderSize();

    CubemapQuickRender();
    Renderer *createRenderer() const;

    Scheme scheme() const;
    float yRotation() const;

    void setScheme(Scheme _scheme);
    void setYRotation(float _yRotation);

    Q_INVOKABLE void saveToFileCubemap(QString fileName);
    Q_INVOKABLE void loadFromFileEquRectMap(QString fileName);

public slots:
    void cubemapReady(QImage);

signals:
    void schemeChanged(CubemapQuickRender::Scheme);
    void imageLoaded(QImage);
    void yRotationChanged(float);
private:
    void updateRenderSize();
    Scheme m_scheme;
    QImage m_cubemapImage;
    QSize m_renderSize;
    int m_faceSideSize;
    float m_yRotation;
};

#endif // CUBEMAPQUICKRENDER_H
