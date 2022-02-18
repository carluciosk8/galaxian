/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "AlienShot.h"

#include <cassert>


AlienShot::AlienShot()
: GameObject("ALIEN SHOT"),
  m_bActive(false),
  m_fVelX(0.0f),
  m_fVelY(-300.0f)
{
    m_opSprite = new GLSprite("resources/alienshot.png");
    m_fRadius = 1.0f;
}



AlienShot::~AlienShot()
{
    delete m_opSprite;
}



void AlienShot::update(float dt)
{
    if ( m_bActive )
    {
        float dx = m_fVelX * dt;
        float dy = m_fVelY * dt;
        translate(dx,dy);

        if (m_fPosY < 0.0f)
        {
            m_bActive = false;
        }
    }
}



void AlienShot::draw()
{
    assert(m_opSprite);
    if ( m_bActive )
        m_opSprite->draw((int)m_fPosX, (int)m_fPosY, m_fScale, m_fAngle);
}



void AlienShot::start(float x, float y, float vx)
{
    if ( ! m_bActive )
    {
        m_bActive = true;
        translateTo(x, y);
        m_fVelX = vx;
    }
}
