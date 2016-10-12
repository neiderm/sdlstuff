/*This source code copyrighted by Lazy Foo' Productions (2004-2015)
and may not be redistributed without written permission.*/

//Using SDL, SDL_image, SDL_ttf, standard IO, math, and strings
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <stdio.h>
#include <string>

#include "black3.h" // RGB_palette_ptr
#include "black4.h" // sprite
//#include "black5.h"
//#include "black6.h"
//#include "black8.h"
//#include "black9.h"
#include "black11.h"

#include "ltexture.h"

//Screen dimension constants
static const int SCREEN_WIDTH = 640;
static const int SCREEN_HEIGHT = 480;
static const int SCREEN_FPS = 60;
static const int SCREEN_TICK_PER_FRAME = 1000 / SCREEN_FPS;


//Starts up SDL and creates window
static bool init();

//Loads media
static bool loadMedia();

//Frees media and shuts down SDL
static void close();

//The window we'll be rendering to
static SDL_Window* gWindow = NULL;

//The window renderer
extern SDL_Renderer* gRenderer; // TODO: nasty globals

//Globally used font
extern TTF_Font *gFont; // TODO: nasty globals

//Rendered texture
static LTexture gTextTexture;



static bool init()
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


static bool loadMedia()
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


static void close()
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



object test_object;   // the test object



int wd_main( int argc, char* argv[] )
{
    int index;   // looping variable
    char buffer[80]; // used to print strings

    // try this one first, if it fails, no other cleanup needed at this point
    if (!PLG_Load_Object(&test_object,argv[1],1))
        return 1; // epic fail

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



            // position the object
            test_object.world_pos.x = 0;
            test_object.world_pos.y = 0;
            test_object.world_pos.z = 300;

            // create the sin/cos lookup tables used for the rotation function
            Build_Look_Up_Tables();

            // set viewing distance
            viewing_distance = 250;


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
                            test_object.world_pos.y-=5;
                            break;

                        case SDLK_DOWN:
                            test_object.world_pos.y+=5;
                            break;

                        case SDLK_LEFT:
                            test_object.world_pos.x-=5;
                            break;

                        case SDLK_RIGHT:

                            test_object.world_pos.x+=5;
                            break;

                        case SDLK_PAGEUP:
                            test_object.world_pos.z+=15;
                            break;

                        case SDLK_PAGEDOWN:
                            test_object.world_pos.z-=15;
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
                            break;

                        case SDLK_DOWN:
                            break;

                        case SDLK_LEFT:
                            break;

                        case SDLK_RIGHT:
                            break;

                        default:
                            break;
                        }
                    }
                }



                // rotate the object on all three axes
                Rotate_Object((object_ptr)&test_object,2,4,6);


                // convert the local coordinates into camera coordinates for projection
                // note the viewer is at (0,0,0) with angles 0,0,0 so the transformaton
                // is simply to add the world position to each local vertex

                for (index=0; index<test_object.num_vertices; index++)
                {
                    test_object.vertices_camera[index].x =
                        test_object.vertices_local[index].x+test_object.world_pos.x;

                    test_object.vertices_camera[index].y =
                        test_object.vertices_local[index].y+test_object.world_pos.y;

                    test_object.vertices_camera[index].z =
                        test_object.vertices_local[index].z+test_object.world_pos.z;

                } // end for index



                // draw the object
                Draw_Object_Wire((object_ptr)&test_object);


                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );


                // print out position of object
                sprintf(buffer,"Object is at (%d,%d,%d)     ",(int)test_object.world_pos.x,
                        (int)test_object.world_pos.y,
                        (int)test_object.world_pos.z);

                //Render text
                if( !gTextTexture.loadFromRenderedText( buffer, { 255, 0, 0 } ) )
                {
                    printf( "Failed to render text texture!\n" );
                }
                int x = 0, y = 0;
                gTextTexture.render( x, y );


                // display double buffer



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
