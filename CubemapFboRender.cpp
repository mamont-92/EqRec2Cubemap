#include "CubemapFboRender.h"
#include <QOpenGLFramebufferObjectFormat>
#include <QMatrix4x4>
#include <QDebug>
#include <QImage>

const int textureUint = 0;

CubemapFboRender::CubemapFboRender(QObject *parent) : QObject(parent), QQuickFramebufferObject::Renderer(), QOpenGLFunctions(),
    m_equrectangleMap(QOpenGLTexture::Target2D)
{
    initializeOpenGLFunctions();
    m_shaderProgram.addCacheableShaderFromSourceFile(QOpenGLShader::Vertex, ":/shaders/cubemapUnwrapFromEquRectmap.vert");
    m_shaderProgram.addCacheableShaderFromSourceFile(QOpenGLShader::Fragment, ":/shaders/cubemapUnwrapFromEquRectmap.frag");

    if(!m_shaderProgram.link())
        qDebug() << "Error: can't link shader!";

    m_matrixUniformId = m_shaderProgram.uniformLocation("projModelViewMat");
    m_vertexAttribId = m_shaderProgram.attributeLocation("vertex");
    m_cubemapCoordsAttribId = m_shaderProgram.attributeLocation("cubemapCoords");
    m_textureSamplerId = m_shaderProgram.uniformLocation("equrectangleTexture");

    initDataBuffer();
}

void CubemapFboRender::render() {
    glClearColor(1.0f, 1.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDepthMask(false);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    QMatrix4x4 projModelViewMat;

    if(m_equrectangleMap.isCreated()){
        m_shaderProgram.bind();
        m_shaderProgram.setUniformValue(m_matrixUniformId, projModelViewMat);
        m_equrectangleMap.bind(textureUint);
        m_shaderProgram.setUniformValue(m_textureSamplerId, textureUint);
        drawGeometry();
        m_shaderProgram.release();
    }

    QImage img = framebufferObject()->toImage(false);
    emit imageReady(img);
}

QOpenGLFramebufferObject * CubemapFboRender::createFramebufferObject(const QSize &size) {
    QOpenGLFramebufferObjectFormat format;
    format.setAttachment(QOpenGLFramebufferObject::CombinedDepthStencil);
    format.setSamples(4);
    return new QOpenGLFramebufferObject(size, format);
}


void CubemapFboRender::initDataBuffer()
{
    m_schemeData << SchemeElement{QVector3D{-1.0f,-1.0f, 0.0f}, QVector3D{-1.0f,-1.0f, 1.0f}};
    m_schemeData << SchemeElement{QVector3D{-1.0f, 1.0f, 0.0f}, QVector3D{-1.0f, 1.0f, 1.0f}};
    m_schemeData << SchemeElement{QVector3D{ 1.0f, 1.0f, 0.0f}, QVector3D{ 1.0f, 1.0f, 1.0f}};

    m_schemeData << SchemeElement{QVector3D{-1.0f,-1.0f, 0.0f}, QVector3D{-1.0f,-1.0f, 1.0f}};
    m_schemeData << SchemeElement{QVector3D{ 1.0f, 1.0f, 0.0f}, QVector3D{ 1.0f, 1.0f, 1.0f}};
    m_schemeData << SchemeElement{QVector3D{ 1.0f,-1.0f, 0.0f}, QVector3D{ 1.0f,-1.0f, 1.0f}};

    /*m_schemeData << SchemeElement{QVector3D{-1.0f,-1.0f, 1.0f}, QVector3D{-1.0f,-1.0f, 1.0f}};
    m_schemeData << SchemeElement{QVector3D{-1.0f, 1.0f, 1.0f}, QVector3D{-1.0f, 1.0f, 1.0f}};
    m_schemeData << SchemeElement{QVector3D{1.0f, 1.0f, 1.0f}, QVector3D{ 1.0f, 1.0f, 1.0f}};

    m_schemeData << SchemeElement{QVector3D{-1.0f,-1.0f, 1.0f}, QVector3D{-1.0f,-1.0f, 1.0f}};
    m_schemeData << SchemeElement{QVector3D{1.0f, 1.0f, 1.0f}, QVector3D{ 1.0f, 1.0f, 1.0f}};
    m_schemeData << SchemeElement{QVector3D{1.0f,-1.0f, 1.0f}, QVector3D{ 1.0f,-1.0f, 1.0f}};*/
}


void CubemapFboRender::drawGeometry()
{
    m_shaderProgram.enableAttributeArray(m_vertexAttribId);
    m_shaderProgram.enableAttributeArray(m_cubemapCoordsAttribId);


    float * vertexPtr = reinterpret_cast<float*>(&(m_schemeData[0].screenPos));
    float * coordsPtr = reinterpret_cast<float*>(&(m_schemeData[0].cubemapCoords));

    m_shaderProgram.setAttributeArray(m_vertexAttribId, vertexPtr, 3, sizeof(SchemeElement));
    m_shaderProgram.setAttributeArray(m_cubemapCoordsAttribId, coordsPtr, 3, sizeof(SchemeElement));
    glDrawArrays(GL_TRIANGLES, 0, m_schemeData.size());

    m_shaderProgram.disableAttributeArray(m_vertexAttribId);
    m_shaderProgram.disableAttributeArray(m_cubemapCoordsAttribId);
}

void CubemapFboRender::setImage(QImage img)
{
    qDebug() << "set image to texture" << img;
    m_equrectangleMap.destroy();
    m_equrectangleMap.setData(img, QOpenGLTexture::DontGenerateMipMaps);
    m_equrectangleMap.setMagnificationFilter(QOpenGLTexture::Nearest);
    m_equrectangleMap.setMinificationFilter(QOpenGLTexture::Nearest);
    update();
}

/*void CubemapFboRender::setScheme(QImage)
{
    qDebug() << "new scheme in render";
}*/
