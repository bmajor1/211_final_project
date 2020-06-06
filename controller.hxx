#pragma once

#include "model.hxx"
#include "view.hxx"

class Controller : public ge211::Abstract_game
{
public:

    explicit Controller(int balls);

protected:

    void on_mouse_down(ge211::Mouse_button, ge211::Basic_position<double>);

    void on_mouse_move(ge211::Basic_position<double>);

    void on_frame(double dt) override;

    void place_white_ball(ge211::Basic_position<double>);

// Computes the velocity of the white ball upon striking by the player.
// Needs input from UI
    void player_hit_white_ball(ge211::Basic_position<double>);

// Computes the velocity of the white ball upon striking by the 'AI'. Note this
// is determined without the use
// of the UI. All that is necessary is a randomly (see Open questions) generated
// vector (in the physics sense) representing the line
// of fire from the AI.
    void ai_hit_white_ball();

    void draw(ge211::Sprite_set&) override;

    ge211::Dimensions initial_window_dimensions() const override;

    std::string initial_window_title() const override;


private:

    Model model_;
    View  view_;
    ge211::Basic_position<double> temp_position_;

};