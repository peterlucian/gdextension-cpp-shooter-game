#ifndef BULLET_H
#define BULLET_H

#include <godot_cpp/classes/node3d.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/classes/rigid_body3d.hpp>
#include <godot_cpp/classes/node.hpp>

namespace godot {

enum Bullet_Type {
    Player_bullet,
    Enemy_Bullet
};

class Bullet : public RigidBody3D {
	GDCLASS(Bullet, RigidBody3D)
private:
    Vector3 direction;
    float speed;
    Bullet_Type type_of;

protected:
	static void _bind_methods();
    
public:
    Bullet(/* args */);
    ~Bullet();

    void _ready();
    void _physics_process(double delta);
    //void _on_body_entered(Node3D* body);

    void set_Bullet_type(Bullet_Type type){ type_of = type; }
    Bullet_Type get_Bullet_type() const { return type_of; } 
    void set_direction(const Vector3& dir);
};

}

#endif