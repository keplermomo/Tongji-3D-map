#include "wall.h"
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <cmath>


bool IfBoom(glm::vec3 pos_a, struct Wall wall[], int wall_length,glm::vec3 gardenpos,float radius) {
	bool m = 0, n = 0, p = 0,k=0;
	bool final = 0;
	k = sqrt((gardenpos.x - pos_a.x)*(gardenpos.x - pos_a.x) + (gardenpos.z - pos_a.z)*(gardenpos.z - pos_a.z))<radius;//中心花坛圆形碰撞
	if (k == 1)return 1;
	for (int i = 0; i < wall_length; i++) {
		m = fabs(wall[i].pos.x + wall[i].length / 2 - pos_a.x) < (wall[i].length + 2.5) / 2;
		n = fabs(wall[i].pos.z + wall[i].width / 2 - pos_a.z) < (wall[i].width + 2.5) / 2;
		p = fabs(wall[i].pos.y + wall[i].height / 2 - pos_a.y - 2.5) < (wall[i].height + 5) / 2;
		if (m && n && p) return 1;
	}
	return 0;
}


glm::vec3 RealPosition(glm::vec3 originpos, glm::vec3 newpos, struct Wall wall[], int wall_length, float t,float m,glm::vec3 gardenpos,float radius) {
	glm::vec3 finalpos;
	float i(0);
	if (IfBoom(newpos, wall, wall_length,gardenpos,radius)) {
		for (i = t; i < m; i = i + t) {
			if (IfBoom(newpos + glm::vec3(0, i, 0), wall, wall_length,gardenpos,radius) == 0)
				break;
		}
		if (i < m)
			finalpos = newpos + glm::vec3(0, i, 0);
		else
			finalpos = originpos;
	}
	else
		finalpos = newpos;


	return finalpos;
}

bool falling(glm::vec3 nowpos, struct Wall wall[], int wall_length,float t,glm::vec3 gardenpos,float radius) {
	if (IfBoom(nowpos - glm::vec3(0,t,0), wall, wall_length,gardenpos,radius))
		return 0;
	else
		return 1;
}