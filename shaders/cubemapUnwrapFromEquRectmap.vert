attribute vec3 vertex;
attribute vec3 cubemapCoords;

uniform mat4 projModelViewMat;

varying vec3 var_cubemapCoords;

void main(void)
{
    var_cubemapCoords = cubemapCoords;
    gl_Position = projModelViewMat * vec4(vertex, 1.0);
}
