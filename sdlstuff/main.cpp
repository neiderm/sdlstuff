#include <iostream>

using namespace std;


extern int wd_main( int argc, char* argv[] );
extern int poly_main( int argc, char* argv[] );
extern int zb_main( int argc, char* argv[] );

#include <string.h>

int main( int argc, char* argv[] )
{
    char cubew[] = "data/CUBEW.PLG";
    char cube[] = "data/CUBE.PLG";
    char pyrmd[] = "data/PYRAMIDW.PLG";
    char *args[2] = { "junk" }; // make sure args[0] is something


///*
    args[1] = cube;
    zb_main(argc, args);
    return 0;
//*/

    args[1] = cube;
    poly_main(argc, args);
    return 0;

    args[1] = cube;
    wd_main(argc, args);
    return 0;

    args[1] = cubew;
    wd_main(argc, args);

    args[1] = pyrmd;
    wd_main(argc, args);


    return 0;
}
