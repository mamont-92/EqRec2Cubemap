#include "CubemapFBO.h"
#include <QOpenGLFramebufferObjectFormat>
#include <QDebug>
#include <CubemapFboRender.h>

//-------------------------------------------------------------------------------
void CubemapQuickRender::setScheme(Scheme _scheme)
{
    Scheme oldScheme = m_scheme;
    m_scheme = _scheme;
    if(m_scheme != oldScheme)
        emit schemeChanged(m_scheme);
}

CubemapQuickRender::Scheme CubemapQuickRender::scheme() const
{
    return m_scheme;
}

CubemapQuickRender::CubemapQuickRender() : QQuickFramebufferObject(),
    m_render(nullptr),
    m_scheme(Scheme::VerticalLine)
{
    connect(this, &CubemapQuickRender::schemeChanged, [this](Scheme _scheme){
       qDebug() << "scheme changed";
    });
}

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
