#ifndef __CDS_WORLD_H
#define __CDS_WORLD_H

#include "allegro5/allegro.h"

#include "sprite.h"
#include "player_sprite.h"
#include <vector>
#include "image_library.h"
#include "image_sequence.h"

namespace csis3700 {
  class world {
  public:

    world(); // "world"'s constructor

    ~world(); // "world"'s destructor

    world(const world& other); // The copy constructor, which we block
    world& operator =(const world& other); // The assignment operator, which we also block

    void handle_event(ALLEGRO_EVENT ev); // Handles all keyboard inputs

    void advance_by_time(double dt); // Advances world by time "dt"

    void draw(); // Draw the WHOLE dang world

    bool should_exit(); //

    void lane_change_up(); // Changes Crash's lane to be the 1 above it, or wraps around
    void lane_change_down(); // Changes Crash's lane to be the 1 below it, or wraps around

    float getRowY(int rowNum);

    void level_2_prep();

    int game_over(){return 0;}

  private:
    image_library *imgLib; // An image library

    image_sequence worldBackground; // Background
    image_sequence crashSeq; // The image_sequence associated w/ Crash
    image_sequence wumpaIS; // Image_sequence associated w/ the Wumpa fruits
    image_sequence elementalIS; // Image_sequence associated w/ the Elementals
    image_sequence springIS; // Image_sequence associated w/ the springs

    std::vector<wumpas*> wumpas_10; // A vector of pointers to "wumpas" sprites
    std::vector<elementals*> elementals_10; // A vector of pointers to "elementals" sprites
    std::vector<sprite*> springs; // A vector of pointers to "sprites"
    std::vector<int> row_gen; // Vector containing
    int wumpas_coll; // Or this
    int wumpas_on_screen = 0; // This 1 isn't any better

    player_sprite *crash; // The player: Crash Bandicoot

    int worldWidth; // Will be populated w/ the world's width

    vec2d *crashLoc; // Will keep track of Crash's position in X & Y coordinates
    double crashSpeed; // Crash's speed across the screen
    int row = 1; // His initial row number

    ALLEGRO_TRANSFORM t; // I'm PRETTY sure we'll only need 1 transform

    double worldTime; // Guess what this is?

    void set_crashLoc(float xNew, float yNew);
    void resolve_collisions(); // It, uhh, resolves collisions

    // Level 2 stuff!!!!

    image_sequence level2;
    bool nextLevel;

    // These aren't being used right now:
    player_sprite *player;
    std::vector<sprite*> sprites;
  };
}

#endif /* WORLD_H */
