#include <iostream>

using namespace std;

int lazy_main( int argc, char* argv[] );
int wd_main( int argc, char* argv[] );

int main( int argc, char* argv[] )
{
#if 0
    lazy_main(argc, argv);
#else
    char cube[] = "CUBEW.PLG";
    char pyrmd[] = "PYRAMIDW.PLG";
    char *args[2] = { "junk" }; // make sure args[0] is something
    args[1] = cube;
    wd_main(argc, args);
    args[1] = pyrmd;
    wd_main(argc, args);
    return 0;
#endif
    return 0;
}
