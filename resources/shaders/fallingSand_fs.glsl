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

    finalColor = vec4(noisyColor, color.a);
}
