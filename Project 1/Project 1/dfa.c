//
//  dfa.c
//  Project 1
//
//  Created by Ryan Hilton on 9/20/20.
//  Copyright © 2020 Ryan Hilton. All rights reserved.
//

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include "dfa.h"
#include <string.h>


struct state{ //creates a STATES struct whcih includes transitions
                 //for 128 characters
    int transition[128];
    bool isAccepting;

}; typedef struct state STATE;

struct DFA{ //creates a DFA struct with a variable for number of states,
                 //what the current state is, and a pointer array of STATES
    int numOfStates;
    int currentState;
    STATE* stateArray;
}; typedef struct DFA DFA;



/**
 * Allocate and return a new DFA containing the given number of states.
 */
extern DFA* DFA_new(int nstates){
    DFA dfa = (DFA*)malloc(sizeof(struct DFA));
    dfa->numOfStates = nstates;
    dfa->currentState = 0;
    dfa->stateArray = (STATE *)malloc(nstates*sizeof(STATE));
    for (int i = 0; i < nstates; i++) {
        dfa->stateArray[i].isAccepting = false;
    }
    for (int i = 0; i < nstates; i++) {
        for (int j = 0; j < 128; j++) {
            dfa->stateArray[i].transition[j] = -1;
        }
    }
    return dfa;
}

/**
 * Free the given DFA.
 */
extern void DFA_free(DFA* dfa){
    free(dfa->stateArray);
    free(dfa);
};

/**
 * Return the number of states in the given DFA.
 */
extern int DFA_get_size(DFA* dfa){
    return dfa->numOfStates;
}

/**
 * Return the state specified by the given DFA's transition function from
 * state src on input symbol sym.
 */
extern int DFA_get_transition(DFA* dfa, int src, char sym){
    return dfa->stateArray[src].transition[sym];
}

/**
 * For the given DFA, set the transition from state src on input symbol
 * sym to be the state dst.
 */
extern void DFA_set_transition(DFA* dfa, int src, char sym, int dst){
    dfa->stateArray[src].transition[sym] = dst;
}

/**
 * Set the transitions of the given DFA for each symbol in the given str.
 * This is a nice shortcut when you have multiple labels on an edge between
 * two states.
 */
extern void DFA_set_transition_str(DFA* dfa, int src, char *str, int dst){
    for (int i = 0; str[i] != '\0'; i++) {
        dfa->stateArray[src].transition[str[i]] = dst;
    }
}

/**
 * Set the transitions of the given DFA for all input symbols.
 * Another shortcut method.
 */
extern void DFA_set_transition_all(DFA* dfa, int src, int dst){
    for (int i = 0; i < 128; i++) {
        dfa->stateArray[src].transition[i] = dst;
    }
}
//sets the transition for all characters except for the
//characters in exc which will not be able to transition
extern void DFA_set_transition_exception(DFA* dfa, int src, int dst, char* exc) {
    for (int i = 0; i < 128; i++) {
        dfa->stateArray[src].transition[i] = dst;
    }
    for (int i = 0; exc[i] != '\0'; i++) {
        dfa->stateArray[src].transition[exc[i]] = -1;
    }
}

/**
 * Set whether the given DFA's state is accepting or not.
 */
extern void DFA_set_accepting(DFA* dfa, int state, bool value){
    dfa->stateArray[state].isAccepting = value;
}

/**
 * Return true if the given DFA's state is an accepting state.
 */
extern bool DFA_get_accepting(DFA* dfa, int state){
    return dfa->stateArray[state].isAccepting;
}

//sets the current state of the DFA
void DFA_set_current_state(DFA *dfa, int statenum) {
    dfa->currentState = statenum;
}

/**
 * Run the given DFA on the given input string, and return true if it accepts
 * the input, otherwise false.
 */
extern bool DFA_execute(DFA* dfa, char *input){
    for (int i = 0; input[i] != '\0'; i++) {
        int dst = dfa->stateArray[dfa->currentState].transition[input[i]];
        DFA_set_current_state(dfa, dst);
        if (dfa->currentState == -1) {
            return false;
        }
    }
    int final = dfa->currentState;
    return DFA_get_accepting(dfa, final);
}
