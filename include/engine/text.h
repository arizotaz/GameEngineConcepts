// #############################################################################
// # text.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       03/14/2026
// # Last Modification:  03/17/2026
// #############################################################################
// # Text Rendering and Object
// #############################################################################
// # This header is responsible for creating and renderering text to the screen.
// # It hold the code to load a fond and render text with said font.
// #############################################################################

#ifndef GEC_ENGINE_TEXT_H
#define GEC_ENGINE_TEXT_H 1

#include <engine/structs.h>

#include <engine/freetype_safe.h>

#include <iostream>
#include <map>
#include <string>

#include <glad/glad.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>

namespace GEC {
namespace TextRender {

    struct Character {
        GLuint TextureID;
        int Width;
        int Height;
        int BearingX;
        int BearingY;
        GLuint Advance;
    };

    /** Font Class, used to load a font file and return the texture and locations of each character */
    class Font {
    public:
        static Font* GetDefault()
        {
            static GEC::TextRender::Font* globalFont;
            if (globalFont == nullptr) globalFont = new GEC::TextRender::Font(RESOURCES_PATH "arial.ttf", 48);
            return globalFont;
        }

        /**
         * Constructor for the Font, requires a font file and size to load.
         * @param path - file location of font file
         * @param size - font size
         */
        Font(const std::string& path, unsigned int size)
        {
            // Set the size
            this->size = size;

            // Load Freetype loaded
            FT_Library ft;
            FT_Init_FreeType(&ft);

            // Create the font atlas
            FT_Face face;
            FT_New_Face(ft, path.c_str(), 0, &face);
            FT_Set_Pixel_Sizes(face, 0, size);

            // Unpack the texture atlas
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            // Load each character
            for (unsigned char c = 0; c < 128; c++) {
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                    continue;

                // Load atlas texture
                GLuint texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);

                // Push pixel buffer into texture
                glTexImage2D(
                    GL_TEXTURE_2D,
                    0,
                    GL_ALPHA,
                    face->glyph->bitmap.width,
                    face->glyph->bitmap.rows,
                    0,
                    GL_ALPHA,
                    GL_UNSIGNED_BYTE,
                    face->glyph->bitmap.buffer);

                // Filters and Clamps
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                // Create Character Data
                Character ch = {
                    texture,
                    static_cast<int>(face->glyph->bitmap.width),
                    static_cast<int>(face->glyph->bitmap.rows),
                    face->glyph->bitmap_left,
                    face->glyph->bitmap_top,
                    static_cast<GLuint>(face->glyph->advance.x)
                };

                characters.insert({ c, ch });
            }

            // Freeup freetype
            FT_Done_Face(face);
            FT_Done_FreeType(ft);
        }

        /**
         * Return the character data for a given character
         * @param char - the character to query
         * @return a stucture containing all information about the character and location in the atlas
         */
        const Character& GetCharacter(char c) const { return characters.at(c); }

        /**
         * Return the size of the loaded font
         * @return the size of the loaded font
         */
        const float Size() const { return size; }

    private:
        std::map<char, Character> characters;
        float size = 0;
    };

    /**
     * Renderers Text
     */
    class Text {
    public:

        Text() {
            font = Font::GetDefault();
        }
        
        /**
         * Text Consturctor
         * @param f - the font to use for the text render
         */
        Text(Font* f)
            : font(f)
        {
        }

        /** Gets the size of the rendered text
         * @returns Vector2 where first() is the width and second() is the height
         */
        GEC::Vector2<float, float> GetSize(float scale)
        {
            int width = 0;
            int height = 0;
            int x = 0;
            for (char c : text) {
                const Character& ch = font->GetCharacter(c);

                float xpos = ch.BearingX * scale;
                float ypos = (ch.Height) * scale;

                float w = ch.Width * scale;
                float h = ch.Height * scale;

                if (h > height)
                    height = h;

                x += (ch.Advance >> 6) * scale;

                width = x;
            }

            return GEC::Vector2<float, float>(width, height);
        }

        /**
         * Sets the text string to render
         */
        void SetText(const std::string& str) { text = str; }

        /**
         * Sets the align configuration to render the text
         */
        void Align(int horizontal, int vertical)
        {
            this->v_align = vertical;
            this->h_align = horizontal;
        }

        /**
         * Render the text to the screen
         * @param x - the x position of the text
         * @param y.- the y position of the text
         * @param z - the z position of the text
         * @param fontSize - the height of the characters
         */
        void Render(float x, float y, float z, float fontSize = 1.0f)
        {

            float scale = fontSize / font->Size();

            GEC::Vector2<float, float> s = GetSize(scale);
            if (h_align == 1)
                x -= s.First() / 2;
            if (h_align == 2)
                x -= s.First();
            if (v_align == 1)
                y -= s.Second() / 2;
            if (v_align == 2)
                y -= s.Second();

            glEnable(GL_TEXTURE_2D);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            glPushMatrix();
            {
                glTranslatef(0, 0, z);
                for (char c : text) {
                    const Character& ch = font->GetCharacter(c);

                    float xpos = x + ch.BearingX * scale;
                    float ypos = y + (ch.BearingY) * scale;

                    float w = ch.Width * scale;
                    float h = -ch.Height * scale;

                    glBindTexture(GL_TEXTURE_2D, ch.TextureID);

                    glBegin(GL_TRIANGLES);
                    // First triangle
                    glTexCoord2f(0.0f, 0.0f);
                    glVertex3f(xpos, ypos, 0);
                    glTexCoord2f(1.0f, 0.0f);
                    glVertex3f(xpos + w, ypos, 0);
                    glTexCoord2f(1.0f, 1.0f);
                    glVertex3f(xpos + w, ypos + h, 0);

                    // Second triangle
                    glTexCoord2f(0.0f, 0.0f);
                    glVertex3f(xpos, ypos, 0);
                    glTexCoord2f(1.0f, 1.0f);
                    glVertex3f(xpos + w, ypos + h, 0);
                    glTexCoord2f(0.0f, 1.0f);
                    glVertex3f(xpos, ypos + h, 0);
                    glEnd();

                    x += (ch.Advance >> 6) * scale;
                }
            }
            glPopMatrix();
        }

    private:
        std::string text;
        Font* font;
        float v_align = 0, h_align = 0;
    };

}
}

#endif