#include "map_layout.h"
#include "block.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/classes/control.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void MapLayout::_physics_process(double delta)
{
    for (size_t i = 0; i < level.size(); ++i) {
        Block* block_ptr = Object::cast_to<Block>(level[i]);
        if (block_ptr != nullptr && block_ptr->get_hp() <= 0) {
            block_ptr->queue_free();  // Mark the block for deletion
            level[i] = nullptr;       // Set the reference in the vector to nullptr
        }
    }
}

void MapLayout::_ready()
{
    if (Engine::get_singleton()->is_editor_hint()) {
        UtilityFunctions::print("Running in the editor, skipping timer setup");
        return;
    }

    Control* game_over_menu = get_node<Control>("../GameOverMenu");
    if (game_over_menu) {
        game_over_menu->set_visible(false); // Hide the game over menu after resetting
    }

    m_packed_resource = resource_load->load("res://block.tscn");
    //in.open("map.dat", std::ios::binary);
    //in.read(map_layout, sizeof(map_layout));
    reset_layout();

    // float step = 0.0f;
    // float count = 0.0f;
    // for (int i = 0; i < sizeof(map_layout); count++, i++){
    //     if(map_layout[i] == '\0')
    //         break;
    //     if(i%20 == 0 && i!=0){
    //         step+=1;
    //         count = 0;
    //     }
    //     if(map_layout[i] == 'x'){
    //         Node3D* temp = spawn();
    //         Vector3 pos = {count,0.5f,step};
    //         temp->set_global_position(pos);
    //         if(!temp->is_inside_tree())
    //             add_child(temp);
    //         level.push_back(temp);
    //     } else {
    //         level.push_back(nullptr);
    //     }
    // }

}

void MapLayout::reset_layout(){
    if(!level.empty()){
        for (auto block : level){
            if(block != nullptr){
                block->queue_free();
            }
        }
        
        level.clear();
    }
        

    float step = 0.0f;
    float count = 0.0f;
    for (int i = 0; i < sizeof(map_layout); count++, i++){
        if(map_layout[i] == '\0')
            break;
        if(i%20 == 0 && i!=0){
            step+=1;
            count = 0;
        }
        if(map_layout[i] == 'x'){
            Node3D* temp = spawn();
            Vector3 pos = {count,0.5f,step};
            temp->set_global_position(pos);
            cast_to<Block>(temp)->set_block_owner(Block_owner::Normal);
            if(!temp->is_inside_tree())
                add_child(temp);
            level.push_back(temp);
        } else {
            level.push_back(nullptr);
        }
    }

}

void MapLayout::_bind_methods()
{
}

MapLayout::MapLayout()
{
}

MapLayout::~MapLayout()
{
}

std::vector<Node3D *> &godot::MapLayout::get_level()
{
    return level;
}

void MapLayout::add_new_block(int i, Block_owner owner)
{   
    if(level[i] == nullptr){
        Node3D * new_block = spawn();
        new_block->set_global_position(Vector3(floor((i % 20) - 1), 0.5f, floor(i/20)));
        UtilityFunctions::print("player pos x map", round((i % 20) - 1), "plyare pos z map", round(i/20));
        new_block->add_to_group("Blocks");
        cast_to<Block>(new_block)->set_block_owner(owner);
        add_child(new_block);
        level[i] = new_block;
        //UtilityFunctions::print("vector at i", level[i]);
        if(owner == Block_owner::Player_block){
            if(level[i+1]!= nullptr && cast_to<Block>(level[i+1])->get_block_owner() == Block_owner::Enemy_block){
                level[i+1]->queue_free();
                level[i+1] = nullptr;
            }
            if(level[i-1]!= nullptr && cast_to<Block>(level[i-1])->get_block_owner() == Block_owner::Enemy_block){
                level[i-1]->queue_free();
                level[i-1] = nullptr;
            }
            if(level[i+20]!= nullptr && cast_to<Block>(level[i+20])->get_block_owner() == Block_owner::Enemy_block){
                level[i+20]->queue_free();
                level[i+20] = nullptr;
            }
            if(level[i-20]!= nullptr && cast_to<Block>(level[i-20])->get_block_owner() == Block_owner::Enemy_block){
                level[i-20]->queue_free();
                level[i-20] = nullptr;
            }
        }

        if(owner == Block_owner::Enemy_block){
            if(level[i+1]!= nullptr && cast_to<Block>(level[i+1])->get_block_owner() == Block_owner::Player_block){
                level[i+1]->queue_free();
                level[i+1] = nullptr;
            }
            if(level[i-1]!= nullptr && cast_to<Block>(level[i-1])->get_block_owner() == Block_owner::Player_block){
                level[i-1]->queue_free();
                level[i-1] = nullptr;
            }
            if(level[i+20]!= nullptr && cast_to<Block>(level[i+20])->get_block_owner() == Block_owner::Player_block){
                level[i+20]->queue_free();
                level[i+20] = nullptr;
            }
            if(level[i-20]!= nullptr && cast_to<Block>(level[i-20])->get_block_owner() == Block_owner::Player_block){
                level[i-20]->queue_free();
                level[i-20] = nullptr;
            }
        }
    }
}

Node3D *MapLayout::spawn()
{
    block = cast_to<Node3D>(m_packed_resource->instantiate());
    // floor_one->set_global_position({global_pos.x+640, 308});
    // add_child(floor_one);
    return block;
}
