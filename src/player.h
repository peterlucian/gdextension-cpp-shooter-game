#ifndef PLAYER_H
#define PLAYER_H

#include <godot_cpp/classes/character_body3d.hpp>
#include <godot_cpp/classes/input.hpp>
#include <godot_cpp/classes/area3d.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/class_db.hpp>

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource.hpp>

#include <godot_cpp/classes/input_event_key.hpp>
#include <godot_cpp/classes/input_map.hpp>

#include <godot_cpp/classes/ray_cast3d.hpp>

#include "bullet.h"
#include "map_layout.h"




namespace godot {

class Player : public CharacterBody3D {
	GDCLASS(Player, CharacterBody3D)

private:
    Vector3 velocity;
	
    float speed;
    float gravity;
	float rotation_speed;
	int hp;

	Ref<PackedScene> bullet_scene{};
	
	Bullet* bullet = nullptr;
	MapLayout* map_layout = nullptr;

	Node3D* bullet_pos = nullptr;
	
	Input* input = nullptr;
	InputMap* input_map = nullptr;

	RayCast3D* enemy_look = nullptr;
	RayCast3D* helper = nullptr;
	
	void shoot();
	void add_input_mapping(String, Key);
	void place_block();
	
	
protected:
	static void _bind_methods();

public:
	Player();
	~Player();

	void _physics_process(double delta);
    void _ready();

	void take_damage();
	void set_hp(){ hp = 100;}
    
};

}

#endif