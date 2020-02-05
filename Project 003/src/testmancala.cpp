#include <gtest/gtest.h>
#include "MancalaBoard.h"
#include <iostream>
using namespace std;

TEST(MancalaBoardTest, DefaultBoardTest){
	// Needs to test that default board is correct
	CMancalaBoard board;
	for(int Player = 0; Player < MANCALA_PLAYERS; Player++){
		for(int Pit = 0; Pit < MANCALA_PIT_SLOTS; Pit++){
			EXPECT_EQ(board.PitStoneCount(Player,Pit),4);
		}
	}
	EXPECT_EQ(board.PlayerTurn(),0);
	EXPECT_EQ(board.ToString(), "P1          PITS\n      5   4   3   2   1\n/---------------------------\\\n|   | 4 | 4 | 4 | 4 | 4 |   |\n| 0 |-------------------| 0 |\n|   | 4 | 4 | 4 | 4 | 4 |   |\n\\---------------------------/\n      1   2   3   4   5\n             PITS          P2\n");
	EXPECT_FALSE(board.GameOver());
	EXPECT_EQ(board.ToString(), std::string(board));
	EXPECT_EQ(board.PitStoneCount(0, 0), 4);
	EXPECT_EQ(board.PitStoneCount(1, 0), 4);
}

TEST(MancalaBoardTest, SetBoardTest){
	// Needs to test that setting up an initial board is correct
	int pits[10] = {4,2,2,3,1,3,3,3,3,3};
	int stores[2] = {10, 10};
	CMancalaBoard newBoard = CMancalaBoard(5, pits, stores );
	EXPECT_EQ(newBoard.PlayerScore(0), 10);
	EXPECT_EQ(newBoard.PlayerScore(1), 10);
	EXPECT_EQ(newBoard.ToString(), "P1          PITS\n      5   4   3   2   1\n/---------------------------\\\n|   | 1 | 3 | 2 | 2 | 4 |   |\n|10 |-------------------|10 |\n|   | 3 | 3 | 3 | 3 | 3 |   |\n\\---------------------------/\n      1   2   3   4   5\n             PITS          P2\n");
	EXPECT_EQ(newBoard.ToString(), std::string(newBoard));
}

TEST(MancalaBoardTest, ResetBoardTest){
	// Needs to test that resetting a board is correct
	CMancalaBoard defaultBoard = CMancalaBoard();
	int pits[10] = {2, 3, 4, 5, 5, 1, 1, 1, 1, 1};
	int stores[2] = { 5, 5 };
	CMancalaBoard testBoard = CMancalaBoard(1, pits, stores);
	testBoard.ResetBoard();
	EXPECT_EQ(testBoard.ToString(), defaultBoard.ToString());
}

TEST(MancalaBoardTest, BasicMoveTest){
	// Needs to test that basic non-scoring moves are correct
	CMancalaBoard defaultBoard;
	defaultBoard.Move(0, 0);

	int pits[10] = {0,0,0,8,7,2,1,9,8,0};
	int stores[2] = {2, 3};
	CMancalaBoard longPath {1, pits, stores};
	longPath.Move(1, 3);
	EXPECT_EQ(longPath.PlayerScore(1), 4);
	EXPECT_EQ(longPath.PitStoneCount(1,0), 3);
	EXPECT_EQ(longPath.PitStoneCount(1, 1), 1);
}

TEST(MancalaBoardTest, ScoringMoveTest){
	// Needs to test that basic scoring, non double moves, non steals are correct
	int pits[10] = {5, 4, 4, 0, 5, 0, 6, 5, 5, 5};
	int stores[2] = {1, 0};
	CMancalaBoard testBoard {0, pits, stores};
	EXPECT_EQ(testBoard.PlayerTurn(), 0);
	testBoard.Move(0, 1);
	EXPECT_EQ(testBoard.PlayerTurn(), 0);
	EXPECT_EQ(testBoard.PitStoneCount(0, 2), 5);
	EXPECT_EQ(testBoard.PitStoneCount(0, 3), 1);
	EXPECT_EQ(testBoard.PitStoneCount(0, 4), 6);
	EXPECT_EQ(testBoard.PlayerScore(0), 2);
}

TEST(MancalaBoardTest, DoubleMoveTest){
	// Needs to test that double move is correct
	int pits[10] = { 4,4,4,4,4,4,4,4,4,4 };
	int stores[2] = {0, 0};
	CMancalaBoard doubleMoveBoard {0, pits, stores};
	EXPECT_EQ(doubleMoveBoard.PlayerTurn(), 0);
	doubleMoveBoard.Move(0, 1);
	EXPECT_EQ(doubleMoveBoard.PlayerScore(0), 1);
	EXPECT_EQ(doubleMoveBoard.PlayerTurn(), 0);
	doubleMoveBoard.Move(0, 0);
	EXPECT_EQ(doubleMoveBoard.PlayerTurn(), 1);
	doubleMoveBoard.Move(1, 1);
	EXPECT_EQ(doubleMoveBoard.PlayerTurn(), 1);
	EXPECT_EQ(doubleMoveBoard.PlayerScore(1), 1);

	int pits2[10] = {1,2,1,0,7,2,3,1,8,8};
	int stores2[2] = {3, 2};
	CMancalaBoard doubleMoveBoard2 {0, pits2, stores2};
	EXPECT_EQ(doubleMoveBoard2.PlayerTurn(), 0);
	doubleMoveBoard2.Move(0, 4);
	EXPECT_EQ(doubleMoveBoard2.PlayerTurn(), 1);
	EXPECT_EQ(doubleMoveBoard2.PitStoneCount(0, 4), 0);
	EXPECT_EQ(doubleMoveBoard2.PlayerScore(0), 4);
	EXPECT_EQ(doubleMoveBoard2.PitStoneCount(1, 0), 3);
	EXPECT_EQ(doubleMoveBoard2.PitStoneCount(1, 1), 4);
	EXPECT_EQ(doubleMoveBoard2.PitStoneCount(1, 2), 2);
	EXPECT_EQ(doubleMoveBoard2.PitStoneCount(1, 3), 9);
	EXPECT_EQ(doubleMoveBoard2.PitStoneCount(1, 4), 9);
	EXPECT_EQ(doubleMoveBoard2.PitStoneCount(0, 0), 2);
	EXPECT_EQ(doubleMoveBoard2.PlayerScore(1), 2);
}

TEST(MancalaBoardTest, StealMoveTest){
	// Needs to test that stealing move works correctly
	int pits[10] = {6, 0, 0, 3, 1, 0, 7, 0, 1, 8};
	int stores[2] = {5, 2};
	CMancalaBoard stealBoard {1, pits, stores};
	stealBoard.Move(1, 3);
	EXPECT_EQ(stealBoard.PitStoneCount(1, 4), 9);
	int pits2[10] = {5,4,4,0,5,5,0,5,5,5};
	int stores2[2] = {1, 1};
	CMancalaBoard bugBoard {0, pits2, stores2};
	bugBoard.Move(0, 2);
	EXPECT_EQ(bugBoard.PitStoneCount(0, 3), 1);
	EXPECT_EQ(bugBoard.PitStoneCount(0, 4), 6);
	EXPECT_EQ(bugBoard.PlayerScore(0), 2);
	EXPECT_EQ(bugBoard.PitStoneCount(1, 0), 6);
	EXPECT_EQ(bugBoard.PitStoneCount(1, 3), 5);

	int pits3[10] = {1,0,5,5,5,5,5,5,5,5};
	int stores3[2] = {0,0};
	CMancalaBoard correctSteal {0, pits3, stores3};
	correctSteal.Move(0, 0);
	EXPECT_EQ(correctSteal.PlayerScore(0), 6);
	int pits4[10] = {5,5,5,5,5,1,0,5,5,5};
	int stores4[2] = {0,0};
	CMancalaBoard correctSteal2 {0, pits4, stores4};
	correctSteal2.Move(1, 0);
	EXPECT_EQ(correctSteal2.PlayerScore(1), 6);

	int pits5[10] = {0, 1, 6, 6, 6, 4, 0, 5, 5, 5};
	int stores5[2] = {1, 1};
	CMancalaBoard correctSteal3 {1, pits5, stores5};
	correctSteal3.Move(1, 2);
	EXPECT_EQ(correctSteal3.PitStoneCount(1, 2), 0);
	EXPECT_EQ(correctSteal3.PitStoneCount(1, 3), 6);
	EXPECT_EQ(correctSteal3.PitStoneCount(1, 4), 6);
	EXPECT_EQ(correctSteal3.PlayerScore(1), 2);
	EXPECT_EQ(correctSteal3.PitStoneCount(0, 0), 1);
	EXPECT_EQ(correctSteal3.PitStoneCount(0, 1), 2);

	int pits6[10] = {0, 9, 3, 2, 7, 2, 0, 0, 2, 8};
	int stores6[2] = {3, 4};

	CMancalaBoard correctSteal4 {1, pits6, stores6};
	correctSteal4.Move(1, 0);
	EXPECT_EQ(correctSteal4.PitStoneCount(1, 1), 1);
	EXPECT_EQ(correctSteal4.PitStoneCount(1, 2), 0);
	EXPECT_EQ(correctSteal4.PlayerScore(1), 8);

	int pits7[10] = {0,0,7,0,7,1,2,0,0,7};
	int stores7[2] = {11,4};
	CMancalaBoard stealFromNothing {0, pits7, stores2};
	stealFromNothing.Move(1, 1);
	EXPECT_EQ(stealFromNothing.PitStoneCount(1, 2), 1);
	EXPECT_EQ(stealFromNothing.PitStoneCount(1, 3), 1);


	int pits8[10] = {0,0,0,1,2,3,7,0,3,0};
	int stores8[2] = {8, 14};
	CMancalaBoard player2Steal(1, pits8, stores8);
	player2Steal.Move(1, 3);
	EXPECT_EQ(player2Steal.PlayerScore(1), 15);
	EXPECT_EQ(player2Steal.PitStoneCount(1, 4), 1);
	EXPECT_EQ(player2Steal.PitStoneCount(0, 0), 1);
}

TEST(MancalaBoardTest, CantMoveTest){
	// Needs to test when player can't move correctly
	int pits[10] = {0,0,0,0,0,5,4,3,2,1};
	int stores[2] = {0,0};
	CMancalaBoard testBoard = CMancalaBoard(0, pits, stores);
	EXPECT_EQ(testBoard.Move(0, 1), false);
	EXPECT_EQ(testBoard.Move(0, 2), false);
	EXPECT_EQ(testBoard.PlayerTurn(), 1);
	testBoard.Move(1, 4);
	EXPECT_EQ(testBoard.PlayerTurn(), 1);
	EXPECT_EQ(testBoard.PlayerScore(1), 1);
}

TEST(MancalaBoardTest, BadParametersTest){
	// Needs to check that bad parameters are handled correctly
	CMancalaBoard DefaultBoard;
	EXPECT_FALSE(DefaultBoard.Move(3, 1));
	EXPECT_FALSE(DefaultBoard.Move(0, 6));
	EXPECT_FALSE(DefaultBoard.Move(0, -5));
}
