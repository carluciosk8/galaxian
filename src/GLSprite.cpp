/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "GLSprite.h"

#include <string>
#include <stdexcept>

using std::runtime_error;
using std::string;

#include <SDL/SDL.h>
#include <SDL/SDL_image.h>



GLSprite::GLSprite(const char* cpFile)
{
    loadImage(cpFile);
}



GLSprite::~GLSprite()
{
}


void GLSprite::loadImage(const char* cpFile)
{
    SDL_Surface *surface;
    int mode;

    surface = IMG_Load(cpFile);
    
    if (!surface)
        throw runtime_error( string("Couldn't load image: ") + string(cpFile) );

    if (surface->format->BytesPerPixel == 3)
        mode = GL_RGB;
    else if (surface->format->BytesPerPixel == 4)
        mode = GL_RGBA;
    else
    {
        SDL_FreeSurface(surface);
        throw runtime_error( string("Invalid BPP format for image: ") + string(cpFile) );
    }

    m_iWidth = surface->w;
    m_iHeight = surface->h;

    glGenTextures(1, &m_uiTexHandle);
    glBindTexture(GL_TEXTURE_2D, m_uiTexHandle);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
    glTexImage2D(GL_TEXTURE_2D, 0, mode, m_iWidth, m_iHeight, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

    SDL_FreeSurface(surface);
}



void GLSprite::draw(int x, int y, float scale, float angle)
{
    glEnable(GL_TEXTURE_2D);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glBindTexture(GL_TEXTURE_2D, m_uiTexHandle);

    glLoadIdentity();
    glTranslatef(x, y, 0);
    glRotatef(angle, 0, 0, 1.0);
    glScalef(scale, scale, scale);

    glBegin(GL_QUADS);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
        glTexCoord2f(0.0f, 1.0f); glVertex2f(-m_iWidth/2, -m_iHeight/2);
        glTexCoord2f(1.0f, 1.0f); glVertex2f(m_iWidth/2, -m_iHeight/2);
        glTexCoord2f(1.0f, 0.0f); glVertex2f(m_iWidth/2, m_iHeight/2);
        glTexCoord2f(0.0f, 0.0f); glVertex2f(-m_iWidth/2, m_iHeight/2);
    glEnd();

    glFlush();
    glDisable(GL_TEXTURE_2D);
}
