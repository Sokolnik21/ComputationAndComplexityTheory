#ifndef BALL_PUSHER
#define BALL_PUSHER

#include <set>
#include "DTBoard.h"
#include "Point.h"

enum Direction {
    UP = 0,
    UP_RIGHT,
    RIGHT,
    DOWN_RIGHT,
    DOWN,
    DOWN_LEFT,
    LEFT,
    UP_LEFT,
    NUMBER_OF_DIRECTIONS
};

class BallPusher {
    const DTBoard dtb;
public:
    BallPusher(DTBoard dtb);
    std::tuple<Point, std::set<Point>> push_ball(Point ball, Direction direction);
private:
    bool is_able_to_push(Point ball, Direction direction);
    bool is_able_to_stop(Point ball, Direction direction);
    Point get_next_point_in_line(Point ball, Direction direction);
    bool is_ball_on_diamond(Point ball);
    bool is_ball_in_pit(Point ball);
};

#endif
