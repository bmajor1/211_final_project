#include "model.hxx"

#include <algorithm>

Model::Model(int balls)
    : white_ball({ pt.board.width / 2,
                 5 * pt.board.height / 6})
                 , pt(balls)
                 , turn()
{
    // the players are essentially for updating purposes. The team number of
    // each player is used for comparison (testing) because there is no other
    // invariant differentiating them.
    P1.score    = 0;
    P1.nr_turns = 0;
    P1.is_turn  = true;
    P1.team     = 0;

    P2.score    = 0;
    P2.nr_turns = 0;
    P2.is_turn  = false;
    P2.team     = 1;

    // a dummy player for the end of the game.
    P3.is_turn  = false;
    P3.nr_turns = -1;
    P3.team     = 2;
    P3.score    = 0;

    // initialize the white ball
    white_ball.velocity = {0, 0};
    white_ball.is_alive = false;
    white_ball.radius   = ball_radius;
    white_ball.mass     = ball_mass;
    white_ball.team_no  = -1;

    // set the team nrs of the balls now.
    for (size_t j = 0; j < balls; j++) {
        if (j % 2 == 0) {
            pt.all_balls.at(j).team_no = 0;
        } else {
            pt.all_balls.at(j).team_no = 1;
        }

    }
}
void Model::compute_velocity(Ball &a, Ball &b)
{
    // if two objects of the same mass collide inelastically, they simply
    // swap momenta.
    std::swap(a.velocity, b.velocity);
}

void Model::side_collisions(Ball &ball) {
    Ball temp = ball.next();
    if(temp.hits_top() || temp.hits_bottom(pt.board))
        ball.reflect_vertical();
    if(temp.hits_side(pt.board))
        ball.reflect_horizontal();
    ball = ball.next();
}


void Model::update_balls()
{
    // move every ball, checking for collisions.
    for(size_t i = 0; i < pt.all_balls.size(); i++) {
        for(size_t j = i + 1; j < pt.all_balls.size(); j++) {
            if(pt.all_balls.at(i).collides(pt.all_balls.at(j))) {
                compute_velocity(pt.all_balls.at(i),pt.all_balls.at(j));
            }
            pt.all_balls.at(j) = pt.all_balls.at(j).next();
        }
        pt.all_balls.at(i) = pt.all_balls.at(i).next();
    }

    // compute velocities based on collisions with white ball
    for (Ball &d : pt.all_balls) {
        if(white_ball.collides(d)) {
            compute_velocity(white_ball, d);
        }
    }

    // move the white ball
    white_ball = white_ball.next();
}

// if the balls are slow enough and close to a hole, they will be
// counted as having been sunk. The function clean_dead then takes care
// of the sunk balls.
void Model::sunk() {

    for(Rectangle r: pt.holes)
            for(Ball &ball: pt.all_balls) {
                if (ball.collides_rect(r)
                    && std::abs(ball.velocity.width) < ball_radius
                    && std::abs(ball.velocity.height) < ball_radius) {
                    ball.is_alive = false;
                    ball.velocity = {0, 0};
                }
            }

    for(Rectangle r: pt.holes) {
        if (white_ball.collides_rect(r)
            && std::abs(white_ball.velocity.width) < ball_radius
            && std::abs(white_ball.velocity.height) < ball_radius) {
            white_ball.is_alive = false;
            white_ball.velocity = {0, 0};
        }
    }
}

bool Model::balls_still_moving() {
    // this code block checks if there is any ball that is still moving in
    // the current frame, and returns without doing anything if any still are.
    size_t i = 0;
    for(Ball ball: pt.all_balls) {
        if (ball.velocity.width != 0 || ball.velocity.height != 0) i++;
    }
        if (white_ball.velocity.width != 0
            || white_ball.velocity.height != 0)
            i++;
    return i > 0;
}

void Model::update_turns() {
        if(P1.is_turn) {
            // always start by taking off one, unless we are at -1, in which
            // case do nothing.
            int temp = P1.nr_turns;
            P1.nr_turns = std::max(-1, --temp);

            // now, do the checking. If P1 sunk a ball of the other type, then
            // it hands over the turn, which implies setting its nr_turns to -1,
            // and giving 2 to P2.

            size_t k = 0; // this will allow us to determine if P1 sunk any
            // balls it shouldn't have.
            for (Ball ball: pt.all_balls) {
                if (!ball.is_alive) {
                    if (ball.team_no == 0) {
                        P1.nr_turns++;
                        P1.score++;
                    } else if (ball.team_no == 1) {
                        P2.score++;
                        k++; // no additional score penalty for sinking white
                        // ball
                    }
                }
            }
            if (!white_ball.is_alive) {
                k++;
            }
            if (k > 0) {
                P1.nr_turns = -1;
                P2.nr_turns += 2;
            }
            if (P1.nr_turns == -1) {
                P1.is_turn = false;
                P2.is_turn = true;
            }
        } else if(P2.is_turn) {
            int temp = P2.nr_turns;
            P2.nr_turns = std::max(-1, --temp);
            size_t j = 0;
            for (Ball ball: pt.all_balls) {
                if(!ball.is_alive) {
                    if (ball.team_no == 1) {
                        P2.nr_turns++;
                        P2.score++;
                        j++;
                    } else if (ball.team_no == 0) {
                        P1.score++;
                        j++;
                    }
                }
            }
            if (!white_ball.is_alive) {
                j++;
            }
            if (j > 0) {
                P2.nr_turns = -1;
                P1.nr_turns += 2;
            }

            if(P2.nr_turns == -1) {
                P2.is_turn = false;
                P1.is_turn = true;
            }
    }
    std::vector<Ball>::iterator it = pt.all_balls.begin();
    while(it != pt.all_balls.end()) {
        Ball &ball = *(it);
        if(!ball.is_alive) {
            pt.all_balls.erase(it);
        }
        else {
            it++;
        }
    }
}


void Model::slow_down_balls(double dt) {
    double dv = drag * dt;

    for(Ball ball: pt.all_balls) {
        if (std::abs(ball.velocity.width) < dv &&
            std::abs(ball.velocity.height) < dv) {
            ball.velocity = {0, 0};
            ball = ball.next();
        } else if (ball.velocity.width >= dv &&
                   ball.velocity.height >= dv) {
            ball.velocity -= {dv, dv};
            ball = ball.next();
        } else if (ball.velocity.width >= dv &&
                   ball.velocity.height <= -dv) {
            ball.velocity -= {dv, 0};
            ball.velocity += {0, dv};
            ball = ball.next();
        } else if (ball.velocity.width <= -dv &&
                   ball.velocity.height >= dv) {
            ball.velocity += {dv, 0};
            ball.velocity -= {0, dv};
            ball = ball.next();
        } else if (ball.velocity.width >= dv &&
                 ball.velocity.height <= -dv) {
            ball.velocity -= {dv, 0};
            ball.velocity += {0, dv};
            ball = ball.next();
        }
    }

    if(std::abs(white_ball.velocity.width) < dv &&
       std::abs(white_ball.velocity.height) < dv) {
        white_ball.velocity = {0,0};
        white_ball = white_ball.next();
    } else if(white_ball.velocity.width <= -dv &&
            white_ball.velocity.height <= -dv) {
        white_ball.velocity += {dv,dv};
        white_ball = white_ball.next();
    } else if(white_ball.velocity.width >= dv &&
            white_ball.velocity.height >= dv) {
        white_ball.velocity -= {dv,dv};
        white_ball = white_ball.next();
    } else if(white_ball.velocity.width >= dv &&
            white_ball.velocity.height <= -dv) {
        white_ball.velocity -= {dv,0};
        white_ball.velocity += {0,dv};
        white_ball = white_ball.next();
    } else if(white_ball.velocity.width <= -dv &&
            white_ball.velocity.height >= dv) {
        white_ball.velocity += {dv,0};
        white_ball.velocity -= {0,dv};
        white_ball = white_ball.next();
    }
}

void Model::update(double dt)
{
    // compute collisions instantaneously
    update_balls();
    for(Ball &ball: pt.all_balls) {
        side_collisions(ball);
    }
    // slow down the balls -- the order of computation here is actually
    // justified because collisions are assumed to happen in instants where
    // drag doesn't act on balls.
    slow_down_balls(dt);
    // if the balls are still moving then we return. If we have already updated
    // the turns, we also return.
    if(balls_still_moving()) return;
    // otherwise, we compute the turns and scores
    update_turns();
    // if we're done, we're done
    if(is_game_over()) {
        set_game_over();
        return;
    }
}

int Model::winner() {
    if(P1.score == P2.score) return P3.team;
    else if(P1.score > P2.score) return P1.team;
    else if(P2.score > P1.score) return P2.team;
    else return -1;
}

bool Model::is_game_over() {
    return pt.all_balls.empty();
}

int Model::set_game_over() {
    white_ball.center = pt.board.center();
    // set this to be some useless number
    white_ball.team_no = 2;
    white_ball.velocity = {0,0};
    go = true;
    return winner();
}