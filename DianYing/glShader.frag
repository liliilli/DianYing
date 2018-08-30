#version 430

in VS_OUT {
	vec3 fragColor;
	vec2 texCoord;
} fs_in;

layout(location = 0) out vec4 outColor;

uniform sampler2D uTexture0;

void main() {
    outColor = vec4(fs_in.fragColor * vec3(texture(uTexture0, fs_in.texCoord)), 1.0);
}