#ifndef MAP_LAYOUT_H
#define MAP_LAYOUT_H

#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/classes/packed_scene.hpp>
#include <godot_cpp/classes/resource.hpp>

#include "block.h"
#include <fstream>

namespace godot {

class MapLayout : public Node3D{
    GDCLASS(MapLayout, Node3D)
	
private:
    ResourceLoader* resource_load = ResourceLoader::get_singleton();
    Ref<PackedScene> m_packed_resource{};
    char map_layout[401];
    std::ifstream in;  
    Node3D* block;  
    std::vector<Node3D*> level;  
    //char map_layout[401] = "xxxxxxxxxxxxxxxxxxxxx##################xx##################xx##################xx##################xx##################xx##################xx##################xx##################xx##################xx##################xx##################xx##################xx##################xx##################xx##################xx##################xx##################xx##################xxxxxxxxxxxxxxxxxxxxx";               
                
                            
protected:
	static void _bind_methods();
    
public:
    MapLayout(/* args */);
    ~MapLayout();

    void _physics_process(double delta);
    void _ready();

    std::vector<Node3D*>& get_level();
    
    void add_new_block(int, Block_owner);
    void reset_layout();
    Node3D* spawn();
};

}

#endif