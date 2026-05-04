#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <learnopengl/shader.h>
#include <learnopengl/model.h>
#include <game/player.h>

class Obstacle {
public :
	Model* model;
	glm::vec3 position;
	float speed;
	float startZ;

	Obstacle(Model* m, glm::vec3 startPos, float moveSpeed) {
		model = m;
		position = startPos;
		startZ = startPos.z;
		speed = moveSpeed;
	}

	void update(float deltaTime) {
		position.z += speed * deltaTime;

		if (position.z > 5.0f) {
			position.z = startZ;
		}
	}

	void draw(Shader& shader) {
		glm::mat4 modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, position);
		modelMat = glm::scale(modelMat, glm::vec3(1.5f));
		shader.setMat4("model", modelMat);
		model->Draw(shader);
	}

	bool checkCollision(const Player& player) {
		glm::vec3 pMin = player.getMinBounds();
		glm::vec3 pMax = player.getMaxBounds();

		glm::vec3 oMin = position - glm::vec3(0.5f, 0.3f, 0.15f);
		glm::vec3 oMax = position + glm::vec3(0.5f, 0.3f, 0.15f);

		bool collisionX = pMin.x <= oMax.x && pMax.x >= oMin.x;
		bool collisionY = pMin.y <= oMax.y && pMax.y >= oMin.y;
		bool collisionZ = pMin.z <= oMax.z && pMax.z >= oMin.z;

		return collisionX && collisionY && collisionZ;
	}

};
