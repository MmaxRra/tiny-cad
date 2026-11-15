// shader helpers
#include "shader.h"
#include <optional>

#include <cstdio>
#include <fstream>
#include <sstream>

#include <system_error>
#include <utility>

namespace fs = std::filesystem;

// helpers
namespace {
std::optional<std::string> readFile(const std::string& path) {

	std::ifstream file(path, std::ios::binary);
	if (!file) return std::nullopt;

	std::ostringstream ss;
	ss << file.rdbuf();
	return ss.str();
}

fs::file_time_type fileTime(const std::string& path) {

	std::error_code ec;
	auto t = fs::last_write_time(path, ec);
	return ec ? fs::file_time_type{} : t;
}

GLuint compileStage(GLenum type, const std::string& source, const std::string& path) {

	GLuint shader = glCreateShader(type);
	const char* src = source.c_str();

	glShaderSource(shader, 1, &src, nullptr);
	glCompileShader(shader);

	GLint ok = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);

	if (!ok) {

		char log[1024];
		glGetShaderInfoLog(shader, sizeof(log), nullptr, log);
		std::printf("[shader] compile error in %s:\n%s\n", path.c_str(), log);

		glDeleteShader(shader);
		return 0;
	}

	return shader;
}

// namespace
} 

GLuint ShaderProgram::build(const std::string& basePath) {

	const std::string vertPath = basePath + ".vert";
	const std::string fragPath = basePath + ".frag";

	// get shaders
	auto vertSrc = readFile(vertPath);
	auto fragSrc = readFile(fragPath);

	if (!vertSrc) { std::printf("[shader] could not read %s\n", vertPath.c_str()); return 0; }
	if (!fragSrc) { std::printf("[shader] could not read %s\n", fragPath.c_str()); return 0; }

	// compile shaders
	GLuint vs = compileStage(GL_VERTEX_SHADER, *vertSrc, vertPath);
	if (!vs) return 0;

	GLuint fs = compileStage(GL_FRAGMENT_SHADER, *fragSrc, fragPath);
	if (!fs) { glDeleteShader(vs); return 0; }

	// attach & link shaders
	GLuint program = glCreateProgram();
	glAttachShader(program, vs);
	glAttachShader(program, fs);
	glLinkProgram(program);

	// clean shaders
	glDeleteShader(vs);
	glDeleteShader(fs);

	GLint ok = 0;
	glGetProgramiv(program, GL_LINK_STATUS, &ok);
	
	if (!ok) {
		char log[1024];
		glGetProgramInfoLog(program, sizeof(log), nullptr, log);
		std::printf("[shader] link error in %s:\n%s\n", basePath.c_str(), log);
		glDeleteProgram(program);
		return 0;
	}

	return program;
}

ShaderProgram::ShaderProgram(const std::string& basePath) : m_basePath(basePath) 
{	
	m_id = build(m_basePath);
	updateFileTimes();
}

ShaderProgram::~ShaderProgram() { if (m_id) glDeleteProgram(m_id); }
ShaderProgram::ShaderProgram(ShaderProgram&& other) noexcept
	: m_basePath(std::move(other.m_basePath)),
	m_id(std::exchange(other.m_id, 0)),
	m_vertTime(other.m_vertTime),
	m_fragTime(other.m_fragTime),
	m_uniformCache(std::move(other.m_uniformCache))
{}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& other) noexcept
{
	if (this != &other) {
		if (m_id) glDeleteProgram(m_id);
		m_basePath = std::move(other.m_basePath);
		m_id = std::exchange(other.m_id, 0);
		m_vertTime = other.m_vertTime;
		m_fragTime = other.m_fragTime;
		m_uniformCache = std::move(other.m_uniformCache);
	}
	return *this;
}

void ShaderProgram::updateFileTimes()
{
	m_vertTime = fileTime(m_basePath + ".vert");
	m_fragTime = fileTime(m_basePath + ".frag");
}

bool ShaderProgram::reload()
{
	GLuint fresh = build(m_basePath);
	
	if (!fresh) {
		std::printf("[shader] keeping previous version of %s\n", m_basePath.c_str());
		return false;
	}

	if (m_id) glDeleteProgram(m_id);
	m_id = fresh;
	
	m_uniformCache.clear();
	std::printf("[shader] reloaded %s\n", m_basePath.c_str());
	return true;
}

bool ShaderProgram::reloadIfChanged()
{
	auto vt = fileTime(m_basePath + ".vert");
	auto ft = fileTime(m_basePath + ".frag");
	if (vt == m_vertTime && ft == m_fragTime) return false;

	// Update the times even if the reload fails, so a broken shader
	// isn't retried (and its error re-printed) every check.
	m_vertTime = vt; m_fragTime = ft;
	return reload();
}

GLint ShaderProgram::location(const std::string& name) const
{
	auto it = m_uniformCache.find(name);
	if (it != m_uniformCache.end()) return it->second;

	GLint loc = glGetUniformLocation(m_id, name.c_str());
	m_uniformCache[name] = loc;
	return loc;
}


// utility uniform functions (DSA: no use() needed before setting)
// ------------------------------------------------------------------------

void ShaderProgram::setBool(const std::string& name, bool value) const
{ glProgramUniform1i(m_id, location(name), (int)value); }

void ShaderProgram::setInt(const std::string& name, int value) const
{ glProgramUniform1i(m_id, location(name), value); }

void ShaderProgram::setFloat(const std::string& name, float value) const
{ glProgramUniform1f(m_id, location(name), value); }

// ------------------------------------------------------------------------

void ShaderProgram::setVec2(const std::string& name, const glm::vec2& value) const
{ glProgramUniform2fv(m_id, location(name), 1, &value[0]); }

void ShaderProgram::setVec2(const std::string& name, float x, float y) const
{ glProgramUniform2f(m_id, location(name), x, y); }

// ------------------------------------------------------------------------

void ShaderProgram::setVec3(const std::string& name, const glm::vec3& value) const
{ glProgramUniform3fv(m_id, location(name), 1, &value[0]); }

void ShaderProgram::setVec3(const std::string& name, float x, float y, float z) const
{ glProgramUniform3f(m_id, location(name), x, y, z); }

void ShaderProgram::setVec3Array(const std::string& name, const glm::vec3* values, int count) const
{ glProgramUniform3fv(m_id, location(name), count, &values[0][0]); }

// ------------------------------------------------------------------------

void ShaderProgram::setVec4(const std::string& name, const glm::vec4& value) const
{ glProgramUniform4fv(m_id, location(name), 1, &value[0]); }

void ShaderProgram::setVec4(const std::string& name, float x, float y, float z, float w) const
{ glProgramUniform4f(m_id, location(name), x, y, z, w); }

// ------------------------------------------------------------------------

void ShaderProgram::setMat2(const std::string& name, const glm::mat2& mat) const
{ glProgramUniformMatrix2fv(m_id, location(name), 1, GL_FALSE, &mat[0][0]); }

void ShaderProgram::setMat3(const std::string& name, const glm::mat3& mat) const
{ glProgramUniformMatrix3fv(m_id, location(name), 1, GL_FALSE, &mat[0][0]); }

void ShaderProgram::setMat4(const std::string& name, const glm::mat4& mat) const
{ glProgramUniformMatrix4fv(m_id, location(name), 1, GL_FALSE, &mat[0][0]); }