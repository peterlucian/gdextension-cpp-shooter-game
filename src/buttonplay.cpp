#include "buttonplay.h"
#include <godot_cpp/variant/utility_functions.hpp>
#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/classes/engine.hpp>


using namespace godot;

void ButtonPlay::_bind_methods() {
    // Bind the _on_play_again_pressed method to handle button pressed event
    ClassDB::bind_method(D_METHOD("_on_play_again_pressed"), &ButtonPlay::_on_play_again_pressed);
    ClassDB::bind_method(D_METHOD("_on_hp_zero", "who"), &ButtonPlay::_on_hp_zero);

}

ButtonPlay::ButtonPlay()
{
}

ButtonPlay::~ButtonPlay()
{
}

void ButtonPlay::_ready()
{

    // Connect the button's pressed signal to our custom method
    connect("pressed", Callable(this, "_on_play_again_pressed"));

    root_node = get_tree()->get_current_scene();
    if (!root_node) {
        UtilityFunctions::print("Root node not found.");
        return;
    }

    // Get references to the map layout and player (adjust the node paths as needed)
    map_layout = root_node->get_node<MapLayout>("MapLayout"); // Adjust to your actual map node path
    player = root_node->get_node<Player>("GridContainer/SubViewportContainer/SubViewport/Player"); // Adjust to your actual player node path
    enemy = root_node->get_node<Enemy>("GridContainer/SubViewportContainer2/SubViewport/Enemy");

    // Get reference to the winner Label inside the GameOverMenu
    winner_label = get_node<Label>("../Label");

     // Connect their hp_zero signals to this button
    if (player) {
        player->connect("hp_zero", Callable(this, "_on_hp_zero"));
    }
    if (enemy) {
        enemy->connect("hp_zero", Callable(this, "_on_hp_zero"));
    }
}

void ButtonPlay::_physics_process(double delta)
{
}

// Function that resets the map and player HP when the button is pressed
void ButtonPlay::_on_play_again_pressed() {
     UtilityFunctions::print("Button on play again pressed!! button on play again pressed!!");
    // Reset the map layout
    if (map_layout) {
        map_layout->reset_layout(); // Assumes the map layout has a method to reset it
        UtilityFunctions::print("Inside map layout reset pressed!");
    }

    // Reset the player's HP
    if (player) {
        player->set_hp(); // Assumes the player has a reset_game method that restores HP
        player->set_position({16, 4, 16});
        player->set_rotation_degrees({0, 0, 0});
        UtilityFunctions::print("Player hp reset pressed!");
    }

     // Reset the player's HP
    if (enemy) {
        enemy->set_hp(); // Assumes the player has a reset_game method that restores HP
        enemy->set_position({4, 4, 4});
        enemy->set_rotation_degrees({0, -180, 0});
        UtilityFunctions::print("Enemy hp reset pressed!");
    }

    // Optionally, hide the game over menu if needed
    Control* game_over_menu = root_node->get_node<Control>("GameOverMenu");
    if (game_over_menu) {
        game_over_menu->set_visible(false); // Hide the game over menu after resetting
    }

    // Unpause the game if it was paused
    //get_tree()->set_pause(false);
}

void  ButtonPlay::_on_hp_zero(String who) {
        UtilityFunctions::print("Signal aquired!! signal aquired!");

        if (winner_label) {
            // Set the winner label based on who reached 0 HP first
            if (who == "Player") {
                winner_label->set_text("Enemy Wins!");
            } else if (who == "Enemy") {
                winner_label->set_text("Player Wins!");
            }
        }

        // Show the game over menu
        Control* game_over_menu = root_node->get_node<Control>("GameOverMenu");        
        if (game_over_menu) {
            game_over_menu->set_visible(true);
        }

        // Pause the game
        //get_tree()->set_pause(true);
    }