#include "controller.hxx"

Controller::Controller(int balls)
        : model_(balls)
        , view_(model_)
        , temp_position_(model_.pt.board.width / 2,
                         model_.pt.board.height / 2)

{
    // throw an error if you input a number of balls that is not OK.
    if(balls == 6 || balls == 10 || balls == 15) {
        model_.white_ball.center = {model_.pt.board.width / 2,
        model_.pt.board.height - 3 * ball_radius};
    }
    else {
        throw ge211::Client_logic_error("You can only "
                                        "play with 6, 10, or 15 "
                                        "balls.");
    }

}

// delegates to the view, draws the board.
void Controller::draw(ge211::Sprite_set& sprites)
{
    if(!model_.white_ball.is_alive && model_.P1.is_turn) {
        sprites.add_sprite(view_.wb_sprite,{(int) temp_position_.x,
                                            (int) temp_position_.y}, 2);
    }
    view_.draw(sprites);
}

// the scene dimensions, determined dynamically based on number of balls used.
ge211::Basic_dimensions<int> Controller::initial_window_dimensions() const
{
    return view_.initial_window_dimensions();
}

// title of the game.
std::string Controller::initial_window_title() const
{
    return view_.initial_window_title();
}

//moves the player hover location to the mouse
void Controller::on_mouse_move(ge211::Basic_position<double> position)
{
    if(model_.P1.is_turn && model_.white_ball.team_no == -1) {
        temp_position_ = position;
    }
}

// if the white ball is dead, the mouse position places the white ball.
// Once the ball is placed, the next click launches the ball.

void Controller::on_mouse_down
        (ge211::Mouse_button button, ge211::Basic_position<double> position)
{
    if (model_.P1.is_turn) {
        if (!model_.balls_still_moving() && !model_.white_ball.is_alive) {
            place_white_ball(temp_position_);
        } else if (!model_.balls_still_moving() && model_.white_ball.is_alive) {
            player_hit_white_ball(temp_position_);
        }
    }
}

// to place the ball.
void Controller::place_white_ball(ge211::Basic_position<double> pos) {
    if(model_.P1.is_turn) {
        model_.white_ball.center = pos;
    }
    if(model_.P2.is_turn) {
        // ai always places the ball back at the starting position
        model_.white_ball.center = {model_.pt.board.width / 2,
                                      5 * model_.pt.board.height / 6};
    }
    model_.white_ball.velocity = {0, 0};
    model_.white_ball.is_alive = true;

}

void Controller::player_hit_white_ball(ge211::Basic_position<double> pos)
{
    // The white ball is hit by the cue. The idea is that the velocity of
    // the white ball should be proportional to the distance from which it
    // is struck. Executive decision: set it to 1.
    model_.white_ball.velocity = {-pos.x + model_.white_ball.center.x,
                                  -pos.y + model_.white_ball.center.y};
    model_.white_ball          = model_.white_ball.next();
}

void Controller::ai_hit_white_ball()
{
    if(model_.P2.is_turn) {
        if(!model_.balls_still_moving() && !model_.white_ball.is_alive) {
            // the input position is irrelevant since the ai always places this
            // at a fixed spot.
            place_white_ball({0,0});
        }
        if(!model_.balls_still_moving() && model_.white_ball.is_alive) {
            // we won't run into an index-out-of-bound issue since this
            // function will be called after model update, which sets
            // the game over if the vector containing playable balls is
            // empty.
            ge211::Basic_position<double> pos =
                    model_.pt.all_balls.front().center;
            ge211::Basic_dimensions<double> d = pos - model_.white_ball.center;
            // distance between positions
            double a = std::sqrt(d.width * d.width + d.height * d.height);
            // probe ball
            Ball b0 = model_.pt.all_balls.front();
            // this will be our displacement vector
            ge211::Basic_dimensions<double> z;
            for (Ball ball: model_.pt.all_balls)
                // look at any (live) balls that P2 needs to sink -- if it is
                // closer than the probe ball, swap them, keep going until the
                // closest ball is found.
                if (ball.team_no == 1) {
                    z = ball.center - model_.white_ball.center;
                    double dz = std::sqrt(z.width * z.width
                            + z.height * z.height);
                    if (dz < a) { b0 = ball; } // swap the balls if this is
                    // closer
                }
            // found closest ball, compute distance, set it equal
            // to the velocity.
            ge211::Basic_dimensions<double> dist =
                    b0.center - model_.white_ball.center;
            model_.white_ball.velocity = dist; // points in the correct dir.
            model_.white_ball = model_.white_ball.next();
        }
    }
}

void Controller::on_frame(double dt) {
    model_.update(dt);
    ai_hit_white_ball();
}