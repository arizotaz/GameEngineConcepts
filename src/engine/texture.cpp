// #############################################################################
// # texture.cpp
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       02/05/2026
// # Last Modification:  02/05/2026
// #############################################################################
// # Super simple OpenGL texture loading using the stb_image.h library
// #############################################################################
// # This file provides sources of the functions listed in Texture.h
// #############################################################################

// Include STB
#if defined(use_freeglut)
#include <glad/glad.h>
#define FREEGLUT_STATIC
#include <GL/freeglut.h>
#else
#include <GLUT/glut.h>
#endif
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include <iostream>
#include <vector>

// Header file for functions defined here
#include <engine/texture.h>

namespace GEC {

Texture::Texture(char* location)
{
    this->location = location;
}
char* Texture::GetLocation() const
{
    return this->location;
}
void Texture::Load()
{
    char* loc = this->location;
    loc_s = location;

    // Load the image and save raw pixel data as unsigned bytes
    unsigned char* pixels = stbi_load(location, &width, &height, &bpp, 4);

    // Declare our texture, this will be filled with the texture ID
    unsigned int texture;

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    // Tell OpenGL to generate a texture slot and bind it to the current texture
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);

    // These are option values, but they are nice to set
    {
        // Set OpenGL texture Wrapping
        // I prefer CLAMP_TO_EDGE but GL_REPEAT works too, look up the different OpenGL Texture Wrap Types
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

        // Set the type of Texture Filtering
        // "GL_LINEAR" is default, I like GL_NEAREST.  Again, look up all the options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    }

    // If the loaded pixel data is value
    if (pixels) {
        // Load the texture data into the currently bound texture slot
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

        // Generate the mipmap cause why not
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // Frees the image data stored in ram from stb
    stbi_image_free(pixels);

    // Return the texture ID
    this->id = texture;

    std::cout << "[Texture] Loaded glTexture " << id << " from location \"" << location << "\'" << std::endl;
}
void Texture::Destroy()
{
    const GLuint tid = this->id;
    glDeleteTextures(1, &tid);
    std::cout << "[Texture] Destroyed glTexture " << id << std::endl;
}
void Texture::Bind() const
{
    GLuint id = ID();
    glBindTexture(GL_TEXTURE_2D, id);
}
unsigned int Texture::ID() const
{
    return this->id;
}
int Texture::Width() const
{
    return this->width;
}
int Texture::Height() const
{
    return this->height;
}
Texture::~Texture()
{
    this->Destroy();
}

TextureEngine::TextureEngine()
{
    LoadTexture("engine::err", RESOURCES_PATH "error.png");
}
void TextureEngine::AddTexture(const char* id, Texture* tex)
{
    this->textures.insert({ id, tex });
}
void TextureEngine::LoadTexture(const char* id, char* file_location)
{
    Texture* t = new Texture(file_location);
    t->Load();
    std::cout << "[TextureEngine] Registering texture \"" << id << "\", as alias for glTexture " << t->ID() << std::endl;
    AddTexture(id, t);
}
Texture* TextureEngine::GetTexture(const char* id)
{
    return textures[id];
}
bool TextureEngine::Exists(const char* id)
{
    return textures.find(id) != textures.end();
}
void TextureEngine::RemoveTexture(const char* id)
{
    Texture* t = GetTexture(id);
    if (t) {
        std::cout << "[TextureEngine] Removing texture \"" << id << "\", alias for glTexture " << t->ID() << std::endl;
        t->Destroy();
        delete t;
        textures.erase(id);
    }
}
TextureEngine::~TextureEngine()
{
    std::vector<std::string> keys;
    for (const auto& pair : textures) {
        keys.push_back(pair.first);
    }

    // Step 2: loop through keys and destroy textures
    for (const auto& id : keys)
        RemoveTexture(id.c_str());
}

}