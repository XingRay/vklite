#version 460
#extension GL_ARB_separate_shader_objects: enable

layout (location = 0) in vec2 inTextureCoordinates;

layout (set = 0, binding = 0) uniform sampler2D textureSampler;

layout (location = 0) out vec4 outColor;

void main() {
    outColor = texture(textureSampler, inTextureCoordinates);
}