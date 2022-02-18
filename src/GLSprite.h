/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __GLSPRITE_H__
#define __GLSPRITE_H__

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>


class GLSprite
{
    public:
        GLSprite(const char* cpFile);
        ~GLSprite(void);

        void loadImage(const char* cpFile);
        void draw(int x, int y, float scale, float agle);

    protected:
        int  m_iWidth;
        int  m_iHeight;

        GLuint m_uiTexHandle;
};


#endif
