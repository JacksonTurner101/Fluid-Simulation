#pragma once

#include <string>
#include "unordered_map"

class Shader {
	public:
		Shader(const std::string& vertexShader, const std::string fragmentShader);
		~Shader();
		void Bind() const;
		void Unbind() const;
		void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);

	private :
		unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
		unsigned int CompileShader(const std::string& source, unsigned int type);
		std::string ReadAndReturnFileString(const std::string& filePath);
		int GetUniformLocation(const std::string& name);
		unsigned int m_Id;
		std::unordered_map<std::string, int> m_UniformLocationCache;
};