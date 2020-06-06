//
// Created by benmajor on 5/30/2020.
//

#include "ball.hxx"

// -1 is the team number, generally, for a ball not in play. In practice,
// it will only be used to place the white ball. Here it's nice because it
// should ensure down the line that the ball will not be altered until it
// is appropriate.
Ball::Ball(Position position)
        : radius(ball_radius)
        , velocity({0,0})
        , center(position)
        , mass(ball_mass)
        , team_no(0)
        , is_alive(true)
{}

// This differs from the brick_out functions in that we do not have a constant
// screen layout, i.e. it is determined dynamically, hence it's not smart to
// write these functions to take in constant inputs.

bool Ball::hits_bottom(Rectangle rec)
{
    return center.y + radius > rec.width;
}

bool Ball::hits_top()
{
    return center.y < radius;
}

bool Ball::hits_side(Rectangle rec)
{
    return center.x < radius ||
           (center.x + radius > rec.width);
}

// (Pre)condition: both balls are live.
bool Ball::collides(Ball b) {
    return
            b.is_alive && (center.x - b.center.x) * (center.x - b.center.x)
            + (center.y - b.center.y) * (center.y - b.center.y)
            < 4 * radius * radius;
}

bool Ball::collides_rect(Rectangle r) {

    return !(
            (center.x + radius) < r.x ||
            (center.x - radius) > (r.x + r.width) ||
            (center.y + radius) < r.y ||
            (center.y - radius) > (r.y + r.height)
    );
}



// Cribbed straight from brick_out. It kind of does exactly what I want
// it to do.

Ball Ball::next()
{
    Ball result(*this);
    Position ctr = result.center;
    ctr = {ctr.x + result.velocity.width,
           ctr.y + result.velocity.height };
    result.center = ctr;
    return result;
}

void Ball::reflect_vertical()
{
    velocity.height *= -1;
}

void Ball::reflect_horizontal()
{
    velocity.width *= -1;
}

/*
bool operator==(Ball const& b1, Ball &b2)
{
    return
            b1.center == b2.center &&
            b1.radius == b2.radius &&
            b1.velocity == b2.velocity &&
            b1.is_alive == b2.is_alive;
}

bool operator!=(Ball &b1, Ball &b2)
{
    return !(b1 == b2);
}*/
