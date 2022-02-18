/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __SPACESHIPSHOT_H__
#define __SPACESHIPSHOT_H__

#include "GameObject.h"

#include "Sound.h"


class SpaceShipShot : public GameObject
{
    public:
        SpaceShipShot();
        virtual ~SpaceShipShot();

        virtual void update(float dt);
        virtual void draw();

        void start(float x);
        inline bool isActive() { return m_bActive; }
        inline void deactive() { m_bActive = false; }


    protected:
        bool  m_bActive;
        float  m_fVel;

        Sound*  m_opShipShotSnd;
};


#endif
