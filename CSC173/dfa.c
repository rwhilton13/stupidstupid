//
//  dfa.c
//  CSC173
//
//  Created by Ryan Hilton on 9/18/20.
//  Copyright © 2020 Ryan Hilton. All rights reserved.
//

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h> // used by toString()

#include "IntHashSet.h"
#include "dfa.h"
#include "IntSet.h"

struct state {
    int transitions[128];
    bool accept;
    
}; typedef struct state state;

struct DFA {
    int nstates;
    state* states;
    IntHashSet **convertStates;
};

DFA new_DFA(int nstates) {
    DFA this = (DFA)malloc(sizeof(struct DFA));
    if (this == NULL) {
        return NULL;
    }
    this->states = (state*)calloc(nstates, sizeof(state));
    this->nstates = nstates;
    for (int i = 0; i < nstates; i++) {
        this->states[i].accept = false;
    }
    for (int i = 0; i < nstates; i++) {
        for (int j = 0; j < 128; j++) {
            this->states[i].transitions[j] = -1;
        }
    }
    this->convertStates = malloc(128*sizeof(IntHashSet*));
    for(int i = 0; i < nstates; i++){
        this->convertStates[i] = malloc(128*sizeof(IntHashSet));
        for(int j = 0; j < 128; j++){
            this->convertStates[i][j] = new_IntHashSet(1);
        }
    }
    return this;
}

void DFA_free(DFA dfa){
    if (dfa == NULL) {
        return;
    }
    free(dfa->states);
    free(dfa);
}

int DFA_get_size(DFA dfa) {
    return dfa->nstates;
}

void DFA_Convert_States(DFA dfa, IntHashSet set, int row, int col){
    dfa->convertStates[row][col] = set;
}

int DFA_get_transition(DFA dfa, int src, char sym){
    return dfa->states[src].transitions[(int)sym];
}

void DFA_set_transition(DFA dfa, int src, char sym, int dst) {
    dfa->states[src].transitions[sym] = dst;
}
    
void DFA_set_transition_str(DFA dfa, int src, char *str, int dst){
    for(int i = 0; str[i] != '\0'; i++){
        dfa->states[src].transitions[str[i]] = dst;
    }
}
    
void DFA_set_transition_all(DFA dfa, int src, int dst) {
    for(int i = 0; i < 128; i++){
        dfa->states[src].transitions[i] = dst;
    }
}

void DFA_set_accepting(DFA dfa, int state, bool value){
    dfa->states[state].accept = value;
}

bool DFA_get_accepting(DFA dfa, int state) {
    return dfa->states[state].accept;
}

bool DFA_execute(DFA dfa, char *input){
    int currentState = 0;
    for(int i = 0; input[i] != '\0'; i++){
        currentState = dfa->states[currentState].transitions[input[i]];
        if(currentState == -1){
            return false;
        }
    }
    return DFA_get_accepting(dfa, currentState);
}

void DFA_print(DFA dfa) {
}
