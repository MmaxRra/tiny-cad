#pragma once

#include <glad/glad.h>
#include <string>

class Texture {

public:

	explicit Texture(const std::string& file, bool smooth = true);
	~Texture();

	Texture(const Texture&) = delete;
	Texture& operator=(const Texture&) = delete;
	Texture(Texture&& other) noexcept;
	Texture& operator=(Texture&& other) noexcept;

	bool   isValid() const { return m_id != 0; }
	GLuint id()      const { return m_id; }
	int    width()   const { return m_width; }
	int    height()  const { return m_height; }

private:

	GLuint m_id = 0;
	int    m_width = 0;
	int    m_height = 0;
};