#ifndef GAME_H_
#define GAME_H_

#include "SDL2/SDL.h"

// window dimensions
const int window_height = 408;
const int window_width = 280;
/* const int window_height = 850; */
/* const int window_width = 583; */
const int fps = 32;

// game data
struct GameData {
    unsigned int points;
    float epsilon;
    unsigned int episode;
};

// bird class
class Bird {
    public:
        // constructor
        Bird();

        // jump
        void jump();

        // update
        void update();

        // get width
        int get_width();

        // get height
        int get_height();
        
        // get x
        int get_x();

        //  get y
        int get_y();

        // die
        void die();

        // get velocity 
        int get_velocity(); 

        // get_accleration
        int get_acceleration();

    private:
        // measurements
        int height;
        int width;

        // position
        int x;
        int y;

        // vertical movement
        int acceleration;
        int velocity;
        int max_velocity;
        int min_velocity;
        int jump_acceleration;

        // jumps
        bool jumped;
};

// pipe class
class Pipe {
    public:
        // constructor
        Pipe(int in_height);

        // check deletion
        bool check_deletion();

        // update
        void update();
        
        // get width
        int get_width();

        // get height
        int get_height();

        // get x
        int get_x();

        // get velocity
        int get_velocity();

    
    private:
        // measurements
        int width;
        int height;

        // position
        int x; // mid-value

        // horizontal movement
        int acceleration;
        int velocity;
};

// render functions
void render_bird(SDL_Renderer *renderer, Bird &bird);
void render_top_pipe(SDL_Renderer *renderer, Pipe &pipe);
void render_bottom_pipe(SDL_Renderer *renderer, Pipe &pipe);

// collision functions
bool handle_bottom_collision(Bird &bird, Pipe &pipe);
bool handle_top_collision(Bird &bird, Pipe &pipe);

// addition function
bool check_pipe_addition(Bird &bird, Pipe *pipe);

// helper functions
int get_random_value(int min, int max);
int get_max(int a, int b);


#endif // GAME_H_
