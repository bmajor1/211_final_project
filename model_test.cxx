#include "model.hxx"
#include <catch.hxx>

#include <sstream>

using namespace ge211;

struct Test_access
{
    Model& m;


    void compute_velocity(Ball &a, Ball &b)
    {
        return m.compute_velocity(a, b);
    }

    void update_balls()
    {
        return m.update_balls();
    }

    void sunk()
    {
        return m.sunk();
    }

    bool balls_still_moving()
    {
        return m.balls_still_moving();
    }


    void update_turns()
    {
        return m.update_turns();
    }

};

TEST_CASE("builds model, throws error") {
    // the number of balls is enforced.
    CHECK_THROWS_AS(Model(13), ge211::Client_logic_error);
    CHECK_NOTHROW(Model(6));
    Model m = Model(10);
    Test_access t{m};

    CHECK(m.P1.score == 0);
    CHECK(m.P1.nr_turns == 0);
    CHECK(m.P1.is_turn);
    CHECK(m.P1.team == 0);

    CHECK(m.P2.score == 0);
    CHECK(m.P2.nr_turns == 0);
    CHECK_FALSE(m.P2.is_turn);
    CHECK(m.P2.team == 1);

    CHECK_FALSE(m.P3.is_turn);
    CHECK(m.P3.nr_turns == -1);
    CHECK(m.P3.team == 2);
    CHECK(m.P3.score == 0);

    // initialize the white ball
    CHECK(m.white_ball.velocity.width == 0);
    CHECK(m.white_ball.velocity.height == 0);
    CHECK_FALSE(m.white_ball.is_alive);
    // test some balls to see if the team numbers were set correctly.
    CHECK(m.pt.all_balls.at(0).team_no == 0);
    CHECK(m.pt.all_balls.at(2).team_no == 0);
    CHECK(m.pt.all_balls.at(8).team_no == 0);
    CHECK(m.pt.all_balls.at(3).team_no == 1);
    CHECK(m.pt.all_balls.at(7).team_no == 1);
}

TEST_CASE("velocity computations")
{
    // the first row of collisions, should work like Newton's cradle
    int balls = 6;
    Model model = Model(balls);
    Test_access t{model};
    CHECK(model.pt.all_balls.size() == 6);
    // first row:
    model.pt.all_balls.at(0).velocity = {35,0};
    //t.update_balls();
    model.pt.all_balls.at(0) = model.pt.all_balls.at(0).next();
    CHECK(model.pt.all_balls.at(0).center.x == 3 * ball_radius + 35);
    CHECK(model.pt.all_balls.at(0).collides(model.pt.all_balls.at(1)));
    CHECK(model.pt.all_balls.at(1).collides(model.pt.all_balls.at(0)));

    // every ball technically collides with itself, but this doesn't change
    // their velocity according to compute_velocity
    t.compute_velocity(model.pt.all_balls.at(0),model.pt.all_balls.at(1));

    CHECK(model.pt.all_balls.at(0).velocity.width == 0);
    CHECK(model.pt.all_balls.at(1).velocity.width == 35);

    model.pt.all_balls.at(0) = model.pt.all_balls.at(0).next();
    model.pt.all_balls.at(1) = model.pt.all_balls.at(1).next();

    CHECK(model.pt.all_balls.at(1).collides(model.pt.all_balls.at(2)));
    CHECK(model.pt.all_balls.at(0).velocity.width == 0);

    t.compute_velocity(model.pt.all_balls.at(1),model.pt.all_balls.at(2));

    CHECK(model.pt.all_balls.at(1).velocity.width == 0);
    CHECK(model.pt.all_balls.at(2).velocity.width == 35);
}

TEST_CASE("testing update ball, sunk")
{
    int         balls = 6;
    Model       m     = Model(balls);
    Test_access t{m};

    // to the left and above the hole in the bottom right.
    m.pt.all_balls.at(0).center = {m.pt.hole4.center().x - 2 * ball_radius,
                                   m.pt.hole4.center().y - 2 * ball_radius};
    m.white_ball.center         = {m.pt.hole4.center().x - (3 * ball_radius -
                                   5),m.pt.hole4.center().y - (3 *
                                   ball_radius - 5)};
    m.white_ball.is_alive = true;
    m.white_ball.velocity = {ball_radius + 5, ball_radius + 5};
    CHECK(m.white_ball.collides(m.pt.all_balls.at(0)));
    t.update_balls();
    CHECK(m.pt.all_balls.at(0).velocity.width == ball_radius + 5);

    m.pt.all_balls.at(1).velocity = {2 * ball_radius, 0};
    CHECK_FALSE(m.pt.all_balls.at(1).collides(m.pt.all_balls.at(2)));
    t.update_balls();
    CHECK(m.pt.all_balls.at(1).velocity.width == 0);
    CHECK(m.pt.all_balls.at(2).velocity.width == 2 * ball_radius);
    t.update_balls();
    // meanwhile, the ball sent by the white ball should have struck hole4.
    CHECK(m.pt.all_balls.at(0).collides_rect(m.pt.hole4));
    CHECK(m.pt.all_balls.at(0).is_alive);

    // however, it is not going slow enough to have been sunk.
    t.sunk();
    CHECK(m.pt.all_balls.at(0).is_alive);

    // Now, let's test actually sinking a ball. This hole is the bottom left
    // one.
    m.pt.all_balls.at(3).center = {m.pt.hole3.center().x + ball_radius / 2,
                                   m.pt.hole3.center().y - ball_radius / 2};
    m.pt.all_balls.at(3).velocity = {- ball_radius / 2, ball_radius / 2};
    t.update_balls();
    CHECK(m.pt.all_balls.at(3).collides_rect(m.pt.hole3));
    t.sunk();
    CHECK_FALSE(m.pt.all_balls.at(3).is_alive);

    // Now the white ball.
    m.white_ball.center = {m.pt.hole2.center().x - ball_radius / 2,
                                   m.pt.hole2.center().y + ball_radius / 2};
    m.white_ball.velocity = { ball_radius / 2, - ball_radius / 2};
    t.update_balls();
    CHECK(m.white_ball.collides_rect(m.pt.hole2));
    t.sunk();
    CHECK_FALSE(m.white_ball.is_alive);
}

TEST_CASE("balls still moving, updating turns, scores")
{
    int         balls = 6;
    Model       m     = Model(balls);
    Test_access t{m};
    CHECK_FALSE(t.balls_still_moving());
    // returns true even if the balls are only moving slowly.
    m.pt.all_balls.at(5).velocity = {0, ball_radius / 2};
    t.update_balls();
    CHECK(t.balls_still_moving());
    CHECK(m.P1.is_turn);

    m.pt.all_balls.at(0).center = m.pt.hole2.center();
    m.pt.all_balls.at(1).center = m.pt.hole5.center();
    CHECK(m.pt.all_balls.at(0).collides_rect(m.pt.hole2));
    CHECK(m.pt.all_balls.at(1).collides_rect(m.pt.hole5));
    t.sunk();
    t.update_turns();
    // these should be alive because they refer to the first two elements of
    // the vector, not the two we just killed.
    CHECK(m.pt.all_balls.at(0).is_alive);
    CHECK(m.pt.all_balls.at(1).is_alive);
    CHECK(m.P1.score == 1);
    CHECK(m.P2.score == 1);
    // these two balls now died
    CHECK(m.pt.all_balls.size() == 4);
    CHECK(m.P1.nr_turns == -1);
    CHECK(m.P2.nr_turns == 2);
    CHECK_FALSE(m.P1.is_turn);
    CHECK(m.P2.is_turn);
    // sinking white ball
    m.white_ball.is_alive = true;
    m.white_ball.center = m.pt.hole6.center();
    t.sunk();
    t.update_turns();
    CHECK(m.pt.all_balls.size() == 4);
    CHECK_FALSE(m.P2.is_turn);
    CHECK(m.P1.is_turn);
    CHECK(m.P2.nr_turns == -1);
    // note that this is the number of turns P1 should have as when it is P1's
    // turn again, it will go down to 0.
    CHECK(m.P1.nr_turns == 1);
}

TEST_CASE("game is over")
{

}