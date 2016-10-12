/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, SDL_ttf, standard IO, math, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>

#include "ltexture.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int SCREEN_FPS = 60;
const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;


//Starts up SDL and creates window
bool init();

//Loads media
bool loadMedia();

//Frees media and shuts down SDL
void close();

//The window we'll be rendering to
SDL_Window* gWindow = NULL;

//The window renderer
SDL_Renderer* gRenderer = NULL;

//Globally used font
TTF_Font *gFont = NULL;

//Rendered texture
LTexture gTextTexture;



bool init()
{
    //Initialization flag
    bool success = true;

    //Initialize SDL
    if( SDL_Init( SDL_INIT_VIDEO ) < 0 )
    {
        printf( "SDL could not initialize! SDL Error: %s\n", SDL_GetError() );
        success = false;
    }
    else
    {
        //Set texture filtering to linear
        if( !SDL_SetHint( SDL_HINT_RENDER_SCALE_QUALITY, "1" ) )
        {
            printf( "Warning: Linear texture filtering not enabled!" );
        }

        //Create window
        gWindow = SDL_CreateWindow( "SDL Tutorial", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN );
        if( gWindow == NULL )
        {
            printf( "Window could not be created! SDL Error: %s\n", SDL_GetError() );
            success = false;
        }
        else
        {
            //Create vsynced renderer for window
            gRenderer = SDL_CreateRenderer( gWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC );

            if( gRenderer == NULL )
            {
                printf( "Renderer could not be created! SDL Error: %s\n", SDL_GetError() );
                success = false;
            }
            else
            {
                //Initialize renderer color
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );

                //Initialize PNG loading
                int imgFlags = IMG_INIT_PNG;
                if( !( IMG_Init( imgFlags ) & imgFlags ) )
                {
                    printf( "SDL_image could not initialize! SDL_image Error: %s\n", IMG_GetError() );
                    success = false;
                }

                //Initialize SDL_ttf
                if( TTF_Init() == -1 )
                {
                    printf( "SDL_ttf could not initialize! SDL_ttf Error: %s\n", TTF_GetError() );
                    success = false;
                }
            }
        }
    }

    return success;
}

bool loadMedia()
{
    //Loading success flag
    bool success = true;

    //Open the font
    gFont = TTF_OpenFont( "lazy.ttf", 28 );

    if( gFont == NULL )
    {
        printf( "Failed to load lazy font! SDL_ttf Error: %s\n", TTF_GetError() );
        success = false;
    }
    else
    {
        //Render text
        SDL_Color textColor = { 0, 0, 0 };
        if( !gTextTexture.loadFromRenderedText( "The quick brown fox jumps over the lazy dog", textColor ) )
        {
            printf( "Failed to render text texture!\n" );
            success = false;
        }
    }

    return success;
}

void close()
{
    //Free loaded images
    gTextTexture.free();

    //Free global font
    TTF_CloseFont( gFont );
    gFont = NULL;

    //Destroy window
    SDL_DestroyRenderer( gRenderer );
    SDL_DestroyWindow( gWindow );
    gWindow = NULL;
    gRenderer = NULL;

    //Quit SDL subsystems
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
}


typedef struct
{
    //The X and Y offsets of the dot
    int mPosX, mPosY;
    int mDeltaX, mDeltaY;
    int mS; // total distance (pixels) moved in any direction
} dot_t;


int lazy_main( int argc, char* args[] )
{
    char textbuf[256];
    dot_t dot;
    dot.mS = 0;
    dot.mPosX = 1;
    dot.mPosY = 1;
    dot.mDeltaX = 0;
    dot.mDeltaY = 0;

    //Start up SDL and create window
    if( !init() )
    {
        printf( "Failed to initialize!\n" );
    }
    else
    {
        //Load media
        if( !loadMedia() )
        {
            printf( "Failed to load media!\n" );
        }
        else
        {
            //Main loop flag
            bool quit = false;

            //Event handler
            SDL_Event e;

            //The clock time when the timer started
            int mStartTicks;


            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );


            //While application is running
            while( !quit )
            {
                //Start cap timer
                //Get the current clock time
                mStartTicks = SDL_GetTicks(); // capTimer.start();


                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    //User presses a key
                    else if( e.type == SDL_KEYDOWN )
                    {
                        switch( e.key.keysym.sym )
                        {
                        case SDLK_UP:
                            //if (dot.mPosY > 0)
                            dot.mDeltaY = -1;
                            break;

                        case SDLK_DOWN:
                            //if (dot.mPosY < SCREEN_HEIGHT)
                            dot.mDeltaY = +1;
                            break;

                        case SDLK_LEFT:
                            //if (dot.mPosX > 0)
                            dot.mDeltaX = -1;
                            break;

                        case SDLK_RIGHT:
                            //if (dot.mPosY < SCREEN_WIDTH)
                            dot.mDeltaX = +1;
                            break;

                        case SDLK_PLUS:
                            break;

                        case SDLK_MINUS:
                            break;

                        case SDLK_ESCAPE:
                            quit = true;
                            break;

                        default:
                            break;
                        }
                    }
                    else if( e.type == SDL_KEYUP )
                    {
                        switch( e.key.keysym.sym )
                        {
                        case SDLK_UP:
                            if (dot.mDeltaY == -1)  dot.mDeltaY = 0;
                            break;

                        case SDLK_DOWN:
                            if (dot.mDeltaY == +1)  dot.mDeltaY = 0;
                            break;

                        case SDLK_LEFT:
                            if (dot.mDeltaX == -1)  dot.mDeltaX = 0;
                            break;

                        case SDLK_RIGHT:
                            if (dot.mDeltaX == +1)  dot.mDeltaX = 0;
                            break;

                        default:
                            break;
                        }
                    }
                }
                int tmp;
                int oldX = dot.mPosX;
                int oldY = dot.mPosY;

                tmp = dot.mPosX + dot.mDeltaX;
                if (tmp >= 0 && tmp < SCREEN_WIDTH)
                    dot.mPosX = tmp;

                tmp = dot.mPosY + dot.mDeltaY;
                if (tmp >= 0 && tmp < SCREEN_HEIGHT)
                    dot.mPosY = tmp;

                if (dot.mPosX != oldX  ||  dot.mPosY != oldY )
                    dot.mS += 1;

                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );



                //Render X/Y text
                SDL_Color textColor = { 0, 0, 0 };
                sprintf(textbuf, "x=%d, y=%d", dot.mPosX , dot.mPosY);
                if( !gTextTexture.loadFromRenderedText( textbuf, { 0, 0, 255 } ) )
                {
                    printf( "Failed to render text texture!\n" );
                }
                gTextTexture.render( SCREEN_WIDTH - gTextTexture.getWidth(),
                                     SCREEN_HEIGHT - gTextTexture.getHeight() );


                //Render Ds text
                sprintf(textbuf, "dot.mS=%d", dot.mS);
                if( !gTextTexture.loadFromRenderedText( textbuf, { 255, 0, 0 } ) )
                {
                    printf( "Failed to render text texture!\n" );
                }
                gTextTexture.render( ( SCREEN_WIDTH - gTextTexture.getWidth() ) / 2, ( SCREEN_HEIGHT - gTextTexture.getHeight() ) / 2 );


                //Draw previous dot
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
                SDL_RenderDrawPoint( gRenderer, oldX, oldY );

                //Draw dot
                SDL_SetRenderDrawColor( gRenderer, 0x00, 0x00, 0x00, 0xFF );
                SDL_RenderDrawPoint( gRenderer, dot.mPosX , dot.mPosY );


                //Update screen
                SDL_RenderPresent( gRenderer );


                //If frame finished early
                int frameTicks = SDL_GetTicks() - mStartTicks; // capTimer.getTicks();
                if( frameTicks < SCREEN_TICK_PER_FRAME )
                {
                    //Wait remaining time
                    SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
                }
            }
        }
    }

    //Free resources and close SDL
    close();

    return 0;
}
