#version 430

layout (location = 0) in vec3 dyPosition;
layout (location = 1) in vec3 dyNormal;
layout (location = 2) in vec2 dyTexCoord0;

out gl_PerVertex {
    vec4 gl_Position;
};

out VS_OUT {
	vec3 fragColor;
	vec3 normal;
	vec2 texCoord;
} vs_out;

void main() {
    gl_Position = vec4(dyPosition, 1.0);
    vs_out.fragColor	= dyNormal;
	vs_out.normal		= dyNormal;
	vs_out.texCoord		= dyTexCoord0;
}
