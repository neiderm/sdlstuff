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



SDL_Surface *gSurface;



static void close()
{

SDL_FreeSurface( gSurface ); // I guess


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



unsigned int _ouble_buffer[SCREEN_WIDTH * (SCREEN_HEIGHT + 1)];
unsigned char *double_buffer;     // the double buffer
unsigned int double_buffer_height;    // height of double buffer
unsigned int double_buffer_size;      // total size of buffer in bytes



//
// must be called AFTER renderer is created!
//
int Create_Double_Buffer(int num_lines)
{
// allocate enough memory to hold the double buffer
#if 0
    if ((double_buffer = (unsigned char far *)_fmalloc(SCREEN_WIDTH * (num_lines + 1)))==NULL)
    {
        printf("\nCouldn't allocate double buffer.");
        return(0);
    }    // end if couldn't allocate
#else
//    double_buffer = _ouble_buffer;
#endif
    // set the height of the buffer and compute it's size
    double_buffer_height = num_lines;
    double_buffer_size = SCREEN_WIDTH * num_lines/2;

    // fill the buffer with black
    memset(double_buffer, 0, SCREEN_WIDTH * num_lines);



#if 1
    /* or using the default masks for the depth: */
    gSurface = SDL_CreateRGBSurface(0,SCREEN_WIDTH,SCREEN_HEIGHT,32,0,0,0,0);

#else

SDL_Surface* SDL_CreateRGBSurfaceFrom(_ouble_buffer, // void*  pixels,
                                      SCREEN_WIDTH, // int    width,
                                      SCREEN_HEIGHT, // int    height,
                                      32, // int    depth,
                                      int    pitch,
                                      Uint32 Rmask,
                                      Uint32 Gmask,
                                      Uint32 Bmask,
                                      Uint32 Amask)

#endif


    // everything was ok
    return(1);

} // end Init_Double_Buffer



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


            // allocate double buffer
            int h = 0; // tmp TODO
            Create_Double_Buffer(h); // needs to be part of init() ... depends on height/width


            // set viewing distance
            viewing_distance = 250;

            bool pause_rotation = 0;


SDL_Texture *newTexture;


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

                        case SDLK_p:
                            pause_rotation ^= 1;
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
                if (!pause_rotation)
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




                //Clear screen
                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                SDL_RenderClear( gRenderer );



        //Create texture from surface pixels
        newTexture = SDL_CreateTextureFromSurface( gRenderer, gSurface );

// now Draw_Object_Wire() etc or whatever other screen drawing


                // display double buffer
                //Render texture to screen
                SDL_RenderCopy( gRenderer, newTexture, NULL, NULL );

SDL_DestroyTexture( newTexture ); // GN: idfk



                // draw the object
                Draw_Object_Wire((object_ptr)&test_object);




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
