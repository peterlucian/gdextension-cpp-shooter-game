#include "enemy.h"
#include "bullet.h"

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/variant/callable.hpp>
#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_map.hpp>
#include <godot_cpp/classes/engine.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void Enemy::_bind_methods() {
    //ADD_SIGNAL(MethodInfo("hp_zero"));
    ADD_SIGNAL(MethodInfo("hp_zero", PropertyInfo(Variant::STRING, "who")));
}


void Enemy::_ready(){
    input = Input::get_singleton();
    input_map = InputMap::get_singleton();

    if (!Engine::get_singleton()->is_editor_hint()) {
        UtilityFunctions::print("Running in the editor, skipping timer setup");
        if(input_map){
            add_input_mapping("enemy_frente", Key::KEY_UP);
            add_input_mapping("enemy_tras", Key::KEY_DOWN);
            add_input_mapping("enemy_direita", Key::KEY_RIGHT);
            add_input_mapping("enemy_esquerda", Key::KEY_LEFT);
            add_input_mapping("enemy_blocos", Key::KEY_K);
            add_input_mapping("enemy_disparar", Key::KEY_L);
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

void Enemy::take_damage()
{
    hp -= 20;
    UtilityFunctions::print("Enemy taking damage");
    if (hp <= 0) {
        UtilityFunctions::print("Enemy hp is zero");
        emit_signal("hp_zero", "Enemy"); // Emit a signal when HP reaches zero
    }
}

Enemy::Enemy() : speed(10.0f), gravity(-9.8f), rotation_speed(1.5f), hp(100) {
    // Other initialization code if necessary
}

Enemy::~Enemy() {
	// Add your cleanup here.
}

void Enemy::add_input_mapping(String action_name, Key key_code) {
    // Create a new action if it doesn't exist
    //String action_name = "move_forward";
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

void Enemy::shoot() {
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
            
            cast_to<Bullet>(bullet)->set_Bullet_type(Bullet_Type::Enemy_Bullet);
            // Add the bullet to the scene
            get_parent()->add_child(bullet);
        } else {
            UtilityFunctions::print("Error: Failed to instance bullet!");
        }
    } else {
        UtilityFunctions::print("Error: Bullet scene is not valid!");
    }
}


void Enemy::_physics_process(double delta) {
        velocity = Vector3();
        velocity.y += gravity * delta;

        if (!Engine::get_singleton()->is_editor_hint()) {
            if (input->is_action_pressed("enemy_frente")) {
                velocity.z -= 1;
            }
            if (input->is_action_pressed("enemy_tras")) {
                velocity.z += 1;
            }
            if (input->is_action_pressed("enemy_esquerda")) {
                rotate_y(rotation_speed * delta);
            }
            if (input->is_action_pressed("enemy_direita")) {
                rotate_y(-rotation_speed * delta);
            }
            
            if (input->is_action_just_pressed("enemy_disparar")) {
                shoot();
            }   

            if (input->is_action_just_pressed("enemy_blocos")) {
                place_block();
            }
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

void Enemy::place_block() {
    Vector3 enemy_pos = get_global_position();
    Vector3 rotation = get_global_rotation_degrees();
    int vec_pos_block = (round(enemy_pos.x) + 1) + (round(enemy_pos.z) * 20) - 1;
    
    if (rotation.y >= -45 && rotation.y <= 45)
        vec_pos_block = vec_pos_block - 20 + 1; //previous row + 1 cell, top
    else if (rotation.y > 45 && rotation.y <= 135)
        ;//pass normal, left
    else if (rotation.y <= -45 && rotation.y >= -135)
        vec_pos_block = vec_pos_block + 2; // same row + 2 cells, right
    else
        vec_pos_block = vec_pos_block + 20 + 1;

    map_layout->add_new_block(vec_pos_block, Block_owner::Enemy_block);
    UtilityFunctions::print("player pos x ", enemy_pos.x, "plyare pos z ", enemy_pos.z);
    UtilityFunctions::print("vec pos block", vec_pos_block);
}