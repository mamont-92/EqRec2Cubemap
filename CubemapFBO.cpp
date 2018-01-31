#include "CubemapFBO.h"
#include <QOpenGLFramebufferObjectFormat>
#include <QDebug>
#include <CubemapFboRender.h>

//-------------------------------------------------------------------------------

CubemapQuickRender::CubemapQuickRender() : QQuickFramebufferObject(),
    m_render(nullptr) {}

CubemapQuickRender::Renderer * CubemapQuickRender::createRenderer() const
{
    if(m_render)
        delete m_render;
    m_render = new CubemapFboRender;
    return m_render;
}

void CubemapQuickRender::saveToFileCubemap(QString fileName)
{
    qDebug() << "save to file" << fileName;
}

void CubemapQuickRender::loadFromFileEquRectMap(QString fileName)
{
    qDebug() << "load from file" << fileName;
}
