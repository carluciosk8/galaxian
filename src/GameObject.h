/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__


#include <string>

using std::string;

#include "GLSprite.h"


class GameObject
{
    public:
        GameObject(const char* cpName);
        virtual ~GameObject();

        virtual void update(float dt) = 0;
        virtual void draw() = 0;

        void print();

        inline void scale(float s) { m_fScale *= s; }
        inline void rotate(float a) { m_fAngle += a; }
        inline void translateX(float x) { m_fPosX += x; }
        inline void translateY(float y) { m_fPosY += y; }
        inline void translate(float x, float y) { m_fPosX += x ; m_fPosY += y; }

        inline void scaleTo(float s) { m_fScale = s; }
        inline void rotateTo(float a) { m_fAngle = a; }
        inline void translateXTo(float x) { m_fPosX = x; }
        inline void translateYTo(float y) { m_fPosY = y; }
        inline void translateTo(float x, float y) { m_fPosX = x ; m_fPosY = y; }

        inline float getScale() { return m_fScale; }
        inline float getAngle() { return m_fAngle; }
        inline float getPosX() { return m_fPosX; }
        inline float getPosY() { return m_fPosY; }

        inline bool testCollision(const GameObject* obj)
        {
            if (
                ( (m_fPosX - obj->m_fPosX)*(m_fPosX - obj->m_fPosX) + (m_fPosY - obj->m_fPosY)*(m_fPosY - obj->m_fPosY) )
                <
                (m_fRadius + obj->m_fRadius)*(m_fRadius + obj->m_fRadius)
               )
                return true;
            else
                return false;
        }


    protected:
        string  m_sName;

        float  m_fPosX;
        float  m_fPosY;
        float  m_fScale;
        float  m_fAngle;
        float  m_fRadius;

        GLSprite*  m_opSprite;
};



#endif
