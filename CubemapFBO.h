#ifndef CUBEMAPFBO_H
#define CUBEMAPFBO_H

#include <QtQuick/QQuickFramebufferObject>

class CubemapFboRender;

class CubemapQuickRender : public QQuickFramebufferObject
{
    Q_OBJECT
public:
    CubemapQuickRender();
    Renderer *createRenderer() const;

    Q_INVOKABLE void saveToFileCubemap(QString fileName);
    Q_INVOKABLE void loadFromFileEquRectMap(QString fileName);
private:
    mutable CubemapFboRender * m_render;
};

#endif // CUBEMAPFBO_H
