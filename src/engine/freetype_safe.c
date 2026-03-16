#include <engine/freetype_safe.h>

#include <ft2build.h>
#include FT_FREETYPE_H

FT_Error Safe_Request_Size(FT_Face face,
                           FT_F26Dot6 char_width,
                           FT_F26Dot6 char_height,
                           FT_UInt horz_resolution,
                           FT_UInt vert_resolution)
{
    if (!face)
        return FT_Err_Invalid_Face_Handle;  // public API error code

    /* If the face has no size object, create one */
    if (!face->size)
    {
        FT_Error error = FT_New_Size(face, &face->size);
        if (error)
            return error;
    }

    /* Handle zero width/height */
    if (char_width == 0)
        char_width = char_height;
    else if (char_height == 0)
        char_height = char_width;

    /* Use the public API to set the character size */
    return FT_Set_Char_Size(face, char_width, char_height,
                            horz_resolution, vert_resolution);
}