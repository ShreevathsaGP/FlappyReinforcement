#include <iostream>
#include <string>
#include "SDL2/SDL.h"
#include "learner.h"
#include "game.h"

using namespace std;

// learner -> constructor()
Learner::Learner() {
    // reward variables
    death_reward = -1000;
    standard_reward = 15;
    reward = standard_reward;

    // action variables
    no_actions = 2;
    action = 0;
    
    // discretization variables
    discrete_x = 7;
    discrete_y = 21;
    x_discretizer = 40;
    y_discretizer = 40;

    // q variables
    learning_rate = 0.6;
    discount = 0.6;

    // fill in q_table
    for (int j = 0; j < (discrete_x); j++) {
        for (int k = 0; k < (discrete_y); k++)  {
            for (int l = 0; l < no_actions; l++) {
                q_table[j][k][l] = 0;
            }
        }
    }
}

// learner -> observe()
void Learner::observe(Pipe *bottom_pipe, Bird &bird) {
    observation[0] = bottom_pipe -> get_x() - bird.get_x();
    observation[1] = (window_height - bottom_pipe -> get_height()) - bird.get_y();
}

// learner -> observe_restult()
void Learner::observe_result(Pipe *bottom_pipe, Bird &bird, bool dead) {
    if (dead) { reward = death_reward; } else { reward = standard_reward; }
    new_observation[0] = bottom_pipe -> get_x() - bird.get_x();
    new_observation[1] = (window_height - bottom_pipe -> get_height()) - bird.get_y();

    cout << new_observation[0] << " " << new_observation[1] << endl;
}

// learner -> take_action()
int Learner::take_action() {
    if (q_table[discretize_x(observation[0])][discretize_y(observation[1])][1] > q_table[discretize_x(observation[0])][discretize_y(observation[1])][0]) {
        action = 1;
    } else { action = 0; }
    return action;
}

// learner -> discretize_x()
int Learner::discretize_x(int x_in) {
    return (x_in / x_discretizer) - 1;
}

// learner -> discretize_y()
int Learner::discretize_y(int y_in) {
    if (y_in < 0) { y_in = (-1 * y_in) + window_height; }
    return (y_in / y_discretizer);
}

// learner -> update_q_value()
void Learner::update_q_value() {
    q_table[discretize_x(observation[0])][discretize_y(observation[1])][action] = (1 - learning_rate) * q_table[discretize_x(observation[0])][discretize_y(observation[1])][action] + discount * (reward + get_max(q_table[discretize_x(new_observation[0])][discretize_y(new_observation[1])][0], q_table[discretize_x(new_observation[0])][discretize_y(new_observation[1])][1]));
}
