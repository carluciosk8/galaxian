/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "SpaceShipShot.h"

#include <cassert>


SpaceShipShot::SpaceShipShot(void)
: GameObject("SHIP SHOT"),
  m_bActive(false),
  m_fVel(1000.0f)
{
    m_opSprite = new GLSprite("resources/shipshot.png");
    m_opShipShotSnd = new Sound("resources/shipshot.wav");

    m_fRadius = 1.0f;
}



SpaceShipShot::~SpaceShipShot(void)
{
    delete m_opSprite;
}



void SpaceShipShot::update(float dt)
{
    if ( m_bActive )
    {
        float dy = m_fVel * dt;
        translateY(dy);

        if (m_fPosY > 600.0f)
        {
            m_bActive = false;
        }
    }
}



void SpaceShipShot::draw()
{
    assert(m_opSprite);
    if ( m_bActive )
        m_opSprite->draw((int)m_fPosX, (int)m_fPosY, m_fScale, m_fAngle);
}



void SpaceShipShot::start(float x)
{
    if ( ! m_bActive )
    {
        m_opShipShotSnd->play();
        m_bActive = true;
        translateTo(x, 50.0f);
    }
}
