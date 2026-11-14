#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>
#include <filesystem>
#include <unordered_map>

class ShaderProgram {

public:

	explicit ShaderProgram(const std::string& basePath);
	~ShaderProgram();

	ShaderProgram(const ShaderProgram&) = delete;
	ShaderProgram& operator=(const ShaderProgram&) = delete;
	ShaderProgram(ShaderProgram&& other) noexcept;
	ShaderProgram& operator=(ShaderProgram&& other) noexcept;
	
	void use()	   const { glUseProgram(m_id); }
	bool isValid() const { return m_id != 0; }

	GLuint id()	const { return m_id; }
	const std::string& path() const { return m_basePath; }
	

	bool reload();
	bool reloadIfChanged();

	// utility functions
	void setBool  (const std::string& name, bool value) const;
	void setInt   (const std::string& name, int value) const;
	void setFloat (const std::string& name, float value) const;
	void setVec2  (const std::string& name, const glm::vec2& value) const;
	void setVec2  (const std::string& name, float x, float y) const;
	void setVec3  (const std::string& name, const glm::vec3& value) const;
	void setVec3  (const std::string& name, float x, float y, float z) const;
	void setVec3Array(const std::string& name, const glm::vec3* values, int count) const;

	void setVec4(const std::string& name, const glm::vec4& value) const;
	void setVec4(const std::string& name, float x, float y, float z, float w) const;
	void setMat2(const std::string& name, const glm::mat2& mat) const;
	void setMat3(const std::string& name, const glm::mat3& mat) const;
	void setMat4(const std::string& name, const glm::mat4& mat) const;

private:

	std::string m_basePath;
	GLuint		m_id = 0;

	std::filesystem::file_time_type m_vertTime {};
	std::filesystem::file_time_type m_fragTime {};

	mutable std::unordered_map<std::string, GLint> m_uniformCache;
	GLint location(const std::string& name) const;
	void updateFileTimes();


	static GLuint build(const std::string& basePath);
};