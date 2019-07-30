#include "BallPusher.h"

#include "Utilities.h"
#include <iostream>

BallPusher::BallPusher(DTBoard dtb): dtb(dtb) { }

std::tuple<Point, std::set<Point>> BallPusher::push_ball(Point ball, Direction direction) {
    std::set<Point> diamonds;
    Point curr_position = ball;
    if(is_ball_on_diamond(curr_position)) {
        diamonds.insert(curr_position);
    }
    while(is_able_to_push(curr_position, direction)) {
        curr_position = get_next_point_in_line(curr_position, direction);
        if(is_ball_on_diamond(curr_position)) {
            diamonds.insert(curr_position);
        }
        if(is_ball_in_pit(curr_position)) {
            break;
        }
    }
    if(is_able_to_stop(curr_position, direction)) {
        return std::make_tuple(curr_position, diamonds);
    }
    else {
        return std::make_tuple(ball, std::set<Point>());
    }
}

bool BallPusher::is_able_to_push(Point ball, Direction direction) {
    Point destination = get_next_point_in_line(ball, direction);
    char destination_cell = dtb.cells.at(destination.get_row())[destination.get_col()];
    return DTBoard::is_cell_in_pattern(destination_cell, {' ', '+', 'O', '.'});
}

bool BallPusher::is_able_to_stop(Point ball, Direction direction) {
    char base_cell = dtb.cells.at(ball.get_row())[ball.get_col()];
    if(DTBoard::is_cell_in_pattern(base_cell, {'O'})) {
        return true;
    }
    else {
        Point destination = get_next_point_in_line(ball, direction);
        char destination_cell = dtb.cells.at(destination.get_row())[destination.get_col()];
        return DTBoard::is_cell_in_pattern(destination_cell, {'#'});
    }
}

Point BallPusher::get_next_point_in_line(Point ball, Direction direction) {
    int row = ball.get_row();
    int col = ball.get_col();
    Point destination = Point();
    switch(direction) {
        case UP :
            destination.set_position(Point(row - 1, col));
            break;
        case UP_RIGHT :
            destination.set_position(Point(row - 1, col + 1));
            break;
        case RIGHT :
            destination.set_position(Point(row, col + 1));
            break;
        case DOWN_RIGHT :
            destination.set_position(Point(row + 1, col + 1));
            break;
        case DOWN :
            destination.set_position(Point(row + 1, col));
            break;
        case DOWN_LEFT :
            destination.set_position(Point(row + 1, col - 1));
            break;
        case LEFT :
            destination.set_position(Point(row, col - 1));
            break;
        case UP_LEFT :
            destination.set_position(Point(row - 1, col - 1));
            break;
    }
    return destination;
}

bool BallPusher::is_ball_on_diamond(Point ball) {
    char cell = dtb.cells.at(ball.get_row())[ball.get_col()];
    if(DTBoard::is_cell_in_pattern(cell, {'+'}))
        return true;
    else
        return false;
}

bool BallPusher::is_ball_in_pit(Point ball) {
    char cell = dtb.cells.at(ball.get_row())[ball.get_col()];
    if(DTBoard::is_cell_in_pattern(cell, {'O'}))
        return true;
    else
        return false;
}

#if IS_DEBUG_BALL_PUSHER_ON
int main(int argc, char * argv[]) {
    std::string file_path(argv[1]);
    const DTBoard dtb = DTBoard(file_path);
    BallPusher bp(dtb);
    std::tuple<Point, std::set<Point>> result = bp.push_ball(Point(6, 2), Direction::UP_RIGHT);
    std::cout << std::get<0>(result).get_row() << ' ' << std::get<0>(result).get_col() << '\n';
    std::cout << std::get<1>(result).size() << '\n';

    std::cout << dtb.cells.at(1)[3] << "\n";
    for(int i = 1; i < 7; i++) {
        for(int j = 1; j < 9; j++) {
            std::cout << i << ',' << j << ": ";
            for(int k = 0; k < 8; k++) {
                result = bp.push_ball(Point(i, j), Direction(k));
                std::cout << std::get<1>(result).size() << ' ';
            }
            std::cout << '\n';
        }
        std::cout << '\n';
    }

    // std::cout << (std::make_tuple(1, 2) == std::make_tuple(1, 1)) << '\n';
    return 0;
}
#endif
