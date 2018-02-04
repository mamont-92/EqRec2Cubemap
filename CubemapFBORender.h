#ifndef CUBEMAPFBORENDER_H
#define CUBEMAPFBORENDER_H

#include <QObject>
#include <QQuickFramebufferObject>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QVector>
#include <QVector3D>
#include <QOpenGLTexture>
#include <QImage>
#include <QHash>

#include "CubemapQuickRender.h"

class CubemapFBORender : public QObject, public QQuickFramebufferObject::Renderer, protected QOpenGLFunctions
{
    Q_OBJECT
public:
    CubemapFBORender(QObject *parent=Q_NULLPTR);
    void render();
    QOpenGLFramebufferObject *createFramebufferObject(const QSize &size);

signals:
    void imageReady(QImage);

public slots:
    void setImage(QImage img);
    void setYRotation(float _yRotation);
    void setScheme(CubemapQuickRender::Scheme);

protected:
    void synchronize(QQuickFramebufferObject *item);

private:
    struct SchemeDataElement;

    void initDataBuffer();
    void drawGeometry();

    QOpenGLShaderProgram m_shaderProgram;
    int m_vertexAttribId, m_cubemapCoordsAttribId;
    int m_matrixUniformId;
    int m_textureSamplerId;
    int m_yRotationUniformId;

    float m_yRotation;

    QVector<SchemeDataElement> m_schemeData;
    QHash<int, int> m_schemeDataStartInd; //(int)enum -> int
    QHash<int, QSize> m_schemeProjectionSize; //(int)enum -> QSize
    QOpenGLTexture m_equrectangleMap;
    CubemapQuickRender::Scheme m_scheme;
    size_t m_faceRealSize;
    QSize m_outSize;
};

#endif // CUBEMAPFBORENDER_H
