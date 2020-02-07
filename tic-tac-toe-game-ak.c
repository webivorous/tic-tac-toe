#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// Following constants are defined to be easily used afterwards in the program 

const noughts = 0;  
const crosses = 1;
const outOfBound = 2; 
const emptyValue = 3;

const humanPlayerWins = 0;
const computerPlayerWins = 1;
const gameDraw = 2;

const int Directions[4] = { 1, 5, 4, 6 };

// The ArrayConversion constant array converts the padded array of 25 to 9 elements based array which is used while taking input from the user

const int ArrayConversion[9] = {
	6, 7, 8,
	11,12,13,
	16,17,18
};

const int MiddleValue = 4;
const int CornerValue[4] = { 0, 2, 6, 8 };

int ply = 0;
int positions = 0;
int maxPly = 0;

int GetNumForDir(int startSq, const int dir, const int *GameBoard, const int us) {
	int found = 0;
	while(GameBoard[startSq] != outOfBound) {		
		if(GameBoard[startSq] != us) {	
			break;
		}
		found++;	
		startSq += dir;
	}	
	return found;
}


//The ThreeCombinationFind function is used to find the combination of 3 noughts/zeroes in the game which determines the winning of the game by either the computer AI or human player
int ThreeCombinationFind(const int *GameBoard, const int ouri, const int us) {

	int Diri = 0;
	int Dir = 0;
	int countThree = 1;
	
	for(Diri = 0; Diri < 4; ++Diri) {
		Dir = Directions[Diri];
		countThree += GetNumForDir(ouri + Dir, Dir, GameBoard, us);
		countThree += GetNumForDir(ouri + Dir * -1, Dir * -1, GameBoard, us);
		if(countThree == 3) {
			break;
		}
		countThree = 1;
	}
	return countThree;
}


//This function utilizes the ThreeCombinationFind function defined above to scan through the whole Tic Tac Toe board
int ThreeCombinationFindAllGameBoard(const int *GameBoard, const int us) {
	int threeFound = 0;
	int i;
	for(i = 0; i < 9; ++i) {
		if(GameBoard[ArrayConversion[i]] == us) {
			if(ThreeCombinationFind(GameBoard, ArrayConversion[i], us) == 3) {				
				threeFound = 1; 
				break;
			}
		}
	}	
	return threeFound;
}

int EvalForWin(const int *GameBoard, const int us) {

	if(ThreeCombinationFindAllGameBoard(GameBoard, us) != 0)
		return 1;
	if(ThreeCombinationFindAllGameBoard(GameBoard, us ^ 1) != 0)
		return -1;
	
	return 0;
}


// This function implements the classic MinMax algorithm which is a recursive algorithm used to select the most optimum move based on the opponents move 
// Further details of this algorithm can be understood here https://towardsdatascience.com/tic-tac-toe-creating-unbeatable-ai-with-minimax-algorithm-8af9e52c1e7d?gi=89bf25a19383
int MinMaxAI(int *GameBoard, int side) {

	
	int moveValueList[9];
	int moveValueCount = 0;
	int bestScore = -2;
	int score = -2;
	int bestmoveValue = -1;
	int moveValue;
	int i;
	
	if(ply > maxPly) maxPly = ply;	
	positions++;
	
	if(ply > 0) {
		score = EvalForWin(GameBoard, side);
		if(score != 0) {					
			return score;
		}		
	}
	
	
	for(i = 0; i < 9; ++i) {
		if( GameBoard[ArrayConversion[i]] == emptyValue) {
			moveValueList[moveValueCount++] = ArrayConversion[i];
		}
	}
	
	
	for(i = 0; i < moveValueCount; ++i) {
		moveValue = moveValueList[i];
		GameBoard[moveValue] = side;	
		
		ply++;
		score = -MinMaxAI(GameBoard, side^1);
		if(score > bestScore) {			
			bestScore = score;	
			bestmoveValue = moveValue;
		}
		GameBoard[moveValue] = emptyValue;
		ply--;
	}
	
	if(moveValueCount==0) {
		bestScore = ThreeCombinationFindAllGameBoard(GameBoard, side);	
	}
	
	if(ply!=0)
		return bestScore;	
	else 
		return bestmoveValue;	
}

//Used to intialize the board as suggested by the name by intiaally setting the values of the board square to empty using GameBoard function

void BoardIntialization(int *GameBoard) {
	int i = 0;
	
	for(i = 0; i < 25; ++i) {
		GameBoard[i] = outOfBound;
	}
	
	for(i = 0; i < 9; ++i) {
		GameBoard[ArrayConversion[i]] = emptyValue;
	}	
}


// As suggested by name, this functions prints the current state of the board after every move (by player or computer AI)
void DisplayGameGameBoard(const int *GameBoard) {
	int i = 0;
	char pceChars[] = "OX|-";	
	
	printf("\n\nGameBoard:\n\n");
	for(i = 0; i < 9; ++i) {
		if(i!=0 && i%3==0) {
			printf("\n\n");
		}
		printf("%4c",pceChars[GameBoard[ArrayConversion[i]]]);
	}
	printf("\n");
}


//Checks whether the board square has an empty value 

int HasemptyValue(const int *GameBoard) {
	int i = 0;
	
	for(i = 0; i < 9; ++i) {
		if( GameBoard[ArrayConversion[i]] == emptyValue) return 1;
	}
	return 0;
}

void MakemoveValue(int *GameBoard, const int sq, const side) {
	GameBoard[sq] = side;
}

int GetNextBest(const int *GameBoard) {

	int ourmoveValue = ArrayConversion[MiddleValue];
	if(GameBoard[ourmoveValue] == emptyValue) {
		return ourmoveValue;
	}
	
	int i = 0;
	ourmoveValue = -1;
	
	for(i = 0; i < 4; i++) {
		ourmoveValue = ArrayConversion[CornerValue[i]];
		if(GameBoard[ourmoveValue] == emptyValue) {
			break;
		}
		ourmoveValue = -1;
	}
	
	return ourmoveValue;
}

int GetWinningmoveValue(int *GameBoard, const int side) {

	int ourmoveValue = -1;
	int winFound = 0;
	int i = 0;
	
	for(i = 0; i < 9; ++i) {
		if( GameBoard[ArrayConversion[i]] == emptyValue) {
			ourmoveValue = ArrayConversion[i];
			GameBoard[ourmoveValue] = side;
			
			if(ThreeCombinationFind(GameBoard, ourmoveValue, side) == 3) {
				winFound = 1;
			}	
			GameBoard[ourmoveValue] = emptyValue;
			if(winFound == 1) {
				break;
			}
			ourmoveValue = -1;
		};
	} 
	return ourmoveValue;
}

//Function to get the move played by the AI using MinMaxAI (MinMax Algorithm implementing function)

int AIMoveFetch(int *GameBoard, const int side) {
	
	ply=0;
	positions=0;
	maxPly=0;
	int best = MinMaxAI(GameBoard, side);
	printf("Finished Searching positions:%d maxDepth:%d bestmoveValue:%d\n",positions,maxPly,best);
	return best;
	
}

//Function that takes input from the user and checks if the input value(from 1-9) is valid and not exceeding that

int PlayerMoveFetch(const int *GameBoard) {
	
	char userInput[4];
	
	int moveValueOk = 0;
	int moveValue = -1;
	
	while (moveValueOk == 0) {
	
		printf("Please enter a moveValue from 1 to 9:");		
		fgets(userInput, 3, stdin);
		fflush(stdin);
		
		if(strlen(userInput) != 2) {
			printf("Invalid strlen()\n");
			continue;			
		}
		
		if( sscanf(userInput, "%d", &moveValue) != 1) {
			moveValue = -1;
			printf("Invalid sscanf()\n");
			continue;
		}
		
		if( moveValue < 1 || moveValue > 9) {
			moveValue = -1;
			printf("Invalid range\n");
			continue;
		}
		
		moveValue--; 
		
		if( GameBoard[ArrayConversion[moveValue]]!=emptyValue) {
			moveValue=-1;
			printf("Square not available\n");
			continue;
		}
		moveValueOk = 1;
	}
	printf("Making moveValue...%d\n",(moveValue+1));
	return ArrayConversion[moveValue];
}


// This is main function of the program that uses other functions to start the game and give proper sequence to it
void GameStartsNow() {

	int GameOver = 0;
	int Side = crosses;
	int LastmoveValueMade = 0;
	int GameBoard[25];
	
	BoardIntialization(&GameBoard[0]);
	DisplayGameGameBoard(&GameBoard[0]);
	
	while(!GameOver) {
		if(Side==noughts) {	
			LastmoveValueMade = PlayerMoveFetch(&GameBoard[0]);
			MakemoveValue(&GameBoard[0],LastmoveValueMade,Side);
			Side=crosses;
		} else {
			LastmoveValueMade = AIMoveFetch(&GameBoard[0], Side);
			MakemoveValue(&GameBoard[0],LastmoveValueMade,Side);
			Side=noughts;
			DisplayGameGameBoard(&GameBoard[0]);
		}
		
		// Check Three in a row condition and if it exits cease the game
		if( ThreeCombinationFind(GameBoard, LastmoveValueMade, Side ^ 1) == 3) {
			printf("Game Ends!\n");
			GameOver = 1;
			if(Side==noughts) {
				printf("Computer AI Wins\n");
			} else {
				printf("Human Player Wins\n");
			}
		}	
		
		// Game Draw Condition 
		if(!HasemptyValue(GameBoard)) {
			printf("Game over!\n");
			GameOver = 1;
			printf("Game is Draw\n");
		}
	}
	
	
	DisplayGameGameBoard(&GameBoard[0]);
}

int main() {		
	
	srand(time(NULL));
	GameStartsNow();  // Let's start the game from the main function of the program
		
	return 0;
}
