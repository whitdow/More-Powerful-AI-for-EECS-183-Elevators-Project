/*
 * Copyright 2023 University of Michigan EECS183
 *
 * Move.cpp
 * Project UID 848fee0125dbb5eb53ed294f20dbef81
 *
 * <#Names#>
 * <#Uniqnames#>
 *
 * Final Project - Elevators
 */

#include <cmath>
#include <sstream>
#include <stdio.h>      
#include <stdlib.h>
#include "Move.h"

using namespace std;

Move::Move(string commandString) : Move() {
	//TODO: Implement non-default constructor
	if (commandString == "") {
		isPass = true;
	}
	else if (commandString == "S" || commandString == "s") {
		isSave = true;
	}
	else if (commandString == "Q" || commandString == "q") {
		isQuit = true;
	}
	else if (commandString.length() >= 3 && commandString[0] == 'e') {
		elevatorId = commandString[1] - '0';

		if (commandString[2] == 'p') {
			isPickup = true;
		}
		else if (commandString[2] == 'f' && commandString.length() >= 4) {
			targetFloor = commandString[3] - '0';
		}
	}
}

bool Move::isValidMove(Elevator elevators[NUM_ELEVATORS]) const {
	//TODO: Implement isValidMove
	if (isPass || isSave || isQuit) {
		return true;
	}
	if (elevatorId < 0 || elevatorId >= NUM_ELEVATORS) {
		return false;
	}
	if (elevators[elevatorId].isServicing()) {
		return false;
	}
	if (isPickup) {
		return true;
	}
	if (targetFloor < 0 || targetFloor >= NUM_FLOORS) {
		return false;
	}
	if (targetFloor == elevators[elevatorId].getCurrentFloor()) {
		return false;
	}
	return true;
}

void Move::setPeopleToPickup(const string& pickupList, const int currentFloor,
	const Floor& pickupFloor) {
	//TODO: Implement setPeopleToPickup
	numPeopleToPickup = 0;
	totalSatisfaction = 0;

	for (int i = 0; i < pickupList.length(); ++i) {
		int index = pickupList[i] - '0';
		peopleToPickup[numPeopleToPickup] = index;
		++numPeopleToPickup;

		Person p = pickupFloor.getPersonByIndex(index);
		totalSatisfaction += MAX_ANGER - p.getAngerLevel();

		if (i == 0) {
			targetFloor = p.getTargetFloor();
		}
		else if (abs(p.getTargetFloor() - currentFloor) > abs(targetFloor - currentFloor)) {
			targetFloor = p.getTargetFloor();
		}
	}
}

//////////////////////////////////////////////////////
////// DO NOT MODIFY ANY CODE BENEATH THIS LINE //////
//////////////////////////////////////////////////////

Move::Move() {
	elevatorId = -1;
	targetFloor = -1;
	numPeopleToPickup = 0;
	totalSatisfaction = 0;
	isPass = false;
	isPickup = false;
	isSave = false;
	isQuit = false;
}

bool Move::isPickupMove() const {
	return isPickup;
}

bool Move::isPassMove() const {
	return isPass;
}

bool Move::isSaveMove() const {
	return isSave;
}

bool Move::isQuitMove() const {
	return isQuit;
}

int Move::getElevatorId() const {
	return elevatorId;
}

int Move::getTargetFloor() const {
	return targetFloor;
}

int Move::getNumPeopleToPickup() const {
	return numPeopleToPickup;
}

int Move::getTotalSatisfaction() const {
	return totalSatisfaction;
}

void Move::setTargetFloor(int inTargetFloor) {
	targetFloor = inTargetFloor;
}

void Move::copyListOfPeopleToPickup(int newList[MAX_PEOPLE_PER_FLOOR]) const {
	for (int i = 0; i < numPeopleToPickup; ++i) {
		newList[i] = peopleToPickup[i];
	}
}