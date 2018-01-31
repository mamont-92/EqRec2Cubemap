#include "CubemapFboRender.h"
#include <QOpenGLFramebufferObjectFormat>

CubemapFboRender::CubemapFboRender() : QQuickFramebufferObject::Renderer(), QOpenGLFunctions()
{
    initializeOpenGLFunctions();
}

void CubemapFboRender::render() {
    glClearColor(0.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
}

QOpenGLFramebufferObject * CubemapFboRender::createFramebufferObject(const QSize &size) {
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);
    return new QOpenGLFramebufferObject(size, format);
}
