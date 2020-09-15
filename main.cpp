#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <iostream>

using namespace std;

// data structures for snow flakes
const int total_flakes = 900;
const int total_layers = 3;
const int disp_width = 640;
const int disp_height = 400;

struct particle
{
    int x,y;
    int layer;
};

// Function that generates a random snowflake particle
void init_particle_snowflake(particle *flakes)
{
    for (int i = 0; i < total_flakes; i++)
    {
       flakes[i].x = rand() % disp_width;
       flakes[i].y = rand() % disp_height;
       flakes[i].layer = rand()%total_layers;
    }
}

// Function to draw all particles of snowflakes
void draw_particle_flakes(particle *flakes)
{
    for (int i = 0; i < total_flakes; i++)
    {
        // Where the black, gray, white colours are on the 13h colour scale
        unsigned char layer_color = 255 - (63 * flakes[i].layer);
        ALLEGRO_COLOR snow_color = al_map_rgb(layer_color, layer_color, layer_color);
        // , flakes[i].layer * 5 % 20
        // can do some stupid map since we have 3 layers (0, 1, 2)
        // layer 1 = white 255, 255, 255
        // layer 2 = silver 192 192 192 
        // layer 3 = gray 128 128 128 
        al_put_pixel(flakes[i].x, flakes[i].y, snow_color);
    }
}

// Update the particle flakes (location/layer)
void update_particle_flakes(particle *flakes)
{
    for (int i = 0; i < total_flakes; i++)
    {
        // Drop the particle down, velocity depends on layer
        flakes[i].y += flakes[i].layer+1;

        // Check wrap around
        // If off screen, generate new particle at top
        if (flakes[i].y > (disp_height - 1))
        {
            flakes[i].x = rand() % disp_width;
            flakes[i].y = 0;
            flakes[i].layer = rand() % total_layers;
        }

        // Generate small left/right motion
        flakes[i].x = (flakes[i].x + (2 - rand() % 5)) % disp_width;
    }
}

int main(int argc, char *argv[])
{
    // Allegro stuff
    al_init();
    al_install_keyboard();

    // 8 bit colour
    // al_set_new_display_option(ALLEGRO_COLOR_SIZE, 8, ALLEGRO_REQUIRE);
    ALLEGRO_DISPLAY *disp = al_create_display(disp_width, disp_height);
    al_clear_to_color(al_map_rgb(0, 0, 0));
    al_flip_display();

    // Key events for closing
    ALLEGRO_EVENT_QUEUE *event_queue = al_create_event_queue();
    ALLEGRO_EVENT event;
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    // Create the snowflakes
    particle *flakes = new particle[total_flakes];

    // Populate initial snowflake information
    for (int i = 0; i < total_flakes; i++)
    {
        init_particle_snowflake(flakes);
    }

    bool run = true;

    while (run)
    {
       // Close if user presses keyboard
       al_wait_for_event_timed(event_queue, &event, 0.0167);
       if (event.type == ALLEGRO_EVENT_KEY_DOWN)
       {
           cout << "Goodbye" << endl;
           run = false;
       }

       al_clear_to_color(al_map_rgb(0, 0, 0));
       update_particle_flakes(flakes);
       draw_particle_flakes(flakes);
       al_flip_display();
       al_wait_for_vsync();
    }

    al_destroy_display(disp);

	return 0;

}
