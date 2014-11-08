/**
 * DarkFlame Copyright (C) 2014 Alexey Shumeiko
 *
 * This file is part of DarkFlame.
 *
 * DarkFlame is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * DarkFlame is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with DarkFlame.  If not, see <http://www.gnu.org/licenses/>.
 *
 * For more details see LICENSE file.
 */

#include <string.h>
#include <string>
#include <fstream>
#include "texture.hpp"
#include "logger.hpp"

using namespace std;
using namespace renderer;
using namespace renderer::texture;

#if !defined (WINDOWS)

#pragma push
#pragma pack(1)

typedef struct tagBITMAPFILEHEADER {
  unsigned short bfType;
  unsigned long  bfSize;
  unsigned short bfReserved1;
  unsigned short bfReserved2;
  unsigned long  bfOffBits;
} BITMAPFILEHEADER, *PBITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
  unsigned long  biSize;
  signed long    biWidth;
  signed long    biHeight;
  unsigned short biPlanes;
  unsigned short biBitCount;
  unsigned long  biCompression;
  unsigned long  biSizeImage;
  signed long    biXPelsPerMeter;
  signed long    biYPelsPerMeter;
  unsigned long  biClrUsed;
  unsigned long  biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;

#pragma pop

#endif // WINDOWS

enum FILE_EXT {
    FEXT_unknown,
    FEXT_bmp,
    FEXT_jpg,
    FEXT_tga,
    FEXT_txt,
    FEXT_wav,
    FEXT_lnd,
    FEXT_cms
};

static FILE_EXT get_file_ext(const char* name)
{
    string ext;
    string nf(name);
    size_t p = nf.rfind('.');

    if (p > nf.size() || p <= 0) {
        return FEXT_unknown;
    }

    p++;
    ext.assign(nf, p, 10);

    if (ext.empty())
        return FEXT_unknown;
    if (ext == "jpg")
        return FEXT_jpg;
    if (ext == "bmp")
        return FEXT_bmp;
    if (ext == "tga")
        return FEXT_tga;
    if (ext == "cms")
        return FEXT_cms;
    if (ext == "lnd")
        return FEXT_lnd;
    if (ext == "txt")
        return FEXT_txt;
    if (ext == "wav")
        return FEXT_wav;

    return FEXT_unknown;
}

DF_TEXTURE::DF_TEXTURE()
{
    data = NULL;
    x = y = 0;
    bpp = 0;
    format = 0;
}

DF_TEXTURE::~DF_TEXTURE()
{
    delete[] data;
}

DF_TEXTURE::DF_TEXTURE(const DF_TEXTURE& m)
{
    x = m.x;
    y = m.y;
    bpp = m.bpp;
    format = m.format;
    data = new char[x * y];
    memcpy(data, m.data, sizeof(data));
}

DF_TEXTURE& DF_TEXTURE::operator =(const DF_TEXTURE& m)
{
    if (&m == this) {
        return *this;
    }

    x = m.x;
    y = m.y;
    bpp = m.bpp;
    format = m.format;

    delete[] data;
    data = new char[x * y];
    memcpy(data, m.data, sizeof(data));

    return *this;
}

bool DF_TEXTURE::load(const char* name)
{
    FILE_EXT ext = get_file_ext(name);

    if (ext == FEXT_unknown) {
        DFLOG.add("\tunknown type of file\n");
        return false;
    }

    if (ext == FEXT_bmp)
        return load_bmp(name);
    if (ext == FEXT_tga)
        return load_tga(name);
    if (ext == FEXT_jpg)
        return load_jpg(name);

    DFLOG.add("\tthis type doesn't valid for textures\n");
    return false;
}

static void get_packets(char* data, int width, int height, int depth, fstream& file)
{
    int current_byte = 0;
    int same_color;
    char header, buffer8[4];
    unsigned short buffer16;
    int _bpp;

    _bpp = (depth == 16) ? 3 : depth / 8;

    while (current_byte < width * height * _bpp) {
        file.read(&header, sizeof(char));
        same_color = (header & 0x7F) + 1;

        if (header & 0x80) {

            if (depth == 32)
                file.read(buffer8, sizeof(char) * 4);
            if (depth == 24)
                file.read(buffer8, sizeof(char) * 3);
            if (depth == 16)
                file.read((char*)&buffer16, sizeof(unsigned short));
            if (depth == 8)
                file.read(buffer8, sizeof(char));

            for (int i = 0; i < same_color; ++i) {
                if (depth == 32) {
                    data[current_byte++] = buffer8[0];
                    data[current_byte++] = buffer8[1];
                    data[current_byte++] = buffer8[2];
                    data[current_byte++] = buffer8[3];
                }
                if (depth == 24) {
                    data[current_byte++] = buffer8[0];
                    data[current_byte++] = buffer8[1];
                    data[current_byte++] = buffer8[2];
                }
                if (depth == 16) {
                    data[current_byte++] = (buffer16 & 0x1F) << 3;
                    data[current_byte++] = ((buffer16 >> 5) & 0x1F) << 3;
                    data[current_byte++] = ((buffer16 >> 10) & 0x1F) << 3;
                }
                if (depth == 8) {
                    data[current_byte++] = buffer8[0];
                }
            }

        } else {

            for (int i = 0; i < same_color; ++i) {
                if (depth == 32) {
                    file.read(buffer8, sizeof(char) * 4);
                    data[current_byte++] = buffer8[0];
                    data[current_byte++] = buffer8[1];
                    data[current_byte++] = buffer8[2];
                    data[current_byte++] = buffer8[3];
                }
                if (depth == 24) {
                    file.read(buffer8, sizeof(char) * 3);
                    data[current_byte++] = buffer8[0];
                    data[current_byte++] = buffer8[1];
                    data[current_byte++] = buffer8[2];
                }
                if (depth == 16) {
                    file.read((char*)&buffer16, sizeof(unsigned short));
                    data[current_byte++] = (buffer16 & 0x1F) << 3;
                    data[current_byte++] = ((buffer16 >> 5) & 0x1F) << 3;
                    data[current_byte++] = ((buffer16 >> 10) & 0x1F) << 3;
                }
                if (depth == 8) {
                    file.read(buffer8, sizeof(char));
                    data[current_byte++] = buffer8[0];
                }
            }
        }
    }
}

static bool is_power_of_2(long a)
{
    int bitsSet = 0;

    for (int i = 0; i < 32; ++i) {
        if (a & (1 << i)) {
            bitsSet++;
        }
    }

    return (bitsSet == 1);
}

bool DF_TEXTURE::load_tga(const char* name)
{
    unsigned char IDLength;
    unsigned char ColorMapType;
    unsigned char ImageType;
    unsigned short ColorMapStart;
    unsigned short ColorMapLength;
    unsigned char ColorMapDepth;
    unsigned short XOffset;
    unsigned short YOffset;
    unsigned short Width;
    unsigned short Height;
    unsigned char Depth;
    unsigned char ImageDescriptor;
    char *ColorMap = NULL;
    delete[] data;
    fstream file;

    file.open(name, ios::in | ios::binary);
    if (file.fail()) {
        DFLOG.addf("\tcannot open %s\n", name);
        return false;
    }

    file.read((char*)&IDLength, sizeof(unsigned char));
    file.read((char*)&ColorMapType, sizeof(unsigned char));
    file.read((char*)&ImageType, sizeof(unsigned char));
    file.read((char*)&ColorMapStart, sizeof(unsigned short));
    file.read((char*)&ColorMapLength, sizeof(unsigned short));
    file.read((char*)&ColorMapDepth, sizeof(unsigned char));
    file.read((char*)&XOffset, sizeof(unsigned short));
    file.read((char*)&YOffset, sizeof(unsigned short));
    file.read((char*)&Width, sizeof(unsigned short));
    file.read((char*)&Height, sizeof(unsigned short));
    file.read((char*)&Depth, sizeof(unsigned char));
    file.read((char*)&ImageDescriptor, sizeof(unsigned char));
    file.ignore(IDLength);

    x = Width;
    y = Height;
    bpp = Depth;
    if (!is_power_of_2(x) || !is_power_of_2(y)) {
        DFLOG.add("\nWARNING: image dimensions isn't a power of two\n");
    }

    switch(ImageType) {

    case 1:
        if(ColorMapType == 1 && ColorMapDepth == 24) {
            ColorMap = new char[ColorMapLength * ColorMapDepth / 8];
            file.read(ColorMap, sizeof(char) * ColorMapLength * ColorMapDepth / 8);
        } else {
            file.close();
            DFLOG.add("\nERROR: only 24bit paletted images are supported.\n");
            return false;
        }
        break;

    case 2:
        break;

    case 3:
        break;

    case 9:
        if(ColorMapType == 1 && ColorMapDepth == 24) {
            ColorMap = new char[ColorMapLength * ColorMapDepth / 8];
            file.read(ColorMap, sizeof(char) * ColorMapLength * ColorMapDepth / 8);
        } else {
            file.close();
            DFLOG.add("\nERROR: only 24bit paletted images are supported.\n");
            return false;
        }
        break;

    case 10:
        break;

    case 11:
        break;

    default:
        file.close();
        DFLOG.add("\nERROR: unsupported image type.\n");
        return false;
    }

    switch (bpp) {

    case 32:
        data = new char[x * y * 4];
        if (ImageType == 2) {
            file.read(data, sizeof(char) * x * y * 4);
            format = GL_BGRA;
            break;
        }
        if (ImageType == 10) {
            get_packets(data, x, y, bpp, file);
            format = GL_BGRA;
            break;
        }
        format = GL_BGRA;
        break;

    case 24:
        data = new char[x * y * 3];
        if (ImageType == 2) {
            file.read(data, sizeof(char) * x * y * 3);
            format = GL_BGR;
            break;
        }
        if (ImageType == 10) {
            get_packets(data, x, y, bpp, file);
            format = GL_BGR;
            break;
        }
        format = GL_BGR;
        break;

    case 16:
        data = new char[x * y * 3];
        if (ImageType == 2) {
            unsigned short* buffer;
            buffer = new unsigned short[x * y];
            file.read((char*)buffer, sizeof(unsigned short) * x * y);
            for (int i = 0; i < x * y; ++i)
            {
                data[3 * i    ] = (buffer[i] & 0x1F) << 3;
                data[3 * i + 1] = ((buffer[i] >> 5) & 0x1F) << 3;
                data[3 * i + 2] = ((buffer[i] >> 10) & 0x1F) << 3;
            }
            delete[] buffer;
            bpp = 24;
            format = GL_BGR;
            break;
        }

        if (ImageType == 10) {
            get_packets(data, x, y, bpp, file);
            bpp = 24;
            format = GL_BGR;
            break;
        }

        bpp = 24;
        format = GL_BGR;
        break;

    case 8:
        if (ColorMapType == 1 && ColorMapDepth == 24) {
            unsigned char *buffer;
            data = new char[x * y * 3];
            buffer = new unsigned char[x * y];

            if (ImageType == 9) {
                get_packets((char*)buffer, x, y, bpp, file);
            } else {
                file.read((char*)buffer, sizeof(char) * x * y);
            }

            for (int i = 0; i < x * y; ++i) {
                data[3 * i    ] = ColorMap[3 * buffer[i]];
                data[3 * i + 1] = ColorMap[3 * buffer[i] + 1];
                data[3 * i + 2] = ColorMap[3 * buffer[i] + 2];
            }

            delete[] buffer;
            delete[] ColorMap;
            ColorMap = NULL;
            bpp = 24;
            format = GL_BGR;
            break;
        }

        if (ImageType == 3) {
            data = new char[x * y];
            file.read(data, sizeof(char) * x * y);
            format = GL_ALPHA;
            break;
        }

        if (ImageType == 11) {
            data = new char[x * y];
            get_packets(data, x, y, bpp, file);
            format = GL_ALPHA;
            break;
        }

        format = GL_ALPHA;
        break;

    default:
        delete[] ColorMap;
        ColorMap = NULL;
        DFLOG.add("\tunsupported .tga format\n");
        file.close();
        return false;
    }

    delete[] ColorMap;
    file.close();
    return true;
}

bool DF_TEXTURE::load_bmp(const char* name)
{
    BITMAPFILEHEADER h1;
    BITMAPINFOHEADER h2;
    fstream file;

    file.open(name, ios::in | ios::binary);
    if (file.fail()) {
        DFLOG.addf("\tcannot open %s\n", name);
        return false;
    }

    file.read((char*)&h1, sizeof(BITMAPFILEHEADER));

    /* check BMP id */
    if (h1.bfType != 0x4d42) {
        DFLOG.addf("WARNING: %s isn't a .bmp file\n", name);
        file.close();
        return false;
    }

    file.read((char*)&h2, sizeof(BITMAPINFOHEADER));

    /* 1-bit plane supported */
    if (h2.biPlanes != 1) {
        DFLOG.add("\tinvalid number of bit-planes\n");
        file.close();
        return false;
    }
    if (h2.biCompression) {
        DFLOG.add("\tcannot read compressed .bmp's\n");
        file.close();
        return false;
    }

    x = h2.biWidth;
    y = h2.biHeight;
    bpp = h2.biBitCount;
    if (!is_power_of_2(x) || !is_power_of_2(y)) {
        DFLOG.add("\nWARNING: image dimensions isn't a power of two\n");
    }

    switch (bpp) {
    case 32:
        format = GL_RGBA;
        break;
    case 24:
        format = GL_RGB;
        break;
    case 8:
        format = GL_ALPHA;
        break;
    default:
        DFLOG.add("\tunsupported .bmp format: only 24, 32 bit images are supported\n");
        file.close();
        return false;
    }

    file.seekg(h1.bfOffBits);

    int k = 0;
    char color[4];
    delete[] data;

    switch (format) {

    case GL_RGBA:
        data = new char[x * y * 4];
        for (int i = 0; i < x * y; ++i) {
            file.read(color, 4);

            /* changing BGRA -> RGBA */
            data[k++] = color[2];
            data[k++] = color[1];
            data[k++] = color[0];
            data[k++] = color[3];
        }
        break;

    case GL_RGB:
        data = new char[x * y * 3];
        for (int i = 0; i < x * y; ++i) {
            file.read(color, 3);

            /* changing BGR -> RGB */
            data[k++] = color[2];
            data[k++] = color[1];
            data[k++] = color[0];
        }
        break;

    case GL_ALPHA:
        data = new char[x * y];
        for (int i = 0; i < x * y; ++i) {
            file.read(color, 1);
            data[k++] = color[0];
        }
        break;

    default:
        DFLOG.add("DAMAGE: trying to use unknown .bmp format.\n");
        file.close();
        return false;
    }

    file.close();
    return true;
}

bool DF_TEXTURE::load_jpg(const char* name)
{
    return false;
}

int TEXTURE::list_size = 0;
LIST_TEXT* TEXTURE::list = NULL;

/* if current TEXTURE object is not assigned to the list element */
#define NIL -1

TEXTURE::TEXTURE()
{
    pntr = NIL;
}

TEXTURE::TEXTURE(const char* namefile)
{
    pntr = NIL;
    init(namefile);
}

TEXTURE::TEXTURE(void* data, int x, int y, const char* name)
{
    pntr = NIL;
    init(data, x, y, name);
}

TEXTURE::~TEXTURE()
{
    delete_from_list(pntr);
}

TEXTURE::TEXTURE(const TEXTURE& M)
{
    if (M.pntr == NIL) {
        return;
    }

    pntr = M.pntr;
    list[pntr].count++;
}

TEXTURE& TEXTURE::operator =(const TEXTURE& M)
{
    if (&M == this) {
        return *this;
    }

    delete_from_list(pntr);

    pntr = M.pntr;
    if (M.pntr != NIL) {
        list[pntr].count++;
    }

    return *this;
}

void TEXTURE::init(const char* file_name, bool mip_map, bool clamp_to_edge)
{
    if (pntr != NIL) {
        delete_from_list(pntr);
    }

    DFLOG.addf("Loading texture: %s", file_name);

    if (is_in_list(file_name)) {

        DFLOG.addf("\t...texture already exist.\n");
        pntr = add_to_list(file_name);
        return;

    } else {

        pntr = add_to_list(file_name);

        /* loading the image */
        DF_TEXTURE* t;
        t = new DF_TEXTURE;

        if (!t->load(file_name)) {
            DFLOG.addf("\t...cannot load image, trying default...\n");

            delete t;
            TEXTURE* def = new TEXTURE;
            def->init("data\\textures\\default.bmp", mip_map, clamp_to_edge);

            *this = *def;
            delete def;

            return;
        }

        /* texture creation */
        glGenTextures(1, &list[pntr].id);
        glBindTexture(GL_TEXTURE_2D, list[pntr].id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,GL_LINEAR);

        if (clamp_to_edge) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }

        if (mip_map) {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
            gluBuild2DMipmaps(GL_TEXTURE_2D, t->bpp / 8, t->x, t->y, t->format, GL_UNSIGNED_BYTE, t->data);
        } else {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexImage2D(GL_TEXTURE_2D, 0, t->bpp / 8, t->x, t->y, 0, t->format, GL_UNSIGNED_BYTE, t->data);
        }

        delete t;
        DFLOG.addf("\t...texture load successfully.\n");
        return;
    }
}
void TEXTURE::init(void* data, int x, int y, const char* name, bool mip_map, bool clamp_to_edge, GLenum format)
{
    if (!data || !name) {
        return;
    }

    if (pntr != NIL) {
        delete_from_list(pntr);
    }

    DFLOG.addf("Loading texture: %s", name);

    if (is_in_list(name)) {
        DFLOG.addf("\t...texture already exist.\n");
        pntr = add_to_list(name);
        return;
    }

    pntr = add_to_list(name);
    glGenTextures(1, &list[pntr].id);
    glBindTexture(GL_TEXTURE_2D, list[pntr].id);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    if (clamp_to_edge) {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    } else {
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    }

    if (mip_map) {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_NEAREST);
        gluBuild2DMipmaps(GL_TEXTURE_2D, 3, x, y, format, GL_UNSIGNED_BYTE, data);
    } else {
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, x, y, 0, format, GL_UNSIGNED_BYTE, data);
    }

    DFLOG.addf("\t...texture load successfully.\n");
    return;
}

TEXTURE::TEXTURE(int begin_x, int begin_y, int size_x, int size_y, const char* name)
{
    if (begin_x < 0 || begin_y < 0 || size_x <= 0 || size_y <= 0) {
        return;
    }

    if (is_in_list(name)) {
        glBindTexture(GL_TEXTURE_2D, list[pntr].id);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, begin_x, begin_y, size_x, size_y, 0);
        return;
    }

    if (pntr != NIL) {
        delete_from_list(pntr);
    }

    pntr = add_to_list(name);
    glGenTextures(1, &list[pntr].id);
    glBindTexture(GL_TEXTURE_2D, list[pntr].id);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, begin_x, begin_y, size_x, size_y, 0);
}

void TEXTURE::copy_screen(int begin_x, int begin_y, int size_x, int size_y, const char* name)
{
    if (begin_x < 0 || begin_y < 0 || size_x <= 0 || size_y <= 0) {
        return;
    }

    if (is_in_list(name)) {
        glBindTexture(GL_TEXTURE_2D, list[pntr].id);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, begin_x, begin_y, size_x, size_y, 0);
        return;
    }

    if (pntr != NIL) {
        delete_from_list(pntr);
    }

    pntr = add_to_list(name);
    glGenTextures(1, &list[pntr].id);
    glBindTexture(GL_TEXTURE_2D, list[pntr].id);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, begin_x, begin_y, size_x, size_y, 0);
}

void TEXTURE::copy_shadow(int begin_x, int begin_y, int size_x, int size_y, const char* name)
{
    if (begin_x < 0 || begin_y < 0 || size_x <= 0 || size_y <= 0) {
        return;
    }

    if (is_in_list(name)) {
        glBindTexture(GL_TEXTURE_2D, list[pntr].id);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_LUMINANCE);
        glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FAIL_VALUE_ARB, 0.5);
        glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, begin_x, begin_y, size_x, size_y, 0);
        return;
    }

    if (pntr != NIL) {
        delete_from_list(pntr);
    }

    pntr = add_to_list(name);
    glGenTextures(1, &list[pntr].id);
    glBindTexture(GL_TEXTURE_2D, list[pntr].id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_DEPTH_TEXTURE_MODE_ARB, GL_LUMINANCE);
    glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_MODE_ARB, GL_COMPARE_R_TO_TEXTURE_ARB);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FUNC_ARB, GL_LEQUAL);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_COMPARE_FAIL_VALUE_ARB, 0.5);
    glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, begin_x, begin_y, size_x, size_y, 0);
}

bool TEXTURE::is_in_list(const char* name)
{
    for (int i = 0; i < list_size; ++i) {
        if ((strstr(list[i].name, name) != NULL) && (list[i].count != 0)) {
            return true;
        }
    }
    return false;
}

bool TEXTURE::delete_from_list(int p)
{
    if (p >= list_size || p < 0) {
        DFLOG.addf("WARNING: trying to delete texture with pointer %d, list_size = %d\n", p, list_size);
        return false;
    }

    if (list[p].count < 0) {
        DFLOG.addf("WARNING: trying to delete texture (pointer %d) with negative count: %d\n", p, list[p].count);
        return false;
    }

    if (list[p].count > 1) {
        DFLOG.addf("Deleting texture pointer for %s\n", list[p].name);
        list[p].count--;
    } else {
        DFLOG.addf("Deleting texture memory for %s\n", list[p].name);
        glDeleteTextures(1, &list[p].id);
        list[p].count--;
    }

    pntr = NIL;
    return true;
}

int TEXTURE::add_to_list(const char* name)
{
    /* check if texture already loaded */
    for (int i = 0; i < list_size; ++i) {
        if (strstr(list[i].name, name) != NULL) {
            list[i].count++;
            DFLOG.addf("Increase texture %s usage counter.\n", list[i].name);
            return i;
        }
    }

    /* realloc texture list */
    LIST_TEXT* temp = new LIST_TEXT[list_size + 1];

    if (temp == NULL) {
        DFLOG.addf("ERROR: realloc textures list failed.\n");
        return NIL;
    }

    /* adding new texture to the end of the list */
    temp[list_size].count = 1;
    strcpy(temp[list_size].name, name);

    if (list != NULL) {
        /* move old data to new list */
        for (int i = 0; i < list_size; ++i) {
            temp[i] = list[i];
        }
        /* release old list */
        delete[] list;
    }

    list_size++;
    list = temp;
    temp = NULL;

    DFLOG.addf("New texture %s added to list successfully.\n", list[list_size - 1].name);
    return (list_size - 1);
}

TEXTURE WHITE_NULL;
