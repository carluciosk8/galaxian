/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include <cstdlib>

#include "Background.h"

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

#include "GLSprite.h"

static const int MAX_STARS = 150;


Background::Background()
:  m_vfStarPosX(MAX_STARS),
   m_vfStarPosY(MAX_STARS),
   m_vfStarVelY(MAX_STARS)
{
    m_opBackgroundSprite = new GLSprite("resources/background.png");

    for (unsigned int i=0 ; i<MAX_STARS ; ++i)
    {
        m_vfStarPosX[i] = rand() % 800;
        m_vfStarPosY[i] = rand() % 600;
        m_vfStarVelY[i] = -(((float)rand()/RAND_MAX)*70.0f + 40.0f);
    }
}



Background::~Background()
{
}



void Background::update(float dt)
{
    for (unsigned int i=0 ; i<MAX_STARS ; ++i)
    {
        float dy = m_vfStarVelY[i] * dt;
        m_vfStarPosY[i] += dy;
        if ( m_vfStarPosY[i] < 0.0f )
            m_vfStarPosY[i] = 600.0f;
    }
}



void Background::draw()
{
    glLoadIdentity();

    m_opBackgroundSprite->draw(400, 300, 1.0f, 0.0f);

    glLoadIdentity();
    glBegin(GL_POINTS);
        for (unsigned int i=0 ; i<MAX_STARS ; ++i)
            glVertex2f(m_vfStarPosX[i], m_vfStarPosY[i]);
    glEnd();

    glFlush();
}
