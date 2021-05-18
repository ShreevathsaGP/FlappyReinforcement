#ifndef LEARNER_H_
#define LEARNER_H_

#include "SDL2/SDL.h"
#include "game.h"

// learner class
class Learner {
    public:
        // constructor
        Learner();

        // make observation
        void observe(Pipe* bottom_pipe, Bird &bird);

        // observe the resul
        void observe_result(Pipe *bottom_pipe, Bird &bird, bool dead);

        // take action
        int take_action();

        // update q-value
        void update_q_value();

    private:
        // disretize x
        int discretize_x(int x_in);

        // discretize y
        int discretize_y(int y_in);

        // reward variables
        short int chosen_pipe_index;
        short int death_reward;
        short int standard_reward;
        short int reward;

        // action variables
        short int no_actions;
        short int action; // nothing = 0 | jump = 1

        // observations
        short int observation[2];
        short int new_observation[2];
        
        // discretization variables
        int discrete_x;
        int discrete_y;
        int x_discretizer;
        float y_discretizer;

        // q variables
        float q_table[7][21][2]; // [x-pipe][y-pipemiddle][action]
        float learning_rate;
        float discount;
};

#endif //LEARNER_H_
