// #############################################################################
// # texture.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       02/05/2025
// # Last Modification:  02/13/2025
// #############################################################################
// # This file serves as simple, yet functional example of how to use the
// # include directory of this project
// #############################################################################
// # This header exposses the LoadTexture function of this project.
// # Usage:
// #     unsigned int textureID = LoadTexture(fileURL);
// #
// #     Where textureID is the returned OpenGL texture ID that can be bounded
// #
// #############################################################################

#ifndef TEXTURES_H
#define TEXTRUES_H 1

#include <iostream>
#include <map>

namespace GEC {

/**
 * A simple container for OpenGL Textures
 */
class Texture {
public:
    /**
     * Declare Texture with it's file location
     */
    Texture(char* location);

    /** Loads the texture into openGL */
    void Load();

    /** Destroys the generated glTexture */
    void Destroy();

    /** Bind the texture to the current context */
    void Bind() const;

    /** Return the file location of the texture */
    char* GetLocation() const;
    /** Return the OpenGl texture ID */
    unsigned int ID() const;

    /** Returns the width of the image in pixels */
    int Width() const;

    /** Returns the height of the image in pixels */
    int Height() const;

    ~Texture();

private:
    unsigned int id;
    char* location;
    int width, height, bpp;
    std::string loc_s;
};

/**
 * Texture Engine
 * Stores, Loads, Destroys, and Manages different openGL textures
 */
class TextureEngine {
public:
    /**
     * Get the TextureEngine singleton object
     */
    static TextureEngine& GetInstance()
    {
        static TextureEngine engine;
        return engine;
    }
    TextureEngine(const TextureEngine&) = delete;
    TextureEngine& operator=(const TextureEngine&) = delete;

    /** Register a texture as an ID
     * @param id - ID to store the texture as
     * @param texture - Texture to Register
     */
    void AddTexture(const char* id, Texture* tex);

    /** Create a texture object and register it as ID
     * @param id - ID to store the texture as
     * @param file_location - location of the texture to load
     */
    void LoadTexture(const char* id, char* file_location);

    /** Returns the texture object at ID
     * @param id - Texture ID
     * @return Texture* - texture object or nullptr
     */
    Texture* GetTexture(const char* id);

    /** Remove Texture from Engine and Destroy it's openGL texture
     * @param id - Texture id
     */
    void RemoveTexture(const char* id);

    /** Returns true if the textures has been added ot hte registry
     * @param id - Texture ID
     * @return bool - Is registered
     */
    bool Exists(const char* id);

private:
    TextureEngine();
    ~TextureEngine();

    std::map<std::string, Texture*> textures;
};

/**
 * Loads a texture for use in OpenGL with STB
 *
 * Usage:
 *      unsigned int textureID = LoadTexture(fileURL)
 *
 * textureID - the generated OpenGL Texture ID
 *
 * Bind texture with
 * glBindTexture(GL_TEXTURE_2D, textureID);

 */
unsigned int LoadTexture(char* location);

}

#endif