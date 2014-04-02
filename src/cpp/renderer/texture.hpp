#ifndef __TEXTURE_HPP__
#define __TEXTURE_HPP__

#include "renderer.hpp"

namespace renderer
{

namespace texture
{

class DF_TEXTURE
{
public:
    char* data;
    int   x;
    int   y;
    char  bpp;
    GLenum format;

private:
    bool load_tga(const char*);
    bool load_bmp(const char*);
    bool load_jpg(const char*);

public:
    DF_TEXTURE();
    ~DF_TEXTURE();

    DF_TEXTURE(const DF_TEXTURE&);
    DF_TEXTURE& operator =(const DF_TEXTURE&);

    bool load(const char*);
};

struct LIST_TEXT
{
    GLuint id; // texture ID
    char name[255];      // name of texture (key for resource manager)
    int count;           // count of user of the texture
};

} // namespace texture

class TEXTURE
{
private:
    static texture::LIST_TEXT* list;
    static int list_size;

    int pntr;            // list index

private:
    bool is_in_list(const char* name);
    bool delete_from_list(int p);
    int add_to_list(const char* name);

public:
    TEXTURE();
    TEXTURE(int begin_x, int begin_y, int size_x, int size_y, const char* name);
    TEXTURE(const char* name);
    TEXTURE(void*, int x, int y, const char* name);
    ~TEXTURE();

    TEXTURE(const TEXTURE&);
    TEXTURE& operator =(const TEXTURE&);

    void init(const char*, bool mip_maps = true, bool clamp_to_edge = false);
    void init(void*, int x, int y, const char* name, bool mip_maps = true, bool clamp_to_edge = false, GLenum format = GL_RGB);
    void copy_screen(int begin_x, int begin_y, int size_x, int size_y, const char* name);
    void copy_shadow(int begin_x, int begin_y, int size_x, int size_y, const char* name);

    void bind() const {
        if (pntr >= 0 && pntr < list_size) {
            glBindTexture(GL_TEXTURE_2D, list[pntr].id);
        }
    }
};

extern TEXTURE WHITE_NULL;

} // namespace renderer

#endif  // __TEXTURE_HPP__
