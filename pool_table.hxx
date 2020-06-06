#pragma once

#include "ball.hxx"

// The scene will have standard dimensions proportional to the number of balls.
// More balls, bigger board. EA. It's in the game.

// while it may seem like players need to be compared using a dedicated
// function, in fact two players are the same iff they have the same team nr.

class pool_table
{

public:

    // The only allowed number of balls for arranging purposes are three small
// but not too small triangular numbers, i.e. numbers that can be written
// as 1+2+...+n for some n. In particular, the game allows 6, 10, or 15 balls.

// I will always allow twice as many balls as there are in the top row of
// the usual triangular arrangement of billiards to fit on the board,
// which means 4 times the number in the top row (3, 4, 5, respectively)
// times the radius of one ball. Similarly, I want there to be twice as
// much room vertically.
    pool_table(int n);

    Rectangle board;

    std::vector<Rectangle> holes;
    std::vector<Ball> all_balls;

    Dimensions scene_dims(int x);
    Dimensions hole_dims = {ball_radius, ball_radius};

    // The holes are the bounding boxes of circles the size of balls. No
    // circle shape in ge211 that I could find.
    Rectangle hole1;
    Rectangle hole2;
    Rectangle hole3;
    Rectangle hole4;
    Rectangle hole5;
    Rectangle hole6;

private:
    // Creates an arrangement of m balls of alternating teams. Some
    // extra tweaking is necessary on the team numbers because the odd/even
    // difference is hard to reconcile based only on the value of m
    void arrange_line(Position pos, int m);

    void arrange_balls(int nr_balls);

    // Arranges the balls to be in their intended positions
// at the start of the game.

// Similar to Ball::next, but this is for initialization purposes.
// the model needs access to this function because the white ball needs to
// be spawned at the beginning of each turn.
    Ball spawn_ball(Position pos)
    {
        Ball result(pos);
        return result;
    }
};