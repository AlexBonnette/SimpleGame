#ifndef __CDS_PLAYER_SPRITE_H
#define __CDS_PLAYER_SPRITE_H

#include "phys_sprite.h"

namespace csis3700 {
    class player_sprite : public phys_sprite {
        public:
            player_sprite(float initial_x = 0, float initial_y = 0, float initial_vx = 0, float initial_vy = 0);
            virtual bool is_passive() const;
            virtual void set_on_ground (bool v);
            virtual void advance_by_time (double dt);
            virtual void resolve (const collision& collision, sprite* other);

            rectangle bounding_box() const;
            rectangle collision_rectangle(const sprite& other) const;
            bool collides_with(sprite &other);

        private:
            bool on_ground;
    };
}


#endif /* PLAYER_SPRITE_H */
