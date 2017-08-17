#include "image_sequence.h"
#include "allegro5/allegro.h"
#include <cassert>


namespace csis3700 {

    image_with_offset::image_with_offset(ALLEGRO_BITMAP* i, double o) {
        image = i;
        offset = o;
    }

    image_sequence::image_sequence() {
        current = 0;
        last_change_time = 0;
        loop_index = 0;
    }

    void image_sequence::add_image(ALLEGRO_BITMAP *image, double offset) {
        assert(image != NULL);
        images.push_back(image_with_offset(image, offset));
    }

    void image_sequence::draw_current(float x, float y) {
        al_draw_bitmap(images[current].image, x, y, 0);
    }

    void image_sequence::draw(double time, float x, float y) {
        assert(images.size() > 0);
        if (images.size() == 1) {
            draw_current(x, y);
            return;
        }
        image_with_offset visible = images[current];
        size_t next_index = current+1;
        if (next_index >= images.size()) {
            next_index = loop_index;
        }
        image_with_offset next = images[next_index];
        if (time - last_change_time > next.offset) {
            current = next_index;
            last_change_time = time;
        }
        draw_current(x, y);
    }

    int image_sequence::get_width() const {
        assert(images.size() > 0);
        return al_get_bitmap_width(images[0].image);
    }

    int image_sequence::get_height() const {
        assert(images.size() > 0);
    return al_get_bitmap_height(images[0].image);
    }

    void image_sequence::set_loop_index(size_t loop_index) {
        assert(loop_index < images.size());
        this->loop_index = loop_index;
    }

}


/*
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_timer_event_source(timer);
    al_start_timer(timer);

    image_library *repo = image_library::get();

    image_sequence seq;

    seq.add_image(repo -> get("StickMan1.png"), 0);
    seq.add_image(repo -> get("StickMan2.png"), 0.1);
    seq.add_image(repo -> get("StickMan3.png"), 0.1);
    seq.add_image(repo -> get("StickMan2.png"), 0.1);

    image_sequence smoke;
    seq.add_image(repo -> get("KillerSmoke.png"), 0);

    ALLEGRO_BITMAP *background = repo -> get("Background.png");

    double time = 0;

    ALLEGRO_TRANSFORM t;


    double player_x = 50;
    double player_v = 20; // This is player velocity in px./sec.

    double smokeX = 850;
    double smokeY = 100;
    double smokeVX = -150;
    double smokeVY = 10;

    int backgroundWidth = al_get_bitmap_width(background);

    ALLEGRO_EVENT ev;

    do {

        al_wait_for_event(event_queue, &ev);

        if (ev.type == ALLEGRO_EVENT_TIMER){
            redraw = true;
            time += 1.0/FPS;
            player_x += player_v * 1.0/FPS;
            smokeX += smokeVX * 1.0/FPS;
            smokeY += smokeVY * 1.0/FPS;
        }

        if (redraw && al_is_event_queue_empty(event_queue)){
            redraw = false;
            al_identity_transform(&t);
            al_translate_transform(&t, -player_x+50, 0);
            al_use_transform(&t);
            al_draw_bitmap(background, (int(player_x)/backgroundWidth)*backgroundWidth, 0, 0);
            al_draw_bitmap(background, backgroundWidth + (int(player_x)/backgroundWidth)*backgroundWidth, 0, 0);
            al_draw_bitmap(background, -backgroundWidth + (int(player_x)/backgroundWidth)*backgroundWidth, 0, 0);

            seq.draw(time, player_x, 300); // Draws the the sequence (which is the animated stick figure)
            smoke.draw(time, smokeX, smokeY);
            al_identity_transform(&t);
            al_use_transform(&t);

            al_flip_display();
        } while(ev.type != ALLEGRO_EVENT_DISPLAY_CLOSE);

    }
*/
