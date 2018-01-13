
#include "union.hpp"

int main_union(int argc, char* argv[])
{
    
    
    
    
    // Polygon_set set;
    // Polygon mypolygon;
    // // 406.57526510583596746073, 269.66562651951716134135
    // // 390.32129064299675746952, 229.08488829424484833908
    // // 432.97199225576792969150, 212.00229459102456530672
    // // 449.22496671860716332958, 252.58403281629688308385
    // mypolygon.push_back(Point(0.5, 0.0));
    // mypolygon.push_back(Point(1.0, 0.5));
    // mypolygon.push_back(Point(0.5, 1.0));
    // mypolygon.push_back(Point(0.0, 0.5));
    // set.join(mypolygon);

    // Polygon mypolygon2;
    // mypolygon2.push_back(Point(1.5,1.5));
    // mypolygon2.push_back(Point(2.5,1.5));
    // mypolygon2.push_back(Point(2.5,2.5));
    // mypolygon2.push_back(Point(1.5,2.5));
    // set.join(mypolygon2);

    // std::list<Polygon_with_holes> output;
    // set.polygons_with_holes(std::back_inserter(output));
    // for(auto& p: output)
    // {
    //     auto b = p.outer_boundary();
    //     for(auto it = b.vertices_begin(); it != b.vertices_end(); ++it)
    //         printf("%lf, %lf\n", it->x(), it->y());
    //     printf("\n");
    // }

    Union u{0.5, 0.5, 0.5};
    u.load_file("/mnt/f/DL/total-text/preprocess.test/00000002");
    u.find_polygons();
    u.process_polygons();
    u.paint_image("/mnt/f/DL/total-text/preprocess.test/00000002.png");
    return 0;
}
