#pragma once

// Std. Includes
#include <vector>

// GL Includes
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>
#include "wall.h"

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum Camera_Movement {
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	JUMP
};

extern float gardenradius;
extern glm::vec3 posG;
extern float allowhight;
extern const int cubelength;
extern bool boom;
extern Wall cube[];
extern bool walk;
int doublejump = 0;


// Default camera values
const glm::vec3 TARGET = glm::vec3(0, 0, 0);
const glm::vec3 POSITION = glm::vec3(0.0f, 0.50f, -0.50f);
const glm::vec3 FRONT = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 UP = glm::vec3(0.0f, 1.0f, 0.0f);
const GLfloat YAW = 90.0f;
const GLfloat PITCH = -30.0f;
const GLfloat SPEED = 15.0f;
const GLfloat SENSITIVTY = 0.25f;
const GLfloat ZOOM = 45.0f;


// An abstract camera class that processes input and calculates the corresponding Eular Angles, Vectors and Matrices for use in OpenGL
class Camera
{
public:
	// Camera Attributes
	glm::vec3 Target;
	glm::vec3 Position;
	glm::vec3 Front;
	glm::vec3 Up;
	glm::vec3 Right;
	glm::vec3 WorldUp;
	glm::vec3 TargetFront;
	// Eular Angles
	GLfloat Yaw;
	GLfloat Pitch;
	// Camera options
	GLfloat MovementSpeed;
	GLfloat MouseSensitivity;
	GLfloat Zoom;

	// Constructor with vectors
	Camera(glm::vec3 target = TARGET, glm::vec3 position = POSITION, glm::vec3 front = FRONT, glm::vec3 up = UP, GLfloat yaw = YAW, GLfloat pitch = PITCH) : MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		this->Target = target;
		this->Position = position;
		this->WorldUp = up;
		this->Yaw = 90;
		this->Pitch = pitch;
		this->Front = front;
		this->TargetFront = front;
		this->updateCameraVectors();
	}
	// Constructor with scalar values
	Camera(GLfloat posX, GLfloat posY, GLfloat posZ, GLfloat upX, GLfloat upY, GLfloat upZ, GLfloat yaw, GLfloat pitch) : Front(glm::vec3(0.0f, -30.0f, 30.0f)), MovementSpeed(SPEED), MouseSensitivity(SENSITIVTY), Zoom(ZOOM)
	{
		this->Position = glm::vec3(posX, posY, posZ);
		this->WorldUp = glm::vec3(upX, upY, upZ);
		this->Yaw = yaw;
		this->Pitch = pitch;
		this->updateCameraVectors();
	}

	// Returns the view matrix calculated using Eular Angles and the LookAt Matrix
	glm::mat4 GetViewMatrix()
	{
		return glm::lookAt(this->Position, this->Position + this->Front, this->Up);
	}

	

	// Processes input received from any keyboard-like input system. Accepts input parameter in the form of camera defined ENUM (to abstract it from windowing systems)
	void ProcessKeyboard(Camera_Movement direction, GLfloat deltaTime)
	{
		GLfloat velocity = this->MovementSpeed * deltaTime;
		if (direction == FORWARD) {
			glm::vec3 temp;
			temp = this->Target;
			if (falling(temp, cube, cubelength, 0.1, posG, gardenradius)) {
				temp += this->TargetFront * velocity * (GLfloat)0.5;
			}
			else
				temp += this->TargetFront * velocity;
			this->Target = RealPosition(this->Target, temp, cube, cubelength, 0.01, allowhight, posG, gardenradius);
//			if (!IfBoom(temp,cube,2)) {
//					this->Target = temp;
//		}
		}

		if (direction == BACKWARD) {
			//this->Position -= this->TargetFront * velocity;
			glm::vec3 temp;
			temp = this->Target;
			if (falling(temp, cube, cubelength, 0.1,posG, gardenradius)) {
				temp -= this->TargetFront * velocity * (GLfloat)0.5;
			}
			else
			    temp -= this->TargetFront * velocity;

			this->Target = RealPosition(this->Target,temp,cube,cubelength,0.01,allowhight,posG, gardenradius);
		}
		if (direction == LEFT) {
			//this->Yaw -= this->MouseSensitivity;
			glm::vec3 temp;
			temp = this->Target;
			if (falling(temp, cube, cubelength, 0.1, posG, gardenradius)) {
				temp -= this->Right * velocity * (GLfloat)0.5;
			}
			else
			    temp -= this->Right * velocity;
			this->Target = RealPosition(this->Target, temp, cube, cubelength, 0.01, allowhight, posG, gardenradius);
		}
			
		if (direction == RIGHT) {
			//this->Yaw += this->MouseSensitivity;
			glm::vec3 temp;
			temp = this->Target;
			if (falling(temp, cube,cubelength, 0.1, posG, gardenradius)) {
				temp += this->Right * velocity * (GLfloat)0.5;
			}
			else
			temp += this->Right * velocity;
			this->Target = RealPosition(this->Target, temp, cube, cubelength, 0.01, allowhight, posG, gardenradius);
		}

		if (direction == JUMP) {

			if (!falling(this->Target, cube, cubelength, 1,posG,gardenradius))

				this->Target += glm::vec3(0, 6, 0);
			
		}




		this->updateCameraVectors();
	}

	// Processes input received from a mouse input system. Expects the offset value in both the x and y direction.
	void ProcessMouseMovement(GLfloat xoffset, GLfloat yoffset, GLboolean constrainPitch = true)
	{
		xoffset *= this->MouseSensitivity;
		yoffset *= this->MouseSensitivity;

		this->Yaw += xoffset;

		//std::cout << this->Position.x+cos(xoffset) << std::endl;
		
		this->Pitch += yoffset;


		// Make sure that when pitch is out of bounds, screen doesn't get flipped
		if (constrainPitch)
		{
			if (this->Pitch > 89.0f)
				this->Pitch = 89.0f;
			if (this->Pitch < -89.0f)
				this->Pitch = -89.0f;
		}

		// Update Front, Right and Up Vectors using the updated Eular angles
		this->updateCameraVectors();
	}

	// Processes input received from a mouse scroll-wheel event. Only requires input on the vertical wheel-axis
	void ProcessMouseScroll(GLfloat yoffset)
	{
		if (this->Zoom >= 1.0f && this->Zoom <= 45.0f)
			this->Zoom -= yoffset;
		if (this->Zoom <= 1.0f)
			this->Zoom = 1.0f;
		if (this->Zoom >= 45.0f)
			this->Zoom = 45.0f;
	}
	
	void ReturnOriginView(glm::vec3 position)
	{
		this->Front = FRONT;
		this->WorldUp = UP;
		this->Yaw = YAW;
		this->Pitch = PITCH;
		this->Zoom = ZOOM;
		this->updateCameraVectors();
	}
private:
	// Calculates the front vector from the Camera's (updated) Eular Angles
	void updateCameraVectors()
	{
		// Calculate the new Front vector
		glm::vec3 front;
		front.x = cos(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		front.y = sin(glm::radians(this->Pitch));
		front.z = sin(glm::radians(this->Yaw)) * cos(glm::radians(this->Pitch));
		this->Front = glm::normalize(front);
		front.y = 0;
		this->TargetFront = glm::normalize(front);
		// Also re-calculate the Right and Up vector
		this->Right = glm::normalize(glm::cross(this->Front, this->WorldUp));  // Normalize the vectors, because their length gets closer to 0 the more you look up or down which results in slower movement.
		this->Up = glm::normalize(glm::cross(this->Right, this->Front));
		this->Position = this->Target - this->Front * 15.0f;
		this->Position.y = this->Target.y+10;
		
		
		std::cout << this->Target.x << " " << this->Target.y << " " << this->Target.z << std::endl;
	}
};