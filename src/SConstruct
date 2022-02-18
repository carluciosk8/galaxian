##***************************************************************************
##    Original code written in: Apr 2006    
##    copyright: (C) Carlucio Santos Cordeiro
##***************************************************************************
sources = [
'src/Alien.cpp',
'src/AlienShot.cpp',
'src/Background.cpp',
'src/GLSprite.cpp',
'src/Game.cpp',
'src/GameObject.cpp',
'src/Music.cpp',
'src/Sound.cpp',
'src/SpaceShip.cpp',
'src/SpaceShipShot.cpp',
'src/main.cpp'
]

libs = ['GL', 'GLU', 'SDL', 'SDL_image', 'SDL_mixer']


Program('galaxian', sources, LIBS=libs, CCFLAGS='`sdl-config --cflags`')
