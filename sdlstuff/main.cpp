#include <iostream>

using namespace std;

int lazy_main( int argc, char* argv[] );
int wd_main( int argc, char* argv[] );

int main( int argc, char* argv[] )
{
#if 1
    lazy_main(argc, argv);
#else
char *asdf[2] = {"CUBEW.PLG" , "PYRAMIDW.PLG" };
    wd_main(argc, &asdf[0]);
    return 0;
//    wd_main(argc, asdf[1]);
//    wd_main(argc, argv);
#endif
    return 0;
}
