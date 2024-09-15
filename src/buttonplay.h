#ifndef BUTTON_PLAY_H
#define BUTTON_PLAY_H

#include "godot_cpp/classes/button.hpp"
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/label.hpp>


#include "player.h"
#include "enemy.h"
#include "map_layout.h"

namespace godot {


class ButtonPlay : public Button {
	GDCLASS(ButtonPlay, Button)
private:
    Label* winner_label;
    Player* player;
    Enemy* enemy;
    MapLayout* map_layout;
    Node* root_node;
   

protected:
	static void _bind_methods();
    
public:
    ButtonPlay(/* args */);
    ~ButtonPlay();

    void _ready();
    void _physics_process(double delta);

    void _on_hp_zero(String who);
    void _on_play_again_pressed();

};

}

#endif