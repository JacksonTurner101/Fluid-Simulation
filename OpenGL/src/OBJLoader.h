#pragma once

#include <string>
#include "unordered_map"
#include "glm\glm.hpp"

class OBJLoader {
public:

	OBJLoader();
	~OBJLoader();
	void ReadOBJFile(std::string filePath);
private:

};