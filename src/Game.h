/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#ifndef __GAME_H__
#define __GAME_H__

#include <SDL/SDL.h>

#include <vector>

using std::vector;


class SpaceShip;
class SpaceShipShot;
class Alien;
class AlienShot;
class Background;
class Music;
class Sound;
class GLSprite;

class Game
{
    public:
        Game();
        ~Game();

        void run();
        void pause();
        void step();

        static Uint32 shotCallback(Uint32 interval, void *param);
        static Uint32 attackCallback(Uint32 interval, void *param);
        static AlienShot* getInactiveAlienShot();

    protected:
        void setupSDL();
        void setupOpenGL();

        void startScreen();
        void gameWin();
        void gameOver();

        void processEvents();
        void updateWorld();
        void drawScreen();
        void testCollisions();

        void drawSpeed();


        float  m_fActualTime;
        float  m_fLastFrameTime;
        static bool m_bPaused;
        bool m_bStepMode;
        bool m_bGameRun;
        bool m_bGameWin;
        bool m_bGameEnd;

        SpaceShip*  m_opSpaceShip;
        SpaceShipShot*  m_opSpaceShipShot;
        Background*  m_opBackground;
        static vector<Alien*>  m_vAliens;
        static vector<AlienShot*>  m_vAliensShot;

        Music*  m_opMusic;
        Sound*  m_opAlienExp;
        Sound*  m_opShipExp;
        GLSprite*  m_opTitleSprite;

        SDL_TimerID  shotCallbackId;
        SDL_TimerID  attackCallbackId;
};



#endif
