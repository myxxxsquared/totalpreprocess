
#include "data.hpp"
#include "process.hpp"
#include "resize.hpp"

int main_resize(int argc, char* argv[])
{
    if(argc != 7)
        throw "useage: totalpreprocess resize size image polygon output beginid";

    int size = atoi(argv[2]);
    const char* imagefile = argv[3];
    const char* polyfile = argv[4];
    const char* outputfile = argv[5];
    int beginid = atoi(argv[6]);

    Resize r;
    r.target_height = r.target_width = size;
    r.current = beginid;
    r.loadfile(polyfile, imagefile);
    r.generate(outputfile);
    return 0;
}
