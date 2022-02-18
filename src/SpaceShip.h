/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __SPACESHIP_H__
#define __SPACESHIP_H__

#include "GameObject.h"


class SpaceShip : public GameObject
{
    public:
        SpaceShip();
        virtual ~SpaceShip();

        inline void setVel(float v) { m_fVel = v; }
        inline float getVel() { return m_fVel; }

        virtual void update(float dt);
        virtual void draw();

        enum EState
        {
            STATE_DEFENDING,
            STATE_DYING,
            STATE_DEAD
        };

        EState getState() { return m_eState; }
        int getLives() { return m_iLives; }
        void setLives(int iLives) { m_iLives = iLives; }

        void die();
        void respawn();


    protected:
        float  m_fVel;
        int  m_iLives;
        int  m_iDieTime;
        EState  m_eState;
};


#endif
