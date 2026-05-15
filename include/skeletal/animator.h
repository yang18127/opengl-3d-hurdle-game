// Source : LearnOpenGL (https://learnopengl.com/Guest-Articles/2020/Skeletal-Animation)
#pragma once

#include <common.h>
#include <map>
#include <vector>
#include <assimp/scene.h>
#include <assimp/Importer.hpp>
#include <skeletal/animation.h>
#include <skeletal/bone.h>

class Animator
{
public:
	Animator(Animation* animation)
	{
		m_CurrentTime = 0.0;
		m_CurrentAnimation = animation;

		m_FinalBoneMatrices.reserve(200);

		for (int i = 0; i < 200; i++)
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
	}

	void UpdateAnimation(float dt, bool isLoop = true)
	{
		m_DeltaTime = dt;
        if (m_CurrentAnimation)
        {
            m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;

            if (isLoop)
            {
                m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
            }
            else
            {
                if (m_CurrentTime >= m_CurrentAnimation->GetDuration())
                    m_CurrentTime = m_CurrentAnimation->GetDuration() - 0.001f;
            }

            CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
        }
	}

	void PlayAnimation(Animation* pAnimation)
	{
		m_CurrentAnimation = pAnimation;
		m_CurrentTime = 0.0f;
	}

	void Reset() { m_CurrentTime = 0.0f; }

	void CalculateBoneTransform(const AssimpNodeData* node, glm::mat4 parentTransform)
	{
		std::string nodeName = node->name;
		glm::mat4 nodeTransform = node->transformation;

		Bone* Bone = m_CurrentAnimation->FindBone(nodeName);

		if (Bone)
		{
			Bone->Update(m_CurrentTime);
			nodeTransform = Bone->GetLocalTransform();
		}

		glm::mat4 globalTransformation = parentTransform * nodeTransform;

		const auto& boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
		auto it = boneInfoMap.find(nodeName);
		if (it != boneInfoMap.end())
		{
			int index = boneInfoMap.at(nodeName).id;
			glm::mat4 offset = boneInfoMap.at(nodeName).offset;
			m_FinalBoneMatrices[index] = globalTransformation * offset;
		}

		for (int i = 0; i < node->childrenCount; i++)
			CalculateBoneTransform(&node->children[i], globalTransformation);
	}

	const std::vector<glm::mat4>& GetFinalBoneMatrices() const
	{
		return m_FinalBoneMatrices;
	}

private:
	std::vector<glm::mat4> m_FinalBoneMatrices;
	Animation* m_CurrentAnimation;
	float m_CurrentTime;
	float m_DeltaTime;

};