
#include <cstdio>
#include <cstring>

int main_generator(int argc, char* argv[]);
int main_viewer(int argc, char* argv[]);

int main(int argc, char* argv[])
{
    int (*inner_main)(int, char*[]) = nullptr;
    if(argc >= 2)
    {
        if(strcmp(argv[1], "generator") == 0)
            inner_main = main_generator;
        else if(strcmp(argv[1], "viewer") == 0)
            inner_main = main_viewer;
    }

    try
    {
        if(!inner_main)
            throw "useage: totalpreprocess generator|viewer ...";
        return inner_main(argc, argv);
    }
    catch(const char* errstr)
    {
        fprintf(stderr, "error: %s\n", errstr);
        return -1;
    }
}
