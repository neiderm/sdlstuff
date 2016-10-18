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
#include "black15.h"

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


extern dir_3d view_angle;



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
static unsigned char Current_color = 0xff;

static unsigned char _ouble_buffer[SCREEN_WIDTH * SCREEN_HEIGHT];

unsigned char *double_buffer;     // the double buffer
unsigned int *video_buffer;



// this functions copies the double buffer into the video buffer at the
// starting y location
//
void Display_Double_Buffer(unsigned char *buffer,int y)
{
    int i;

    for (i = 0; i < SCREEN_WIDTH * SCREEN_HEIGHT; i++)
    {
        video_buffer[i] = Color_registers[ double_buffer[i] ];
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
    memset(double_buffer, 0xab, SCREEN_WIDTH * num_lines);

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


// this function is used to write a color register with the RGB value
// within "color"
//
void Write_Color_Reg(int index, RGB_color_ptr color)
{
    Color_registers[index] = 0x00000000;
    // scale up the palette colors from 6-bits to 8-bits intensity
    Color_registers[index] |= color->red   * 4 << 16;
    Color_registers[index] |= color->green * 4 <<  8;
    Color_registers[index] |= color->blue  * 4 <<  0;
}



///////////////////////////////////////////////////////////////////////////////
// this function will write to the color registers using the data in the
// sen palette structure
//
void Write_Palette(int start_reg, int end_reg, RGB_palette_ptr the_palette)
{

    int index;

// write all the registers
    for (index=start_reg; index<=end_reg; index++)
    {
        // write the color registers using the data from the sent palette
        Write_Color_Reg(index,(RGB_color_ptr)&(the_palette->colors[index]));
    }
}


/*
* tmp data structure for saving triangle
*/
typedef struct pnt2d_t
{
    int x;
    int y;
} t_pnt2d;
typedef struct mytri_t
{
    t_pnt2d p0;
    t_pnt2d p1;
    t_pnt2d p2;
    int clr;
} t_mytri;



int wd_main( int argc, char* argv[] )
{
    object test_object[4];   // the test object
    int index;

// load in the objects from the command line
    for (index=0; index<4; index++)
    {
        if (!PLG_Load_Object((object_ptr)&test_object[index],argv[1],1))
        {
            printf("\nCouldn't find file %s",argv[1]);
            return 1;

        } // end if

    } // end for index



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

            bool pause_rotation = 1;
            bool draw_frame = true; // for testing/debugging, only refresh if 'r' key


            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );

            // position the object
            for (index=0; index<4; index++)
                Position_Object((object_ptr)&test_object[index],-150+index*100,0,300);


// set the viewpoint
            view_point.x = 0;
            view_point.y = 0;
            view_point.z = 0;


            // create the sin/cos lookup tables used for the rotation function
            Build_Look_Up_Tables();

            // allocate double buffer
            Create_Double_Buffer(0); // height/width are fixed


            // read the color palette off disk
            Load_Palette_Disk("data/standard.pal",(RGB_palette_ptr)&color_palette_3d);
            Write_Palette(0,255,(RGB_palette_ptr)&color_palette_3d);


            // set viewing distance
            viewing_distance = 250;



            int line_x = 0, line_y = 0; // test 2d lines
            int xr, yr, zr;
            mytri_t gtri; // test the 2d triangle

            //While application is running
            while( !quit )
            {
                SDL_Texture *newTexture;

                char buffer[80]; // used to print strings

                //Start cap timer
                //Get the current clock time
                mStartTicks = SDL_GetTicks(); // capTimer.start();



                //Clear screen
                /*
                                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                                SDL_RenderClear( gRenderer );
                */
                // erase the screen
                Fill_Double_Buffer(0);

                xr = 0;
                yr = 0;
                zr = 0;

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
                        line_x += 1;
                        line_y += 1;

                        switch( e.key.keysym.sym )
                        {
                        case SDLK_UP:
                            view_point.y+=5;
                            break;

                        case SDLK_DOWN:
                            view_point.y-=5;
                            break;

                        case SDLK_RIGHT:
                            view_point.x+=5;
                            break;

                        case SDLK_LEFT:
                            view_point.x-=5;
                            break;

                        case SDLK_PAGEUP:
                            view_point.z+=5;
                            break;

                        case SDLK_PAGEDOWN:
                            view_point.z-=5;
                            break;


                        case SDLK_z:
                            if ((view_angle.ang_x+=5)>360)
                                view_angle.ang_x = 0;
                            break;

                        case SDLK_a:
                            if ((view_angle.ang_x-=5)<0)
                                view_angle.ang_x = 360;
                            break;

                        case SDLK_x:
                            if ((view_angle.ang_y+=5)>360)
                                view_angle.ang_y = 0;
                            break;

                        case SDLK_s:
                            if ((view_angle.ang_y-=5)<0)
                                view_angle.ang_y = 360;
                            break;

                        case SDLK_c:
                            if ((view_angle.ang_z+=5)>360)
                                view_angle.ang_z = 0;
                            break;

                        case SDLK_d:
                            if ((view_angle.ang_z-=5)<0)
                                view_angle.ang_z = 360;
                            break;


                        case SDLK_p:
                            pause_rotation ^= 1;
                            break;

                        case SDLK_r:
                            draw_frame = true;
                            break;
#if 0
                        case SDLK_x:
                            xr = 2;
                            break;

                        case SDLK_y:
                            yr = 2;
                            break;

                        case SDLK_z:
                            zr = 2;
                            break;
#endif

                        case SDLK_EQUALS: // SDLK_PLUS:
                            Current_color += 1;
                            break;

                        case SDLK_MINUS:
                            Current_color -= 1;
                            break;

                        case SDLK_ESCAPE:
                            quit = true;
                            break;

                        case SDLK_t:
                            gtri.p0.x = rand()%SCREEN_WIDTH;
                            gtri.p0.y = rand()%SCREEN_HEIGHT;
                            gtri.p1.x = rand()%SCREEN_WIDTH;
                            gtri.p1.y = rand()%SCREEN_HEIGHT;
                            gtri.p2.x = rand()%SCREEN_WIDTH;
                            gtri.p2.y = rand()%SCREEN_HEIGHT;
                            gtri.clr = Current_color; // rand()%256;
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


                /*
                                if (!draw_frame)
                                    continue;
                                draw_frame = false;
                */

                // draw the 2d triangle at its current location
                Draw_Triangle_2D(gtri.p0.x, gtri.p0.y,
                                 gtri.p1.x, gtri.p1.y,
                                 gtri.p2.x, gtri.p2.y,
                                 gtri.clr);

                // draw some 2d lines
                if (line_x < SCREEN_WIDTH)
                    Draw_Line(line_x, 0, line_x, SCREEN_HEIGHT,  0xba, double_buffer); // ha ha should be (SCREEN_HEIGHT-1)
                if (line_y < SCREEN_HEIGHT)
                    Draw_Line(0, line_y, (SCREEN_WIDTH * 3) / 4, line_y,  0xab, double_buffer);


                // rotate the object on all three axes
//                if (!pause_rotation)
//                  Rotate_Object((object_ptr)&test_object, xr, yr, zr);

                // create the global world to camera transformation matrix
                Create_World_To_Camera();

                for (index=0; index<4; index++)
                {
                    // test if this object should be processed
                    if (!Remove_Object(&test_object[index],OBJECT_CULL_XYZ_MODE))
                    {
                        // convert to world coordinates
                        Local_To_World_Object((object_ptr)&test_object[index]);

                        // shade and remove backfaces, ignore the backface part for now
                        // notice that backface shadin and backface removal is done in world coordinates
                        Remove_Backfaces_And_Shade((object_ptr)&test_object[index]);

                        // convert to camera coordinates
                        World_To_Camera_Object((object_ptr)&test_object[index]);

                        // draw the object
                        Draw_Object_Solid((object_ptr)&test_object[index]);

                    } // end if object is visible
                    else
                    {
                        sprintf(buffer,"%d, ",index);
                    }
                } // end for index


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
                sprintf(buffer, "C=%d", Current_color);

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


#define NR_OBJS 16
//extern vector_3d light_source;
int poly_main( int argc, char* argv[] )
{
    object test_objects[NR_OBJS];   // the test object
    int index;

    // load in the objects from the command line
    for (index=0; index<NR_OBJS; index++)
    {
        if (!PLG_Load_Object((object_ptr)&test_objects[index],argv[1],1))
        {
            printf("\nCouldn't find file %s",argv[1]);
            return 1;

        } // end if

    } // end for index



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

            bool pause_rotation = 1;
            bool draw_frame = true; // for testing/debugging, only refresh if 'r' key


            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );

            // position the object
            for (index=0; index<NR_OBJS; index++)
            {
                //Position_Object((object_ptr)&test_object[index],-150+index*100,0,300);
                test_objects[index].world_pos.x=-200 + (index%4)*100;
                test_objects[index].world_pos.y=0;
                test_objects[index].world_pos.z=200 + 300*(index>>2);
            }

            // set the viewpoint
            view_point.x = 0;
            view_point.y = 0;
            view_point.z = 0;


            // create the sin/cos lookup tables used for the rotation function
            Build_Look_Up_Tables();

            // allocate double buffer
            Create_Double_Buffer(0); // height/width are fixed


            // read the color palette off disk
            Load_Palette_Disk("data/standard.pal",(RGB_palette_ptr)&color_palette_3d);
            Write_Palette(0,255,(RGB_palette_ptr)&color_palette_3d);


            // set viewing distance
            viewing_distance = 250;



            int line_x = 0, line_y = 0; // test 2d lines
            int xr, yr, zr;
            mytri_t gtri; // test the 2d triangle

            //While application is running
            while( !quit )
            {
                SDL_Texture *newTexture;

                char buffer[80]; // used to print strings

                //Start cap timer
                //Get the current clock time
                mStartTicks = SDL_GetTicks(); // capTimer.start();



                //Clear screen
                /*
                                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                                SDL_RenderClear( gRenderer );
                */
                // erase the screen
                Fill_Double_Buffer(0);

                xr = 0;
                yr = 0;
                zr = 0;

                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    float x,y,z;     // working variables

                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    //User presses a key
                    else if( e.type == SDL_KEYDOWN )
                    {
                        line_x += 1;
                        line_y += 1;

                        switch( e.key.keysym.sym )
                        {

                            // move the light source

                        case SDLK_t:
                        {
                            x = light_source.x;

                            y = cos(.2)*light_source.y - sin(.2) * light_source.z;
                            z = sin(.2)*light_source.y + cos(.2) * light_source.z;

                            light_source.y = y;
                            light_source.z = z;
                        }
                        break;

                        case SDLK_g:
                        {
                            y = light_source.y;

                            x = cos(.2)*light_source.x + sin(.2) * light_source.z;
                            z = -sin(.2)*light_source.x + cos(.2) * light_source.z;

                            light_source.x = x;
                            light_source.z = z;
                        }
                        break;

                        case SDLK_b:
                        {

                            z = light_source.z;

                            x = cos(.2)*light_source.x - sin(.2) * light_source.y;
                            y = sin(.2)*light_source.x + cos(.2) * light_source.y;

                            light_source.x = x;
                            light_source.y = y;
                        }
                        break;

                        case SDLK_y:
                        {
                            x = light_source.x;

                            y = cos(-.2)*light_source.y - sin(-.2) * light_source.z;
                            z = sin(-.2)*light_source.y + cos(-.2) * light_source.z;

                            light_source.y = y;
                            light_source.z = z;
                        }
                        break;

                        case SDLK_h:
                        {
                            y = light_source.y;

                            x = cos(-.2)*light_source.x + sin(-.2) * light_source.z;
                            z = -sin(-.2)*light_source.x + cos(-.2) * light_source.z;

                            light_source.x = x;
                            light_source.z = z;
                        }
                        break;

                        case SDLK_n:
                        {
                            z = light_source.z;

                            x = cos(-.2)*light_source.x - sin(-.2) * light_source.y;
                            y = sin(-.2)*light_source.x + cos(-.2) * light_source.y;

                            light_source.x = x;
                            light_source.y = y;
                        }
                        break;

                        case SDLK_UP:
                            view_point.y+=20;
                            break;

                        case SDLK_DOWN:
                            view_point.y-=20;
                            break;

                        case SDLK_RIGHT:
                            view_point.x+=20;
                            break;

                        case SDLK_LEFT:
                            view_point.x-=20;
                            break;

                        case SDLK_KP_PLUS:
                            view_point.z+=20;
                            break;

                        case SDLK_KP_MINUS:
                            view_point.z-=20;
                            break;

                        case SDLK_z:
                            if ((view_angle.ang_x+=5)>360)
                                view_angle.ang_x = 0;
                            break;

                        case SDLK_a:
                            if ((view_angle.ang_x-=5)<0)
                                view_angle.ang_x = 360;
                            break;

                        case SDLK_x:
                            if ((view_angle.ang_y+=5)>360)
                                view_angle.ang_y = 0;
                            break;

                        case SDLK_s:
                            if ((view_angle.ang_y-=5)<0)
                                view_angle.ang_y = 360;
                            break;

                        case SDLK_c:
                            if ((view_angle.ang_z+=5)>360)
                                view_angle.ang_z = 0;
                            break;

                        case SDLK_d:
                            if ((view_angle.ang_z-=5)<0)
                                view_angle.ang_z = 360;
                            break;


                        case SDLK_p:
                            pause_rotation ^= 1;
                            break;

                        case SDLK_r:
                            draw_frame = true;
                            break;
#if 0
                        case SDLK_x:
                            xr = 2;
                            break;

                        case SDLK_y:
                            yr = 2;
                            break;

                        case SDLK_z:
                            zr = 2;
                            break;
#endif

                        case SDLK_EQUALS: // SDLK_PLUS:
                            Current_color += 1;
                            break;

                        case SDLK_MINUS:
                            Current_color -= 1;
                            break;

                        case SDLK_ESCAPE:
                            quit = true;
                            break;

                        case SDLK_SLASH:
                            gtri.p0.x = rand()%SCREEN_WIDTH;
                            gtri.p0.y = rand()%SCREEN_HEIGHT;
                            gtri.p1.x = rand()%SCREEN_WIDTH;
                            gtri.p1.y = rand()%SCREEN_HEIGHT;
                            gtri.p2.x = rand()%SCREEN_WIDTH;
                            gtri.p2.y = rand()%SCREEN_HEIGHT;
                            gtri.clr = Current_color; // rand()%256;
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


                /*
                                if (!draw_frame)
                                    continue;
                                draw_frame = false;
                */

                // draw the 2d triangle at its current location
                Draw_Triangle_2D(gtri.p0.x, gtri.p0.y,
                                 gtri.p1.x, gtri.p1.y,
                                 gtri.p2.x, gtri.p2.y,
                                 gtri.clr);

                // draw some 2d lines
                if (line_x < SCREEN_WIDTH)
                    Draw_Line(line_x, 0, line_x, SCREEN_HEIGHT,  0xba, double_buffer); // ha ha should be (SCREEN_HEIGHT-1)
                if (line_y < SCREEN_HEIGHT)
                    Draw_Line(0, line_y, (SCREEN_WIDTH * 3) / 4, line_y,  0xab, double_buffer);


                // rotate the object on all three axes
//                if (!pause_rotation)
//                  Rotate_Object((object_ptr)&test_object, xr, yr, zr);
                // rotate one of the objects
                if (!pause_rotation)
                    Rotate_Object(&test_objects[0],3,6,9);


                // create the global world to camera transformation matrix
                Create_World_To_Camera();

                Generate_Poly_List(NULL,RESET_POLY_LIST);


                for (index=0; index<NR_OBJS; index++)
                {
                    // test if this object should be processed
                    if (!Remove_Object(&test_objects[index],OBJECT_CULL_XYZ_MODE))
                    {
                        // convert to world coordinates
                        Local_To_World_Object((object_ptr)&test_objects[index]);

                        // shade and remove backfaces, ignore the backface part for now
                        // notice that backface shadin and backface removal is done in world coordinates
                        Remove_Backfaces_And_Shade((object_ptr)&test_objects[index]);

                        // convert to camera coordinates
                        World_To_Camera_Object((object_ptr)&test_objects[index]);


                        // clip the objects polygons against viewing volume
                        Clip_Object_3D(&test_objects[index],CLIP_Z_MODE);

                        // generate the final polygon list
                        Generate_Poly_List(&test_objects[index],ADD_TO_POLY_LIST);

                    } // end if object is visible
                    else
                    {
                        sprintf(buffer,"%d, ",index);
                    }
                } // end for index


                // sort the polygons
                Sort_Poly_List();

                // draw the polygon list
                Draw_Poly_List();


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
                sprintf(buffer, "C=%d", Current_color);

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




//
#define Z_OBJS 4
//
int zb_main( int argc, char* argv[] )
{

    object test_objects[Z_OBJS];   // the test object
    int index;

    // load in the objects from the command line
    for (index=0; index<Z_OBJS; index++)
    {
        if (!PLG_Load_Object((object_ptr)&test_objects[index],argv[1],1))
        {
            printf("\nCouldn't find file %s",argv[1]);
            return 1;

        } // end if

    } // end for index



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

            bool pause_rotation = 1;
            bool draw_frame = true; // for testing/debugging, only refresh if 'r' key


            //Clear screen
            SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
            SDL_RenderClear( gRenderer );

            // position the object
            for (index=0; index<Z_OBJS; index++)
            {
                //Position_Object((object_ptr)&test_object[index],-150+index*100,0,300);
                test_objects[index].world_pos.x=-200 + (index%4)*100;
                test_objects[index].world_pos.y=0;
                test_objects[index].world_pos.z=200 + 300*(index>>2);
            }

            // set the viewpoint
            view_point.x = 0;
            view_point.y = 0;
            view_point.z = 0;


            // create the sin/cos lookup tables used for the rotation function
            Build_Look_Up_Tables();

            // allocate double buffer
            Create_Double_Buffer(SCREEN_HEIGHT);

            // create a 200 line z buffer (128k)
            Create_Z_Buffer(SCREEN_HEIGHT);

            // initialize the z buffer with a distant value
            Fill_Z_Buffer(16000);



            // read the color palette off disk
            Load_Palette_Disk("data/standard.pal",(RGB_palette_ptr)&color_palette_3d);
            Write_Palette(0,255,(RGB_palette_ptr)&color_palette_3d);


            // set viewing distance
            viewing_distance = 250; // grrrr globals (was this not set in solzdemo.c.?



            int line_x = 0, line_y = 0; // test 2d lines
            int xr, yr, zr;
            mytri_t gtri; // test the 2d triangle

            //While application is running
            while( !quit )
            {
                SDL_Texture *newTexture;

                char buffer[80]; // used to print strings

                //Start cap timer
                //Get the current clock time
                mStartTicks = SDL_GetTicks(); // capTimer.start();


                //Clear screen
                /*
                                SDL_SetRenderDrawColor( gRenderer, 0xFF, 0xFF, 0xFF, 0xFF );
                                SDL_RenderClear( gRenderer );
                */
                // erase the screen
                Fill_Double_Buffer(0);

                // initialize z buffer
                Fill_Z_Buffer(16000);


                xr = 0;
                yr = 0;
                zr = 0;

                //Handle events on queue
                while( SDL_PollEvent( &e ) != 0 )
                {
                    float x,y,z;     // working variables

                    //User requests quit
                    if( e.type == SDL_QUIT )
                    {
                        quit = true;
                    }
                    //User presses a key
                    else if( e.type == SDL_KEYDOWN )
                    {
                        line_x += 1;
                        line_y += 1;

                        switch( e.key.keysym.sym )
                        {

                            // move the light source

                        case SDLK_t:
                        {
                            x = light_source.x;

                            y = cos(.2)*light_source.y - sin(.2) * light_source.z;
                            z = sin(.2)*light_source.y + cos(.2) * light_source.z;

                            light_source.y = y;
                            light_source.z = z;
                        }
                        break;

                        case SDLK_g:
                        {
                            y = light_source.y;

                            x = cos(.2)*light_source.x + sin(.2) * light_source.z;
                            z = -sin(.2)*light_source.x + cos(.2) * light_source.z;

                            light_source.x = x;
                            light_source.z = z;
                        }
                        break;

                        case SDLK_b:
                        {

                            z = light_source.z;

                            x = cos(.2)*light_source.x - sin(.2) * light_source.y;
                            y = sin(.2)*light_source.x + cos(.2) * light_source.y;

                            light_source.x = x;
                            light_source.y = y;
                        }
                        break;

                        case SDLK_y:
                        {
                            x = light_source.x;

                            y = cos(-.2)*light_source.y - sin(-.2) * light_source.z;
                            z = sin(-.2)*light_source.y + cos(-.2) * light_source.z;

                            light_source.y = y;
                            light_source.z = z;
                        }
                        break;

                        case SDLK_h:
                        {
                            y = light_source.y;

                            x = cos(-.2)*light_source.x + sin(-.2) * light_source.z;
                            z = -sin(-.2)*light_source.x + cos(-.2) * light_source.z;

                            light_source.x = x;
                            light_source.z = z;
                        }
                        break;

                        case SDLK_n:
                        {
                            z = light_source.z;

                            x = cos(-.2)*light_source.x - sin(-.2) * light_source.y;
                            y = sin(-.2)*light_source.x + cos(-.2) * light_source.y;

                            light_source.x = x;
                            light_source.y = y;
                        }
                        break;

                        case SDLK_UP:
                            view_point.y+=20;
                            break;

                        case SDLK_DOWN:
                            view_point.y-=20;
                            break;

                        case SDLK_RIGHT:
                            view_point.x+=20;
                            break;

                        case SDLK_LEFT:
                            view_point.x-=20;
                            break;

                        case SDLK_KP_PLUS:
                            view_point.z+=20;
                            break;

                        case SDLK_KP_MINUS:
                            view_point.z-=20;
                            break;

                        case SDLK_LEFTBRACKET:
                            Scale_Object(&test_objects[0],1.1);
                            break;

                        case SDLK_RIGHTBRACKET:
                            Scale_Object(&test_objects[0],.9);
                            break;

                        case SDLK_z:
                            if ((view_angle.ang_x+=5)>360)
                                view_angle.ang_x = 0;
                            break;

                        case SDLK_a:
                            if ((view_angle.ang_x-=5)<0)
                                view_angle.ang_x = 360;
                            break;

                        case SDLK_x:
                            if ((view_angle.ang_y+=5)>360)
                                view_angle.ang_y = 0;
                            break;

                        case SDLK_s:
                            if ((view_angle.ang_y-=5)<0)
                                view_angle.ang_y = 360;
                            break;

                        case SDLK_c:
                            if ((view_angle.ang_z+=5)>360)
                                view_angle.ang_z = 0;
                            break;

                        case SDLK_d:
                            if ((view_angle.ang_z-=5)<0)
                                view_angle.ang_z = 360;
                            break;


                        case SDLK_q:
                            test_objects[0].world_pos.x-=2;
                            break;

                        case SDLK_w:
                            test_objects[0].world_pos.x+=2;
                            break;

                        case SDLK_p:
                            pause_rotation ^= 1;
                            break;

                        case SDLK_r:
                            draw_frame = true;
                            break;
#if 0
                        case SDLK_x:
                            xr = 2;
                            break;

                        case SDLK_y:
                            yr = 2;
                            break;

                        case SDLK_z:
                            zr = 2;
                            break;
#endif

                        case SDLK_EQUALS: // SDLK_PLUS:
                            Current_color += 1;
                            break;

                        case SDLK_MINUS:
                            Current_color -= 1;
                            break;

                        case SDLK_ESCAPE:
                            quit = true;
                            break;

                        case SDLK_SLASH:
                            gtri.p0.x = rand()%SCREEN_WIDTH;
                            gtri.p0.y = rand()%SCREEN_HEIGHT;
                            gtri.p1.x = rand()%SCREEN_WIDTH;
                            gtri.p1.y = rand()%SCREEN_HEIGHT;
                            gtri.p2.x = rand()%SCREEN_WIDTH;
                            gtri.p2.y = rand()%SCREEN_HEIGHT;
                            gtri.clr = Current_color; // rand()%256;
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


                /*
                                if (!draw_frame)
                                    continue;
                                draw_frame = false;
                */

                // draw the 2d triangle at its current location
                Draw_Triangle_2D(gtri.p0.x, gtri.p0.y,
                                 gtri.p1.x, gtri.p1.y,
                                 gtri.p2.x, gtri.p2.y,
                                 gtri.clr);


                // draw some 2d lines
                if (line_x < SCREEN_WIDTH)
                    Draw_Line(line_x, 0, line_x, SCREEN_HEIGHT,  0xba, double_buffer); // ha ha should be (SCREEN_HEIGHT-1)
                if (line_y < SCREEN_HEIGHT)
                    Draw_Line(0, line_y, (SCREEN_WIDTH * 3) / 4, line_y,  0xab, double_buffer);


                // rotate the object on all three axes
//                if (!pause_rotation)
//                  Rotate_Object((object_ptr)&test_object, xr, yr, zr);
                // rotate one of the objects
                if (!pause_rotation)
                    Rotate_Object(&test_objects[0],3,6,9);


                // create the global world to camera transformation matrix
                Create_World_To_Camera();

                // reset polygon list
                Generate_Poly_List(NULL,RESET_POLY_LIST);


                for (index=0; index<Z_OBJS; index++)
                {
                    // test if this object should be processed
                    if (!Remove_Object(&test_objects[index],OBJECT_CULL_XYZ_MODE))
                    {
                        // convert to world coordinates
                        Local_To_World_Object((object_ptr)&test_objects[index]);

                        // shade and remove backfaces, ignore the backface part for now
                        // notice that backface shadin and backface removal is done in world coordinates
                        Remove_Backfaces_And_Shade((object_ptr)&test_objects[index]);

                        // convert to camera coordinates
                        World_To_Camera_Object((object_ptr)&test_objects[index]);


                        // clip the objects polygons against viewing volume
                        Clip_Object_3D(&test_objects[index],CLIP_Z_MODE);

                        // generate the final polygon list
                        Generate_Poly_List(&test_objects[index],ADD_TO_POLY_LIST);

                    } // end if object is visible
                    else
                    {
                        sprintf(buffer,"%d, ",index);
                    }
                } // end for index


                // draw the polygon list using z buffer
                Draw_Poly_List_Z();


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
                sprintf(buffer, "C=%d", Current_color);

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
