#include "CubemapFBO.h"
#include <QOpenGLFramebufferObjectFormat>
#include <QDebug>
#include <CubemapFboRender.h>
//#include <QImage>

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
    m_scheme(Scheme::VerticalLine)
{
    connect(this, &CubemapQuickRender::schemeChanged, [this](Scheme _scheme){
       qDebug() << "scheme changed";
    });
}

CubemapQuickRender::Renderer * CubemapQuickRender::createRenderer() const
{
    CubemapFboRender * render = new CubemapFboRender;

    connect(this, &CubemapQuickRender::imageLoaded, render, &CubemapFboRender::setImage, Qt::QueuedConnection);
    //connect(this, &CubemapQuickRender::schemeChanged, render, &CubemapFboRender::setScheme, Qt::QueuedConnection);

    return render;
}

void CubemapQuickRender::saveToFileCubemap(QString fileName)
{
    qDebug() << "save to file" << fileName;
}

void CubemapQuickRender::loadFromFileEquRectMap(QString fileName)
{
    qDebug() << "load from file" << fileName;
    QImage img(fileName);
    emit imageLoaded(img);
    update(); //enforce render updating and process it's queued signals
}
