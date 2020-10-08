//
//  nfa.c
//  CSC173
//
//  Created by Ryan Hilton on 9/19/20.
//  Copyright © 2020 Ryan Hilton. All rights reserved.
//

#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h> // used by toString()

#include "IntHashSet.h"
#include "nfa.h"
#include "dfa.h"
#include "IntSet.h"
#include "math.h"


struct NFA {
    int nstates;
    IntHashSet **states; //first bracket is current state, second bracket is sym in int, and result is set of transitioning states
    //state *states;
    IntHashSet currentStates;
    int *acceptingStates;
    int acceptingStateNum;
}; typedef struct NFA* NFA;


NFA new_NFA(int nstates) {
    NFA this = (NFA)malloc(sizeof(struct NFA));
    this->nstates = nstates;
    this->states = malloc(128*sizeof(IntHashSet*));
    this->acceptingStates = (int*)calloc(nstates,sizeof(int));
    this->acceptingStateNum = 0;
    this->currentStates = new_IntHashSet(128);
    
    for(int i = 0; i < nstates; i++){
        this->states[i] = malloc(128*sizeof(IntHashSet));
        for(int j = 0; j < 128; j++){
            this->states[i][j] = new_IntHashSet(1);
        }
    }
    
    
    return this;
}

void NFA_free(NFA nfa) {
    free(nfa->states);
    free(nfa);
}

int NFA_get_size(NFA nfa) {
    return nfa->nstates;
}

IntHashSet NFA_get_transitions(NFA nfa, int state, char sym) {
    return nfa->states[state][(int)sym];
}


void NFA_add_transition(NFA nfa, int src, char sym, int dst) {
    IntHashSet_insert(nfa->states[src][sym], dst);
    printf("%d -> %d w %c\n",src,dst, sym);
}


void NFA_add_transition_str(NFA nfa, int src, char *str, int dst) {
    for(int i = 0; str[i] != '\0'; i++){
        IntHashSet_insert(nfa->states[src][str[i]], dst);
    }
}

void NFA_add_transition_all(NFA nfa, int src, int dst) {
    for(int i = 0; i < 128; i++){
        if(IntHashSet_isEmpty(nfa->states[src][i])){
            nfa->states[src][i] = new_IntHashSet(128);
            IntHashSet_insert(nfa->states[src][i], dst);
            printf("%d -> %d w %d\n",src,dst,i);
        } else {
            IntHashSet_insert(nfa->states[src][i], dst);
        }
    }
}
void NFA_set_accepting(NFA nfa, int state, bool value) {
    nfa->acceptingStates[state] = value;
    nfa->acceptingStateNum++;
}

bool NFA_get_accepting(NFA nfa, int state) {
    return nfa->acceptingStates[state];
}

bool NFA_execute(NFA nfa, char *input) {
    IntHashSet tempSet = new_IntHashSet(128); //the set of current states that you check
    IntHashSet tempSet1 = new_IntHashSet(128);
    IntHashSet unionSet;
    IntHashSet_insert(tempSet, 0);
    for(int q = 0; q < nfa->nstates; q++){
        for(int w = 0; w < 128; w++){
            printf("%s -> (%d,%d)\n",IntHashSet_toString(nfa->states[q][w]),q,w);
        }
        printf("\n");
    }
    
    
    for(int i = 0; input[i] != '\0'; i++){ //loops through characters
        printf("%c\n",input[i]);
        IntHashSet_union(tempSet1, tempSet);
        
        IntHashSetIterator iterator = IntHashSet_iterator(tempSet1);
        unionSet = new_IntHashSet(128);
        while(IntHashSetIterator_hasNext(iterator)){
            
            int iteratedVal = IntHashSetIterator_next(iterator);
            //IntHashSet_union(tempSet1, NFA_get_transitions(nfa, iteratedVal, i));
            tempSet1 = NFA_get_transitions(nfa, iteratedVal, input[i]);
            //tempSet1 = NFA_get_transitions(nfa, IntHashSetIterator_next(iterator), i);
            
            printf("%s\n",IntHashSet_toString(tempSet1));

            IntHashSet_union(unionSet, tempSet1);

            if(!IntHashSetIterator_hasNext(iterator)){
                tempSet = unionSet;
                break;
            }
        }
        //loop through each character. find all the states it can go to from previous set of states. then loop to the next character, this time looking from all the states it went to from last iteration.
    }

    nfa->currentStates = tempSet;
    IntHashSetIterator iterators = IntHashSet_iterator(nfa->currentStates);
    while(IntHashSetIterator_hasNext(iterators)){
//        if(nfa->states[IntHashSetIterator_next(iterators)].accept){
//            return true;
//        }
        if(nfa->acceptingStates[IntHashSetIterator_next(iterators)]){
            return true;
        }
        if(!IntHashSetIterator_hasNext(iterators)){
                break;
        }
    }
    return false;
}
//Steven and I could not finish the conversion function. We couldnt figure out how to redirect transitions.
DFA convert(NFA nfa) {
    DFA dfa = (DFA)malloc(sizeof(DFA));
    //dfa = new_DFA(NFA_get_size(nfa)); + number of duplicates
    
    dfa = new_DFA(pow(2,NFA_get_size(nfa)));
    
    for(int i = 0; i < NFA_get_size(nfa); i++){
        for(int j = 0; j < 128; j++){ //this gets to the NFA bitset transition
            if(IntHashSet_count(nfa->states[i][j]) > 1){
                DFA_Convert_States(dfa, nfa->states[i][j], i+(nfa->nstates), j); //i+nstates moves to open area
                DFA_set_transition(dfa, i, j, i+(nfa->nstates));
                nfa->states[i][j] = NULL;
            } else if(IntHashSet_count(nfa->states[i][j]) == 1){
                DFA_Convert_States(dfa, nfa->states[i][j], i, j);
            }
        }
    }
    return dfa;
}

void NFA_print(NFA nfa);
