#ifndef BLOCK_H
#define BLOCK_H

#include <godot_cpp/classes/node3d.hpp>

#include <godot_cpp/classes/ref.hpp>
#include <godot_cpp/classes/material.hpp> 
#include <godot_cpp/classes/standard_material3d.hpp>

namespace godot {

enum Block_owner {
    Player_block,
    Enemy_block,
    Normal
};

class Block : public Node3D {
	GDCLASS(Block, Node3D)
private:
    int hp;
    Block_owner block_own;

    static bool materials_initialized;
    static Ref<StandardMaterial3D> player_material;
    static Ref<StandardMaterial3D> enemy_material;

    static void initialize_materials();

protected:
	static void _bind_methods();
    
public:
    Block(/* args */);
    ~Block();

    void _ready();
    void _physics_process(double delta);

    void set_block_owner(Block_owner owner){
        block_own = owner;
    }
    Block_owner get_block_owner() const {return block_own; }
    
    int get_hp() const { return hp; }
    void take_damage(int);
};

}

#endif