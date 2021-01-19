#include <string>
#include <array>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "shaderProgram.hpp"
#include "vector3.hpp"
#include "projectionMatrix.hpp"

/**The TextRenderer uses libFontConfig and libFreeType to load the first 128 characters of the ASCII table into textures.
 * You can then use this object to render text to given coordinates on the screen.
 * This object allocates OpenGL textures, an OpenGL shader and an OpenGL VBO to aid rendering.*/
class TextRenderer {
    private:
        struct Character {
            struct Size {
                unsigned int width;
                unsigned int rows;
            };
            Size size;
            struct Bearing {
                FT_Int left;
                FT_Int top;
            };
            Bearing bearing;
            float advance;
        };

        std::array<struct Character, 128> characters;
        std::array<GLuint, 128> textures;
        GLuint textVBO, textVAO;
        ShaderProgram shader;
        float lineSpacing64thsPixel;

        std::vector<std::string> splitString(const std::string& str, const float maxLineLenPix, const float scale) const;
        void renderTextLine(const std::string& line, float x, float y, const float scale) const;
    public:

        enum class VerticalAlignment {
            top,
            center,
            bottom
        };

        enum class HorizontalAlignment {
            left,
            center,
            right
        };

        /**Default constructor
         * @param fontHint Can be almost anything, is passed to libFontConfig, who will attempt to match it to a font.
         * @param pixelWidthHint Is passed to libFreeType, see FT_Set_Pixel_Sizes.
         * @param pixelHeightHint Is passed to libFreeType, see FT_Set_Pixel_Sizes.
         */
        TextRenderer(const std::string &fontHint = "serif", const unsigned int pixelWidthHint = 0, const unsigned int pixelHeightHint = 48);
        ~TextRenderer(void);

        TextRenderer(const TextRenderer&) = delete;
        TextRenderer& operator=(const TextRenderer&) = delete;

        void renderText(const ProjectionMatrix& mat, const std::string &text, float x, float y, const float scale, const float maxLineLenPix = 0,
                        const VerticalAlignment vAlign = VerticalAlignment::top,
                        const HorizontalAlignment hAlign = HorizontalAlignment::left,
                        const Vector3& textColor = Vector3(1, 1, 1),
                        const bool addBackgroundColor = false, const Vector3& backgroundColor = Vector3(0, 0, 0)) const;
};
