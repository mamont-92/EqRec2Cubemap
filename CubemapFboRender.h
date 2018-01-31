#ifndef CUBEMAPFBORENDER_H
#define CUBEMAPFBORENDER_H

#include <QObject>
#include <QQuickFramebufferObject>
#include <QOpenGLFunctions>

class CubemapFboRender : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions
{
public:
    CubemapFboRender();

    void render();

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);
};

#endif // CUBEMAPFBORENDER_H
