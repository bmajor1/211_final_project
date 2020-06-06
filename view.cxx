//
// Created by benmajor on 5/30/2020.
//

#include "view.hxx"

using namespace ge211;

View::View(Model const& model)
        : model_(model)
{}

void View::draw(Sprite_set& set)
{
    build_board(set);
    build_balls(set);

    if (model_.go) {
        switch (model_.winner_) {
        case 0:
            winner_message.reconfigure(ge211::Text_sprite::Builder(sans_)
                                               << "Game is over. Player won!");
            break;
        case 1:
            winner_message.reconfigure(ge211::Text_sprite::Builder(sans_)
                                               << "Game is over. AI won!");
            break;
        case 2:
            winner_message.reconfigure(ge211::Text_sprite::Builder(sans_)
                                               << "Game is over. It's a tie!");
        default:;
        }

        set.add_sprite(winner_message,
                       {(int) model_.pt.board.dimensions().width / 2,
                        (int) model_.pt.board.dimensions().height / 2},
                       3);
    }

}

void View::build_balls(Sprite_set& set) {
    // always set the white ball - sprite; the only time it is invisible is
    // between being sunk and being replaced, which is a short amount of time
    // and hard to implement (I think).
    if(model_.white_ball.is_alive) {
        ge211::Basic_position<int> wb_pos = {(int) model_.white_ball.center.x,
                                             (int) model_.white_ball.center.y};
        set.add_sprite(wb_sprite, wb_pos, 2);
    }
    // add the rest of the balls
    for(Ball ball: model_.pt.all_balls) {
        if (ball.is_alive) {
            ge211::Basic_position<int> b_pos = {(int) ball.center.x,
                                                (int) ball.center.y};
            if(ball.team_no == 1) {
                set.add_sprite(b1_sprite, b_pos, 2);
            } else if(ball.team_no == 0) {
                set.add_sprite(b0_sprite, b_pos, 2);
            }
        }
    }
}

ge211::Basic_dimensions<int> View::initial_window_dimensions() const
{
    ge211::Basic_dimensions<int> wd = {(int) model_.pt.board.dimensions()
                                       .width , (int) model_.pt.board
                                       .dimensions().height};
    return wd;
}
std::string View::initial_window_title() const
{
    return "8-ball q";
}
