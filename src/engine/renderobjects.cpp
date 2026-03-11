#if defined(use_freeglut)
#include <glad/glad.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#else
#include <GLUT/glut.h>
#endif

#include <engine/renderobjects.h>
#include <engine/texture.h>
#include <engine/tools.h>

void GEC::Render::ClearScreen()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void GEC::Render::ClearScreen(float r, float g, float b)
{
    glClearColor(r / 255.0f, g / 255.0f, b / 255.0f, 1.0f);
}

void GEC::Render::SetColor(float r, float g, float b, float a)
{
    glColor4d(r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}
void GEC::Render::SetColor(float r, float g, float b)
{
    SetColor(r, g, b, 255);
}
void GEC::Render::SetColor(float l, float a)
{
    SetColor(l, l, l, a);
}
void GEC::Render::SetColor(float l)
{
    SetColor(l, 255);
}

void GEC::Render::Rect(float x, float y, float width, float height)
{
    Rect(x, y, 0, width, height);
}
void GEC::Render::Rect(float x, float y, float z, float width, float height)
{

    glDisable(GL_TEXTURE_2D);
    float rot = 0;
    glPushMatrix();
    {
        glTranslatef(x, y, z);
        glRotatef(rot, 0, 0, 1);

        glBegin(GL_QUADS);
        {
            glVertex2f(-width / 2, -height / 2);
            glVertex2f(-width / 2, height / 2);
            glVertex2f(width / 2, height / 2);
            glVertex2f(width / 2, -height / 2);
        }
        glEnd();
    }
    glPopMatrix();
}

void GEC::Render::Triangle(float x, float y, Vector2<float, float> first, Vector2<float, float> second, Vector2<float, float> third, float rot)
{
    Triangle(x, y, 0, Vector3<float, float, float>(first.First(), first.Second(), 0), Vector3<float, float, float>(second.First(), second.Second(), 0), Vector3<float, float, float>(third.First(), third.Second(), 0), rot);
}
void GEC::Render::Triangle(float x, float y, float z, Vector3<float, float, float> first, Vector3<float, float, float> second, Vector3<float, float, float> third, float rot)
{

    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    {
        glTranslatef(x, y, z);
        glRotatef(rot, 0, 0, 1);
        glBegin(GL_TRIANGLES);
        {
            glVertex3f(first.First(), first.Second(), first.Third());
            glVertex3f(second.First(), second.Second(), first.Third());
            glVertex3f(third.First(), third.Second(), first.Third());
        }
        glEnd();
    }
    glPopMatrix();
}


void GEC::Render::Line(Vector2<float, float> from, Vector2<float, float> to, float width)
{
    Line(Vector3<float, float, float>(from.First(), from.Second(), 0), Vector3<float, float, float>(to.First(), to.Second(), 0), width);
}
void GEC::Render::Line(Vector3<float, float, float> a, Vector3<float, float, float> b, float width)
{
    glDisable(GL_TEXTURE_2D);
    glPushMatrix();
    {
        glLineWidth(3.0f);

        glBegin(GL_LINES);
        {
            glVertex3f(a.First(), a.Second(), a.Third());
            glVertex3f(b.First(), b.Second(), b.Third());
        }
        glEnd();
    }
    glPopMatrix();
}

void GEC::Render::Arrow(Vector2<float, float> from, Vector2<float, float> to, float lineWidth, float headSize)
{
}
void GEC::Render::Arrow(Vector3<float, float, float> from, Vector3<float, float, float> to, float lineWidth, float headSize)
{
    Line(from, to, lineWidth);

    float angle = GEC::Tools::AngleBetween(
        Vector2<float, float>(from.First(), from.Second()),
        Vector2<float, float>(to.First(), to.Second()));
    angle = 90 + GEC::Tools::RadiansToDegress(angle);

    glPushMatrix();
    {
        GEC::Render::Triangle(to.First(), to.Second(),
            GEC::Vector2<float, float>(-headSize / 2, headSize / 2),
            GEC::Vector2<float, float>(0, -headSize / 2),
            GEC::Vector2<float, float>(headSize / 2, headSize / 2), angle);
    }
    glPopMatrix();
}

void GEC::Render::Image(const char* texid, float x, float y, float width, float height)
{
    Image(texid, x, y, 0, width, height);
}
void GEC::Render::Image(const char* texid, float x, float y, float z, float width, float height)
{

    Texture* tex = TextureEngine::GetInstance().GetTexture(texid);
    if (tex == nullptr) {
        tex = TextureEngine::GetInstance().GetTexture("engine::err");
        if (tex == nullptr)
            return;
    }

    tex->Bind();

    glEnable(GL_TEXTURE_2D);
    float rot = 0;
    glPushMatrix();
    {
        glTranslatef(x, y, z);
        glRotatef(rot, 0, 0, 1);

        glBegin(GL_QUADS);
        {
            glTexCoord2f(0, 1);
            glVertex2f(-width / 2, -height / 2);

            glTexCoord2f(0, 0);
            glVertex2f(-width / 2, height / 2);

            glTexCoord2f(1, 0);
            glVertex2f(width / 2, height / 2);

            glTexCoord2f(1, 1);
            glVertex2f(width / 2, -height / 2);
        }
        glEnd();
    }
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}

void GEC::Render::Sprite(const char* texid, float x, float y, float width, float height, GEC::Vector2<int, int> selection, int numOfSpritesInRow)
{
    Sprite(texid, x, y, 0, width, height, selection, numOfSpritesInRow);
}

void GEC::Render::Sprite(const char* texid, float x, float y, float z, float width, float height, GEC::Vector2<int, int> selection, int numOfSpritesInRow)
{
    Texture* tex = TextureEngine::GetInstance().GetTexture(texid);
    if (tex == nullptr) {
        tex = TextureEngine::GetInstance().GetTexture("engine::err");
        if (tex == nullptr)
            return;
    }

    float sheetSize = tex->Width();
    float spritesInRow = numOfSpritesInRow;
    float imageX = selection.First() * (sheetSize / spritesInRow);
    float imageY = selection.Second() * (sheetSize / spritesInRow);

    tex->Bind();

    glEnable(GL_TEXTURE_2D);
    float rot = 0;
    glPushMatrix();
    {
        glTranslatef(x, y, z);
        glRotatef(rot, 0, 0, 1);

        glBegin(GL_QUADS);
        {
            glTexCoord2f((imageX) / sheetSize, (imageY + (sheetSize / spritesInRow)) / sheetSize);
            glVertex2f(-width / 2, -height / 2);

            glTexCoord2f((imageX) / sheetSize, (imageY) / sheetSize);
            glVertex2f(-width / 2, height / 2);

            glTexCoord2f((imageX + (sheetSize / spritesInRow) - 0.1f) / sheetSize, (imageY) / sheetSize);
            glVertex2f(width / 2, height / 2);

            glTexCoord2f((imageX + (sheetSize / spritesInRow) - 0.1f) / sheetSize, (imageY + (sheetSize / spritesInRow)) / sheetSize);
            glVertex2f(width / 2, -height / 2);
        }
        glEnd();
    }
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
}
