
#include "font.h"
#include "../render/batch2d.h"

#include <cstdint>
#include <cstdio>
#include <fstream>
#include <iterator>
#include <optional>
#include <vector>

namespace {

	std::optional<std::vector<uint8_t>> readBinaryFile(const std::string& path) {

		std::ifstream file(path, std::ios::binary);
		if (!file) return std::nullopt;

		return std::vector<uint8_t>((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
	}
}

Font::Font(const std::string& file, float pixelHeight) : m_pixelHeight(pixelHeight) {

	const std::string path = "assets/fonts/" + file;
	auto ttf = readBinaryFile(path);

	if (!ttf) {

		std::printf("[font] could not read %s\n", path.c_str());
		return;
	}

	std::vector<uint8_t> bitmap(AtlasSize * AtlasSize);
	int result = stbtt_BakeFontBitmap(
		ttf->data(), 
		0, 
		pixelHeight, 
		bitmap.data(), 
		AtlasSize, 
		AtlasSize, 
		FirstChar, 
		CharCount, 
		m_glyphs.data()
	);

	if (result <= 0) {

		std::printf("[font] %s at %.0f px does not fit a %dx%d atlas\n", path.c_str(), pixelHeight, AtlasSize, AtlasSize);
		return;
	}

	glCreateTextures(GL_TEXTURE_2D, 1, &m_texture);
	glTextureStorage2D(m_texture, 1, GL_R8, AtlasSize, AtlasSize);

	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);   // rows are tightly packed bytes
	glTextureSubImage2D(m_texture, 0, 0, 0, AtlasSize, AtlasSize, GL_RED, GL_UNSIGNED_BYTE, bitmap.data());
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);   // back to the default

	glTextureParameteri(m_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(m_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTextureParameteri(m_texture, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(m_texture, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

	// 4. sample as (1, 1, 1, coverage): white text with coverage as alpha,
	//    so the UI shader's  vColor * texture  gives text in vColor
	const GLint swizzle[4] = { GL_ONE, GL_ONE, GL_ONE, GL_RED };
	glTextureParameteriv(m_texture, GL_TEXTURE_SWIZZLE_RGBA, swizzle);
}

Font::~Font() { if (m_texture) glDeleteTextures(1, &m_texture); }

void Font::drawText(Batch2d& batch, float x, float y, const std::string& text, const glm::vec4& color) const {

	if (!isValid()) return;

	float penX = x, penY = y;
	for (unsigned char c : text) {

		if (c < FirstChar || c >= FirstChar + CharCount) continue;   // skip non-ASCII for now
		stbtt_aligned_quad q;

		stbtt_GetBakedQuad(m_glyphs.data(), AtlasSize, AtlasSize, c - FirstChar, &penX, &penY, &q, 1);
		batch.texturedRect(q.x0, q.y0, q.x1 - q.x0, q.y1 - q.y0, m_texture, { q.s0, q.t0 }, { q.s1, q.t1 }, color);
	}
}

float Font::textWidth(const std::string& text) const {

	float width = 0.0f;
	for (unsigned char c : text) {

		if (c < FirstChar || c >= FirstChar + CharCount) continue;
		width += m_glyphs[c - FirstChar].xadvance;
	}

	return width;
}