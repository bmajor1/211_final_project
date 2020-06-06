#pragma once

#include <pool_table.hxx>

struct Player {
    int nr_turns;
    int score;
    bool is_turn;
    // either 0 1 or 2 depending on team (2 if neither), for comparison
    int team;
};

class Model {

public:

    explicit Model(int balls);

    //pool_table pool_table(int n) const;
    class pool_table pt;
    // the white ball
    Ball white_ball;
    // the team number of the current player: 0 for user, 1 for ai, 2 for
    // neither
    int turn;

    // since each player has four relevant members that are used in-game and
    // for testing, I found this to be more efficient than declaring four extra
    // members of model.
    Player P1;
    Player P2;
    Player P3;

    int winner_ = winner();
    bool go = is_game_over();

private:

// if(ui is not one of the accepted values) throw_error;
// Finds the balls a given ball has collided with.

    bool ball_alive(Ball b) {
    return b.is_alive;
    }

    void compute_velocity(Ball &a, Ball &b);

    void side_collisions(Ball &ball);

    void update_balls();

    // marks sunk balls as sunk by setting them dead.
    void sunk();

    // Updates the turns and scores.
    void update_turns();

    double drag = ball_radius;
public:

    int winner();

    void slow_down_balls(double dt);

    bool balls_still_moving();

    void update(double dt);
// Declares the triumphant winner!

// Returns whether the game is finished. This is true when all the balls are
// sunk.

    bool is_game_over();
// I think you can figure this one out.
    int set_game_over();

    friend struct Test_access;

};

