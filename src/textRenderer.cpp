#include <sstream>
#include <iostream>
#include <fontconfig/fontconfig.h>

#include "textRenderer.hpp"
#include "ftErrorToString.hpp"

TextRenderer::TextRenderer(const std::string &fontHint, const unsigned int pixelWidthHint, const unsigned int pixelHeightHint) : shader("shaders/text.vert", "shaders/text.frag")
{
    FcPattern *pat = FcNameParse((const FcChar8*)fontHint.c_str());
    FcBool success = FcConfigSubstitute(NULL, pat, FcMatchPattern);
    if (success == FcFalse) {
        FcPatternDestroy(pat);
        FcFini();
        std::ostringstream errStream;
        errStream << "FcConfigSubstitute failed for string" << fontHint;
        throw std::runtime_error(errStream.str());
    }
    FcDefaultSubstitute(pat);
    FcResult result;
    FcPattern* font = FcFontMatch(NULL, pat, &result);
    std::string fontFileName;

    if (font) {
        FcChar8* file = nullptr;
        if (FcPatternGetString(font, FC_FILE, 0, &file) == FcResultMatch && file != nullptr) {
            fontFileName = std::string(reinterpret_cast<const char*>(file));
        }
    }
    // Uninitialize fontConfig, then check for succes,
    // this prevents having to write the same code twice.
    FcPatternDestroy(font);
    FcPatternDestroy(pat);
    FcFini();

    if (fontFileName.size() == 0) {
        std::ostringstream errStream;
        errStream << "Deriving a font path from font hint " << fontHint << " failed!";
        throw std::runtime_error(errStream.str());
    }

    // Initialize freeType
    FT_Library ft;
    FT_Error ftErr = FT_Init_FreeType(&ft);
    if (ftErr) {
        std::ostringstream errStream;
        errStream << "FT_Init_FreeType failed. Error: " << ftstrerror(ftErr);
        throw std::runtime_error(errStream.str());
    }
    // Load the font face
    FT_Face face;
    ftErr = FT_New_Face(ft, fontFileName.c_str(), 0, &face);
    if (ftErr) {
        FT_Done_FreeType(ft);
        std::ostringstream errStream;
        errStream << "FT_New_Face failed. Error: " << ftstrerror(ftErr) << " font name: " << fontFileName;
        throw std::runtime_error(errStream.str());
    }
    // Set the pixel size
    ftErr = FT_Set_Pixel_Sizes(face, pixelWidthHint, pixelHeightHint);
    if (ftErr) {
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
        std::ostringstream errStream;
        errStream << "FT_Set_Pixel_Sizes failed. Error: " << ftstrerror(ftErr) << " width hint: " << pixelWidthHint << " height hint: " << pixelHeightHint;
        throw std::runtime_error(errStream.str());
    }
    // Disable byte-alignment restriction
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    // Generate the textures
    glGenTextures(textures.size(), textures.data());
    for (std::size_t c = 0; c < textures.size(); ++c) {
        // Load the character glyph
        ftErr = FT_Load_Char(face, c, FT_LOAD_RENDER);
        if (ftErr) {
            glDeleteTextures(textures.size(), textures.data());
            FT_Done_Face(face);
            FT_Done_FreeType(ft);
            std::ostringstream errStream;
            errStream << "FT_Load_Char failed on char " << c;
            throw std::runtime_error(errStream.str());
        }
        glBindTexture(GL_TEXTURE_2D, textures[c]);
        glTexImage2D(GL_TEXTURE_2D,
                     0,
                     GL_RED,
                     face->glyph->bitmap.width,
                     face->glyph->bitmap.rows,
                     0,
                     GL_RED,
                     GL_UNSIGNED_BYTE,
                     face->glyph->bitmap.buffer);
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // Store the data
        characters[c].size.width = face->glyph->bitmap.width;
        characters[c].size.rows = face->glyph->bitmap.rows;
        characters[c].bearing.left = face->glyph->bitmap_left;
        characters[c].bearing.top = face->glyph->bitmap_top;
        characters[c].advance = face->glyph->advance.x;
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // Create the VBO which will be used to render the text.
    glGenVertexArrays(1, &textVAO);
    glGenBuffers(1, &textVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    // We create a buffer of this size, which is enough for a square with texture coordinates.
    // Remember that a square is two triangles, which explains the six. The triangles have a x,y coordinate and
    // a x, y texture coordinate.
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    // How the data should be interpret, so each vertex has 4 elements, no stride.
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    // Unbind them all
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

TextRenderer::~TextRenderer(void)
{
    glDeleteTextures(textures.size(), textures.data());
    glDeleteVertexArrays(1, &textVAO);
    glDeleteBuffers(1, &textVBO);
}

void TextRenderer::renderText(const std::string &text, float x, float y, float scale, const Vector3& color, const ProjectionMatrix& mat) const
{
        this->shader.use();
        shader.setUniformMatrix4v("projection", 1, true, mat.data());
        shader.setUniform3f("textColor", color);
        glActiveTexture(GL_TEXTURE0);
        glBindVertexArray(textVAO);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        // render
        for (const char& c : text) {
            std::size_t chIndex = c;
            if (chIndex > characters.size()){
                chIndex = '?';
            }
            struct Character ch = characters[chIndex];
            float xpos = x + ch.bearing.left * scale;
            float ypos = y - (ch.size.rows - ch.bearing.top) * scale;

            float w = ch.size.width * scale;
            float h = ch.size.rows * scale;
            // update VBO for each character
            float vertices[6][4] = {
                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos,     ypos,       0.0f, 1.0f },
                { xpos + w, ypos,       1.0f, 1.0f },

                { xpos,     ypos + h,   0.0f, 0.0f },
                { xpos + w, ypos,       1.0f, 1.0f },
                { xpos + w, ypos + h,   1.0f, 0.0f }
            };
            // render glyph texture over quad
            glBindTexture(GL_TEXTURE_2D, textures[chIndex]);
            // update content of VBO memory
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
            // render quad
            glDrawArrays(GL_TRIANGLES, 0, 6);
            // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
            x += (ch.advance / 64) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
        }
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
}
