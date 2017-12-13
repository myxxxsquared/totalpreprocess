
#include "totalpreprocess.hpp"
#include "data.hpp"

using std::vector;
using std::string;
using cv::Point2d;
using std::istream;

vector<vector<Point2d>> loadfile(istream& ifs)
{
    string line;

    vector<vector<Point2d>> result;
    vector<Point2d> current;

    while(ifs.good())
    {
        getline(ifs, line);
        if(line.size() == 0 || line[0] == 'n' || line[0] == 'e')
        {
            if(current.size() != 0)
            {
                result.push_back(current);
                current.clear();
            }
            continue;
        }
        if(line[0] == 'p')
        {
            int x, y;
            sscanf(line.c_str(), "p %d, %d", &x, &y);
            current.push_back(Point2d(x, y));
        }
    }

    return result;
}
