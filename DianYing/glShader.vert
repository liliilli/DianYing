
#version 430

out gl_PerVertex {
    vec4 gl_Position;
};

out VS_OUT {
	vec3 fragColor;
	vec2 texCoord;
} vs_out;

vec2 positions[3] = vec2[](
    vec2(0.0, -0.5),
    vec2(0.5, 0.5),
    vec2(-0.5, 0.5)
);

vec3 colors[3] = vec3[](
    vec3(1.0, 0.0, 0.0),
    vec3(0.0, 1.0, 0.0),
    vec3(0.0, 0.0, 1.0)
);

vec2 texCoords[3] = vec2[](
	vec2(  0, 0),
	vec2(  1, 1),
	vec2(  0, 1)
);

void main() {
    gl_Position = vec4(positions[gl_VertexID], 0.0, 1.0);
    vs_out.fragColor = colors[gl_VertexID];
	vs_out.texCoord  = texCoords[gl_VertexID];
}