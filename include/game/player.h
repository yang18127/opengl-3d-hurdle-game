#pragma once

#include <common.h>
#include <learnopengl/shader.h>
#include <learnopengl/model.h>

class Player {
public:
	Model* model;
	glm::vec3 position;
	float yVelocity;
	float gravity;
	bool isJumping;

	Player(Model* m, glm::vec3 startPos) {
		model = m;
		position = startPos;
		yVelocity = 0.0f;
		gravity = -20.0f;
		isJumping = false;
	}

	void jump() {
		if (!isJumping) {
			yVelocity = 7.0f;
			isJumping = true;
		}
	}

	void update(float deltaTime) {
		if (isJumping) {
			position.y += yVelocity * deltaTime;
			yVelocity += gravity * deltaTime;
			if (position.y <= 0.0f) {
				position.y = 0.0f;
				isJumping = false;
				yVelocity = 0.0f;
			}

		}
	}

	void draw(Shader& shader) {
		glm::mat4 modelMat = glm::mat4(1.0f);
		modelMat = glm::translate(modelMat, position);
		modelMat = glm::scale(modelMat, glm::vec3(0.01f));
		modelMat = glm::rotate(modelMat, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		shader.setMat4("model", modelMat);
		model->Draw(shader);
	}

	glm::vec3 getMinBounds() const { return position - glm::vec3(0.35f, 0.2f, 0.0f); }
	glm::vec3 getMaxBounds() const { return position + glm::vec3(0.35f, 0.2f, 0.0f); }
};