#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;
layout (location = 3) in ivec4 boneIds;
layout (location = 4) in vec4 weights;

out vec3 FragPos;
out vec3 Normal;
out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

const int MAX_BONES = 200;
const int MAX_BONE_INFLUENCE = 4;
uniform mat4 finalBoneMatrices[MAX_BONES];

void main()
{
	vec4 totalPosition = vec4(0.0);
	vec3 totalNormal = vec3(0.0);

	for(int i = 0 ; i < MAX_BONE_INFLUENCE ; i++)
	{
		if(boneIds[i] == -1)
			continue;

		if(boneIds[i] >= MAX_BONES)
		{
			totalPosition = vec4(aPos, 1.0f);
			break;
		}

		vec4 localPosition = finalBoneMatrices[boneIds[i]] * vec4(aPos, 1.0f);
		totalPosition += localPosition * weights[i];

		vec3 localNormal = mat3(finalBoneMatrices[boneIds[i]]) * aNormal;
		totalNormal += localNormal * weights[i];
	}

	FragPos = vec3(model * totalPosition);
	Normal = mat3(transpose(inverse(model))) * totalNormal;
	TexCoords = aTexCoords;

	gl_Position = projection * view * vec4(FragPos, 1.0);
}