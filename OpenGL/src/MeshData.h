#pragma once

#include "vector"
#include "glm/glm.hpp"

struct MeshData {

	std::vector <glm::vec3> vertices;
	std::vector<glm::vec2> textureCoord;
	std::vector<glm::vec3> normals;
	std::vector<int> indices;
};