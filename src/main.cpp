/***************************************************************************
    Original code written in: Apr 2006    
    copyright: (C) Carlucio Santos Cordeiro
 ***************************************************************************/
#include <iostream>
#include <stdexcept>

using std::runtime_error;
using std::cerr;
using std::endl;

#include "Game.h"


int main(int argc, char** argv)
{
    Game*  opGalaxian;

    try
    {
        opGalaxian = new Game();
        opGalaxian->run();
    }
    catch (runtime_error err)
    {
        cerr << "Runtime error: " << err.what() << endl;
    }

    return 0;
}
