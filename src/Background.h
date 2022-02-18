/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __BACKGROUND_H__
#define __BACKGROUND_H__


#include <vector>

using std::vector;

class GLSprite;

class Background
{
    public:
        Background();
        virtual ~Background();

        void update(float dt);
        void draw();


    protected:
        vector<float>  m_vfStarPosX;
        vector<float>  m_vfStarPosY;
        vector<float>  m_vfStarVelY;

        GLSprite*  m_opBackgroundSprite;
};



#endif
