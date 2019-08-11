#include <tuple>

#ifndef POINT
#define POINT

class Point {
protected:
    std::tuple<int, int> position;
public:
    Point();
    Point(int row, int col);
    std::tuple<int, int> get_position();
    void set_position(Point new_position);
    int get_row();
    int get_col();
    bool is_point_valid();
    friend bool operator<(const Point& l, const Point& r);
    friend bool operator!=(const Point& l, const Point& r);
};
#endif
