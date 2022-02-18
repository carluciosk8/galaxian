/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include "Game.h"

#include <string>
#include <stdexcept>

using std::runtime_error;
using std::string;

#ifdef _WIN32
#include <windows.h>
#endif
#include <GL/gl.h>

#include "SpaceShip.h"
#include "SpaceShipShot.h"
#include "Alien.h"
#include "AlienShot.h"
#include "Background.h"
#include "Music.h"
#include "Sound.h"
#include "GLSprite.h"


#define SCREEN_WIDTH    800
#define SCREEN_HEIGHT   600
#define SCREEN_FLAGS    SDL_OPENGL


vector<Alien*>  Game::m_vAliens;
vector<AlienShot*>  Game::m_vAliensShot;
bool  Game::m_bPaused = false;

Game::Game()
{
    setupSDL();
    setupOpenGL();

    m_fActualTime = 0;
    m_fLastFrameTime = 0;
    m_bStepMode = false;
    m_bGameWin = false;
    m_bGameRun = true;
    m_bGameEnd = false;
    m_bPaused = true;

    m_opSpaceShip = new SpaceShip();
    m_opSpaceShipShot = new SpaceShipShot();
    m_opBackground = new Background();

    for (int i=0 ; i<15 ; ++i)
        m_vAliensShot.push_back(new AlienShot());

    m_opMusic = new Music("resources/galaxian.ogg");
    m_opAlienExp = new Sound("resources/alienexp.wav");
    m_opShipExp = new Sound("resources/explosion.wav");
    m_opTitleSprite = new GLSprite("resources/title.png");

}



Game::~Game()
{
    delete m_opSpaceShip;
    delete m_opSpaceShipShot;
    delete m_opBackground;
}



void Game::setupSDL()
{
    if ( SDL_Init(SDL_INIT_EVERYTHING) < 0 )
        throw runtime_error( string("Couldn't initialize SDL: ") + string(SDL_GetError()) );

    atexit(SDL_Quit);

    SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
    SDL_GL_SetAttribute( SDL_GL_DEPTH_SIZE, 16 );
    SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );

    const SDL_VideoInfo* video = SDL_GetVideoInfo();

    if( !video )
        throw runtime_error( string("Couldn't get video information: ") + string(SDL_GetError()) );

    if( SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, video->vfmt->BitsPerPixel, SCREEN_FLAGS ) == 0 )
        throw runtime_error( string("Couldn't set video mode: ") + string(SDL_GetError()) );

    shotCallbackId = SDL_AddTimer(1000, shotCallback, NULL);
    attackCallbackId = SDL_AddTimer(5000, attackCallback, NULL);

    if(Mix_OpenAudio(44100, AUDIO_S16, 2, 4096))
    {
        printf("Unable to open audio!\n");
        exit(1);
    }

    SDL_ShowCursor(SDL_DISABLE);
}



void Game::setupOpenGL()
{
    float aspect = (float)SCREEN_WIDTH / (float)SCREEN_HEIGHT;

    glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, SCREEN_WIDTH, 0, SCREEN_HEIGHT, -1, 1);
    glMatrixMode(GL_MODELVIEW);


    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);					// Black Background
    glClearDepth(1.0f);							// Depth Buffer Setup
    glDepthFunc(GL_LEQUAL);							// Type Of Depth Testing
    glEnable(GL_DEPTH_TEST);						// Enable Depth Testing
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);			// Enable Alpha Blending (disable alpha testing)
    glEnable(GL_BLEND);							// Enable Blending       (disable alpha testing)
    glAlphaFunc(GL_GREATER,0.1f);						// Set Alpha Testing     (disable blending)
    glEnable(GL_ALPHA_TEST);						// Enable Alpha Testing  (disable blending)
    //glEnable(GL_TEXTURE_2D);						// Enable Texture Mapping
    //glEnable(GL_CULL_FACE);							// Remove Back Face
}



void Game::run()
{
    SDL_WarpMouse(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
    SDL_Event event;
    while (SDL_PollEvent(&event));

    m_opSpaceShip->translateTo(400, 52);

    //m_opMusic->play();

    while (! m_bGameEnd)
    {
        startScreen();

        for (int x=0 ; x<15 ; ++x)
            m_vAliens.push_back(new Alien(x, 0));
        for (int x=0 ; x<15 ; ++x)
            m_vAliens.push_back(new Alien(x, 1));
        for (int x=0 ; x<15 ; ++x)
            m_vAliens.push_back(new Alien(x, 2));
        for (int x=0 ; x<15 ; ++x)
            m_vAliens.push_back(new Alien(x, 3));
        for (int x=0 ; x<15 ; ++x)
            m_vAliens.push_back(new Alien(x, 4));

        m_opSpaceShip->setLives(5);
        m_bGameRun = true;
        Alien::resetFormation();
        m_fLastFrameTime = (float)SDL_GetTicks()/1000.0f;

        while (m_bGameRun)
        {
            processEvents();
            updateWorld();
            testCollisions();
            drawScreen();

            if (m_bStepMode)
                step();
        }

        for (unsigned int i=0 ; i<m_vAliens.size() ; ++i)
            delete m_vAliens[i];

        m_vAliens.clear();

        if ( m_bGameWin )
            gameWin();
        else
            gameOver();
    }
}



void Game::updateWorld()
{
    m_fActualTime = (float)SDL_GetTicks()/1000.0f;
    float dt = m_fActualTime - m_fLastFrameTime;
    m_fLastFrameTime = m_fActualTime;

    m_opSpaceShip->update(dt);
    m_opSpaceShipShot->update(dt);
    m_opBackground->update(dt);

    Alien::updateFormation(dt);
    for (unsigned int i=0 ; i<m_vAliens.size() ; ++i)
    {
        m_vAliens[i]->update(dt);
        if ( m_vAliens[i]->getState() == Alien::STATE_ATTACKING )
        {
            if ( ((float)rand() / RAND_MAX) < 0.0015f )
            {
                AlienShot* opShot;
                opShot = getInactiveAlienShot();
                if (opShot)
                    m_vAliens[i]->shot(opShot);
            }
        }
    }

    for (unsigned int i=0 ; i<m_vAliensShot.size() ; ++i)
        m_vAliensShot[i]->update(dt);

    // Check for game win
    if ( m_vAliens.size() == 0 )
    {
        m_bGameWin = true;
        m_bGameRun = false;
    }

    // Check for game over
    if ( m_opSpaceShip->getLives() <= 0 )
    {
        m_bGameWin = false;
        m_bGameRun = false;
    }

    if ( m_opSpaceShip->getState() != SpaceShip::STATE_DEFENDING )
        m_opSpaceShip->respawn();
}



void Game::drawScreen()
{
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    m_opBackground->draw();
    drawSpeed();
    m_opSpaceShip->draw();
    m_opSpaceShipShot->draw();

    for (unsigned int i=0 ; i<m_vAliens.size() ; ++i)
        m_vAliens[i]->draw();

    for (unsigned int i=0 ; i<m_vAliensShot.size() ; ++i)
        m_vAliensShot[i]->draw();

    SDL_GL_SwapBuffers();
}



void Game::drawSpeed()
{
    glLoadIdentity();

    int iMouseX;
    SDL_GetMouseState(&iMouseX, NULL);

    glBegin(GL_QUADS);
        glColor3f(1.0f, 1.0f, 1.0f);
        glVertex2f(SCREEN_WIDTH/2, 5.0f);
        glVertex2f(SCREEN_WIDTH/2, 20.0f);
        glVertex2f(iMouseX, 20.0f);
        glVertex2f(iMouseX, 5.0f);
    glEnd();

    glFlush();
}



void Game::startScreen()
{
    m_fLastFrameTime = (float)SDL_GetTicks()/1000.0f;

    SDL_Event event;
    bool bStart = false;


    while ( ! bStart )
    {
        while ( SDL_PollEvent( &event ) )
        {
            switch( event.type )
            {
                case SDL_KEYDOWN:
                {
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            exit(0);
                        break;
                    }
                }
                break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT)
                        bStart = true;
                break;
            }
        }

            m_fActualTime = (float)SDL_GetTicks()/1000.0f;
            float dt = m_fActualTime - m_fLastFrameTime;
            m_fLastFrameTime = m_fActualTime;
            m_opBackground->update(dt);

            glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
            m_opBackground->draw();
            m_opTitleSprite->draw(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, 1.0f, 0.0f);
            SDL_GL_SwapBuffers();
    }
    m_bPaused = false;
}



void Game::gameWin()
{
    GLSprite oGameWinSprite("resources/gamewin.png");

    Uint32 uiTime = SDL_GetTicks();
    float fScale = 0.0f;
    while ( (SDL_GetTicks() - uiTime) < 1000)
    {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        m_opBackground->draw();
        oGameWinSprite.draw(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, fScale, 0.0f);
        fScale += 1.0f / 30.0f;
        SDL_GL_SwapBuffers();
        SDL_Delay(30);
    }
    SDL_Delay(6000);
}



void Game::gameOver()
{
    GLSprite oGameOverSprite("resources/gameover.png");

    SDL_Delay(1000);
    Uint32 uiTime = SDL_GetTicks();
    float fScale = 0.0f;
    while ( (SDL_GetTicks() - uiTime) < 1000)
    {
        glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
        m_opBackground->draw();
        oGameOverSprite.draw(SCREEN_WIDTH/2, SCREEN_HEIGHT/2, fScale, 0.0f);
        fScale += 1.0f / 30.0f;
        SDL_GL_SwapBuffers();
        SDL_Delay(30);
    }
    SDL_Delay(5000);
}



void Game::processEvents()
{
    SDL_Event event;


    while( SDL_PollEvent( &event ) )
    {
        switch( event.type )
        {
            case SDL_KEYDOWN:
            {
                switch (event.key.keysym.sym)
                {
                    case SDLK_ESCAPE:
                        exit(0);
                    break;

                    case SDLK_r:
                        m_bGameWin = false;
                        m_bGameRun = false;
                    break;
                }
            }
            break;

            case SDL_MOUSEMOTION:
                m_opSpaceShip->setVel(((float)event.motion.x-400.0f)*4.0f);
            break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    if ( m_opSpaceShip->getState() == SpaceShip::STATE_DEFENDING )
                        m_opSpaceShipShot->start(m_opSpaceShip->getPosX());
                }
                else if (event.button.button == SDL_BUTTON_RIGHT)
                {
                    pause();
                }
                else if (event.button.button == SDL_BUTTON_MIDDLE)
                {
                    m_bStepMode = true;
                }
            break;

            case SDL_QUIT:
                exit (0);
            break;
        }
    }
}



void Game::testCollisions()
{
    // Check SpaceShipShot x Alien Collision
    if ( m_opSpaceShipShot->isActive() )
    {
        for (vector<Alien*>::iterator i=m_vAliens.begin() ; i!=m_vAliens.end() ; ++i)
        {
            if ( (*i)->testCollision(m_opSpaceShipShot) )
            {
                m_opAlienExp->play();
                GameObject* opObj = (*i);
                m_vAliens.erase(i);
                delete opObj;
                m_opSpaceShipShot->deactive();
                break;
            }
        }
    }

    // Check AlienShot x SpaceShip and AlienShot x SpaceShipShot Collision
    for (vector<AlienShot*>::iterator i=m_vAliensShot.begin() ; i!=m_vAliensShot.end() ; ++i)
    {
        if ( (*i)->isActive() && (*i)->testCollision(m_opSpaceShip) && m_opSpaceShip->getState() == SpaceShip::STATE_DEFENDING )
        {
            m_opShipExp->play();
            (*i)->deactive();
            m_opSpaceShip->die();
        }
        else if ( (*i)->isActive() && (*i)->testCollision(m_opSpaceShipShot) )
        {
            m_opSpaceShipShot->deactive();
            (*i)->deactive();
        }
    }

    // Check Alien x SpaceShip Collision
    for (vector<Alien*>::iterator i=m_vAliens.begin() ; i!=m_vAliens.end() ; ++i)
    {
        if ( (*i)->testCollision(m_opSpaceShip) && m_opSpaceShip->getState() == SpaceShip::STATE_DEFENDING )
        {
            m_opShipExp->play();
            GameObject* opObj = (*i);
            m_vAliens.erase(i);
            delete opObj;
            m_opSpaceShip->die();
            break;
        }
    }
}



AlienShot* Game::getInactiveAlienShot()
{
    for (vector<AlienShot*>::iterator i=m_vAliensShot.begin() ; i!=m_vAliensShot.end() ; ++i)
        if ( ! (*i)->isActive() )
            return (*i);
    return NULL;
}



Uint32 Game::shotCallback(Uint32 interval, void *param)
{
    if ( ! m_bPaused && m_vAliens.size()>0 )
    {
        int iNumShot = (rand() % 3);

        AlienShot* opShot;
        for (unsigned int i=0 ; i<iNumShot ; ++i)
        {
            opShot = getInactiveAlienShot();
            int alien = rand() % m_vAliens.size();
            if (opShot)
                m_vAliens[alien]->shot(opShot);
        }
    }

    return interval;
}



Uint32 Game::attackCallback(Uint32 interval, void *param)
{
    if (! m_bPaused && m_vAliens.size()>0)
    {
        int iNumAttack = (rand() % 3)+2;

        for (unsigned int i=0 ; i<iNumAttack ; ++i)
        {
            int alien = rand() % m_vAliens.size();
            m_vAliens[alien]->attack();
        }
    }

    return interval;
}



void Game::pause()
{
    SDL_Event event;
    m_bPaused = true;


    while ( m_bPaused )
    {
        while( SDL_PollEvent( &event ) )
        {
            switch( event.type )
            {
                case SDL_KEYDOWN:
                {
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            exit(0);
                        break;
                    }
                }
                break;

                case SDL_MOUSEMOTION:
                    m_opSpaceShip->setVel(((float)event.motion.x-400.0f)*4.0f);
                break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_RIGHT)
                        m_bPaused = false;
                break;
            }
        }
    }
    m_fLastFrameTime = (float)SDL_GetTicks()/1000.0f;
}



void Game::step()
{
    SDL_Event event;
    m_bPaused = true;
    bool paused = true;


    m_opSpaceShip->print();
    if ( m_opSpaceShipShot->isActive() )
        m_opSpaceShipShot->print();
    for (vector<Alien*>::iterator i=m_vAliens.begin() ; i!=m_vAliens.end() ; ++i)
        (*i)->print();
    for (vector<AlienShot*>::iterator i=m_vAliensShot.begin() ; i!=m_vAliensShot.end() ; ++i)
    {
        if ( (*i)->isActive() )
            (*i)->print();
    }

    while ( paused )
    {
        while( SDL_PollEvent( &event ) )
        {
            switch( event.type )
            {
                case SDL_KEYDOWN:
                {
                    switch (event.key.keysym.sym)
                    {
                        case SDLK_ESCAPE:
                            exit(0);
                        break;
                    }
                }
                break;

                case SDL_MOUSEMOTION:
                    m_opSpaceShip->setVel(((float)event.motion.x-400.0f)*4.0f);
                break;

                case SDL_MOUSEBUTTONDOWN:
                    if (event.button.button == SDL_BUTTON_LEFT)
                    {
                        m_opSpaceShipShot->start(m_opSpaceShip->getPosX());
                    }
                    if (event.button.button == SDL_BUTTON_MIDDLE)
                        paused = false;
                    else if (event.button.button == SDL_BUTTON_RIGHT)
                    {
                        paused = false;
                        m_bPaused = false;
                        m_bStepMode = false;
                    }
                break;
            }
        }
    }
    m_fLastFrameTime = (float)SDL_GetTicks()/1000.0f - 0.01f;
}
