/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __ALIEN_H__
#define __ALIEN_H__

#include "GameObject.h"
#include "Sound.h"


class AlienShot;

class Alien : public GameObject
{
    public:
        Alien(float fFormationPosX, float fFormationPosY);
        virtual ~Alien();

        static void resetFormation();
        static void updateFormation(float dt);

        virtual void update(float dt);
        virtual void draw();

        void shot(AlienShot* opShot);
        void attack();

        enum EState
        {
            STATE_FORMATION,
            STATE_ATTACKING,
            STATE_DYING,
            STATE_DEAD
        };

        EState getState() { return m_eState; }
        void setState(EState eState) { m_eState = eState; }


    protected:
        float  m_fFormationPosX;
        float  m_fFormationPosY;
        float  m_fAttackPosX;
        float  m_fAttackPosY;
        float  m_fAttackVelX;
        float  m_fAttackVelY;
        float  m_fAttackAccX;
        float  m_fShotProb;
        float  m_fAttackProb;
        EState  m_eState;

        Sound*  m_opAttackSnd;

        static float  ms_fFormPosX;
        static float  ms_fFormPosY;
        static float  ms_fFormVel;
        static int  ms_iAttacking;
        static int  ms_iMaxAttacking;
};


#endif
