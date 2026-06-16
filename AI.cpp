/*
 * Copyright 2023 University of Michigan EECS183
 *
 * AI.cpp
 * Project UID 848fee0125dbb5eb53ed294f20dbef81
 *
 * <#Names#>
 * <#Uniqnames#>
 *
 * Final Project - Elevators
 */

#include "AI.h"
#include <cassert>
#include <vector>
#include <cstdlib>
#include <cmath>
#include <algorithm>
#include <memory>
#include <fstream>

// This file is used only in the Reach, not the Core.
// You do not need to make any changes to this file for the Core

/*
* Requires: buildingState is a valid state representing the current building
* Modifies: Nothing
* Effects:  Returns a string that represents the decision the AI makes
*           given the current state of the building, which it reads
*           from buildingState.
*           The string should share the exact format as a
*           human player's move input.
*/
string getAIMoveString(const BuildingState& buildingState) {
    //srand(time(0));

    // Use this array to calculate the value of some given move
    // Iterate over it, and then moves the corrisponding elevator to the highest value floor
    // default value is 0---means that all of the elevators are servicing
    double posValue[NUM_ELEVATORS][NUM_FLOORS];
    double value;
    double maxValue = 0;
    int chosenElevator;
    int chosenFloor;
    string moveString = "";

    // Determine the available elevators
    // If none are available, return "" to pass the move
    // Continue to pass for as long as no elevators are available


    // Automatically sets the value of a move with a servicing elevator to 0
    // i.e. Can not use

    // If there is already an elevator going to a floor, we should reduce the value of sending another elevator to that floor

    for (auto& r : posValue) {
        for (double& d : r) {
            d = 0;
        }
    }

    for (int i = 0; i < NUM_ELEVATORS; i++) {
        for (int j = 0; j < NUM_FLOORS; j++) {
            if (buildingState.elevators[i].isServicing) {
                break;
            }
            else {
                double currentFloor = buildingState.elevators[i].currentFloor;
                int distance = fabs(currentFloor - j);

                int nearExploadMultiplier = 1;


                double totalAnger = 0;


                for (int i = 0; i < MAX_PEOPLE_PER_FLOOR; i++) {
                    totalAnger = totalAnger + buildingState.floors[j].people[i].angerLevel;

                    if (buildingState.floors[j].people[i].angerLevel + (distance % TICKS_PER_ANGER_INCREASE) == 9) {

                        if (buildingState.floors[j].people[i].angerLevel == 7) {
                            nearExploadMultiplier += 1;
                        }
                        else if (buildingState.floors[j].people[i].angerLevel == 8) {
                            nearExploadMultiplier += 2;
                        }
                        else if (buildingState.floors[j].people[i].angerLevel == 9) {
                            nearExploadMultiplier += 5;
                        }
                    }
                }


                




                double numPeople = buildingState.floors[j].numPeople;

                // The redundancy discount only improves performance with the "smart" pickuplist implimentation
                // I believe the improved efficiency of that implimentation allows the AI to edge out gains from
                // Something about the inefficiencies of the "dumb" pickup string system 
                double redundancyDiscount = 1;

                int e0Target = buildingState.elevators[0].targetFloor;
                int e1Target = buildingState.elevators[1].targetFloor;
                int e2Target = buildingState.elevators[2].targetFloor;

                switch (i) {
                    case 0: 
                        if (e1Target == j || e2Target == j) {
                            redundancyDiscount = .25;
                        }
                        break;
                    case 1:
                        if (e0Target == j || e2Target == j) {
                            redundancyDiscount = .25;
                        }
                        break;
                    case 2:
                        if (e0Target == j || e1Target == j) {
                            redundancyDiscount = .25;
                        }
                        break;
                    default: 
                        redundancyDiscount = 1;
                        break;

                }
                    


                //double randomNum = ((rand() % 9) + 1);


                value = ((totalAnger + (2 * numPeople)) * 100) / max( (((pow( distance + 1, 2 )) - .5 * nearExploadMultiplier) + 5), 1.0 );

                //value += 2 * nearExploadMultiplier;
                value *= redundancyDiscount;
                


                posValue[i][j] = value;

            }
            
        }
    }

    //ofstream myFile;
    //myFile.open("MoveListOutput.csv", ios::app);
   
    //myFile << buildingState.turn << "\n";
    //for (int i = 0; i < NUM_ELEVATORS; i++) {
    //    myFile << "E" << i << ", ";
    //    for (int j = 0; j < NUM_FLOORS; j++) {
    //        myFile << posValue[i][j] << ", ";
    //    }
    //    myFile << "\n";
    //}

    //myFile << "\n \n";


    // Updates max value
    // Upates the elevator and target floor with the optimal move
    for (int i = 0; i < NUM_ELEVATORS; i++) {
        for (int j = 0; j < NUM_FLOORS; j++) {
            if (posValue[i][j] > maxValue) {
                maxValue = posValue[i][j];
                chosenElevator = i;
                chosenFloor = j;
            }
        }
    }
   

    // Concats a moveString based on move type and returns
    if (maxValue == 0) {
        // Default of empty string
        return moveString;
    }
    else if (buildingState.elevators[chosenElevator].currentFloor == chosenFloor) {
        moveString = "e" + to_string(chosenElevator) + "p";
        return moveString;
    }
    else {
        moveString = "e" + to_string(chosenElevator) + "f" + to_string(chosenFloor);
        return moveString;
    }

    // Create some function that selectes an elevator based on the total anger on some floor
    // And it's proximity
    // Prioritize moving far-away elevator to full, angry floors over closer floors

}

/*
* Requires: buildingState is a valid state representing the current building
*           move is a pickup move that was generated by getAIMoveString()
*           floorToPickup represents the floor the pickup move occurred.
* Modifies: Nothing
* Effects:  Returns a string representing which people indices
*           should be picked up. The string should share the exact format
*           as a human player's pickup list input.
*/
string getAIPickupList(const Move& move, const BuildingState& buildingState,
                       const Floor& floorToPickup) {

    //int floor = buildingState.elevators[move.getElevatorId()].currentFloor;

    string pickupList = "";
    
    

    // Write implimentation to figure out all of the possible pickup lists
    // And then rate them by the number of people in it, the total rage level
    // And the total number of rage at the target position and two adjacent positions

    vector<shared_ptr<Person>> peopleOnFloor;

    int people = floorToPickup.getNumPeople();

    for (int i = 0; i < people; i++) {
        peopleOnFloor.push_back(make_shared<Person>(floorToPickup.getPersonByIndex(i)));
    }



    vector<shared_ptr<Person>> peopleGoingUp;
    vector<shared_ptr<Person>> peopleGoingDown;

    for (shared_ptr<Person> p : peopleOnFloor) {

        if (p->getTargetFloor() > p->getCurrentFloor()) {
            peopleGoingUp.push_back(p);
        }
        else {
            peopleGoingDown.push_back(p);
        }

    }

    //for (shared_ptr<Person> p : peopleGoingUp) {
    //    cout << "Up: " << p->getTargetFloor() << endl;
    //}
    //for (shared_ptr<Person> p : peopleGoingDown) {
    //    cout << "Down: " << p->getTargetFloor() << endl;
    //}



    vector<pair<double, vector<shared_ptr<Person>>>> ratedPickupLists;

    auto size = peopleGoingUp.size();

    //cout << size << endl;
   

    for (int i = 0; i < (1 << size); i++) {

        vector<shared_ptr<Person>> subPerson;

        for (int j = 0; j < size; j++) {
            if (i & (1 << j)) {
                subPerson.push_back(peopleGoingUp[j]);
            }

        }

        pair<double, vector<shared_ptr<Person>>> pairedSubPerson = { 0, subPerson };
        ratedPickupLists.push_back(pairedSubPerson);

    }

    size = peopleGoingDown.size();

    for (int i = 0; i < (1 << size); i++) {
        vector<shared_ptr<Person>> subPerson;

        for (int j = 0; j < size; j++) {
            if (i & (1 << j)) {
                subPerson.push_back(peopleGoingDown[j]);
            }
        }

        pair<double, vector<shared_ptr<Person>>> pairedSubPerson = { 0, subPerson };
        ratedPickupLists.push_back(pairedSubPerson);

    }

    //cout << ratedPickupLists.size() << endl;
    //for (auto p : ratedPickupLists) {
    //    for (shared_ptr<Person> person : p.second) {
    //        cout << person->getTargetFloor();
    //    }
    //    cout << endl;
    //}

    

    //cout << ratedPickupLists.size();

    for (auto& p : ratedPickupLists) {

        double pickupListRating = 0;

        auto size = p.second.size();
        double totalListAnger = 0;
        double distance = 0;
        double currentFloor = 0;
        int target = 0;

        if (p.second.size() > 0) {
            for (shared_ptr<Person> p : p.second) {


                totalListAnger += p->getAngerLevel();

                currentFloor = p->getCurrentFloor();

                if (distance < fabs(currentFloor - p->getTargetFloor())) {
                    distance = fabs(currentFloor - p->getTargetFloor());
                    target = p->getTargetFloor();


                }
            }


            double wightedAdjacentAnger = 0;
            double numPeopleAtTarget = 0;


            for (int i = 0; i < buildingState.floors[target].numPeople; i++) {
                wightedAdjacentAnger += buildingState.floors[target].people[i].angerLevel;
                //numPeopleAtTarget += buildingState.floors[target].numPeople;

            }

            if (NUM_FLOORS > target + 1) {
                for (int i = 0; i < buildingState.floors[target + 1].numPeople; i++) {
                    wightedAdjacentAnger += .05 * buildingState.floors[target + 1].people[i].angerLevel;
                    //numPeopleAtTarget += .5 * buildingState.floors[target + 1].numPeople;

                }
            }
            if (target > 0) {
                for (int i = 0; i < buildingState.floors[target - 1].numPeople; i++) {
                    wightedAdjacentAnger += .05 * buildingState.floors[target - 1].people[i].angerLevel;
                    //numPeopleAtTarget += .5 * buildingState.floors[target - 1].numPeople;

                }
            }

            


            //int random = (rand() % 5) + 1;

            pickupListRating = ((totalListAnger * 125) + ((wightedAdjacentAnger + numPeopleAtTarget) * 10) + (5 * size)) / ((distance + 1) * 5);

            p.first = pickupListRating;

        }
    }

    //string fileName = "C:\\Users\\dwhit\\Desktop\\EECS 183 Projects\\Playable Elevators\\Playable Elevators\\OutPutCSV\\PickupListScoresForTurn" + to_string(buildingState.turn) + ".csv";
    //ofstream out;
    //out.open(fileName, ios::app);

    //out << buildingState.turn << ",Value, Pickuplist \n";

    //for (auto p : ratedPickupLists) {
    //    out << " ," << p.first << ", L: ";
    //    
    //    for (int i = 0; i < p.second.size(); i++) {
    //        for (int j = 0; j < peopleOnFloor.size(); j++) {
    //            if (p.second[i] == peopleOnFloor[j]) {
    //                out << to_string(j);
    //            }
    //        }
    //    }
    //    out << "\n";
    //}

    //out << "\n \n";


    double maxScore = 0;
    vector<shared_ptr<Person>> selectedPickupList;

    for (auto p : ratedPickupLists) {
        if (p.first >= maxScore) {
            maxScore = p.first;
            selectedPickupList = p.second;
        }
    }




    for (shared_ptr<Person> p : selectedPickupList) {
        for (int i = 0; i < peopleOnFloor.size(); i++) {
            if (p == peopleOnFloor[i]) {
                pickupList.append(to_string(i));
            }
        }
    }


    //for (int i = 0; i < MAX_PEOPLE_PER_FLOOR; i++) {
    //    if (find(selectedPickupList.begin(), selectedPickupList.end(), buildingState.floors[floor]) != selectedPickupList.end()) {
    //        pickupList.append(to_string(i));
    //    }
    //}

     


/*
    int numPeople = floorToPickup.getNumPeople();
    
    // If no one to pickup, no one on list
    if (numPeople == 0) {
        return "";
    }
    
    // Determine the direction by the first person
    Person first = floorToPickup.getPersonByIndex(0);
    int direction = 0;
    if (first.getTargetFloor() > first.getCurrentFloor()) {
        direction = 1;
    }
    else {
        direction = -1;
    }
    
    // Only picking up people going in same direction
    for (int i = 0; i < numPeople; i++) {
        Person peep = floorToPickup.getPersonByIndex(i);
        int personDirection = (peep.getTargetFloor() > peep.getCurrentFloor()) ? 1 : -1;
        
        if (personDirection == direction) {
            pickupList += to_string(i);
        }
    }
  
    */
    return pickupList;
}



// Among other things, we have tried to weight the anger levels of the floors around the target floor differently than the anger of the
// target floor itself
// We have tried to add a multiplier to increase the value of moves that keep the elevator closer to the middle
// We have tried to add a multiplier to increase the value of moves to floors where the people are about to expload

// All of these only reduced out score. The strength of anger and distance as predictors of the value of some move are too strong