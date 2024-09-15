#ifndef HPPOTION_H
#define HPPOTION_H

#include <godot_cpp/classes/node3d.hpp>

namespace godot {

class HpPotion : public Node3D {
	GDCLASS(HpPotion, Node3D)
private:

protected:
	static void _bind_methods(){};
    
public:
    HpPotion(/* args */){};
    ~HpPotion(){};
};

}

#endif


