#include <sstream>
#include <iostream>
#include <fontconfig/fontconfig.h>

#include "textRenderer.hpp"
#include "ftErrorToString.hpp"

std::vector<std::string> TextRenderer::splitString(const std::string& str, float maxLineLenPix, const float scale) const
{
    std::vector<std::string> result;
    std::stringstream ss(str);
    for (std::string line; std::getline(ss, line, '\n'); ) {
        if (maxLineLenPix == 0) {
            result.push_back(line);
        } else {
            std::string::size_type lastSplitPos = 0;
            std::string::size_type lastSpacePos = 0;
            while (lastSplitPos < line.length()) {
                std::string::size_type pos = lastSplitPos;
                float pixelCount = 0;
                while (pos < line.length() && pixelCount < maxLineLenPix) {
                    if (line[pos] == ' ')
                        lastSpacePos = pos;
                    pixelCount += (this->characters[line[pos]].advance / 64) * scale;
                    pos++;
                }
                if (pixelCount < maxLineLenPix) {
                    result.push_back(line.substr(lastSplitPos, pos - lastSplitPos));
                    lastSplitPos = pos;
                } else {
                    std::string::size_type endPos = lastSpacePos;
                    if (endPos == lastSplitPos) {
                        if (pos != lastSplitPos + 1) {
                            pos -= 1;
                        }
                        endPos = pos;
                    }
                    std::string s = line.substr(lastSplitPos, endPos - lastSplitPos);
                    // Now remove trailing and leading spaces
                    std::size_t begin = s.find_first_not_of(' ');
                    std::size_t end = s.find_last_not_of(' ');
                    result.push_back(s.substr(begin, end - begin + 1));
                    lastSplitPos = endPos;
                    lastSpacePos = endPos;
                }
            }
        }
    }
    return result;
}

float TextRenderer::getLineLengthPixels(const std::string& line, const float scale) const
{
    float len = 0;
    for (const char& c : line) {
        len += characters[c].advance;
    }
    len /= 64;
    return len * scale;

}

void TextRenderer::renderTextLine(const std::string& line, float x, float y, const float scale) const
{
    for (const char& c : line) {
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
}

TextRenderer::TextRenderer(const std::string &fontHint, const unsigned int pixelWidthHint, const unsigned int pixelHeightHint) :
    textShader("shaders/text.vert", "shaders/text.frag"),
    backgroundShader("shaders/textBackground.vert", "shaders/textBackground.frag")
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
    this->lineSpacing64thsPixel = static_cast<float>(face->size->metrics.height);
    this->descender64thsPixel = static_cast<float>(face->size->metrics.descender);
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
        characters[c].advance = static_cast<float>(face->glyph->advance.x);
    }
    FT_Done_Face(face);
    FT_Done_FreeType(ft);

    // Create the VBO which will be used to render the text.
    glGenVertexArrays(1, &textVAO);
    glGenVertexArrays(1, &bgVAO);
    glGenBuffers(1, &textVBO);
    glGenBuffers(1, &bgVBO);
    glBindVertexArray(textVAO);
    glBindBuffer(GL_ARRAY_BUFFER, textVBO);
    // We create a buffer of this size, which is enough for a square with texture coordinates.
    // Remember that a square is two triangles, which explains the six. The triangles have a x,y coordinate and
    // a x, y texture coordinate, which explains the four.
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
    // How the data should be interpret, so each vertex has 4 elements, no stride.
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
    glEnableVertexAttribArray(0);
    // Unbind them all
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Now for the background
    glBindVertexArray(bgVAO);
    glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
    // This is a simple, 2D square. Which is two triangles, which means 6 sets of 2 coordinates.
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 2, NULL, GL_DYNAMIC_DRAW);
    // How the data should be interpret, so each vertex has 4 elements, no stride.
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), 0);
    glEnableVertexAttribArray(0);

    // Unbind
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

TextRenderer::~TextRenderer(void)
{
    glDeleteTextures(textures.size(), textures.data());
    glDeleteVertexArrays(1, &textVAO);
    glDeleteVertexArrays(1, &bgVAO);
    glDeleteBuffers(1, &textVBO);
    glDeleteBuffers(1, &bgVBO);
}

void TextRenderer::renderBackground(const float y, const float x, const float height, const float length) const
{
    float vertices[] = {
        // First triangle
        x + length, y + height, // Top right
        x + length, y,          // bottom right
        x,          y + height, // Top left
        // Second triangle
        x + length, y,          // Bottom right
        x,          y,          // Bottom left
        x,          y + height, // Top left
    };
    (void)vertices;
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void TextRenderer::renderText(const ProjectionMatrix& mat, const std::string &text, float x, float y, const float scale,
                              const float maxLineLenPix,
                              const VerticalAlignment vAlign,
                              const HorizontalAlignment hAlign,
                              const Vector3& textColor,
                              const bool addBackgroundColor, const Vector3& backgroundColor) const
{
    // Prepare the text shader
    this->textShader.use();
    textShader.setUniformMatrix4v("projection", 1, true, mat.data());
    textShader.setUniform3f("textColor", textColor);
    glActiveTexture(GL_TEXTURE0);
    // Optionally prepare the background shader
    if (addBackgroundColor) {
        this->backgroundShader.use();
        backgroundShader.setUniformMatrix4v("projection", 1, true, mat.data());
        backgroundShader.setUniform3f("backgroundColor", backgroundColor);
    }

    // render
    std::vector<std::string> strList = splitString(text, maxLineLenPix, scale);
    // Process vAlign
    // Top-left is ymax. The y-coordinate passed to renderTextLine represents the absolute bottom on which the glyph is drawn.
    // So, to achieve TextRenderer::VerticalAlignment::Top, we subtract the box height plus the descender.
    // The descender is a negative number and represents the maximum amount any glyph will go below the given y line.
    float lineHeight = (this->lineSpacing64thsPixel / 64) * scale;
    float descender = (this->descender64thsPixel / 64) * scale;
    y -= lineHeight + descender;
    if (vAlign == TextRenderer::VerticalAlignment::center) {
        y += (lineHeight * strList.size()) / 2;
    } else if (vAlign == TextRenderer::VerticalAlignment::bottom) {
        y += (lineHeight * strList.size());
    }
    for (const std::string &s : strList) {
        const float lineLen = getLineLengthPixels(s, scale);
        float actX = x;
        if (hAlign == TextRenderer::HorizontalAlignment::right) {
            actX -= lineLen;
        } else if (hAlign == TextRenderer::HorizontalAlignment::center) {
            actX -= lineLen / 2;
        }
        if (addBackgroundColor) {
            this->backgroundShader.use();
            glBindVertexArray(bgVAO);
            glBindBuffer(GL_ARRAY_BUFFER, bgVBO);
            renderBackground(y + descender, actX, lineHeight, lineLen);
        }
        this->textShader.use();
        glBindVertexArray(textVAO);
        glBindBuffer(GL_ARRAY_BUFFER, textVBO);
        renderTextLine(s, actX, y, scale);
        y -= lineHeight;
    }
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}
