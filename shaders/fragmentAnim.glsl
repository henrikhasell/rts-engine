#version 130

uniform vec3 lightPosition;
uniform vec4 lightAmbientColor;
uniform vec4 lightDiffuseColor;
uniform int useTexture;
uniform sampler2D texture;

in vec4 worldPosition;
in vec3 worldNormal;
in vec4 outColor;
in vec2 outTexCoord;

void main()
{
    vec3 normal = normalize(worldNormal);
    vec3 position = worldPosition.xyz - worldPosition.w;
    vec3 lightVector = normalize(lightPosition);
    vec4 fragColor;

    if (useTexture == 0)
    {
        fragColor = outColor;
    }
    else
    {
        fragColor = texture2D(texture, outTexCoord);
    }

    vec4 ambient = fragColor * lightAmbientColor;
    vec4 diffuse = fragColor * lightDiffuseColor * max(0.0, dot(normal, lightVector));

    gl_FragColor = ambient + diffuse;
}
