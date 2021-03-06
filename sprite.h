#ifndef __CDS_SPRITE_H
#define __CDS_SPRITE_H
#include "allegro5/allegro.h"
#include "image_sequence.h"
#include "vec2d.h"
#include "rectangle.h"
#include <cstdlib>

namespace csis3700 {

  class collision;

  class sprite {
  public:
    sprite(float initial_x, float initial_y);

    void set_image_sequence(image_sequence *s);

    /**
     * Destructor
     */
    virtual ~sprite();


    /**
     * these two should cause errors, no copying!
     */
    sprite(const sprite& other) { assert(false); }
    sprite& operator =(const sprite& other) { assert(false); }

    virtual int get_width() const;

    virtual int get_height() const;

    virtual float get_x() const;

    virtual float get_y() const;

    virtual vec2d get_position() const;

    virtual void set_position(vec2d &p);

    virtual vec2d get_velocity() const = 0;

    virtual void set_velocity(vec2d v) = 0;

    virtual bool is_passive() const;

    /**
     * Draw this sprite.
     */
    virtual void draw();

    /**
     * Returns true iff I collide with other. Default implementation
     * returns true iff my bounding box overlaps other's.
     */
    virtual bool collides_with(sprite &other) = 0;


    /**
     * Move time forward by the specified amount
     */
    virtual void advance_by_time(double dt);

    /**
     * Return this sprite's bounding box
     */
    virtual rectangle bounding_box() const;

    /**
     * Return the intersection of this sprite's bounding box with
     * other's bounding box.
     */
    virtual rectangle collision_rectangle(const sprite& other) const;

    virtual void resolve(const collision& collision, sprite* other) = 0;

  protected:

    /**
     * My position
     */
    vec2d position;

    /**
     * My current image sequence
     */
    image_sequence *sequence;

    /**
     * The time in seconds since the world began ticking
     */
    double time;
  };

  // Wumpas! (i.e. collectibles)

    class wumpas : public sprite {
        public:
            wumpas(float initial_wx, float initial_wy) : sprite(initial_wx,initial_wy){}
            virtual void draw();
            void player_collect();
            bool collides_with(sprite &other){ };
            void set_x_pos_mult(int x) {x_pos_mult = x;}
            int get_x_pos_mult(){return x_pos_mult;}
            virtual vec2d get_velocity() const {}
            virtual void set_velocity(vec2d v) {velocity = v;}
            virtual void resolve(const collision& collision, sprite* other){}
            bool get_collected(){return collected;}
            virtual void advance_by_time(double dt);

        private:
            vec2d score_position = vec2d((get_x()+(x_pos_mult)*42),0);
            vec2d velocity = vec2d(0,0);
            bool collected = false;
            int x_pos_mult = 0;
    };


    // Elementals! (i.e. enemies)

    class elementals : public sprite {
        public:
            elementals(float initial_ex, float initial_ey) : sprite(initial_ex,initial_ey){}
            virtual vec2d get_velocity() const {}
            virtual void set_velocity(vec2d v) {velocity = v;}
            virtual void resolve(const collision& collision, sprite* other){}
            virtual void advance_by_time(double dt);
            bool collides_with(sprite &other){ }
        private:
            vec2d velocity = vec2d(-50,0);

    };
}
#endif /* SPRITE_H */
