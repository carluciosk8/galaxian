/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "Sound.h"



Sound::Sound(const char* cpName)
{
    m_opSound = Mix_LoadWAV(cpName);
    if(!m_opSound)
        printf("Mix_LoadWAV(\"%s\"): %s\n", cpName, Mix_GetError());
}



Sound::~Sound()
{
    Mix_FreeChunk(m_opSound);
}



void Sound::play()
{
    if(Mix_PlayChannel(-1, m_opSound, 0)==-1)
        printf("Mix_PlayChannel: %s\n",Mix_GetError());
}
