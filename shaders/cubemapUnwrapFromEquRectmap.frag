uniform sampler2D equrectangleTexture;
varying vec3 var_cubemapCoords;

vec2 coordsInEqurectMap(vec3 _cubemapCoords)
{
    const float pi = 3.14159;
    const float pi_mult_2 = pi*2;

    vec3 normCubeCoords = normalize(_cubemapCoords);
    float lon = atan(normCubeCoords.z, normCubeCoords.x);
    float lat = acos(normCubeCoords.y);

    float lon_norm = lon/pi_mult_2;
    float lat_norm = lat/pi;

    return vec2(lon_norm, lat_norm);
}

void main(void)
{
    gl_FragColor = texture2D(equrectangleTexture, coordsInEqurectMap(var_cubemapCoords));
}
