#include <iostream>
#include <unordered_set>
#include <unordered_map>
#include <vector>
#include <queue>
#include <ctime>
#include <cstdlib>
#include <string>
#include <algorithm>
#include <fstream>
#include <chrono>
using namespace std;

struct State {
    vector<int> puzzle;       
    int zeroIndex;            // index of the empty space
    int g;                    
    int h;                    
    string stateStr;          // current state
    string parentStr;         // previous state
    
    int f() const {
        return g + h;
    }

    bool operator>(const State& next) const {
        return f() > next.f();
    }
};

string gettingState(const vector<int>& puzzle) {
    string s = "";
    for (int i = 0; i < 9; i++) {
        s += to_string(puzzle[i]);
    }
    return s;
}

int calculateHeuristic(const vector<int>& puzzle, int useMisplacedTiles) {
    int hn = 0;
    for (int i = 0; i < 9; i++) {
        int val = puzzle[i];
        if (val == 0) 
            continue; // empty space isn't considered for count
        if (useMisplacedTiles == 1) {
            if (val != i) 
                hn++;
        }
        else {
            int currRow = i / 3, currCol = i % 3;
            int goalRow = val / 3, goalCol = val % 3;
            hn += abs(i / 3 - val / 3) + abs(i % 3 - val % 3);
        }
    }
    return hn;
}

vector<int> createRandomPuzzle(int& zeroIndex) {
    vector<int> tiles(9);
    for (int i = 0; i < 9; i++)
        tiles[i] = i;
    for (int i = 0; i < 9; i++) {
        int j = rand() % 9;
        if (tiles[i] == 0)
            zeroIndex = j;  // Save index zero
        else if (tiles[j] == 0)
            zeroIndex = i;  // Save index zero
        swap(tiles[i], tiles[j]);  
    }

    int count = 0, savei, savej;
    for (int i = 0; i < 9; i++) {
        for (int j = i + 1; j < 9; j++) {
            if (tiles[j] == 0) { // Empty does not count for inversions
                continue;
            }
            if (tiles[i] > tiles[j]) { // Check for inversions
                count++;  
                savei = i;
                savej = j;
            }
        }
    }
    // If the number of inversions is odd, swap the saved tiles to make it solvable
    if (count % 2 != 0) {
        swap(tiles[savei], tiles[savej]);
    }
    return tiles;
}

vector<pair<int, int>> getValidMoves(int zeroIndex) {
    vector<pair<int, int>> moves;
    int row = zeroIndex / 3, col = zeroIndex % 3;
    if (row > 0)
        moves.push_back({ zeroIndex - 3, 0 }); // Up  (if not in the first row, empty can go up)
    if (col < 2)
        moves.push_back({ zeroIndex + 1, 1 }); // Right (if not in the last column, empty can go right)
    if (row < 2)
        moves.push_back({ zeroIndex + 3, 2 }); // Down (if not in the last row, empty can go down)
    if (col > 0)
        moves.push_back({ zeroIndex - 1, 3 }); // Left (if not in the first column, empty can go left)

    return moves;
}
int pqAstar(vector<int> puzzle, vector<int> goalPuzzle, int heuristic, int zeroIndex, ofstream& outfile) {
    priority_queue<State, vector<State>, greater<State>> pq;  // Priority queue for A*
    unordered_set<string> visited; // Visited Set
    unordered_map<string, string> parentMap;  // To go back on the actual path
    int h = calculateHeuristic(puzzle, heuristic);
    string initStateStr = gettingState(puzzle);
    pq.push({ puzzle, zeroIndex, 0, h, initStateStr, "" });
    visited.insert(initStateStr);  // Set initial state as visited
    parentMap[initStateStr] = "";  // Null because initial
    int totalCost = 0;
    auto start = std::chrono::high_resolution_clock::now(); // Initilize time
    while (!pq.empty()) {
        State current = pq.top();  // Based on f() > next.f();
        pq.pop();
        totalCost++; // Incremented each time
        if (current.puzzle == goalPuzzle) { // Goal reached?
            vector<string> path;
            string trace = current.stateStr;
            while (!trace.empty()) {
                path.push_back(trace);
                trace = parentMap[trace];  // Continues until inital state is reached
            }
            reverse(path.begin(), path.end());  // Reverse to go from end -> start to start -> end
            int count = 0;
            for (const string& stateStr : path) {
                cout << "\nStep: " << ++count << endl;
                outfile << "\nStep: " << count << endl;
                for (int i = 0; i < 9; i++) {
                    cout << stateStr[i] << " ";
                    outfile << stateStr[i] << " ";
                    if (i % 3 == 2) {
                        cout << endl;
                        outfile << endl;
                    }
                }
            }
            cout << "\n" << current.g + 1 << " step solution found.\n";
            outfile << "\n" << current.g + 1 << " step solution found.\n";
            cout << "Total Cost: " << totalCost << endl;
            outfile << "Total Cost: " << totalCost << endl;
            break;
        }
        auto moves = getValidMoves(current.zeroIndex); // Goal not reached, get possible moves
        for (const auto& move : moves) {
            int newIndex = move.first;
            vector<int> newPuzzle = current.puzzle;
            swap(newPuzzle[current.zeroIndex], newPuzzle[newIndex]);  // Swapping empty number (0) and a possible number is the same as moving the empty number
            string newStateStr = gettingState(newPuzzle);
            if (visited.count(newStateStr))
                continue;  // Skip if already visited
            int newH = calculateHeuristic(newPuzzle, heuristic);  // Calculate new heuristic
            pq.push({ newPuzzle, newIndex, current.g + 1, newH, newStateStr, current.stateStr });
            visited.insert(newStateStr);  // Set new state as visited
            parentMap[newStateStr] = current.stateStr;  // Set the parent
        }
    }
    return totalCost;
}
int main() {
    srand(time(0));
    ofstream outfile("output.txt");
    if (!outfile.is_open()) {
        cerr << "Error opening output.txt\n";
        return 1;
    }
    int mode = 0;
    ifstream infile;
    while (mode != 3) {
        if (mode == 0) { // Only do this line at the very beginning
            cout << "100 TEST CASES AUTOMATIC MODE\nEnter 5\n";
            outfile << "100 TEST CASES AUTOMATIC MODE\nEnter 5\n"; 
        }
        cout << "Select:\n[1] Random Puzzle\n[2] Manual Puzzle\n[3] Exit\n";
        outfile << "Select:\n[1] Random Puzzle\n[2] Manual Puzzle\n[3] Exit\n";
        cin >> mode;
        outfile << mode << endl;
        if (mode == 3)
            return 0;
        vector<int> puzzle;
        vector<int> goalPuzzle = { 0,1,2,3,4,5,6,7,8 };
        int zeroIndex = 0;
        int depth;
        if (mode == 1) {
            puzzle = createRandomPuzzle(zeroIndex);
        }
        else if (mode == 2) {
            cout << "Input 8 Puzzle: " << endl;
            outfile << "Input 8 Puzzle: " << endl;
            for (int i = 0; i < 9; i++) {
                int tile;
                cin >> tile;
                outfile << tile << endl;
                puzzle.push_back(tile);
                if (tile == 0) {
                    zeroIndex = i;
                }
            }
            int count = 0;
            for (int i = 0; i < 9; i++) {
                for (int j = i + 1; j < 9; j++) {
                    if (puzzle[i] > puzzle[j]) { // Check for inversions
                        count++;
                    }
                }
            }
            if (count % 2 != 0) {  // If inversions is odd, its not possible
                cout << "Not solvable, odd inversion count: " << count << endl;
                outfile << "Input 8 Puzzle: " << endl;
                return 0;
            }
        }
        else if (mode == 5) {
            cout << "Depth: \n";
            outfile << "Depth: \n";
            cin >> depth;
            outfile << depth << endl;
            infile.open("Length" + to_string(depth) + ".txt");
            if (!infile.is_open()) {
                cerr << "Error opening Length" << depth << ".txt\n";
                return 1;
            }
            string throwaway;
            getline(infile, throwaway);
            while (puzzle.size() < 9) {
                if (throwaway.size() > 0) {
                    if (isdigit(throwaway.at(0))) {
                        puzzle.push_back(stoi(throwaway));
                        if (puzzle.at(puzzle.size()-1) == 0) {
                            zeroIndex = puzzle.size() - 1;
                        }
                    }
                }
                infile >> throwaway;
            }
        }
        cout << "Initial Puzzle:\n"; //Output initial puzzle
        outfile << "Initial Puzzle:\n";
        for (int i = 0; i < 9; i++) {
            cout << puzzle[i] << " ";
            outfile << puzzle[i] << " ";
            if (i % 3 == 2) {
                cout << endl;
                outfile << endl;
            }
        }
        if (mode == 5) {
            pair<double, double> totalTimes = { 0,0 };
            double misplacedCost = 0, manhattanCost = 0;
           for (int i = 0; i < 100; i++) {
                auto start = std::chrono::high_resolution_clock::now();
               misplacedCost += pqAstar(puzzle, goalPuzzle, 1, zeroIndex, outfile); // Misplaced Tiles
                auto end = std::chrono::high_resolution_clock::now();
                cout << "Time taken: " << chrono::duration<double>(end - start).count() << "\n\n";
                outfile << "Time taken: " << chrono::duration<double>(end - start).count() << "\n\n";
                totalTimes.first += chrono::duration<double>(end - start).count();
                start = std::chrono::high_resolution_clock::now();
                manhattanCost += pqAstar(puzzle, goalPuzzle, 2, zeroIndex, outfile); // Manhattan Distance
                end = std::chrono::high_resolution_clock::now();
                cout << "Time taken: " << chrono::duration<double>(end - start).count() << "\n\n";
                outfile << "Time taken: " << chrono::duration<double>(end - start).count() << "\n\n";
                totalTimes.second += chrono::duration<double>(end - start).count();
                puzzle.clear();
                string throwaway;
                getline(infile, throwaway);
                if (infile.eof()) {   // Restart file to ensure 100 repetitions of depth x
                    infile.clear();                      
                    infile.seekg(0, ios::beg);             
                    getline(infile, throwaway);
                }
                while (puzzle.size() < 9) {   // Continues until the size of puzzle is 9
                    if (throwaway.size() > 0) {
                        if (isdigit(throwaway.at(0))) { // Only if a digit is read, is puzzle added to
                            puzzle.push_back(stoi(throwaway));
                            if (puzzle.at(puzzle.size() - 1) == 0) {
                                zeroIndex = puzzle.size() - 1;
                            }
                        }
                    }
                    infile >> throwaway;
                }
           }
           cout << "Average Runtime for Misplaced Tiles for " << depth << " depth: " << totalTimes.first/100 << endl;
           outfile << "Average Runtime for Misplaced Tiles for " << depth << " depth: " << totalTimes.first / 100 << endl;
           cout << "Average Runtime for Manhattan Distance for " << depth << " depth: " << totalTimes.second/100 << endl;
           outfile << "Average Runtime for Manhattan Distance for " << depth << " depth: " << totalTimes.second/100 << endl;
           cout << "Misplaced Tiles cost: " << misplacedCost/100 << endl;
           outfile << "Misplaced Tiles cost: " << misplacedCost / 100 << endl;
           cout << "Manhattan Distance cost: " << manhattanCost / 100 << endl;
           outfile << "Manhattan Distance cost: " << manhattanCost/100 << endl;
          
           
            return 0;
        }
        int heuristic;
        cout << "\nSelect H Function:\n[1] Misplaced Tiles\n[2] Manhattan Distance\n";
        outfile << "\nSelect H Function:\n[1] Misplaced Tiles\n[2] Manhattan Distance\n";
        cin >> heuristic;
        outfile << heuristic << endl;
        auto start = std::chrono::high_resolution_clock::now();
        pqAstar(puzzle, goalPuzzle, heuristic, zeroIndex, outfile);
        auto end = std::chrono::high_resolution_clock::now();
        cout << "Time taken: " << chrono::duration<double>(end - start).count() << "\n\n";
        outfile << "Time taken: " << chrono::duration<double>(end - start).count() << "\n\n";
        if (mode == 3) {
            outfile.close();
            return 0;
        }
    }
}