/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "Music.h"



Music::Music(const char* cpName)
{
    m_opMusic = Mix_LoadMUS(cpName);
    if(!m_opMusic)
        printf("Mix_LoadMUS(\"%s\"): %s\n", cpName, Mix_GetError());
}



Music::~Music()
{
    Mix_FreeMusic(m_opMusic);
}



void Music::play()
{
    if( Mix_PlayMusic(m_opMusic, -1)==-1 )
        printf("Mix_PlayMusic: %s\n", Mix_GetError());
}



void Music::stop()
{
}



void Music::pause()
{
}
