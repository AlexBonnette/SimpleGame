#include "player_sprite.h"
#include "image_library.h"
#include "image_sequence.h"
#include "vec2d.h"
#include "collision.h"
#include <cmath>
using namespace std;

namespace csis3700 {

    player_sprite::player_sprite(float initial_x, float initial_y, float initial_vx, float initial_vy) :
        phys_sprite(initial_x, initial_y, initial_vx, initial_vy) {
    }

    bool player_sprite::is_passive() const {
        return false;
    }

    void player_sprite::set_on_ground(bool v) {
        on_ground = v;
    }

    void player_sprite::advance_by_time(double dt) {
        phys_sprite::advance_by_time(dt);
    }

    void player_sprite::resolve(const collision& collision, sprite *other) {

    }


    rectangle player_sprite::bounding_box() const {
        return rectangle(position, get_width(), get_height());
    }

    rectangle player_sprite::collision_rectangle(const sprite& other) const {
        return bounding_box().intersection(other.bounding_box());
    }


    bool player_sprite::collides_with(sprite &other) {
        return !collision_rectangle(other).is_degenerate();
    }




    /*
    virtual int get_width() const{ }
    virtual int get_height() const{ }

    virtual float get_x() const{ }
    virtual float get_y() const{ }

    virtual vec2d get_position() const{ }

    virtual void set_position(vec2d p){ }
    */
    /*
    virtual vec2d player_sprite::get_velocity() const {
        return velocity;
    }

    virtual void player_sprite::set_velocity(vec2d& v) {
        velocity = v;
    }
    */

}
