/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __ALIENSHOT_H__
#define __ALIENSHOT_H__

#include "GameObject.h"


class AlienShot : public GameObject
{
    public:
        AlienShot();
        virtual ~AlienShot();

        virtual void update(float dt);
        virtual void draw();

        void start(float x, float y, float vx);
        inline bool isActive() { return m_bActive; }
        inline void deactive() { m_bActive = false; }


    protected:
        bool  m_bActive;
        float  m_fVelX;
        float  m_fVelY;
};


#endif
