#include "pool_table.hxx"

// using Rectangle = ge211::Rectangle;
// using Dimensions = ge211::Dimensions;
// using Position = ge211::Position;
// using Csprite = ge211::Circle_sprite;

pool_table::pool_table(int n)
          : all_balls()
          , board()
          , holes()
{

        Dimensions sd = scene_dims(n);

        board = {0, 0, sd.width, sd.height};
        // Six holes, like a regular pool table; four at the corners,
        // two on sides.


        hole1 = {0, 0, ball_radius, ball_radius};
        hole2 = {sd.width - 3 * ball_radius, 0,
                 ball_radius, ball_radius};
        hole3 = {0, sd.height - 3 * ball_radius,
                 ball_radius, ball_radius};
        hole4 = {sd.width - 3 * ball_radius,
                 sd.height - 3 * ball_radius,
                 ball_radius, ball_radius};
        hole5 = {0, sd.height / 2 - ball_radius,
                 ball_radius, ball_radius};
        hole6 = {sd.width - 3 * ball_radius,
                 sd.height / 2 - ball_radius,
                 ball_radius, ball_radius};
        holes.push_back(Rectangle::from_top_left(hole1.top_left(), hole_dims));
        holes.push_back(Rectangle::from_top_left(hole2.top_left(), hole_dims));
        holes.push_back(Rectangle::from_top_left(hole3.top_left(), hole_dims));
        holes.push_back(Rectangle::from_top_left(hole4.top_left(), hole_dims));
        holes.push_back(Rectangle::from_top_left(hole5.top_left(), hole_dims));
        holes.push_back(Rectangle::from_top_left(hole6.top_left(), hole_dims));
        // arrange the balls!
        arrange_balls(n);
}


Dimensions pool_table::scene_dims(int r)
{

        double x = 4 * ball_radius;
        switch (r) {
        case 6:
            x *= 3;
            break;
        case 10:
            x *= 4;
            break;
        case 15:
            x *= 5;
            break;
        default:
            throw ge211::Client_logic_error("You can only play "
                                            "with 6, 10, or 15" "balls.");
        }
        return {x, 2 * x};
    }

    void pool_table::arrange_line(Position pos, int m)
    {
        int y = 2 * ball_radius;
        for (size_t i = 0; i < m; i++) {
            Ball ball = spawn_ball({pos.x + (int) (2 * i) * ball_radius,
                                     pos.y + y});
            all_balls.push_back(ball);
        }
    }

// Always allow space for 2 rows of balls above the top row of arranged balls.
// Tedious function that arranges the balls. I won't worry about the team
// numbers now; instead, I will set them in the model.
    void pool_table::arrange_balls(int n)
    {
        switch (n) {
        case 6:
            arrange_line({3 * ball_radius, 2 * ball_radius}, 3);
            arrange_line({4 * ball_radius, 4 * ball_radius}, 2);
            arrange_line({5 * ball_radius, 6 * ball_radius}, 1);
            break;
        case 10:
            arrange_line({4 * ball_radius, 2 * ball_radius}, 4);
            arrange_line({5 * ball_radius, 4 * ball_radius}, 3);
            arrange_line({6 * ball_radius, 6 * ball_radius}, 2);
            arrange_line({7 * ball_radius, 8 * ball_radius}, 1);
            break;
        case 15:
            arrange_line({5 * ball_radius, 2 * ball_radius}, 5);
            arrange_line({6 * ball_radius, 4 * ball_radius}, 4);
            arrange_line({7 * ball_radius, 6 * ball_radius}, 3);
            arrange_line({8 * ball_radius, 8 * ball_radius}, 2);
            arrange_line({9 * ball_radius, 10 * ball_radius}, 1);
            break;
        default:
            return;
        }
    }



