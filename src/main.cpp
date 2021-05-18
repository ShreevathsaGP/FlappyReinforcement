#include <iostream>
#include <string>
#include "SDL2/SDL.h"
#include "learner.h"
#include "game.h"

using namespace std;

int main() {
    // initialize SDL Components
    SDL_Init(SDL_INIT_VIDEO);
    SDL_Window *window = SDL_CreateWindow("Flappy Reinforcements", 0, 0, window_width, window_height, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer = SDL_CreateRenderer(window, -1, 0);

    // setup fps variables [fps = 30]
    const int fps = 30;
    const int frame_delay = 1000 / fps;
    Uint32 frame_start;
    int frame_time;

    // setup game variables
    GameData game_data;
    game_data.points = 0;
    game_data.epsilon = 0;
    game_data.episode = 0;

    // setup bird
    Bird bird; 

    // setup pipes
    short int no_pipes = 4;
    short int space_between_pipes = 0.314 * window_height;
    short int min_pipe_height = 0.1 * window_height;
    short int max_pipe_height = 0.585 * window_height;
    short int top_height = get_random_value(min_pipe_height, max_pipe_height);
    short int bottom_height = window_height - top_height - space_between_pipes;
    short int last_index = 0;
    Pipe *new_bottom_pipe = new Pipe(bottom_height);
    Pipe *new_top_pipe = new Pipe(top_height);
    Pipe *bottom_pipes[no_pipes];
    Pipe *top_pipes[no_pipes];
    bottom_pipes[0] = new_bottom_pipe;
    top_pipes[0] = new_top_pipe;

    cout << "Space Between Pipes: " << space_between_pipes << endl;

    // learner variables
    Learner learner = Learner();

    // tally variables
    unsigned int episodes_completed;
    unsigned int total_frames;
    
    bool running = true;
    bool learning = false;
    while (running) {
        // total frames
        frame_start = SDL_GetTicks();

        // poll events
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            switch(event.type) {
                // x button
                case SDL_QUIT:
                    running = false;

                // keyboard
                case SDL_KEYDOWN:
                    switch(event.key.keysym.sym) {
                        // escape button
                        case SDLK_ESCAPE:
                            running = false;

                        // space button
                        case SDLK_SPACE:
                            if (!learning) { bird.jump(); }
                    }
            }
        }

        // clear the window to black
        SDL_SetRenderDrawColor(renderer, 0x0, 0x0, 0x0, 0xFF);
        SDL_RenderClear(renderer);
    //--SDL2-------------------------------------------------------------------------------------------------

    //--LEARNER----------------------------------------------------------------------------------------------
    // make observation 
    learner.observe(bottom_pipes[last_index], bird); 

    // take action
    if (learner.take_action() == 1) { bird.jump(); }
    //--LEARNER----------------------------------------------------------------------------------------------

    //--UPDATE-----------------------------------------------------------------------------------------------
        // update & render bird
        bird.update();
        render_bird(renderer, bird);

        // update & render all pipes
        for (int i = 0; i < last_index + 1; i++) {
            top_pipes[i] -> update();
            bottom_pipes[i] -> update();
            render_top_pipe(renderer, *top_pipes[i]);
            render_bottom_pipe(renderer, *bottom_pipes[i]);
        }

        // check deletion of pipes
        if (bottom_pipes[0] -> check_deletion()) {
            // shift_array;
            for (int i = 0; i < last_index; i++) {
                *bottom_pipes[i] = *bottom_pipes[i + 1];
                *top_pipes[i] = *top_pipes[i + 1];
            }

            // free last_index memory
            free(bottom_pipes[last_index]);
            free(top_pipes[last_index]);
            last_index -= 1;
        }

        // check addition of pipes
        if ((check_pipe_addition(bird, bottom_pipes[last_index])) && (last_index < no_pipes))  {
            game_data.points += 1;
            last_index += 1;
            top_height = get_random_value(min_pipe_height, max_pipe_height);
            bottom_height = window_height - top_height - space_between_pipes;
            new_bottom_pipe = new Pipe(bottom_height);
            new_top_pipe = new Pipe(top_height);
            bottom_pipes[last_index] = new_bottom_pipe;
            top_pipes[last_index] = new_top_pipe;
        }

        // check collisions
        for (int i = 0; i < last_index + 1; i++) {
            if (handle_top_collision(bird, *top_pipes[i]) || handle_bottom_collision(bird, *bottom_pipes[i])) {
                
                // observe result
                learner.observe_result(bottom_pipes[last_index], bird, true);
                
                // update q-value
                learner.update_q_value();

                for (int i = 0; i < last_index + 1; i++) {
                    free(top_pipes[i]);
                    free(bottom_pipes[i]);
                }
                top_height = get_random_value(min_pipe_height, max_pipe_height);
                bottom_height = window_height - top_height - space_between_pipes;
                new_bottom_pipe = new Pipe(bottom_height);
                new_top_pipe = new Pipe(top_height);
                bottom_pipes[0] = new_bottom_pipe;
                top_pipes[0] = new_top_pipe;
                last_index = 0;
                game_data.points = 0;
                bird.die();
            }
        }

        // check out of bounds
        if ((bird.get_y() + (bird.get_height()/2) > window_height) || (bird.get_y() - (bird.get_height()/2) < 0)) {
            
            // observe result
            learner.observe_result(bottom_pipes[last_index], bird, true);
             
            // update q-value
            learner.update_q_value();

            for (int i = 0; i < last_index + 1; i++) {
                free(top_pipes[i]);
                free(bottom_pipes[i]);
            }
            top_height = get_random_value(min_pipe_height, max_pipe_height);
            bottom_height = window_height - top_height - space_between_pipes;
            new_bottom_pipe = new Pipe(bottom_height);
            new_top_pipe = new Pipe(top_height);
            bottom_pipes[0] = new_bottom_pipe;
            top_pipes[0] = new_top_pipe;
            last_index = 0;
            game_data.points = 0;
            bird.die();
        }
    //--UPDATE-----------------------------------------------------------------------------------------------

    //--LEARNER----------------------------------------------------------------------------------------------
        // observe result
        learner.observe_result(bottom_pipes[last_index], bird, false);

        // update q-value
        learner.update_q_value();
    //--LEARNER----------------------------------------------------------------------------------------------

    //--RENDER-----------------------------------------------------------------------------------------------
     // present the back buffer
        SDL_RenderPresent(renderer);

        // correct for [fps = 32]
        frame_time = SDL_GetTicks() - frame_start;
        if (frame_delay > frame_time) { SDL_Delay(frame_delay - frame_time); }
        total_frames += 1;
    //--RENDER-----------------------------------------------------------------------------------------------
    }

    //--CLEANUP----------------------------------------------------------------------------------------------
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    //--CLEANUP----------------------------------------------------------------------------------------------

    return 0;
}
