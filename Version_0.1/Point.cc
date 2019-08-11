#include "Point.h"

Point::Point() {}
Point::Point(int row, int col) {
    position = std::make_tuple(row, col);
}
std::tuple<int, int> Point::get_position() {
    return position;
}
void Point::set_position(Point new_position){
    position = new_position.get_position();
}
int Point::get_row() {
    return std::get<0>(position);
}
int Point::get_col() {
    return std::get<1>(position);
}
bool Point::is_point_valid() {
    return (std::get<0>(position) < 0) ? false : true;
}

bool operator<(const Point& l, const Point& r) {
    if(std::get<0>(l.position) < std::get<0>(r.position))
        return true;
    else if(std::get<0>(l.position) == std::get<0>(r.position))
        if(std::get<1>(l.position) < std::get<1>(r.position))
            return true;
    return false;
}

bool operator!=(const Point& l, const Point& r) {
    if(std::get<0>(l.position) != std::get<0>(r.position))
        return true;
    if(std::get<1>(l.position) != std::get<1>(r.position))
        return true;
    return false;
}
