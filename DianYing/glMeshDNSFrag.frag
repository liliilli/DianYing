#version 430

in VS_OUT {
	vec3 fragColor;
	vec3 normal;
	vec2 texCoord;
} fs_in;

layout (location = 0) out vec4 outColor;

uniform sampler2D uTexture0; // Diffuse
uniform sampler2D uTexture1; // Normal
uniform sampler2D uTexture2; // Speclar

vec3 directionalLight	= vec3(0, 1, 0);
vec3 lightColor			= vec3(1, 1, 1);
vec3 ambientStrength 	= vec3(0.1);
vec3 viewVector 		= vec3(0, 0, 1);

vec3 TextureToNormal(vec3 textureColor)
{
	return normalize(textureColor * 2.0f - 1.0f);
}

void main() {
	vec3 ambientColor 		= lightColor * ambientStrength;

	vec3 diffuseColor 		= vec3(texture(uTexture0, fs_in.texCoord).rgb);
	vec3 normalDirection 	= TextureToNormal(texture(uTexture1, fs_in.texCoord).rgb);
	diffuseColor 			= diffuseColor * lightColor;
	//max(0, dot(directionalLight, normalDirection)) *

	vec3 specularValue 		= vec3(texture(uTexture2, fs_in.texCoord).rgb);
	float specularStrength	= 0.5f;

	vec3 blinnDirection		= normalize(viewVector + directionalLight);
	float specularFactor 	= pow(max(0, dot(blinnDirection, normalDirection)), 32);
	vec3 specularColor 		= lightColor * specularValue * specularFactor * specularStrength;

  outColor 					= vec4(ambientColor + diffuseColor + specularColor, 1.0f);
}