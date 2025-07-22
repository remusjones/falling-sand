#version 330

in vec2 fragTexCoord;
out vec4 finalColor;

uniform sampler2D cellTex;
uniform vec2 resolution;

int spatialHash(int x, int y)
{
    return ((x * 73856093) ^ (y * 19349663)) & 0xFF;
}

void main()
{
    vec2 uv = fragTexCoord * resolution;

    int x = int(floor(uv.x));
    int y = int(floor(uv.y));

    vec4 color = texture(cellTex, fragTexCoord);

    int noise = spatialHash(x, y);
    int offset = (noise % 21) - 10;

    vec3 noisyColor = color.rgb + vec3(offset) / 255.0;
    noisyColor = clamp(noisyColor, 0.0, 1.0);

    vec2 pixelSize = 1.0 / resolution;

    vec4 sum = vec4(0.0);
    sum += texture(cellTex, fragTexCoord + vec2(-pixelSize.x, 0.0));
    sum += texture(cellTex, fragTexCoord + vec2(pixelSize.x, 0.0));
    sum += texture(cellTex, fragTexCoord + vec2(0.0, -pixelSize.y));
    sum += texture(cellTex, fragTexCoord + vec2(0.0, pixelSize.y));
    sum *= 0.25;

    vec3 blendedColor = mix(color.rgb, sum.rgb, 0.5);
    finalColor = vec4(mix(blendedColor, noisyColor, 0.5), color.a);
}
