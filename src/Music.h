/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __MUSIC_H__
#define __MUSIC_H__

#include <SDL/SDL_mixer.h>

class Music
{
    public:
        Music(const char* cpName);
        ~Music();

        void play();
        void stop();
        void pause();


    protected:
        Mix_Music*  m_opMusic;
};


#endif
