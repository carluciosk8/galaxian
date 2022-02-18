/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "Alien.h"
#include "AlienShot.h"

#include <cassert>
#include <cmath>

#ifndef M_PI
#define M_PI        3.14159265358979323846
#endif


float  Alien::ms_fFormPosX = 20.0f;
float  Alien::ms_fFormPosY = 580.0f;
float  Alien::ms_fFormVel = 50.0f;
int  Alien::ms_iAttacking = 0;
int  Alien::ms_iMaxAttacking = 3;



Alien::Alien(float fFormationPosX, float fFormationPosY)
: GameObject("ALIEN"),
  m_fFormationPosX(fFormationPosX),
  m_fFormationPosY(fFormationPosY),
  m_eState(Alien::STATE_FORMATION)
{
    m_opSprite = new GLSprite("resources/alien.png");
    m_opAttackSnd = new Sound("resources/attack.wav");

    m_fRadius = 14.0f;
    m_fShotProb = 0.0f;
    m_fAttackProb = 0.0001f;
    m_fAttackVelX = 0.0f;
    m_fAttackVelY = -100.0f;
}



Alien::~Alien()
{
    delete m_opSprite;
}



void Alien::resetFormation()
{
    ms_fFormPosX = 20.0f;
    ms_fFormPosY = 580.0f;
    ms_fFormVel = 40.0f;
    ms_iAttacking = 0;
    ms_iMaxAttacking = 3;
}



void Alien::updateFormation(float dt)
{
    float dx = ms_fFormVel * dt;

    ms_fFormPosX += dx;
    if (ms_fFormPosX < 16.0f)
    {
        ms_fFormVel = 40.0f;
        ms_fFormPosY -= 4;
    }
    else if (ms_fFormPosX > 784.0f-(40*14))
    {
        ms_fFormVel = -40.0f;
        ms_fFormPosY -= 4;
    }
}



void Alien::update(float dt)
{
    if (m_eState == Alien::STATE_FORMATION)
    {
        float x = (((float)rand() / RAND_MAX)*2.0f - 1.0f) + ms_fFormPosX + 40.0f*m_fFormationPosX;
        float y = (((float)rand() / RAND_MAX)*2.0f - 1.0f) + ms_fFormPosY - 40.0f*m_fFormationPosY;
        m_fAttackVelX = 0.0f;
        translateTo(x, y);
    }
    else if(m_eState == Alien::STATE_ATTACKING)
    {
        float dx = m_fAttackVelX * dt;
        float dy = m_fAttackVelY * dt;
        m_fAttackVelX += m_fAttackAccX * dt;
        translate(dx, dy);
        if ( m_fPosY < 0.0f )
        {
            m_eState = Alien::STATE_FORMATION;
            ms_iAttacking--;
            m_fAttackVelX = 0;
        }
        if ( m_fPosX > 500.0f && m_fAttackAccX > 0.0f )
            m_fAttackAccX = -(((float)rand() / RAND_MAX)*50.0f + 50.0f);
        else if ( m_fPosX < 300.0f && m_fAttackAccX < 0.0f )
            m_fAttackAccX = (((float)rand() / RAND_MAX)*50.0f + 50.0f);

        m_fAngle = 180.0f*acos( -m_fAttackVelY / sqrt(m_fAttackVelX*m_fAttackVelX + m_fAttackVelY*m_fAttackVelY) )/M_PI;
        if ( m_fAttackVelX < 0.0f )
            m_fAngle = -m_fAngle;
    }
}



void Alien::draw()
{
    assert(m_opSprite);
    m_opSprite->draw((int)m_fPosX, (int)m_fPosY, m_fScale, m_fAngle);
}



void Alien::shot(AlienShot* opShot)
{
    assert(opShot);
    opShot->start(m_fPosX, m_fPosY, m_fAttackVelX);
}



void Alien::attack()
{
    m_opAttackSnd->play();
    m_fAttackAccX = ((float)rand() / RAND_MAX)*200.0f - 100.0f;
    m_fAttackVelY = -(((float)rand() / RAND_MAX)*40.0f + 80.0f);
    m_eState = Alien::STATE_ATTACKING;
    ms_iAttacking++;
}
