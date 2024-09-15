#include "player.h"
#include "bullet.h"
#include "block.h"

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/classes/mesh_instance3d.hpp>

#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_map.hpp>

#include <godot_cpp/core/math.hpp> // For lerp_angle and other math functions
#include <godot_cpp/variant/vector3.hpp>
#include <godot_cpp/variant/transform3d.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void Player::_bind_methods() {
    // Bind the signal that announces when HP reaches 0
	ADD_SIGNAL(MethodInfo("hp_zero", PropertyInfo(Variant::STRING, "who")));
}


void Player::_ready(){
    input = Input::get_singleton();
    input_map = InputMap::get_singleton();

    enemy_look = get_node<RayCast3D>("enemy_look");
    helper = get_node<RayCast3D>("helper");

    //adding keys
    if (!Engine::get_singleton()->is_editor_hint()) {
        UtilityFunctions::print("Running in the editor, skipping timer setup");
        if(input_map){
            add_input_mapping("player_frente", Key::KEY_W);
            add_input_mapping("player_tras", Key::KEY_S);
            add_input_mapping("player_direita", Key::KEY_D);
            add_input_mapping("player_esquerda", Key::KEY_A);
            add_input_mapping("player_blocos", Key::KEY_G);
            add_input_mapping("player_disparar", Key::KEY_H);
        }
    }
    
    bullet_pos = get_node<Node3D>("bulletPosition");
    Node *root_node = get_tree()->get_current_scene();
    if (!root_node) {
        UtilityFunctions::print("Root node not found.");
        return;
    }

    // Access the MapLayout node
    map_layout = root_node->get_node<MapLayout>("MapLayout");
    if (map_layout) {
        UtilityFunctions::print("MapLayout node found!");
        // You can now interact with the MapLayout node
    } else {
        UtilityFunctions::print("MapLayout node not found.");
    }

    bullet_scene = ResourceLoader::get_singleton()->load("res://bullet.tscn");
    if (bullet_scene.is_null()) {
        UtilityFunctions::print("Error: Bullet scene not found!");
    }

 
}

void Player::take_damage()
{
    hp -= 20;
    UtilityFunctions::print("Player taking damage");
    if (hp <= 0) {
        UtilityFunctions::print("Player hp is zero");
        emit_signal("hp_zero", "Player"); // Emit a signal when HP reaches zero
    }
}

Player::Player() : speed(10.0f), gravity(-9.8f), rotation_speed(1.2f), hp(100) {
    // Other initialization code if necessary
}

Player::~Player() {
	// Add your cleanup here.
}

void Player::add_input_mapping(String action_name, Key key_code) {
    // Create a new action if it doesn't exist
    if (!input_map->has_action(action_name)) {
        input_map->add_action(action_name);
    }

    // Create a new InputEventKey
    Ref<InputEventKey> input_event;
    input_event.instantiate();
    input_event->set_keycode(key_code);
    // Set the key code (e.g., W key)
    //input_event->set_scancode(Key::KEY_W);

    // Add the event to the action
    input_map->action_add_event(action_name, input_event);
}

void Player::shoot() {
    Vector3 rotation = get_global_rotation_degrees();
    UtilityFunctions::print("rotation y", rotation.y);
    if (bullet_scene.is_valid()) {
        RigidBody3D* bullet = cast_to<RigidBody3D>(bullet_scene->instantiate());
        if (bullet) {
            UtilityFunctions::print("Bullet instance created!");
            
            Transform3D bullet_pos_transform = bullet_pos->get_global_transform();
            
            bullet->set_global_transform(bullet_pos_transform);
            Vector3 direction = -get_global_transform().basis.get_column(2); // Assuming the player faces along the Z-axis
            bullet->set_linear_velocity(direction * 30.0); // Set bullet speed
            
            cast_to<Bullet>(bullet)->set_Bullet_type(Bullet_Type::Player_bullet);
            // Add the bullet to the scene
            get_parent()->add_child(bullet);
        } else {
            UtilityFunctions::print("Error: Failed to instance bullet!");
        }
    } else {
        UtilityFunctions::print("Error: Bullet scene is not valid!");
    }
}


void Player::_physics_process(double delta) {
        velocity = Vector3();
        velocity.y += gravity * delta;
        if (!Engine::get_singleton()->is_editor_hint()) {
            if (input->is_action_pressed("player_frente")) {
                velocity.z -= 1;
            }
            if (input->is_action_pressed("player_tras")) {
                velocity.z += 1;
            }
            if (input->is_action_pressed("player_esquerda")) {
                rotate_y(rotation_speed * delta);
            }
            if (input->is_action_pressed("player_direita")) {
                rotate_y(-rotation_speed * delta);
            }
            
            if (input->is_action_just_pressed("player_disparar")) {
                shoot();
            }   

            if (input->is_action_just_pressed("player_blocos")) {
                place_block();
            }
        }
        
        if (enemy_look->is_colliding()) {
            Node3D* collider = cast_to<Node3D>(enemy_look->get_collider());
            if (collider && collider->is_class("Enemy")) {
                Vector3 target_position = collider->get_global_transform().origin;
                helper->look_at(target_position, Vector3(0, 1, 0));
                //UtilityFunctions::print("Its colliding with enemy!!");
            
                // Now, rotate the helper by 180 degrees around the Y axis to account for the negative Z direction
                // Transform3D helper_transform = helper->get_global_transform();
                // helper_transform.basis = helper_transform.basis.rotated(Vector3(0, 1, 0), Math::deg_to_rad(180.0f));
                // helper->set_global_transform(helper_transform);

                Vector3 current_rotation = get_global_transform().basis.get_euler();
                Vector3 helper_rotation = helper->get_global_transform().basis.get_euler();
                
                // Adjust for negative Z direction by rotating 180 degrees
                //helper_rotation.y += Math::deg_to_rad(180.0f);
                
                current_rotation.y = Math::lerp_angle(current_rotation.y, helper_rotation.y , 0.15f );
                set_rotation(current_rotation);
                
            }
        } else {
            helper->set_rotation(Vector3(0, 0, 0));
            //UtilityFunctions::print("Helper Rotation set to zeros!!");
        }
        
        // Normalize direction and scale by speed
        velocity = velocity.normalized() * speed;
        
        // Rotate the velocity vector based on the player's orientation
        velocity = velocity.rotated(Vector3(0, 1, 0), get_rotation().y);
        // Setting velocity
        set_velocity(velocity);
        // Use move_and_slide for movement
        move_and_slide();

}

void Player::place_block() {
    Vector3 player_pos = get_global_position();
    Vector3 rotation = get_global_rotation_degrees();
    int vec_pos_block = (round(player_pos.x) + 1) + (round(player_pos.z) * 20) - 1;
    
    if (rotation.y >= -45 && rotation.y <= 45)
        vec_pos_block = vec_pos_block - 20 + 1; //previous row + 1 cell, top
    else if (rotation.y > 45 && rotation.y <= 135)
        ;//pass normal, left
    else if (rotation.y <= -45 && rotation.y >= -135)
        vec_pos_block = vec_pos_block + 2; // same row + 2 cells, right
    else
        vec_pos_block = vec_pos_block + 20 + 1;

    map_layout->add_new_block(vec_pos_block, Block_owner::Player_block);
    UtilityFunctions::print("player pos x ", player_pos.x, "plyare pos z ", player_pos.z);
    UtilityFunctions::print("vec pos block", vec_pos_block);
}