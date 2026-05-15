// Source : LearnOpenGL (https://learnopengl.com/Guest-Articles/2020/Skeletal-Animation)
#pragma once

#include <common.h>

struct BoneInfo
{
	/*id is index in finalBoneMatrices*/
	int id;

	/*offset matrix transforms vertex from model space to bone space*/
	glm::mat4 offset;

};
#pragma once