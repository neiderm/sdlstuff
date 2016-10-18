#include <iostream>

using namespace std;


extern int poly_main( int argc, char* argv[] );
extern int zb_main( int argc, char* argv[] );

#include <string.h>

int main( int argc, char* argv[] )
{
    char cube[] = "data/CUBE.PLG";
    char dart[] = "data/DART.PLG";
    char diamond[] = "data/DIAMOND.PLG";
    char pyramid[] = "data/PYRAMID.PLG";

    char *args[2] = { "junk" }; // make sure args[0] is something


    args[1] = pyramid; // not right
    args[1] = diamond; // not right
    args[1] = dart;    // not sure
    args[1] = cube;
    zb_main(argc, args);
    return 0;

    args[1] = diamond;
    poly_main(argc, args);
    return 0;

    return 0;
}
