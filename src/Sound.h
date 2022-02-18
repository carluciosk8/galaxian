/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __SOUND_H__
#define __SOUND_H__

#include <SDL/SDL_mixer.h>

class Sound
{
    public:
        Sound(const char* cpName);
        ~Sound();

        void play();


    protected:
        Mix_Chunk*  m_opSound;
};


#endif
