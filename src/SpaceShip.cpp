/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "SpaceShip.h"

#include <SDL/SDL.h>
#include <cassert>



SpaceShip::SpaceShip()
: GameObject("SHIP"),
  m_fVel(0.0f),
  m_iLives(1),
  m_eState(SpaceShip::STATE_DEFENDING)
{
    m_opSprite = new GLSprite("resources/spaceship.png");
    m_fRadius = 22.0f;
}



SpaceShip::~SpaceShip(void)
{
    delete m_opSprite;
}



void SpaceShip::update(float dt)
{
    float dx = m_fVel * dt;
    translateX(dx);

    if (m_fPosX < 32.0f)
        m_fPosX = 32.0f;
    if (m_fPosX > 768.0f)
        m_fPosX = 768.0f;
}


void SpaceShip::draw()
{
    assert(m_opSprite);
    if (m_eState == SpaceShip::STATE_DEFENDING)
        m_opSprite->draw((int)m_fPosX, (int)m_fPosY, m_fScale, m_fAngle);
    else if (m_eState == SpaceShip::STATE_DEAD)
    {
        if ( rand()%2 )
            m_opSprite->draw((int)m_fPosX, (int)m_fPosY, m_fScale, m_fAngle);
    }
}


void SpaceShip::die()
{
    m_eState = SpaceShip::STATE_DYING;
    m_iLives--;
    m_iDieTime = SDL_GetTicks();
}


void SpaceShip::respawn()
{
    if ( (SDL_GetTicks() - m_iDieTime) > 1000 )
        m_eState = SpaceShip::STATE_DEAD;
    if ( (SDL_GetTicks() - m_iDieTime) > 3000 )
        m_eState = SpaceShip::STATE_DEFENDING;
}
