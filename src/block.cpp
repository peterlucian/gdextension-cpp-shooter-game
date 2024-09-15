#include "block.h"

#include <godot_cpp/variant/color.hpp>

#include <godot_cpp/classes/mesh_instance3d.hpp>
#include <godot_cpp/classes/material.hpp> 
#include <godot_cpp/classes/standard_material3d.hpp>
#include <godot_cpp/classes/mesh.hpp>

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

// Initialize the static member variable
Ref<StandardMaterial3D> Block::player_material;
Ref<StandardMaterial3D> Block::enemy_material;
bool Block::materials_initialized = false; 

void Block::_bind_methods()
{
}

Block::Block(): hp(100)
{
}

Block::~Block()
{
}

void Block::_ready()
{
    Block::initialize_materials();

    MeshInstance3D* mesh_instance = get_node<MeshInstance3D>("MeshInstance3D");
    UtilityFunctions::print("Mesh instance is ok!");
  
    if(get_block_owner() == Block_owner::Player_block)
    {  
        //Ref<StandardMaterial3D> new_material = Ref<StandardMaterial3D>(memnew(StandardMaterial3D));
        //new_material->set_albedo(Color(0.9f, 0.3f, 0.9f));
        mesh_instance->set_surface_override_material(0, player_material);
    } 
    if(get_block_owner() == Block_owner::Enemy_block)
    {      
        //Ref<StandardMaterial3D> new_material = Ref<StandardMaterial3D>(memnew(StandardMaterial3D));
        //new_material->set_albedo(Color(0.70f, 0.60f, 0.10f));
        mesh_instance->set_surface_override_material(0, enemy_material);        
    } 
}

void Block::_physics_process(double delta)
{
    // if (hp <=0){
    //     queue_free();
    // }
}

void Block::take_damage(int dmg)
{
    UtilityFunctions::print("take damage was called");

    if(hp>0)
        hp -= dmg;
   
}

void Block::initialize_materials() {
    if (!materials_initialized) {
        // Initialize using memnew() inside this function
        player_material = Ref<StandardMaterial3D>(memnew(StandardMaterial3D));
        enemy_material = Ref<StandardMaterial3D>(memnew(StandardMaterial3D));

        // Set initial albedo values
        player_material->set_albedo(Color(0.9f, 0.3f, 0.9f));
        enemy_material->set_albedo(Color(0.70f, 0.60f, 0.10f));

        materials_initialized = true;  // Ensure it's only done once
    }
}