	#pragma once
#include <glm/glm.hpp>
struct Wall
{
	glm::vec3 pos;
	float length;
	float width;
	float height;

	Wall(glm::vec3 Pos= glm::vec3(0, 0, 0), float l = 0, float w = 0, float h = 0)
	{
		pos = Pos;
		length = l;
		width = w;
		height = h;
	}
};
bool falling(glm::vec3 nowpos, struct Wall wall[], int wall_length, float t, glm::vec3 gardenpos, float radius);
bool IfBoom(glm::vec3 pos_a, struct Wall wall[], int wall_length, glm::vec3 gardenpos, float radius);
glm::vec3 RealPosition(glm::vec3 originpos, glm::vec3 newpos, struct Wall wall[], int wall_length, float t, float m, glm::vec3 gardenpos, float radius);