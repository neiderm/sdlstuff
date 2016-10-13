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
/* static */ SDL_Renderer* gRenderer;

//Globally used font
/* static */ TTF_Font *gFont; // TODO: nasty globals

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



static SDL_Surface *gSurface;



static void close()
{
    /*
        SDL_FreeSurface( gSurface ); // I guess
    */

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



// TODO: nasty globalz

static int Color_registers[256];
unsigned char Current_color = 0xff;

//static unsigned char _ouble_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];
static unsigned char _ouble_buffer[SCREEN_WIDTH * (SCREEN_HEIGHT + 1)];
unsigned char *double_buffer;     // the double buffer
unsigned int *video_buffer;

unsigned char *junk ;

// this functions copies the double buffer into the video buffer at the
// starting y location
//
void Display_Double_Buffer(unsigned char *buffer,int y)
{
    int i;

junk =  &double_buffer[400 * SCREEN_WIDTH];
int asdf = *(junk + 1);
if (asdf != 0xa5)
  printf("Invalide value at %X = %X\n", junk, asdf);

    for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        video_buffer[i] = double_buffer[i];


        // hack for wire demo, since the color is fixed to the PLG file somehow.
        if (0 != double_buffer[i])  video_buffer[i] = Color_registers[Current_color]; //  0xff; // TODO: palette LUT
    }
}

//
// must be called AFTER renderer is created if instantiating gSurface here !
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
    double_buffer = _ouble_buffer;
#endif

    // set the height of the buffer and compute it's size
    num_lines = SCREEN_HEIGHT; // tmp

    // fill the buffer with black
    memset(double_buffer, 0, SCREEN_WIDTH * num_lines);

int junk = sizeof(_ouble_buffer);
    memset(double_buffer, 0xa5, junk);

#if 0
    /*  using the default masks for the depth: */
    gSurface = SDL_CreateRGBSurface(0,SCREEN_WIDTH,SCREEN_HEIGHT,32,0,0,0,0);
#endif

    // everything was ok
    return(1);

} // end Init_Double_Buffer


// this function fills in the double buffer with the sent color
//
void Fill_Double_Buffer(int color)
{
    memset(double_buffer, color, SCREEN_WIDTH * SCREEN_HEIGHT);
}




///////////////////////////////////////////////////////////////////////////////
// this function will write to the color registers using the data in the
// sen palette structure
//
void Write_Palette(int start_reg, int end_reg, RGB_palette_ptr the_palette)
{

    int index; // used for looping

// write all the registers

    for (index=start_reg; index<=end_reg; index++)
    {
        // write the color registers using the data from the sent palette

//        Write_Color_Reg(index,(RGB_color_ptr)&(the_palette->colors[index]));
          Color_registers[index] = 0x00000000;
          // scale up the palette colors from 6-bits to 8-bits intensity
          Color_registers[index] |= (the_palette->colors[index].red   * 4) << 16;
          Color_registers[index] |= (the_palette->colors[index].green * 4) <<  8;
          Color_registers[index] |= (the_palette->colors[index].blue  * 4) <<  0;
    } // end for index

} // end Write_Palette





int wd_main( int argc, char* argv[] )
{
    object test_object;   // the test object

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

            bool pause_rotation = 0;

            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );

            // position the object
            Position_Object((object_ptr)&test_object,0,0,300);

            // create the sin/cos lookup tables used for the rotation function
            Build_Look_Up_Tables();

            // allocate double buffer
            Create_Double_Buffer(0); // height/width are fixed


            // read the color palette off disk
            Load_Palette_Disk("standard.pal",(RGB_palette_ptr)&color_palette_3d);
            Write_Palette(0,255,(RGB_palette_ptr)&color_palette_3d);


            // set viewing distance
            viewing_distance = 250;

            SDL_Texture *newTexture;

            int x = 0, y = 0; // tmp

            //While application is running
            while( !quit )
            {
                int index;   // looping variable
                char buffer[80]; // used to print strings

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
                        x += 1;
                        y += 1;
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

                        case SDLK_EQUALS: // SDLK_PLUS:
                            Current_color += 1;
                            break;

                        case SDLK_MINUS:
                            Current_color -= 1;
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
                /*
                                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                                SDL_RenderClear( gRenderer );
                */
                // erase the screen
                Fill_Double_Buffer(0);

                // draw the object
                Draw_Object_Wire((object_ptr)&test_object);


// test code
                if (x < SCREEN_WIDTH && y < SCREEN_HEIGHT)
                    double_buffer[SCREEN_WIDTH * y + x] = 0xff;
                if (x < SCREEN_WIDTH)
                    Draw_Line(x, 0, x, SCREEN_HEIGHT,  x, double_buffer);
                if (y < SCREEN_HEIGHT)
                    Draw_Line(0, y, (SCREEN_WIDTH * 3) / 4, y,  0xff, double_buffer);



                /*  using the default masks for the depth: */
                gSurface = SDL_CreateRGBSurface(0,SCREEN_WIDTH,SCREEN_HEIGHT,32,0,0,0,0); // SDL_CreateRGBSurfaceFrom ????

                video_buffer = (unsigned int *)gSurface->pixels;

                Display_Double_Buffer(double_buffer,0);

                //Create texture from surface pixels
                newTexture = SDL_CreateTextureFromSurface( gRenderer, gSurface );

                SDL_FreeSurface( gSurface ); // done with it now so free it

                //Render texture to screen
                SDL_RenderCopy( gRenderer, newTexture, NULL, NULL );

                SDL_DestroyTexture( newTexture ); // GN: idfk


                // print out position of object
                sprintf(buffer, "C=%d Object at (%d,%d,%d)",
                        Current_color,
                        (int)test_object.world_pos.x,
                        (int)test_object.world_pos.y,
                        (int)test_object.world_pos.z);

                //Render text
                if( !gTextTexture.loadFromRenderedText( buffer, { 255, 0, 0 } ) )
                {
                    printf( "Failed to render text texture!\n" );
                }

                gTextTexture.render( 0, 0 );


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
