#version 130

in vec4 in_Position;
in vec3 in_Normal;
in vec2 in_TexCoord;
in vec4 in_BoneWeights;
in vec4 in_BoneIndices;

out vec3 ex_Normal;
out vec2 ex_TexCoord;

uniform mat4 projectionMatrix;
uniform mat4 modelMatrix;
uniform mat4 viewMatrix;
uniform mat4 boneMatrices[64];

void main()
{

    vec4 boneWeights = in_BoneWeights;
    boneWeights.w = 1.0 - dot(boneWeights.xyz, vec3(1.0, 1.0, 1.0));

    mat4 transformMatrix = boneWeights.x * boneMatrices[int(in_BoneIndices.x)];
    transformMatrix += boneWeights.y * boneMatrices[int(in_BoneIndices.y)];
    transformMatrix += boneWeights.z * boneMatrices[int(in_BoneIndices.z)];
    transformMatrix += boneWeights.w * boneMatrices[int(in_BoneIndices.w)];

    vec4 modified_Position = transformMatrix * in_Position;
    vec4 modified_Normal = transformMatrix * vec4(in_Normal, 0.0);

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * modified_Position;
    ex_Normal = (modelMatrix * modified_Normal).xyz;
    ex_TexCoord = in_TexCoord;
}
