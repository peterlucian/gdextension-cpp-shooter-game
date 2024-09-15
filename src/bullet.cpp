#include "bullet.h"
#include "block.h"
#include "player.h"
#include "enemy.h"

#include <godot_cpp/classes/scene_tree.hpp>
#include <godot_cpp/classes/scene_tree_timer.hpp>
#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include <godot_cpp/classes/world3d.hpp>
#include <godot_cpp/classes/physics_ray_query_parameters3d.hpp>
#include <godot_cpp/classes/physics_server3d.hpp>
#include <godot_cpp/classes/physics_direct_space_state3d.hpp>
#include <godot_cpp/variant/dictionary.hpp>
#include <godot_cpp/variant/vector3.hpp>

using namespace godot;

void Bullet::_bind_methods()
{
    //ClassDB::bind_method(D_METHOD("_on_body_entered", "body"), &Bullet::_on_body_entered);
}

Bullet::Bullet() : direction(Vector3(0, 0, 0)), speed(5.0f)
{
    //set_contacts_reported(1); // Adjust this number based on how many collisions you want to track
    //set_contact_monitor(true);
    //set_max_contacts_reported(1);
}

Bullet::~Bullet()
{
}

void Bullet::_ready()
{
    
    connect("body_entered", Callable(this, "_on_body_entered"));
    
    if (Engine::get_singleton()->is_editor_hint()) {
        UtilityFunctions::print("Running in the editor, skipping timer setup");
        return;
    }

    // Optionally, queue free the bullet after a certain time to prevent it from existing forever
    SceneTree* tree = get_tree();
    if (tree) {
        tree->create_timer(5.0)->connect("timeout", Callable(this, "queue_free"));
        UtilityFunctions::print("Timer has been set");
    }

}

void Bullet::_physics_process(double delta)
{

    Vector3 start_position = get_global_transform().origin;
    Vector3 direction = -get_global_transform().basis.get_column(2); // Assuming Z is forward
    Vector3 end_position = start_position + direction * 2;

    // Get the space state from the physics server
    //Ref<World3D> world = get_world_3d();
    PhysicsDirectSpaceState3D* space_state = get_world_3d()->get_direct_space_state();

      // Create a new PhysicsRayQueryParameters3D object
    Ref<PhysicsRayQueryParameters3D> ray_query = memnew(PhysicsRayQueryParameters3D);
    ray_query->set_from(start_position);
    ray_query->set_to(end_position);
    //ray_query->set_collision_mask(collision_mask); // Optional: specify a collision mask
    // Optionally exclude certain objects
    //ray_query->set_exclude(Array());

    // Perform the raycast
    Dictionary result = space_state->intersect_ray(ray_query);

    // Check if the raycast hit something
    if (result.size() > 0) {
        Node3D *collider = cast_to<Node3D>(result["collider"]);

        if (collider) {
            // Handle the collision, e.g., apply damage or destroy the bullet
            //UtilityFunctions::print("Bullet hit a",collider->get_class());
            if(collider->get_parent()->is_class("Block")){
                UtilityFunctions::print("Bullet hit a",collider->get_parent()->get_class());
                
                Block *block = cast_to<Block>(collider->get_parent());
                if (block->is_in_group("Blocks")){
                    
                    if (get_Bullet_type() == Bullet_Type::Player_bullet && block->get_block_owner() == Block_owner::Enemy_block) {
                        UtilityFunctions::print("Bullet hit a Enemy Block!");
                        // Call the take_damage method on the player
                        queue_free();
                        block->take_damage(20); 
                        // Queue free the bullet
                        //queue_free();     
                    }  else {
                        UtilityFunctions::print("The object is not in the Blocks group.");
                    }

                    if (get_Bullet_type() == Bullet_Type::Enemy_Bullet && block->get_block_owner() == Block_owner::Player_block) {
                        UtilityFunctions::print("Bullet hit a Player Block!");
                        // Call the take_damage method on the player
                        queue_free();
                        block->take_damage(20); 
                        // Queue free the bullet
                        //queue_free();     
                    }  else {
                        UtilityFunctions::print("The object is not in the Blocks group.");
                    }

                }
            }
            
            //UtilityFunctions::print("Bullet hit a",body->get_class());
            if(collider->is_class("Player") && get_Bullet_type() == Bullet_Type::Enemy_Bullet){
                Player * player = cast_to<Player>(collider);
                queue_free();
                player->take_damage();
                UtilityFunctions::print("Player took 20 damage.");
            }

            if(collider->is_class("Enemy") && get_Bullet_type() == Bullet_Type::Player_bullet){
                Enemy * enemy = cast_to<Enemy>(collider);
                queue_free();
                enemy->take_damage();
                UtilityFunctions::print("Enemy took 20 damage.");
            }
        }
    }

    // Continue moving the bullet or other logic
    // Move the bullet in the direction it's facing
    //translate(direction * speed * delta);
}

// void Bullet::_on_body_entered(Node3D *body)
// {
//     UtilityFunctions::print("Bullet hit a body!");
     
//     if(body->get_parent()->is_class("Block")){
//         UtilityFunctions::print("Bullet hit a",body->get_parent()->get_class());
        
//         Block *block = cast_to<Block>(body->get_parent());
//         if (block->is_in_group("Blocks")){
            
//             if (get_Bullet_type() == Bullet_Type::Player_bullet && block->get_block_owner() == Block_owner::Enemy_block) {
//                 UtilityFunctions::print("Bullet hit a Enemy Block!");
//                 // Call the take_damage method on the player
//                 queue_free();
//                 block->take_damage(20); 
//                 // Queue free the bullet
//                 //queue_free();     
//             }  else {
//                 UtilityFunctions::print("The object is not in the Blocks group.");
//             }

//             if (get_Bullet_type() == Bullet_Type::Enemy_Bullet && block->get_block_owner() == Block_owner::Player_block) {
//                 UtilityFunctions::print("Bullet hit a Player Block!");
//                 // Call the take_damage method on the player
//                 queue_free();
//                 block->take_damage(20); 
//                 // Queue free the bullet
//                 //queue_free();     
//             }  else {
//                 UtilityFunctions::print("The object is not in the Blocks group.");
//             }

//         }
//     }
    
//     //UtilityFunctions::print("Bullet hit a",body->get_class());
//     if(body->is_class("Player") && get_Bullet_type() == Bullet_Type::Enemy_Bullet){
//         Player * player = cast_to<Player>(body);
//         queue_free();
//         player->take_damage();
//         UtilityFunctions::print("Player took 20 damage.");
//     }

//     if(body->is_class("Enemy") && get_Bullet_type() == Bullet_Type::Player_bullet){
//         Enemy * enemy = cast_to<Enemy>(body);
//         queue_free();
//         enemy->take_damage();
//         UtilityFunctions::print("Enemy took 20 damage.");
//     }
// }

void Bullet::set_direction(const Vector3& dir) {
    //direction = dir.normalized();
}