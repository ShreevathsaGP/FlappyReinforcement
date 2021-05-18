#include <iostream>
#include <string>
#include "SDL2/SDL.h"
#include "game.h"

using namespace std;

// bird -> constructor()
Bird::Bird() {
    // measurements
    height = 0.0588 * window_height;
    width = height;

    // position
    x = window_width / 5;
    y = window_height / 2;

    // vertical movement
    acceleration = 1;
    velocity = 0.022 * window_height * -1;
    max_velocity = (-1 * velocity) + 1;
    min_velocity = velocity + 1;
    jump_acceleration = min_velocity;

    // jumps
    jumped = false;
}

// bird -> jump()
void Bird::jump() {
    velocity = jump_acceleration;
    jumped = true;
}

// bird -> update()
void Bird::update() {
    if (velocity < max_velocity && !jumped) {
        velocity += acceleration;
    }
    if (jumped) { jumped = false; }
    y += velocity;
}

// bird -> get_width()
int Bird::get_width() {
    return width;
}

// bird -> get_height()
int Bird::get_height() {
    return height;
}

// bird -> get_x()
int Bird::get_x() {
    return x;
}

// bird -> get_y()
int Bird::get_y() {
    return y;
}

// bird -> die()
void Bird::die() {
    y = window_height / 2;
}

// bird -> get_velocity()
int Bird::get_velocity() {
    return velocity;
}

// bird -> get_acceleration()
int Bird::get_acceleration() {
    return acceleration;
}

// pipe -> constructor()
Pipe::Pipe(int in_height) {
    width = 0.1855 * window_width;
    height = in_height;
    velocity = 0.0135 * window_width;
    x = window_width + (width / 2);
}

// pipe -> check_deletion()
bool Pipe::check_deletion() {
    if (x + (width / 2) < 0) { return true; }
    return false;
}

// pipe -> update()
void Pipe::update() {
    x -= 5;
}

// pipe -> get_width()
int Pipe::get_width() {
    return width;
}

// pipe -> get_height()
int Pipe::get_height() {
    return height;
}

// pipe -> get_x()
int Pipe::get_x() {
    return x;
}

// pipe -> get_velocity()
int Pipe::get_velocity() {
    return velocity;
}

// render_bird()
void render_bird(SDL_Renderer *renderer, Bird &bird) {
    // set white colour
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);

    SDL_Rect rect;
    rect.x = bird.get_x() - (bird.get_width() / 2);
    rect.y = bird.get_y() - (bird.get_height() / 2);
    rect.w = bird.get_width();
    rect.h = bird.get_height();
    SDL_RenderFillRect(renderer, &rect);
}

// render_top_pipe()
void render_top_pipe(SDL_Renderer *renderer, Pipe &pipe) {
    SDL_Rect rect;
    rect.x = pipe.get_x() - (pipe.get_width() / 2);
    rect.y = 0;
    rect.w = pipe.get_width();
    rect.h = pipe.get_height();
    SDL_RenderFillRect(renderer, &rect);
}

// render_bottom_pipe()
void render_bottom_pipe(SDL_Renderer *renderer, Pipe &pipe) {
    SDL_Rect rect;
    rect.x = pipe.get_x() - (pipe.get_width() / 2);
    rect.y = window_height - pipe.get_height();
    rect.w = pipe.get_width();
    rect.h = pipe.get_height();
    SDL_RenderFillRect(renderer, &rect);
}

// handle_bottom_collision()
bool handle_bottom_collision(Bird &bird, Pipe &pipe) {
    if ((bird.get_y() + (bird.get_height() / 2) > (window_height - pipe.get_height()))) {
        if (((bird.get_x() + (bird.get_width() / 2) > pipe.get_x() - (pipe.get_width() / 2)) && (bird.get_x() + (bird.get_width() / 2) < pipe.get_x() + (pipe.get_width() / 2))) || ((bird.get_x() - (bird.get_width() / 2) > pipe.get_x() - (pipe.get_width() / 2)) && (bird.get_x() - (bird.get_width() / 2) < pipe.get_x() + (pipe.get_width() / 2)))) {
            return true;
        }
    }
    return false;
}

// handle_top_collision()
bool handle_top_collision(Bird &bird, Pipe &pipe) {
    if ((bird.get_y() - (bird.get_height() / 2) < pipe.get_height())) {
        if (((bird.get_x() + (bird.get_width() / 2) > pipe.get_x() - (pipe.get_width() / 2)) && (bird.get_x() + (bird.get_width() / 2) < pipe.get_x() + (pipe.get_width() / 2))) || ((bird.get_x() - (bird.get_width() / 2) > pipe.get_x() - (pipe.get_width() / 2)) && (bird.get_x() - (bird.get_width() / 2) < pipe.get_x() + (pipe.get_width() / 2)))) {
            return true;
        }
    }
    return false;
}

// check_pipe_addition()
bool check_pipe_addition(Bird &bird, Pipe *pipe) {
    if (pipe -> get_x() < bird.get_x()) { return true; }
    return false;;
}

// get_random_value()
int get_random_value(int min, int max) {
   static bool first = true;
   if (first) {
      srand( time(NULL) ); //seeding for the first time only!
      first = false;
   }
   return min + rand() % (( max + 1 ) - min);
}

// get_max()
int get_max(int a, int b) {
    if (a > b) { return a; }
    return b;
}
