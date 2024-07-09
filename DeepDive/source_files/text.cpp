#include "../lib.h"
#include "../header_files/text.h"
#include <ft2build.h>
#include <iostream>
using namespace std;
#include FT_FREETYPE_H

struct Character {
	unsigned int textureID; // ID handle of the glyph texture
	ivec2 size;				// Size of glyph
	ivec2 bearing;			// Offset from baseline to left/top of glyph
	unsigned int advance;   // Horizontal offset to advance to next glyph
};

map<GLchar, Character> characters;

void initFreetype() {
	FT_Library ft;
	if (FT_Init_FreeType(&ft)) exit(-1);

	FT_Face face;
	if (FT_New_Face(ft, "fonts/arial.ttf", 0, &face)) exit(-1);
	else {
		// Set size to load glyphs as
		FT_Set_Pixel_Sizes(face, 0, 48);

		// Disable byte-alignment restriction
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		// Load first 128 characters of ASCII set
		for (unsigned char c = 0; c < 128; c++) {
			// Load character glyph 
			if (FT_Load_Char(face, c, FT_LOAD_RENDER)) continue;
			// Generate texture
			unsigned int texture;
			glGenTextures(1, &texture);
			glBindTexture(GL_TEXTURE_2D, texture);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
			);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Store character for later use
			Character character = {
				texture,
				glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				static_cast<unsigned int>(face->glyph->advance.x)
			};
			characters.insert(pair<char, Character>(c, character));
		}
		glBindTexture(GL_TEXTURE_2D, 0);
	}
	// Destroy FreeType once we're finished
	FT_Done_Face(face);
	FT_Done_FreeType(ft);
}

void renderText(int programId_text, mat4 projectionMatrix_text, string text, unsigned int textVAO, unsigned int textVBO, float x, float y, float scale, vec3 color) {
	// Activate corresponding render state
	glUseProgram(programId_text);
	glUniform3f(glGetUniformLocation(programId_text, "textColor"), color.x, color.y, color.z);
	glUniformMatrix4fv(glGetUniformLocation(programId_text, "Projection_text"), 1, GL_FALSE, value_ptr(projectionMatrix_text));

	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(textVAO);

	// iterate through all characters
	string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++) {
		Character ch = characters[*c];

		float xpos = x + ch.bearing.x * scale;
		float ypos = y - (ch.size.y - ch.bearing.y) * scale;

		float w = ch.size.x * scale;
		float h = ch.size.y * scale;
		// Update VBO for each character
		float vertices[6][4] = {
			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos,     ypos,       0.0f, 1.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },

			{ xpos,     ypos + h,   0.0f, 0.0f },
			{ xpos + w, ypos,       1.0f, 1.0f },
			{ xpos + w, ypos + h,   1.0f, 0.0f }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, textVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Advance cursors for next glyph (note that advance is number of 1/64 pixels)
		x += (ch.advance >> 6) * scale; // Bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}
