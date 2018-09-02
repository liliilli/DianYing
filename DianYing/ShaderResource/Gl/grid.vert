#version 430

layout (location = 0) in vec3 dyPosition;

void main() {
    gl_Position = vec4(dyPosition, 1.0);
}