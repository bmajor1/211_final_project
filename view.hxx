#pragma once

#include "model.hxx"

#include <string>

using namespace ge211;

static const int font_size = 20;

class View
{
public:
    explicit View(Model const&);

    void draw(Sprite_set& set);

    std::string initial_window_title() const;

    ge211::Basic_dimensions<int> initial_window_dimensions() const;

    Circle_sprite wb_sprite = Circle_sprite(ball_radius,
            Color::white());
    Circle_sprite b0_sprite = Circle_sprite(ball_radius,
            Color::medium_red());
    Circle_sprite b1_sprite = Circle_sprite(ball_radius,
            Color::medium_yellow());

private:
    Model const& model_;

    Rectangle_sprite pt_sprite = Rectangle_sprite
            ({(int) model_.pt.board.width,
              (int) model_.pt.board.height},
             Color::medium_green());
    Circle_sprite hole_sprite = Circle_sprite(ball_radius, Color::black());
    void build_board(Sprite_set& set) {
        // add pool table sprite

        set.add_sprite(pt_sprite, {0,0},0);
        for(ge211::Basic_rectangle<double> r: model_.pt.holes) {
            ge211::Basic_position<int> pos = {(int) r.center().x,
                                              (int) r.center().y};
            set.add_sprite(hole_sprite, pos, 1);
        }
    }

    void build_balls(Sprite_set&);

    // Mutable Text_sprite that gets set according to who the winner is.
    ge211::Text_sprite winner_message;

    // The font for the letters.
    ge211::Font sans_{"sans.ttf", font_size};

};
