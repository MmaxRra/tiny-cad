
#include "texture.h"
#include <stb_image.h>
#include <cstdio>
#include <utility>

Texture::Texture(const std::string& file, bool smooth) {

	const std::string path = "assets/" + file;
	int channels = 0;
	
	unsigned char* pixels = stbi_load(path.c_str(), &m_width, &m_height, &channels, 4);
	
	if (!pixels) {
	
		std::printf("[texture] could not load %s: %s\n", path.c_str(), stbi_failure_reason());
		m_width = m_height = 0;
		return;
	}
	glCreateTextures(GL_TEXTURE_2D, 1, &m_id);
	glTextureStorage2D(m_id, 1, GL_RGBA8, m_width, m_height);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // safe for any width
	glTextureSubImage2D(m_id, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, pixels);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

	const GLint filter = smooth ? GL_LINEAR : GL_NEAREST;
	glTextureParameteri(m_id, GL_TEXTURE_MIN_FILTER, filter);
	glTextureParameteri(m_id, GL_TEXTURE_MAG_FILTER, filter);
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_id, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// 3. the GPU has its own copy now
	stbi_image_free(pixels);
}

Texture::~Texture() { if (m_id) glDeleteTextures(1, &m_id); }

Texture::Texture(Texture&& other) noexcept
	: m_id(std::exchange(other.m_id, 0)),
	m_width(other.m_width),
	m_height(other.m_height) {}

Texture& Texture::operator=(Texture&& other) noexcept {
	
	if (this != &other) {
		
		if (m_id) glDeleteTextures(1, &m_id);
		
		m_id = std::exchange(other.m_id, 0);
		m_width = other.m_width;
		m_height = other.m_height;
	}

	return *this;
}