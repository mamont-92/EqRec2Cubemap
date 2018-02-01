#ifndef CUBEMAPFBORENDER_H
#define CUBEMAPFBORENDER_H

#include <QObject>
#include <QQuickFramebufferObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QVector>
#include <QVector3D>

struct SchemeElement
{
    QVector3D screenPos, cubemapCoords;
};

class CubemapFboRender : public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions
{
public:
    CubemapFboRender();

    void render();

    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);
private:
    void initDataBuffer();
    void drawGeometry();
    QOpenGLShaderProgram m_shaderProgram;
    int m_vertexAttribId, m_cubemapCoordsAttribId;
    int m_matrixUniformId;
    int m_textureSamplerId;

    QVector<SchemeElement> m_schemeData;
};

#endif // CUBEMAPFBORENDER_H
