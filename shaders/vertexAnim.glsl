#version 130

uniform mat4 projectionMatrix;
uniform mat4 viewMatrix;
uniform mat4 modelMatrix;
uniform mat4 boneMatrices[60];

in vec4 inPosition;
in vec3 inNormal;
in vec4 inColor;
in vec2 inTexCoord;
in vec4 inBoneWeights;
in vec4 inBoneIndices;

out vec4 worldPosition;
out vec3 worldNormal;
out vec4 outColor;
out vec2 outTexCoord;

void main()
{
    vec4 boneWeights = inBoneWeights;
    boneWeights.w = 1.0 - dot(boneWeights.xyz, vec3(1.0, 1.0, 1.0));

    mat4 transformMatrix = boneWeights.x * boneMatrices[int(inBoneIndices.x)];
    transformMatrix += boneWeights.y * boneMatrices[int(inBoneIndices.y)];
    transformMatrix += boneWeights.z * boneMatrices[int(inBoneIndices.z)];
    transformMatrix += boneWeights.w * boneMatrices[int(inBoneIndices.w)];

    vec4 newPosition = transformMatrix * inPosition;
    vec4 newNormal = transformMatrix * vec4(inNormal, 0.0);
    worldNormal = (modelMatrix * newNormal).xyz;

    gl_Position = projectionMatrix * viewMatrix * modelMatrix * newPosition;
    worldPosition = modelMatrix * newPosition;
    outColor = inColor;
    outTexCoord = inTexCoord;
};
