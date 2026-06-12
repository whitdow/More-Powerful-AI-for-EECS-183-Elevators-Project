#include <iostream>
#include <vector>
#include <string>
#include <fstream> 
#include <cstdlib>
#include "Utility.h"

#define FILE "random_game.in"
#define PEOPLE_SPAWN (rand() % 3 == 2) // 1/3
#define NUM_PEOPLE_THAT_SPAWN (rand() % 10) + 1 // 1 - 10

#define STRING(x) to_string(x)

#define SPAWN_TURN STRING(player->spawn_turn)
#define CURRENT_FLOOR STRING(player->current_floor)
#define TARGET_FLOOR STRING(player->target_floor)
#define ANGER STRING(player->anger_level)

struct people_init_string {
    int spawn_turn;
    int current_floor;
    int target_floor;
    int anger_level;

    string out_string;

    people_init_string(int s, int c, int t, int a) :
        spawn_turn(s), current_floor(c), target_floor(t), anger_level(a) {}; 
};  

//-------------------------------------------------------------------------//

void build_out_string(people_init_string * player) {
    player->out_string = SPAWN_TURN 
        + "f" + CURRENT_FLOOR 
        + "t" + TARGET_FLOOR 
        + "a" + ANGER;
}

void print_header(ofstream & out) {
    for (int i = 0; i < 6; i++) {
        out << "0\n";
    }
    for (int i = 0; i < 3; i++) {
        out << "0w\n";
    }

}

people_init_string * people_factory(int i) {
    int spawn_turn = i;
    int current_floor = rand() % 10;
    int target_floor = rand() % 10;
    while (current_floor == target_floor) { target_floor = rand() % 10; }
    int anger = rand() % ( MAX_ANGER - 1 );

    people_init_string * new_people = new people_init_string(spawn_turn, current_floor, target_floor, anger);
    build_out_string(new_people);
    return new_people;
}

void print_people(ofstream & out, const vector<people_init_string*> & list) {
    for (const people_init_string * i : list) {
        out << i->out_string << "\n";
    }
}

void delete_people(vector<people_init_string*> & list) {
    for (people_init_string * it : list) {
        delete it;
    }
}

int main() {
    vector<people_init_string*> people_list;

    ofstream out;
    out.open(FILE);

    if(!out.is_open()) {cout << "FAILED TO OPEN FILE \n"; return -1;}
    
    print_header(out);
    for (int i = 0; i < 100; i++) {
        // randomly selects if people will be created on turn
        // how many people
        // create those people with random stats

        if (PEOPLE_SPAWN) { // 1/8 chance (can be adjusted to make harder rounds)
            // generates people 
            for (int j = 0; j < NUM_PEOPLE_THAT_SPAWN; j++) {
                people_list.push_back(people_factory(i));
            }
        }
    }


    print_people(out, people_list);
    delete_people(people_list);

    return 0;
}