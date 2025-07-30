#include <iostream>
#include <vector>
#include <chrono>
#include <queue>
using namespace std;

const int positionalWinningPositions[8][8] = { // Number of winning positions from the position
	{2, 3, 4, 5, 5, 4, 3, 2},
	{3, 4, 5, 6, 6, 5, 4, 3},
	{4, 5, 6, 7, 7, 6, 5, 4},
	{5, 6, 7, 8, 8, 7, 6, 5},
	{5, 6, 7, 8, 8, 7, 6, 5},
	{4, 5, 6, 7, 7, 6, 5, 4},
	{3, 4, 5, 6, 6, 5, 4, 3},
	{2, 3, 4, 5, 5, 4, 3, 2}
};
struct Comparator {
	bool operator()(const pair<int, int>& a, const pair<int, int>& b) {
		return positionalWinningPositions[a.first][a.second] < positionalWinningPositions[b.first][b.second];
	}
};

vector<vector<char>> board(8, vector<char>(8, '-'));
clock_t startTime;
const int time_limit = 5000; // Time limit of 5 seconds per computer move
char human = 'O', computer = 'X';

bool win(vector<vector<char>> board, char player) { // Four in a row, and Four in a column
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 5; c++) {
			if (board[r][c] == player && board[r][c + 1] == player && board[r][c + 2] == player && board[r][c + 3] == player)
				return true;
		}
	}
	for (int c = 0; c < 8; c++) {
		for (int r = 0; r < 5; r++) {
			if (board[r][c] == player && board[r + 1][c] == player && board[r + 2][c] == player && board[r + 3][c] == player)
				return true;
		}
	}
	return false;
}
bool timeLimit() {
	clock_t currentTime = clock();
	return double(currentTime - startTime) / CLOCKS_PER_SEC * 1000 > time_limit;
}
bool terminalTest(vector<vector<char>> board, int depth) { // In any of these conditions, terminate the search
	return (win(board, human) || win(board, computer) || depth == 0 || timeLimit());
}
int posScore(vector<vector<char>> board, char player) {
	int score = 0;
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			if (board[r][c] == player)
				score += positionalWinningPositions[r][c];
		}
	}
	return score * 2; // Give higher priority to better positions
}
pair<pair<int,int>, int> Score(vector<vector<char>> board, char player) {
	pair<int,int> matches = { 0, 0 }; // .first is num of length 2, .second is num of length 3;
	int panicCheck = 0;
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			if (r < 8 && c < 8) {
				if (board[r][c] == player) {
					int countH = 1, countV = 1;
					char opponent = (player == 'X') ? 'O' : 'X';
					if (c < 5) {
						for (int i = 1; i <= 3; i++) {
							if (board[r][c + i] == player) // Counts in x - - x = 2
								countH++;
							else if (board[r][c + i] == opponent && c > 0 && board[r][c - 1] != '-') { // In the case of o x..... o
								countH = 0;
								break;
							}
						}
					}
					if (r < 5) {
						for (int i = 1; i <= 3; i++) {
							if (board[r + i][c] == player)
								countV++;
							else if (board[r + i][c] == opponent && r > 0 && board[r - 1][c] != '-') {
								countV = 0;
								break;
							}
						}
					}
					if (c < 6) {
						if (c < 5) {
							if (board[r][c + 1] == '-' && board[r][c + 2] == player && board[r][c + 3] == '-' && c > 0 && board[r][c - 1] == '-') // In the case of - x - x -
								panicCheck += 500;
						}
						if (board[r][c + 1] == player && board[r][c + 2] == '-' && c > 0 && board[r][c - 1] == '-') // In the case of - x x -
							panicCheck += 500;
					}
					if (r < 6) {
						if (r < 5) {
							if (board[r + 1][c] == '-' && board[r + 2][c] == player && board[r + 3][c] == '-' && r > 0 && board[r - 1][c] == '-') 
								panicCheck += 500;
						}
						if (board[r + 1][c] == player && board[r + 2][c] == '-' && r > 0 && board[r - 1][c] == '-')
							panicCheck += 500;
					}
					int two = 0, three = 0;
					if (countV == 3) {
						matches.second++;
						r += 4;
					}
					if (countH == 3) {
						matches.second++;
						c += 4;
					}
					if (countH == 2)
						matches.first++;
					if (countV == 2)
						matches.first++;
				}
			}
		}
	}
	return { matches, panicCheck };
}
int utility(vector<vector<char>> board) {
	if (win(board, computer))
		return 99999999;
	if (win(board, human))
		return -99999999;
	pair<pair<int, int>, int> humanScore = Score(board, human);
	pair<pair<int, int>, int> compScore = Score(board, computer);
	return compScore.first.first * 50 + compScore.second + compScore.first.second * 800 + posScore(board, computer) + 1.5 * (-humanScore.first.first * 50 - humanScore.second - humanScore.first.second * 800 - posScore(board, human));
}

vector<pair<int, int>> potentialSuccessors(vector<vector<char>> currentBoard) {
	vector<pair<int, int>> allMoves;
	priority_queue<pair<int,int>, vector<pair<int, int>>, Comparator> pq;
	for (int r = 0; r < 8; r++) {
		for (int c = 0; c < 8; c++) {
			if (currentBoard[r][c] == '-') {
				pq.push({ r, c });
			}
		}
	}
	while (!pq.empty()) {
		allMoves.push_back(pq.top()); // The first sucessors chosen are ones with better positions overall which is good for early game and does not matter much for late game since the total number of sucessors is small
		pq.pop();
	}
	return allMoves;
}
int maxValue(vector<vector<char>> currentBoard, int alpha, int beta, int depth, pair<int, int>& bestMove);
int minValue(vector<vector<char>> currentBoard, int alpha, int beta, int depth, pair<int, int>& bestMove) { 
	if (terminalTest(currentBoard, depth))
		return utility(currentBoard);
	int v = INT_MAX;
	vector<pair<int, int>> successors = potentialSuccessors(currentBoard); 
	for (auto move : successors) {
		vector<vector<char>> nextBoard = currentBoard;
		nextBoard[move.first][move.second] = human;
		pair<int, int> tryMove;
		int val = maxValue(nextBoard, alpha, beta, depth - 1, tryMove);
		if (val < v) { // If a better move is found then the last previous best
			v = val;
			bestMove = move;
		}
		if (v <= alpha) // If the sucessor is bad, do not bother with it
			return v;
		beta = min(beta, v);
	}
	return v;
}
int maxValue(vector<vector<char>> currentBoard, int alpha, int beta, int depth, pair<int, int>& bestMove) {
	if (terminalTest(currentBoard, depth))
		return utility(currentBoard);
	int v = INT_MIN;
	vector<pair<int, int>> successors = potentialSuccessors(currentBoard);
	for (auto move : successors) {
		vector<vector<char>> nextBoard = currentBoard;
		nextBoard[move.first][move.second] = computer;
		pair<int, int> tryMove;
		int val = minValue(nextBoard, alpha, beta, depth - 1, tryMove);
		if (val > v) { // If a better move is found then the last previous best
			v = val;
			bestMove = move; 
		}
		if (v >= beta) // If the sucessor is bad, do not bother with it
			return v;
		alpha = max(alpha, v);
	}
	return v;
}
pair<int, int> alphaBetaPruningIterativeDeepening() {
	startTime = clock();
	pair<int, int> bestMove = { -1,-1 };
	int bestValue = INT_MIN;
	for (int depth = 1; depth <= 5; depth++) { // IDS with max depth of 5
		pair<int, int> move;
		int val = maxValue(board, INT_MIN, INT_MAX, depth, move);
		if (!timeLimit()) {
			bestMove = move;
			bestValue = val;
		}
		else
			break; // Time exceeded, go with current best
	}
	return bestMove;
}
void printBoard() {
	cout << "  ";
	for (int i = 0; i < 8; i++) {
		cout << i + 1 << " ";
	}
	cout << "\n";
	for (int i = 0; i < 8; i++) {
		cout << char('A' + i) << " ";
		for (int j = 0; j < 8; j++) {
			cout << board[i][j] << " ";
		}
		cout << "\n";
	}
}
int main() {
	char first;
	while (true) {
		cout << "Who goes first? (X = Computer, O = Human): ";
		cin >> first;
		first = toupper(first);
		if (first == 'O' || first == 'X')
			break;
	}
	char current = first;
	while (true) {
		printBoard();
		if (win(board, computer)) {
			cout << "\nComputer won!\n";
			break;
		}
		if (win(board, human)) {
			cout << "\nHuman won!\n";
			break;
		}
		if (potentialSuccessors(board).empty()) {
			cout << "\nIt's a draw!\n";
			break;
		}
		if (current == computer) {
			cout << "Computer is thinking...\n";
			pair<int, int> move = alphaBetaPruningIterativeDeepening();
			if (move.first == -1) {
				auto moves = potentialSuccessors(board);
				if (!moves.empty())
					move = moves[0];
			}
			board[move.first][move.second] = computer;
			cout << "Computer moved to " << char('A' + move.first) << (move.second + 1) << "\n";
			current = human;
		}
		else {
			while (true) {
				string humanMove;
				cout << "Enter your move (e.g. A1): ";
				cin >> humanMove;
				if (humanMove.size() == 2 && isalpha(humanMove.at(0)) && isdigit(humanMove.at(1))) {
					char rC = toupper(humanMove.at(0));
					if (rC >= 'A' && rC <= 'H' && humanMove.at(1) >= '1' && humanMove.at(1) <= '8') {
						int r = rC - 'A';
						int c = humanMove.at(1) - '1';
						if (board[r][c] == '-') {
							board[r][c] = human;
							break;
						}
						else
							cout << "Invalid Move, try again\n";
					}
					else
						cout << "Invalid Move, try again\n";
				}
				else
					cout << "Invalid Move, try again\n";
			}
			current = computer;
		}
	}
	return 0;
}