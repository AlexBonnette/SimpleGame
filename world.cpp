#include "world.h"

#include <cassert>
#include <algorithm>

#include "allegro5/allegro.h"
#include "allegro5/allegro_image.h"

#include "sprite.h"
#include "player_sprite.h"
#include "obstruction_sprite.h"
#include "collision.h"


using namespace std;

enum MYKEYS {
   KEY_UP, KEY_DOWN, KEY_RIGHT, KEY_LEFT
};
bool key[4] = { false, false, false, false};


namespace csis3700 {

    float zero = 0;

    const float gravity_acceleration = 600;

    void free_sprite(sprite* s) {
        delete s;
    }

    world::world() {
        worldTime = 0;

        crashLoc = new vec2d(float(30), float(35));

        float cS = float(crashSpeed);

        nextLevel = false;

        wumpas_coll = 0;
        row_gen.push_back(1);
        row_gen.push_back(4);
        row_gen.push_back(7);
        srand(time(NULL));

        image_library *imgLib = image_library::get();
        worldBackground.add_image(imgLib -> get("Background.png"), 0);
        level2.add_image(imgLib -> get("LavaBackground.png"), 0);
        worldWidth = worldBackground.get_width();

        crashSeq.add_image(imgLib -> get("Walking1.png"), 0.1);
        crashSeq.add_image(imgLib -> get("Walking2.png"), 0.1);
        crashSeq.add_image(imgLib -> get("Walking3.png"), 0.1);
        crashSeq.add_image(imgLib -> get("Walking4.png"), 0.1);
        crashSeq.add_image(imgLib -> get("Walking5.png"), 0.1);
        crashSeq.add_image(imgLib -> get("Walking6.png"), 0.1);
        crashSeq.add_image(imgLib -> get("Walking7.png"), 0.1);
        crashSeq.add_image(imgLib -> get("Walking8.png"), 0.1);

        wumpaIS.add_image(imgLib ->get("Wumpa_scaled.png"),0);

        // Wumpas vector
        for(int i = 0; i < 10; ++i){ // Setting up a vector for 10 wumpas
            wumpas* wumpa = new wumpas(rand()%2100+400, row_gen.at(rand()%3)*35);
            wumpa->set_image_sequence(&wumpaIS);
            wumpas_10.push_back(wumpa);
        }

        // Wumpas vector level 2
        for(int i = 11; i < 20; ++i){ // Setting up a vector for 10 wumpas
            wumpas* wumpa = new wumpas(rand()%3000+3250, row_gen.at(rand()%3)*35);
            wumpa->set_image_sequence(&wumpaIS);
            wumpas_10.push_back(wumpa);
        }

        // Elementals vector
        elementalIS.add_image(imgLib ->get("Fire.png"),0);
        for(int i = 0; i < 10; i++){
            elementals* fire = new elementals(rand()%2100+400,row_gen.at(rand()%3)*35);
            fire->set_image_sequence(&elementalIS);
            //fire->set_velocity(vec2d(-rand()%150+75));
            elementals_10.push_back(fire);
        }

        // Elementals vector: Level 2
        elementalIS.add_image(imgLib ->get("Fire.png"),0);
        for(int i = 11; i < 20; i++){
            elementals* fire = new elementals(rand()%3000+3250,row_gen.at(rand()%3)*35);
            fire->set_image_sequence(&elementalIS);
            fire->set_velocity(vec2d(-rand()%100-75));
            elementals_10.push_back(fire);
        }


        // Springs vector
        springIS.add_image(imgLib->get("spring.png"),0);
        for(int i = 0; i < 10; i++){//setting up a vector for 10 springs
            wumpas* spring = new wumpas(rand()%2100+400,row_gen.at(rand()%3)*35);
            spring->set_image_sequence(&springIS);
            springs.push_back(spring);
        }


        // Springs vector: Level 2
        springIS.add_image(imgLib->get("spring.png"),0);
        for(int i = 11; i < 20; i++){//setting up a vector for 10 springs
            wumpas* spring = new wumpas(rand()%3000+3250,row_gen.at(rand()%3)*35);
            spring->set_image_sequence(&springIS);
            springs.push_back(spring);
        }


        crash = new player_sprite(crashLoc -> get_x(), crashLoc -> get_y(), cS, zero);
        crash -> set_image_sequence(&crashSeq);

        crash -> set_velocity(vec2d(float(100), float(0)));
    }


    void world::set_crashLoc(float xNew, float yNew){
        crashLoc -> setPos(xNew, yNew);
    }

    world::world(const world& other) {
        assert(false); // do not copy worlds
    }

    world& world::operator =(const world& other) {
        assert(false); // do not assign worlds
    }

    world::~world() {
        for_each(sprites.begin(), sprites.end(), free_sprite);
        sprites.clear();

        for_each(wumpas_10.begin(), wumpas_10.end(), free_sprite);
        wumpas_10.clear();

        delete crash;
    }

    void world::handle_event(ALLEGRO_EVENT ev) {

        if(ev.type == ALLEGRO_EVENT_KEY_DOWN) {
         switch(ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
               key[KEY_UP] = true;
               lane_change_up();
               break;

            case ALLEGRO_KEY_DOWN:
               key[KEY_DOWN] = true;
               lane_change_down();
               break;

            case ALLEGRO_KEY_RIGHT:
                key[KEY_RIGHT] = true;
                crash -> set_velocity(vec2d(200));
                for (int i = 0; i < 19; ++i){
                    if (wumpas_10.at(i) -> get_collected()){
                        wumpas_10.at(i) -> set_velocity(vec2d(200));
                    }
                }
                break;

            case ALLEGRO_KEY_LEFT:
                key[KEY_LEFT] = true;
                crash -> set_velocity(vec2d(100));
                for (int i = 0; i < 19; ++i){
                    if (wumpas_10.at(i) -> get_collected()){
                        wumpas_10.at(i) -> set_velocity(vec2d(100));
                    }
                }
                break;

         }
      }
        else if(ev.type == ALLEGRO_EVENT_KEY_UP) {
         switch(ev.keyboard.keycode) {
            case ALLEGRO_KEY_UP:
               key[KEY_UP] = false;
               break;

            case ALLEGRO_KEY_DOWN:
               key[KEY_DOWN] = false;
               break;

            case ALLEGRO_KEY_RIGHT:
               key[KEY_RIGHT] = false;
               break;

            case ALLEGRO_KEY_LEFT:
               key[KEY_LEFT] = false;
               break;
         }
      }
    }


    void world::resolve_collisions() {
        // Wumpas collisions
        for(int i = 0; i < 19; ++i){
            if (crash -> collides_with(*wumpas_10.at(i))){
                wumpas_10.at(i) -> player_collect();
                vec2d score_pos = vec2d(crash -> get_x() + wumpas_coll*21, -1);
                wumpas_10.at(i) -> set_position(score_pos);
                wumpas_10.at(i) -> set_velocity(crash -> get_velocity());
                wumpas_coll++;
            }
        }

        // Elementals collisions
        for(int i=0;i<19;i++){
            if(crash->collides_with(*elementals_10.at(i))){
                exit(3);
            }
        }

        // Springs collisions
        for(int i=0;i<19;i++){
            if(crash->collides_with(*springs.at(i))){
                vec2d v= vec2d(springs.at(i)->get_x()-400,crash->get_y());
                crash->set_position(v);
                for(int i = 0; i < 19; i++){
                    if(wumpas_10.at(i)->get_collected()){
                       vec2d b= vec2d(crash->get_x()+21*wumpas_10.at(i)->get_x_pos_mult());
                        wumpas_10.at(i)->set_position(b);
                    }
                }
            }
        }
    }

    void world::advance_by_time(double dt) {
        for(vector<sprite*>::iterator it = sprites.begin(); it != sprites.end(); ++it)
          (*it)->advance_by_time(dt);
        resolve_collisions();
        worldTime += dt;
        crash -> advance_by_time(dt);

        // Advancing the wumpas
        for(int i = 0; i < 10; ++i){
            wumpas_10.at(i)->advance_by_time(dt);
        }

        // Advancing the elementals
        for(int i=0;i<10;i++){
            elementals_10.at(i)->advance_by_time(dt);
        }
        if(nextLevel){
            // Advancing the wumpas
            for(int i = 11; i < 19; ++i){
                wumpas_10.at(i)->advance_by_time(dt);
            }

            // Advancing the elementals
            for(int i=11;i<19;i++){
                elementals_10.at(i)->advance_by_time(dt);
            }
        }

        if (wumpas_coll >= 5){
            nextLevel = true;
        }

        al_identity_transform(&t);
        al_translate_transform(&t, -(crash -> get_x()) + 30, 0);
        al_use_transform(&t);

        if (nextLevel == false){
            worldBackground.draw(worldTime, (int(crash -> get_x())/worldWidth)*worldWidth, 0);
            worldBackground.draw(worldTime, worldWidth + (int(crash -> get_x())/worldWidth)*worldWidth, 0);
            worldBackground.draw(worldTime, -worldWidth + (int(crash -> get_x())/worldWidth)*worldWidth, 0);
        }
        else{
            level2.draw(worldTime, (int(crash -> get_x())/worldWidth)*worldWidth, 0);
            level2.draw(worldTime, worldWidth + (int(crash -> get_x())/worldWidth)*worldWidth, 0);
            level2.draw(worldTime, -worldWidth + (int(crash -> get_x())/worldWidth)*worldWidth, 0);
        }

    }

    void world::draw() {

        if (nextLevel == false)
            worldBackground.draw(worldTime, 0, 0);
        else{
            level2.draw(worldTime, 0, 0);
                // Drawing the wumpa fruits
            for(int i = 11; i < 19; ++i){
                wumpas_10.at(i) -> draw();
            }

            // Drawing the Elementals
            for(int i = 11; i <19; i++){
                elementals_10.at(i) -> draw();
            }

            // Drawing the springs
            for(int i = 11; i < 19; i++){
                springs.at(i) -> draw();
            }

        }
        crash -> draw();

        // Drawing the wumpa fruits
        for(int i = 0; i < 10; ++i){
            wumpas_10.at(i) -> draw();
        }

        // Drawing the Elementals
        for(int i = 0; i < 10; i++){
            elementals_10.at(i) -> draw();
        }

        // Drawing the springs
        for(int i = 0; i < 10; i++){
            springs.at(i) -> draw();
        }

    }

    bool world::should_exit() {
        return false;
    }


    float world::getRowY(int rowNum){

        assert(rowNum == 1 || rowNum == 2 || rowNum == 3);

        float yPos;

        if (rowNum == 1)
            yPos = 35;
        else if (rowNum == 2)
            yPos = 140;
        else if (rowNum == 3)
            yPos = 245;

    return yPos;
    }

    void world::lane_change_up(){
        if (row == 2){
            row = 1;
            set_crashLoc(crash -> get_x(), getRowY(1));
            crash -> set_position(*crashLoc);
        }
        else if (row == 1){
            row = 3;
            set_crashLoc(crash -> get_x(), getRowY(3));
            crash -> set_position(*crashLoc);
        }
        else {
            row = 2;
            set_crashLoc(crash -> get_x(), getRowY(2));
            crash -> set_position(*crashLoc);
        }
    }

    void world::lane_change_down(){
        if (row == 1){
            row = 2;
            set_crashLoc(crash -> get_x(), getRowY(2));
            crash -> set_position(*crashLoc);
        }
        else if (row == 2){
            row = 3;
            set_crashLoc(crash -> get_x(), getRowY(3));
            crash -> set_position(*crashLoc);
        }
        else {
            row = 1;
            set_crashLoc(crash -> get_x(), getRowY(1));
            crash -> set_position(*crashLoc);
        }
    }


} // End of namespace
