#include "sprite.h"
#include "phys_sprite.h"
#include <vector>

using namespace std;

namespace csis3700 {

    phys_sprite::phys_sprite(float initial_x, float initial_y, float initial_vx, float initial_vy) : sprite(initial_x, initial_y) {
        velocity = vec2d(initial_vx, initial_vy);
    }

    void phys_sprite::advance_by_time(double dt) {
        sprite::advance_by_time(dt);
        position = position + dt * velocity;
        velocity = velocity + dt * get_acceleration();
    }

    void phys_sprite::set_velocity(vec2d v) {
        velocity = v;
    }

    void phys_sprite::set_acceleration(vec2d a){
        forces.at(0) = a;
    }

    vec2d phys_sprite::get_acceleration() const {
        vec2d a(0,0);
        for(vector<vec2d>::const_iterator it = forces.begin(); it != forces.end(); ++it)
            a += *it;
    return a;
    }

    vec2d phys_sprite::get_velocity() const {
        return velocity;
    }

    void phys_sprite::add_force(vec2d f) {
        forces.push_back(f);
    }


}

