#version 330 core
out vec4 FragColor;

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoords;

uniform sampler2D texture_diffuse1;
uniform vec3 lightDir;
uniform vec3 viewPos;
uniform vec3 lightColor;

void main() {
	float ambientStrength = 0.3;
	vec3 ambient = ambientStrength * lightColor;

	vec3 norm = normalize(Normal);
	vec3 dir = normalize(-lightDir);
	float diff = max(dot(norm, dir), 0.0);
	vec3 diffuse = diff * lightColor;

	float specularStrength = 0.5;
	vec3 viewDir = normalize(viewPos - FragPos);
	vec3 reflectDir = reflect(-dir, norm);
	float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32.0);
	vec3 specular = specularStrength * spec * lightColor;

	vec4 texColor = texture(texture_diffuse1, TexCoords);
	vec3 result = (ambient + diffuse + specular) * texColor.rgb;
	FragColor = vec4(result, texColor.a);

}