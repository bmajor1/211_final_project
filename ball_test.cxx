//
// Created by benmajor on 5/30/2020.
//

#include "ball.hxx"
#include <catch.hxx>

#include <sstream>

using namespace ge211;

TEST_CASE("ball test")
{
    Ball ball = Ball({0,0});
    CHECK(ball.velocity.height == 0);
    CHECK(ball.velocity.width == 0);
    CHECK(ball.is_alive);
    CHECK(ball.center.x == 0);
    CHECK(ball.center.y == 0);
    CHECK(ball.radius == ball_radius);
    CHECK(ball.mass == ball_mass);
}


TEST_CASE("rest of ball tests")
{
    Ball ball = Ball({150,150});
    ge211::Basic_rectangle<double> rec = {0,0,300,300};
    CHECK_FALSE( ball.hits_side(rec) );
    CHECK_FALSE( ball.hits_bottom(rec) );
    CHECK_FALSE( ball.hits_top() );
    ball.velocity = {146,0};
    ball = ball.next();
    CHECK(ball.hits_side(rec));
    CHECK(ball.is_alive);
    CHECK(ball.radius == ball_radius);
    CHECK(ball.mass == ball_mass);
    ball.velocity = {-30, 15};
    ball = ball.next();
    CHECK_FALSE(ball.hits_side(rec));
    CHECK(ball.is_alive);
    CHECK(ball.radius == ball_radius);
    CHECK(ball.mass == ball_mass);
    ball.reflect_horizontal();
    ball.reflect_vertical();
    CHECK(ball.velocity.width == 30);
    CHECK(ball.velocity.height == -15);
}

TEST_CASE("ball collisions")
{
    Ball ball = Ball({-10,10});
    ball.team_no = 0;
    ge211::Basic_rectangle<double> rec = {0,0,100,200};
    CHECK(ball.collides_rect(rec));
    ball.center = {-5,10};
    CHECK(ball.collides_rect(rec));
    ball.center = {50, 50};
    CHECK(ball.collides_rect(rec));
    Ball orb = Ball({55,56});
    orb.team_no = 0;
    // everything is the same except for their team no
    ball.velocity = {50,46};
    orb.velocity = ball.velocity;
    CHECK_FALSE(orb == ball);
    CHECK(orb.radius == ball.radius);
    CHECK(orb.mass == ball.mass);
    CHECK(ball.collides(orb));
    CHECK(orb.collides(ball));
    CHECK(orb.collides_rect(rec));
    orb = orb.next();
    CHECK_FALSE(ball.collides(orb));
    CHECK_FALSE(orb.collides(ball));
    CHECK(orb.collides_rect(rec));
}
