//
// Created by benmajor on 5/30/2020.
//

#include "pool_table.hxx"
#include <catch.hxx>
#include <sstream>

using namespace ge211;

TEST_CASE("really_build_pt")
{
    pool_table pt = pool_table(6);
    CHECK(pt.board.width == 12 * ball_radius);
    CHECK(pt.board.height == 24 * ball_radius);
    CHECK(pt.all_balls.size() == 6);
    CHECK(pt.all_balls.at(0).team_no == 0);
    CHECK(pt.all_balls.at(1).team_no == 1);
    CHECK(pt.all_balls.at(2).team_no == 0);
    CHECK(pt.all_balls.at(3).team_no == 1);
    CHECK(pt.all_balls.at(4).team_no == 0);
    CHECK(pt.all_balls.at(5).team_no == 1);
    // positions of first and last balls;
    CHECK(pt.all_balls.at(0).center.x == 3 * ball_radius);
    CHECK(pt.all_balls.at(0).center.y == 4 * ball_radius);
    CHECK(pt.all_balls.at(0).velocity.width == 0);
    CHECK(pt.all_balls.at(0).velocity.height == 0);
    CHECK(pt.all_balls.at(0).is_alive);
    CHECK(pt.all_balls.at(5).center.x == 5 * ball_radius);
    CHECK(pt.all_balls.at(5).center.y == 8 * ball_radius);
    CHECK(pt.all_balls.at(5).velocity.width == 0);
    CHECK(pt.all_balls.at(5).velocity.height == 0);
    CHECK(pt.all_balls.at(5).is_alive);
}