#include "CubemapFboRender.h"
#include <QOpenGLFramebufferObjectFormat>
#include <QMatrix4x4>
#include <QDebug>
#include <QImage>

#define _USE_MATH_DEFINES
#include <math.h>

const int textureUint = 0;

CubemapFboRender::CubemapFboRender(QObject *parent) : QObject(parent), QQuickFramebufferObject::Renderer(), QOpenGLFunctions(),
    m_equrectangleMap(QOpenGLTexture::Target2D), m_yRotation(0.0f)
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
    m_yRotationUniformId = m_shaderProgram.uniformLocation("yRotation");
    initDataBuffer();
}

void CubemapFboRender::render() {
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glDepthMask(false);
    glDisable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    QMatrix4x4 projModelViewMat;
    projModelViewMat.ortho(0.0f,6.0f, 0.0f,6.0f, -1.0f, 1.0f);

    if(m_equrectangleMap.isCreated()){
        m_shaderProgram.bind();
        m_shaderProgram.setUniformValue(m_matrixUniformId, projModelViewMat);
        m_equrectangleMap.bind(textureUint);
        m_shaderProgram.setUniformValue(m_textureSamplerId, textureUint);
        float yRotRad = m_yRotation * (M_PI / 180.0);
        m_shaderProgram.setUniformValue(m_yRotationUniformId, yRotRad);
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
    enum CubemapFaces{
                Xp, Xn, Yp, Yn, Zp, Zn
    };

    QVector<QVector3D> cubePoints {
        QVector3D{-1.0f, 1.0f, 1.0f}, //p0
        QVector3D{-1.0f, 1.0f,-1.0f}, //p1
        QVector3D{ 1.0f, 1.0f,-1.0f}, //p2
        QVector3D{ 1.0f, 1.0f, 1.0f}, //p3
        QVector3D{-1.0f,-1.0f, 1.0f}, //p4
        QVector3D{-1.0f,-1.0f,-1.0f}, //p5
        QVector3D{ 1.0f,-1.0f,-1.0f}, //p6
        QVector3D{ 1.0f,-1.0f, 1.0f}  //p7
    };

    QVector<QVector3D> faceUnwrapPoints{
        QVector3D{1.0f, 1.0f, 0.0f},
        QVector3D{1.0f, 0.0f, 0.0f},
        QVector3D{0.0f, 0.0f, 0.0f},
        QVector3D{0.0f, 1.0f, 0.0f}
    };

    struct CubemapSchemeElement{
        CubemapFaces faceType; //just for code understanding
        qint8 pointIndex0, pointIndex1, pointIndex2, pointIndex3;
        qint8 column, row;
    };


    /*QVector<CubemapSchemeElement> verticalCross = { //usual scheme for all cube generators
        { Xp, 5,1,0,4, 1,1},
        { Xn, 2,6,7,3, 1,3},
        { Yp, 1,2,3,0, 1,0},
        { Yn, 6,5,4,7, 1,2},
        { Zp, 4,0,3,7, 0,1},
        { Zn, 6,2,1,5, 2,1}
    };*/

    QVector<CubemapSchemeElement> verticalCross = { //scheme for unity3d??
        { Xp, 6,2,1,5, 1,1},
        { Xn, 3,7,4,0, 1,3},
        { Yp, 2,3,0,1, 1,0},
        { Yn, 7,6,5,4, 1,2},
        { Zp, 5,1,0,4, 0,1},
        { Zn, 7,3,2,6, 2,1}
    };

    QVector<CubemapSchemeElement> horizontalCross = { //usual scheme for all cube generators
        { Xp, 5,1,0,4, 1,1},
        { Xn, 7,3,2,6, 3,1},
        { Yp, 1,2,3,0, 1,0},
        { Yn, 6,5,4,7, 1,2},
        { Zp, 4,0,3,7, 0,1},
        { Zn, 6,2,1,5, 2,1}
    };

    QVector<CubemapSchemeElement> verticalLine = {
        { Xp, 6,2,1,5, 0,0},
        { Xn, 3,7,4,0, 0,1},
        { Yp, 2,3,0,1, 0,2},
        { Yn, 7,6,5,4, 0,3},
        { Zp, 5,1,0,4, 0,4},
        { Zn, 7,3,2,6, 0,5}
    };


    QVector<CubemapSchemeElement> horizontalLine = {
        { Xp, 6,2,1,5, 0,0},
        { Xn, 3,7,4,0, 1,0},
        { Yp, 2,3,0,1, 2,0},
        { Yn, 7,6,5,4, 3,0},
        { Zp, 5,1,0,4, 4,0},
        { Zn, 7,3,2,6, 5,0}
    };



    auto insertDataToSheme = [&cubePoints, &faceUnwrapPoints, this]
            (const QVector<CubemapSchemeElement> & schemeVec)
    {
        for(CubemapSchemeElement  scheme : schemeVec){
            QVector3D delta = QVector3D(scheme.column, scheme.row, 0.0f);
            this->m_schemeData
                    << SchemeDataElement{faceUnwrapPoints[0]+delta,
                        cubePoints[scheme.pointIndex0]
            };
            this->m_schemeData
                    << SchemeDataElement{faceUnwrapPoints[1]+delta,
                        cubePoints[scheme.pointIndex1]
            };
            this->m_schemeData
                    << SchemeDataElement{faceUnwrapPoints[2]+delta,
                        cubePoints[scheme.pointIndex2]
            };
            //second triangle
            this->m_schemeData
                    << SchemeDataElement{faceUnwrapPoints[0]+delta,
                        cubePoints[scheme.pointIndex0]
            };
            this->m_schemeData
                    << SchemeDataElement{faceUnwrapPoints[2]+delta,
                        cubePoints[scheme.pointIndex2]
            };
            this->m_schemeData
                    << SchemeDataElement{faceUnwrapPoints[3]+delta,
                        cubePoints[scheme.pointIndex3]
            };

        }

    };

    insertDataToSheme(verticalCross);
    //insertDataToSheme(horizontalCross);
    //insertDataToSheme(verticalLine);
    //insertDataToSheme(horizontalLine);

}


void CubemapFboRender::drawGeometry()
{
    m_shaderProgram.enableAttributeArray(m_vertexAttribId);
    m_shaderProgram.enableAttributeArray(m_cubemapCoordsAttribId);


    float * vertexPtr = reinterpret_cast<float*>(&(m_schemeData[0].screenPos));
    float * coordsPtr = reinterpret_cast<float*>(&(m_schemeData[0].cubemapCoords));

    m_shaderProgram.setAttributeArray(m_vertexAttribId, vertexPtr, 3, sizeof(SchemeDataElement));
    m_shaderProgram.setAttributeArray(m_cubemapCoordsAttribId, coordsPtr, 3, sizeof(SchemeDataElement));
    glDrawArrays(GL_TRIANGLES, 0, m_schemeData.size());

    m_shaderProgram.disableAttributeArray(m_vertexAttribId);
    m_shaderProgram.disableAttributeArray(m_cubemapCoordsAttribId);
}

void CubemapFboRender::setImage(QImage img)
{
    qDebug() << "set image to texture" << img;
    m_equrectangleMap.destroy();
    m_equrectangleMap.setData(img, QOpenGLTexture::DontGenerateMipMaps);
    m_equrectangleMap.setMagnificationFilter(QOpenGLTexture::Linear);
    m_equrectangleMap.setMinificationFilter(QOpenGLTexture::Linear);
    update();
}

void CubemapFboRender::setYRotation(float _yRotation)
{
    qDebug() << "set y rotation" << _yRotation;
    m_yRotation = _yRotation;
}

/*void CubemapFboRender::setScheme(QImage)
{
    qDebug() << "new scheme in render";
}*/
