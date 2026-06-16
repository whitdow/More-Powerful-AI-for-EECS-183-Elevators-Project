/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Floor.cpp
 * Project UID 848fee0125dbb5eb53ed294f20dbef81
 *
 * <#Names#>
 * <#Uniqnames#>
 *
 * Final Project - Elevators
 */


#include "Floor.h"

using namespace std;

int Floor::tick(int currentTime) {
	//TODO: Implement tick
	int indicesToRemove[MAX_PEOPLE_PER_FLOOR];
	int explodedCount = 0;
	for (int i = 0; i < numPeople; ++i) {
		if (people[i].tick(currentTime)) {
			indicesToRemove[explodedCount] = i;
			++explodedCount;
		}
	}
	//returning 0 to prevent compilation error
	removePeople(indicesToRemove, explodedCount);
	return explodedCount;
}

void Floor::addPerson(Person newPerson, int request) {
	//TODO: Implement addPerson
	if (numPeople < MAX_PEOPLE_PER_FLOOR) {
		people[numPeople] = newPerson;
		++numPeople;

		if (request > 0) {
			hasUpRequest = true;
		}
		else if (request < 0) {
			hasDownRequest = true;
		}
	}
}

void Floor::removePeople(const int indicesToRemove[MAX_PEOPLE_PER_FLOOR],
	int numPeopleToRemove) {
	//TODO: Implement removePeople
	Person keptPeople[MAX_PEOPLE_PER_FLOOR];
	int keptCount = 0;
	for (int i = 0; i < numPeople; ++i) {
		bool removeThisPerson = false;
		for (int j = 0; j < numPeopleToRemove; ++j) {
			if (i == indicesToRemove[j]) {
				removeThisPerson = true;
			}
		}
		if (!removeThisPerson) {
			keptPeople[keptCount] = people[i];
			++keptCount;
		}
	}
	for (int i = 0; i < keptCount; ++i) {
		people[i] = keptPeople[i];
	}
	numPeople = keptCount;
	resetRequests();
}

void Floor::resetRequests() {
	//TODO: Implement resetRequests
	hasUpRequest = false;
	hasDownRequest = false;
	for (int i = 0; i < numPeople; ++i) {
		if (people[i].getTargetFloor() > people[i].getCurrentFloor()) {
			hasUpRequest = true;
		}
		else if (people[i].getTargetFloor() < people[i].getCurrentFloor()) {
			hasDownRequest = true;
		}
	}
}
//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

Floor::Floor() {
	hasDownRequest = false;
	hasUpRequest = false;
	numPeople = 0;
}

void Floor::prettyPrintFloorLine1(ostream& outs) const {
	string up = "U";
	outs << (hasUpRequest ? up : " ") << " ";
	for (int i = 0; i < numPeople; ++i) {
		outs << people[i].getAngerLevel();
		outs << ((people[i].getAngerLevel() < MAX_ANGER) ? "   " : "  ");
	}
	outs << endl;
}

void Floor::prettyPrintFloorLine2(ostream& outs) const {
	string down = "D";
	outs << (hasDownRequest ? down : " ") << " ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "o   ";
	}
	outs << endl;
}

void Floor::printFloorPickupMenu(ostream& outs) const {
	cout << endl;
	outs << "Select People to Load by Index" << endl;
	outs << "All people must be going in same direction!";
	/*  O   O
	// -|- -|-
	//  |   |
	// / \ / \  */
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " O   ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "-|-  ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " |   ";
	}
	outs << endl << "              ";
	for (int i = 0; i < numPeople; ++i) {
		outs << "/ \\  ";
	}
	outs << endl << "INDEX:        ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << i << "   ";
	}
	outs << endl << "ANGER:        ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << people[i].getAngerLevel() << "   ";
	}
	outs << endl << "TARGET FLOOR: ";
	for (int i = 0; i < numPeople; ++i) {
		outs << " " << people[i].getTargetFloor() << "   ";
	}
}

void Floor::setHasUpRequest(bool hasRequest) {
	hasUpRequest = hasRequest;
}

bool Floor::getHasUpRequest() const {
	return hasUpRequest;
}

void Floor::setHasDownRequest(bool hasRequest) {
	hasDownRequest = hasRequest;
}

bool Floor::getHasDownRequest() const {
	return hasDownRequest;
}

int Floor::getNumPeople() const {
	return numPeople;
}

Person Floor::getPersonByIndex(int index) const {
	return people[index];
}