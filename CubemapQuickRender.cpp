#include "CubemapQuickRender.h"
#include <QOpenGLFramebufferObjectFormat>
#include <QDebug>
#include <CubemapFBORender.h>
#include "MapsImageProvider.h"

const float floatEpsilon = 0.00001f;


CubemapQuickRender::CubemapQuickRender() : QQuickFramebufferObject(),
    m_scheme(Scheme::VerticalCross), m_renderSize(64,64)
{

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
    if(!m_cubemapImage.isNull())
        m_cubemapImage.save(fileName);
}

void CubemapQuickRender::loadFromFileEquRectMap(QString fileName)
{
    QImage img(fileName);
    m_faceSideSize = img.height();
    updateRenderSize();
    MapsImageProvider::instance()->setEquirectangleMap(img);
    emit imageLoaded(img);
    update(); //enforce render updating and process it's queued signals
}

QSize CubemapQuickRender::renderSize()
{
    return m_renderSize;
}

void CubemapQuickRender::setScheme(CubemapQuickRender::Scheme _scheme)
{
    if(m_scheme != _scheme){
        m_scheme = _scheme;
        updateRenderSize();
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

void CubemapQuickRender::updateRenderSize()
{
    static QHash<int, QSize> schemeSizes ={ //duplicate date because of different threads render and quick object
        {(int)CubemapQuickRender::Scheme::VerticalCross, QSize(3,4)},
        {(int)CubemapQuickRender::Scheme::HorizontalCross, QSize(4,3)},
        {(int)CubemapQuickRender::Scheme::VerticalLine, QSize(1,6)},
        {(int)CubemapQuickRender::Scheme::HorizontalLine, QSize(6,1)}
    };
    QSize newSize = schemeSizes.value((int)m_scheme)*m_faceSideSize;
    if(m_renderSize  != newSize){
        m_renderSize = newSize;
        emit renderSizeChanged(m_renderSize);
    }
}



