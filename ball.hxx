#pragma once

#include <ge211.hxx>
using Rectangle = ge211::Basic_rectangle<double>;
using Dimensions = ge211::Basic_dimensions<double>;
using Position = ge211::Basic_position<double>;

static const int ball_radius = 20;
static const int ball_mass = 10;

// The Ball struct.
struct Ball
{
    Ball(Position position);

    Position   center;
    Dimensions velocity;
    int        radius = ball_radius;
    int        mass   = ball_mass;
    bool       is_alive;
    int        team_no;

    // Checks if the ball collides with another ball.
    bool collides(Ball b);
    // same with a rectangle.
    bool collides_rect(Rectangle rectangle);

    // Brick_out functions that do the same thing they did then.
    bool hits_top();
    bool hits_bottom(Rectangle);
    bool hits_side(Rectangle);

    void reflect_vertical();

    void reflect_horizontal();

    // Updates the ball states depending on the input from compute velocity.
    Ball next();
};

/*
// I cribbed this from brick_out as well.
bool operator==(Ball&, Ball&);

// Inequality for `Ball`s.
bool operator!=(Ball&, Ball&);

*/
