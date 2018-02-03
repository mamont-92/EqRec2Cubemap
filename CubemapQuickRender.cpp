#include "CubemapQuickRender.h"
#include <QOpenGLFramebufferObjectFormat>
#include <QDebug>
#include <CubemapFBORender.h>

const float floatEpsilon = 0.00001f;


CubemapQuickRender::CubemapQuickRender() : QQuickFramebufferObject(),
    m_scheme(Scheme::VerticalCross)
{
    qDebug() << "<<ololol" << isTextureProvider();
}

CubemapQuickRender::Renderer * CubemapQuickRender::createRenderer() const
{
    CubemapFBORender * render = new CubemapFBORender;

    connect(this, &CubemapQuickRender::imageLoaded, render, &CubemapFBORender::setImage, Qt::QueuedConnection);
    connect(this, &CubemapQuickRender::yRotationChanged, render, &CubemapFBORender::setYRotation, Qt::QueuedConnection);
    connect(this, &CubemapQuickRender::schemeChanged, render, &CubemapFBORender::setScheme, Qt::QueuedConnection);
    connect(render, &CubemapFBORender::imageReady, this, &CubemapQuickRender::cubemapReady, Qt::QueuedConnection);

    return render;
}

void CubemapQuickRender::cubemapReady(QImage img)
{
    m_cubemapImage = img;
}

void CubemapQuickRender::saveToFileCubemap(QString fileName)
{
    qDebug() << "save to file" << fileName;
    if(!m_cubemapImage.isNull())
        m_cubemapImage.save(fileName);
}

void CubemapQuickRender::loadFromFileEquRectMap(QString fileName)
{
    qDebug() << "load from file" << fileName;
    QImage img(fileName);
    emit imageLoaded(img);
    update(); //enforce render updating and process it's queued signals
}



void CubemapQuickRender::setScheme(CubemapQuickRender::Scheme _scheme)
{
    if(m_scheme != _scheme){
        m_scheme = _scheme;
        emit schemeChanged(m_scheme);
        update();
    }
}

void CubemapQuickRender::setYRotation(float _yRotation)
{
    if(fabs(m_yRotation - _yRotation) > floatEpsilon){
        m_yRotation = _yRotation; //% 360.0;
        emit yRotationChanged(m_yRotation);
        update();
    }
}

CubemapQuickRender::Scheme CubemapQuickRender::scheme() const
{
    return m_scheme;
}

float CubemapQuickRender::yRotation() const
{
    return m_yRotation;
}




