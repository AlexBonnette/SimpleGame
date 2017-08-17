#ifndef __CDS_PHYS_SPRITE_H
#define __CDS_PHYS_SPRITE_H

#include <vector>
#include "sprite.h"

namespace csis3700 {

    class collision;

  /**
   * Physical sprites move using an approximation of newtonian
   * kinematics.
   */
  class phys_sprite : public sprite {
  public:
    phys_sprite(float initial_x = 0, float initial_y = 0, float initial_vx = 0, float initial_vy = 0);

    virtual void advance_by_time(double dt);

    virtual vec2d get_acceleration() const;
    virtual void set_acceleration(vec2d a);

    virtual vec2d get_velocity() const;
    virtual void set_velocity(vec2d v);

    virtual void add_force(vec2d f);

    rectangle bounding_box() const = 0;
    rectangle collision_rectangle(const sprite& other) const = 0;
    bool collides_with(sprite &other) = 0;

    void resolve(const collision &collision, sprite *other) = 0;


  private:
    vec2d velocity;
    std::vector<vec2d> forces;
  };
}
#endif /* PHYS_SPRITE_H */
