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

    class Font {
    public:
        Font(const std::string& path, unsigned int size)
        {
            this->size = size;

            FT_Library ft;
            FT_Init_FreeType(&ft);

            FT_Face face;
            FT_New_Face(ft, path.c_str(), 0, &face);
            FT_Set_Pixel_Sizes(face, 0, size);

            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            for (unsigned char c = 0; c < 128; c++) {
                if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                    continue;

                GLuint texture;
                glGenTextures(1, &texture);
                glBindTexture(GL_TEXTURE_2D, texture);

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

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

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

            FT_Done_Face(face);
            FT_Done_FreeType(ft);
        }

        const Character& GetCharacter(char c) const { return characters.at(c); }

        const float Size() const { return size; }

    private:
        std::map<char, Character> characters;
        float size = 0;
    };

    class TextRenderer {
    public:
        TextRenderer()
        {
            glGenBuffers(1, &VBO);
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, nullptr, GL_DYNAMIC_DRAW);
            glBindBuffer(GL_ARRAY_BUFFER, 0);
        }

        void Begin(GLuint attribPos)
        {
            glBindBuffer(GL_ARRAY_BUFFER, VBO);
            glEnableVertexAttribArray(attribPos);
            glVertexAttribPointer(attribPos, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
        }

        GLuint GetVBO() const { return VBO; }

    private:
        GLuint VBO;
    };

    class Text {
    public:
        Text(Font* f)
            : font(f)
        {
        }

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

        void SetText(const std::string& str) { text = str; }
        void Align(int horizontal, int vertical)
        {
            this->v_align = vertical;
            this->h_align = horizontal;
        }

        void Render(float x, float y, float z, float fontSize = 1.0f)
        {

            float scale = fontSize/font->Size();

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