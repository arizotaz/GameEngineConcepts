// #############################################################################
// # freetype_safe.h
// #############################################################################
// # Written by Colton Staiduhar
// # Date Created:       03/16/2025
// # Last Modification:  03/16/2025
// #############################################################################
// # A reimplemnetation of a function for the freetype library
// #############################################################################
// # I have to be completely honest, I have no idea how this code works.  It
// # but it does and I fear I shall never touch it again
// #############################################################################


#ifndef FREETYPE_SAFE_H
#define FREETYPE_SAFE_H

#include <ft2build.h>
#include FT_FREETYPE_H

FT_Error Safe_Request_Size(FT_Face face, FT_Size_Request req);

#endif // FREETYPE_SAFE_H