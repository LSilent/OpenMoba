/////////////////////////////////////////////////////////////////////////////
// This file is part of the OpenMoba Project.
//
// OpenMoba is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// OpenMoba is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with OpenMoba. If not, see <http://www.gnu.org/licenses/>.
/////////////////////////////////////////////////////////////////////////////

#include <string>
#include "constants.h"
#include "functions.h"
#include "globals.h"

SDL_Surface *load_image( std::string filename )
{
    SDL_Surface* loadedImage = NULL;
    SDL_Surface* optimizedImage = NULL;
    loadedImage = IMG_Load( filename.c_str() );
    if( loadedImage != NULL )
    {
        optimizedImage = SDL_DisplayFormatAlpha( loadedImage );
        SDL_FreeSurface( loadedImage );
        if( optimizedImage != NULL )
            SDL_SetColorKey( optimizedImage, SDL_SRCCOLORKEY, SDL_MapRGB( optimizedImage->format, 0, 0xFF, 0xFF ) );
    }
    return optimizedImage;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination, SDL_Rect* clip )
{
    SDL_Rect offset;
    offset.x = x;
    offset.y = y;
    SDL_BlitSurface( source, clip, destination, &offset );
}

bool init()
{
    if (SDLNet_Init() < 0)
    {
        cerr << "SDLNet_Init(): " << SDLNet_GetError() << endl ;
        return false;
    }

    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
        return false;

    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );
    if( screen == NULL )
    {
        return false;
    }
    SDL_WM_SetCaption("OpenMoba", NULL );

    return true;
}

bool load_files()
{
    champion = load_image( "img/champion.png" );
    background = load_image( "img/bg.png" );

    if( champion == NULL )
        return false;

    if( background == NULL )
        return false;
    return true;
}

void clean_up()
{
    SDL_FreeSurface( champion );
    SDL_FreeSurface( background );
    SDL_Quit();
}

bool connect(char* adress, char* port)
{

    IPaddress ip;

    if (SDLNet_ResolveHost(&ip, adress, atoi(port)) < 0)
    {
        cerr << "SDLNet_ResolveHost(): "<< SDLNet_GetError() << endl;
        return false;
    }

    if (!(socket = SDLNet_TCP_Open(&ip)))
    {
        cout << "error";
        cerr << "SDLNet_TCP_Open: " << SDLNet_GetError() << endl;
        return false;
    }

    if (SDLNet_TCP_Send(socket, "Connected", strlen("Connected")+1) < strlen("Connected")+1) {

	    cerr << "SDLNet_TCP_Send: " << SDLNet_GetError() << endl;;
	    exit(1);
	}

    return true;
}

void disconnect()
{
    SDLNet_TCP_Close(socket);
    SDLNet_Quit();
}
