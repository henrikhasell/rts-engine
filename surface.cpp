#include "surface.hpp"
#include <iostream>

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#   define R_MASK 0xff000000
#   define G_MASK 0x00ff0000
#   define B_MASK 0x0000ff00
#   define A_MASK 0x000000ff
#else
#   define R_MASK 0x000000ff
#   define G_MASK 0x0000ff00
#   define B_MASK 0x00ff0000
#   define A_MASK 0xff000000
#endif

#define VERTEX_BUFFER 0
#define UV_BUFFER 1
#define INDEX_BUFFER 2

using namespace Engine;
using namespace GL;

static GLenum Texture_Format(const SDL_Surface *surface)
{
	if(surface->format->BytesPerPixel == 4)
	{
		if(surface->format->Rmask == 0x000000ff)
		{
			return GL_RGBA;
		}
		else
		{
			return GL_BGRA;
		}
	}

	if(surface->format->BytesPerPixel == 3)
	{
		if(surface->format->Rmask == 0x000000ff)
		{
			return GL_RGB;
		}
		else
		{
			return GL_BGR;
		}
    }

    return GL_INVALID_ENUM;
}

static GLuint Texture_Upload(const SDL_Surface *surface)
{
	GLuint texture;
	GLenum texture_format = Texture_Format(surface);
/*
// Black/white checkerboard
float pixels[] = {
0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,
1.0f, 1.0f, 1.0f,   0.0f, 0.0f, 0.0f
};
*/
	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 2, 2, 0, GL_RGB, GL_FLOAT, pixels);
	glTexImage2D(GL_TEXTURE_2D, 0, surface->format->BytesPerPixel, surface->w,
	surface->h, 0, texture_format, GL_UNSIGNED_BYTE, surface->pixels);
	glBindTexture(GL_TEXTURE_2D, 0);

	return texture;
}

Surface::Surface(SDL_Surface *surface) :
    width(surface->w), height(surface->h)
{
    SDL_Surface *optimised = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, R_MASK, G_MASK, B_MASK, A_MASK);
    SDL_BlitSurface(surface, NULL, optimised, NULL);
    texture = Texture_Upload(optimised);

    SDL_FreeSurface(optimised);

    glGenBuffers(3, &buffer[0]);

    vertexArray[0].x = width;
    vertexArray[0].y = 0;

    vertexArray[1].x = 0;
    vertexArray[1].y = 0;

    vertexArray[2].x = 0;
    vertexArray[2].y = height;

    vertexArray[3].x = width;
    vertexArray[3].y = height;

    uvArray[0].x = 1;
    uvArray[0].y = 0;

    uvArray[1].x = 0;
    uvArray[1].y = 0;

    uvArray[2].x = 0;
    uvArray[2].y = 1;

    uvArray[3].x = 1;
    uvArray[3].y = 1;

    indexArray[0] = 0;
    indexArray[1] = 1;
    indexArray[2] = 2;
    indexArray[3] = 0;
    indexArray[4] = 2;
    indexArray[5] = 3;

    glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTEX_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertexArray), vertexArray, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[UV_BUFFER]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(uvArray), uvArray, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDEX_BUFFER]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexArray), indexArray, GL_STATIC_DRAW);

    SDL_FreeSurface(surface);
}

Surface::~Surface()
{
    glDeleteTextures(1, &texture);
    glDeleteBuffers(3, &buffer[0]);
}

void Surface::draw(const Graphics &graphics)
{
    glUseProgram(graphics.program2D.program);

    glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture);
    glUniform1i(graphics.uniformTextureSampler2D, 0);

    glEnableVertexAttribArray(graphics.attributePosition2D);
    glEnableVertexAttribArray(graphics.attributeUV2D);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[VERTEX_BUFFER]);
    glVertexAttribPointer(graphics.attributePosition2D, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ARRAY_BUFFER, buffer[UV_BUFFER]);
    glVertexAttribPointer(graphics.attributeUV2D, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffer[INDEX_BUFFER]);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

    glDisableVertexAttribArray(graphics.attributePosition2D);
    glDisableVertexAttribArray(graphics.attributeUV2D);
}

#undef VERTEX_BUFFER
#undef UV_BUFFER
#undef INDEX_BUFFER

#undef R_MASK
#undef G_MASK
#undef B_MASK
#undef A_MASK
