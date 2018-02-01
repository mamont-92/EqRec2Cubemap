uniform sampler2D equrectangleTexture;
varying vec3 var_cubemapCoords;

vec2 coordsInEqurectMap(vec3 _cubemapCoords)
{
    float pi = 3.14159;
    float pi_mult_2 = 3.14159*2;

    vec3 normCubeCoords = normalize(_cubemapCoords);
    float lon = atan(normCubeCoords.y, normCubeCoords.x);
    float lat = acos(normCubeCoords.z);

    float lon_norm = (lon/pi_mult_2);
    float lat_norm = (lat/pi);

    //return _cubemapCoords.xy;
    return vec2(lon_norm, lat_norm);
}

void main(void)
{
    //gl_FragColor = vec4(var_cubemapCoords, 1.0);
    gl_FragColor = texture2D(equrectangleTexture, coordsInEqurectMap(var_cubemapCoords));
}
