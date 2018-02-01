uniform sampler2D equrectangleTexture;
varying vec3 var_cubemapCoords;

vec2 coordsInEqurectMap(vec3 _cubemapCoords)
{
    return _cubemapCoords.xy;
}

void main(void)
{
    gl_FragColor = vec4(var_cubemapCoords, 1.0);
    //gl_FragColor = vec4(1.0,0.0,0.0, 1.0);
    //gl_FragColor = texture2D(equrectangleTexture, coordsInEqurectMap(var_cubemapCoords));
}
