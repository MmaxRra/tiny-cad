#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <array>
#include <string>

#include <stb_truetype.h>

class Batch2d;
class Font {

public:

	Font(const std::string& file, float pixelHeight);
	~Font();

	Font(const Font&) = delete;
	Font& operator=(const Font&) = delete;

	bool isValid() const { return m_texture != 0; }

	void drawText(Batch2d& batch, float x, float y, const std::string& text, const glm::vec4& color) const;
	float textWidth(const std::string& text) const;    // width in pixels, for centering
	float size() const { return m_pixelHeight; }

private:

	static constexpr int AtlasSize = 512;
	static constexpr int FirstChar = 32;    // space
	static constexpr int CharCount = 95;    // up to '~'

	std::array<stbtt_bakedchar, CharCount> m_glyphs{};
	
	GLuint m_texture = 0;
	float  m_pixelHeight = 0.0f;
};