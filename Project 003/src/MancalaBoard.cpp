#include "MancalaBoard.h"
#include <iostream>
using namespace std;
CMancalaBoard::CMancalaBoard(){
	DTurn = 0;
	for(int Index = 0; Index < MANCALA_TOTAL_PITS; Index++){
		/* Initializing every pit to have 4 stones */
		DPits[Index] = 4;
	}
	for(int Index = 0; Index < MANCALA_PLAYERS; Index++){
		/* Initializing player stores to 0 */
		DStores[Index] = 0;
	}
}

CMancalaBoard::CMancalaBoard(int turn, const int pits[MANCALA_TOTAL_PITS],
	const int stores[MANCALA_PLAYERS]){
	DTurn = turn;
	for(int Index = 0; Index < MANCALA_TOTAL_PITS; Index++){
		DPits[Index] = pits[Index];
	}
	for(int Index = 0; Index < MANCALA_PLAYERS; Index++){
		DStores[Index] = stores[Index];
	}
}

void CMancalaBoard::ResetBoard(){
	/* Reset the player turn */
	DTurn = 0;
	for(int Index = 0; Index < MANCALA_TOTAL_PITS; Index++){
		DPits[Index] = 4;
	}
	/* Added this for loop to reset the stores of players */
	for (int Index = 0; Index < MANCALA_PLAYERS; Index++) {
		DStores[Index] = 0;
	}
}
int CMancalaBoard::PlayerTurn() const{
	int sum = 0;
	for (int i = 0; i < MANCALA_PIT_SLOTS; i++){
		sum += DPits[DTurn * MANCALA_PIT_SLOTS + i];
	}
	if (sum == 0) {
		return (1 - DTurn);
	}
	return DTurn;
}

int CMancalaBoard::PlayerScore(int player) const{
	return DStores[player];
}

int CMancalaBoard::PitStoneCount(int player, int pit){
	return DPits[player * MANCALA_PIT_SLOTS + pit];
}

bool CMancalaBoard::GameOver() const{
	int sum = 0;
	for(int Index = 0; Index < MANCALA_TOTAL_PITS; Index++){
		sum += DPits[Index];
	}
	return sum == 0;
}

std::string CMancalaBoard::ToString() const{
	std::string ReturnString  = "P1          PITS\n";
	ReturnString += "      5   4   3   2   1\n";
	ReturnString += "/---------------------------\\\n";
	ReturnString += "|   |";
	for(int Index = MANCALA_PIT_SLOTS - 1; Index >= 0; Index--){
		if(DPits[Index] >= 10){
			ReturnString += std::to_string(DPits[Index]) + " |";
		}
		else{
			ReturnString += std::string(" ") + std::to_string(DPits[Index]) + " |";
		}
	}
	ReturnString += "   |\n|";

	if(DStores[0] >= 10){
		ReturnString += std::to_string(DStores[0]);
	}
	else{
		ReturnString += std::string(" ") + std::to_string(DStores[0]);
	}
	ReturnString += " |-------------------|";
	if(DStores[1] >= 10){
		ReturnString += std::to_string(DStores[1]);
	}
	else{
		ReturnString += std::string(" ") + std::to_string(DStores[1]);
	}
	ReturnString += " |\n|   |";
	for(int Index = 0; Index < MANCALA_PIT_SLOTS; Index++){
		if(DPits[MANCALA_PIT_SLOTS + Index] >= 10){
			ReturnString += std::to_string(DPits[MANCALA_PIT_SLOTS + Index]) + " |";
		}
		else{
			ReturnString += std::string(" ") + std::to_string(DPits[MANCALA_PIT_SLOTS + Index]) + " |";
		}
	}
	ReturnString += "   |\n";
	ReturnString += "\\---------------------------/\n";
	ReturnString += "      1   2   3   4   5\n";
	ReturnString += "             PITS          P2\n";

	return ReturnString;
}

CMancalaBoard::operator std::string() const{
	std::string ReturnString  = "P1          PITS\n";
	ReturnString += "      5   4   3   2   1\n";
	ReturnString += "/---------------------------\\\n";
	ReturnString += "|   |";
	for(int Index = MANCALA_PIT_SLOTS-1; Index >= 0; Index--){
		if(DPits[Index] >= 10){
			ReturnString += std::to_string(DPits[Index]) + " |";
		}
		else{
			ReturnString += std::string(" ") + std::to_string(DPits[Index]) + " |";
		}
	}
	ReturnString += "   |\n|";

	if(DStores[0] >= 10){
		ReturnString += std::to_string(DStores[0]);
	}
	else{
		ReturnString += std::string(" ") + std::to_string(DStores[0]);
	}
	ReturnString += " |-------------------|";
	if(DStores[1] >= 10){
		ReturnString += std::to_string(DStores[1]);
	}
	else{
		ReturnString += std::string(" ") + std::to_string(DStores[1]);
	}
	ReturnString += " |\n|   |";
	for(int Index = 0; Index < MANCALA_PIT_SLOTS; Index++){
		if(DPits[MANCALA_PIT_SLOTS + Index] >= 10){
			ReturnString += std::to_string(DPits[MANCALA_PIT_SLOTS + Index]) + " |";
		}
		else{
			ReturnString += std::string(" ") + std::to_string(DPits[MANCALA_PIT_SLOTS + Index]) + " |";
		}
	}
	ReturnString += "   |\n";
	ReturnString += "\\---------------------------/\n";
	ReturnString += "      1   2   3   4   5\n";
	ReturnString += "             PITS          P2\n";

	return ReturnString;
}

bool CMancalaBoard::Move(int player, int pit){
	int PitIndex = player * MANCALA_PIT_SLOTS + pit;
	if (PitIndex > (MANCALA_PIT_SLOTS + (player * MANCALA_PIT_SLOTS)) - 1) {
		return false;
	}
	if((PitIndex < 0) or (PitIndex > MANCALA_TOTAL_PITS)){
		return false;
	}
	// If the pit has 0 the move should be invalid
	if(DPits[PitIndex] == 0) {
		return false;
	}
	int Stones = DPits[PitIndex];
	int LastPitDrop = PitIndex;
	DPits[PitIndex] = 0;
	while(Stones > 1){
		PitIndex++;
		if((PitIndex % MANCALA_PIT_SLOTS) == 0){
			if(player == (PitIndex / MANCALA_PIT_SLOTS) - 1){
				DStores[player]++;
				Stones--;
			}
		}
		PitIndex %= MANCALA_TOTAL_PITS;
		if(Stones){
			DPits[PitIndex]++;
			Stones--;
			LastPitDrop = PitIndex;
		}
	}
	PitIndex++;
	PitIndex %= (MANCALA_TOTAL_PITS + 1);
	// Double move logic
	if ((PitIndex % MANCALA_PIT_SLOTS) == 0) {
		if (player == (PitIndex / MANCALA_PIT_SLOTS) - 1){
			DStores[player]++;
			Stones--;
		} else {
			PitIndex %= MANCALA_TOTAL_PITS;
			DPits[PitIndex]++;
			Stones--;
			PitIndex++;
		}
	}
	// StealMove logic
	// Check to make sure it doesn't steal from the other player's pit!
	else if (DPits[PitIndex] == 0 && ((PitIndex < player * MANCALA_PIT_SLOTS + MANCALA_PIT_SLOTS) && (PitIndex > player * MANCALA_PIT_SLOTS))){

			int OppositeSide = 0;
			if (player == 0) {
				OppositeSide = MANCALA_TOTAL_PITS - 1  - (PitIndex % MANCALA_PIT_SLOTS);
			} else {
				OppositeSide = MANCALA_PIT_SLOTS - 1 - (PitIndex % MANCALA_PIT_SLOTS);
			}
			// Should only steal when the opposite side has stones
			if (DPits[OppositeSide] > 0) {
				DStores[player] += DPits[OppositeSide] + 1;
			} else {
				DPits[PitIndex]++;
			}
			DPits[OppositeSide] = 0;
	}
	else if(Stones == 1){
		if((PitIndex % MANCALA_PIT_SLOTS) == 0){
			if(player == (PitIndex / MANCALA_PIT_SLOTS) - 1){
				DStores[player]++;
				Stones--;
			}
		}
		PitIndex %= MANCALA_TOTAL_PITS;
		if(Stones){
			DPits[PitIndex]++;
			Stones--;
			LastPitDrop = PitIndex;
		}
	}
	if ((PitIndex % 5 == 0)) {
		DTurn = DTurn;
	} else {
		DTurn = 1 - DTurn;
	}
	return true;
}
