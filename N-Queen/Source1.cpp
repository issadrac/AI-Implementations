#include <iostream>
#include <vector>
#include <chrono>
using namespace std;
vector<int> generateState(int n) {
	vector<int> initial;
	for (int i = 0; i < n; i++) {
		initial.push_back(rand() % n); // Randomly places a queen in a column where row = index
	}
	return initial;
}
int calculateHeuristic(vector<int> state, int n) {
	int h = 0;
	for (int i = 0; i < n; i++) {
		for (int j = i + 1; j < n; j++) {
			if (state.at(i) == state.at(j) || abs(state.at(i) - state.at(j)) == abs(i - j)) { // If same column or diagonal, row is different because index = row
				h++;
			}
		}
	}
	return h;
}
void printState(vector<int> state, int n) {
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			cout << ((state.at(i) == j) ? "Q " : ". "); // Q represents queen, . represents a blank space
		}
		cout << endl;
	}
	cout << endl;
}
pair<vector<int>, int> hillClimb(vector<int> state, int n) {
	int currentH = calculateHeuristic(state, n);
	int bestH = currentH;
	bool better;
	do {
		vector<int> bestState = state;
		better = false;
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if (state.at(i) == j) // If being moved to the same spot, then skip
					continue;
				vector<int> neighbor = state;
				neighbor.at(i) = j; // Moving to every other column
				int h = calculateHeuristic(neighbor, n);
				if (h < bestH) {
					bestH = h;
					bestState = neighbor;
					better = true;
				}
			}
		}
		if (better) { // Better is set to false at each step, if a step doesnt result in a better heuristic, then it ends the search
			state = bestState;
			currentH = bestH;
		}
	} while (better);
	return { state, currentH };
}
vector<int> chooseParent(vector<vector<int>> population, vector<int> allFitness) {
	int best = rand() % population.size();
	for (int i = 0; i < 5; i++) {  // Choosing the best fitness out of 6 of the population per parent
		int potentialBest = rand() % population.size(); 
		if (allFitness.at(potentialBest) > allFitness.at(best)) {
			best = potentialBest;
		}
	}
	return population.at(best);
}
vector<int> reproduce(vector<int> parent1, vector<int> parent2, int n) {
	int c = rand() % n;
	vector<int> child = parent1;   // Default to parent 1
	for (int i = c; i < n; i++) {
		child.at(i) = parent2.at(i); // Depending c, parent1 positions is replaced with parent2 positions
	}
	return child;
}

pair<double, int> genetic(int n, int foundSolutions = 0, int popNum = 0, double totalTime = 0) {
	auto start = chrono::high_resolution_clock::now();
	bool found = false;
	if (foundSolutions == 3 || popNum == 5) {
		return { totalTime, popNum };  // Only when 3 solutions or 6 iterations has occured
	}
	vector<vector<int>> population;
	for (int i = 0; i < 100; i++) {
		population.push_back(generateState(n));
	}
	for (int i = 0; i < 1000; i++) {
		vector<vector<int>> newPopulation;
		vector<int> allFitness; // Max Conflicts - Heuristic (higher fitness better)
		for (int i = 0; i < population.size(); i++) {
			allFitness.push_back(n * (n - 1) / 2 - calculateHeuristic(population.at(i), n)); // Largest possible value minus the heuristic results in a larger number being more desirable
		}
		for (int j = 0; j < 100; j++) {
			vector<int> child = reproduce(chooseParent(population, allFitness), chooseParent(population, allFitness), n);
			if (rand() % 100 < 25) { // Mutation has a 25% chance
				int row = rand() % n;
				int col = rand() % n;
				child.at(row) = col;
			}
			if (n * (n - 1) / 2 - calculateHeuristic(child, n) == n * (n - 1) / 2) { // If fitness is at its highest possible value (same as h == 0)
				cout << "Solution found in Population: " << popNum << "  Generation: " << i << "\n";
				printState(child, n);
				foundSolutions++;
				found = true;
				auto end = chrono::high_resolution_clock::now();
				totalTime += chrono::duration_cast<chrono::seconds>(end - start).count();
				break;
			}
			newPopulation.push_back(child);
		}
		if (found) {
			break;
		}
		population = newPopulation;
	}
	auto end = chrono::high_resolution_clock::now();
	totalTime += chrono::duration_cast<chrono::seconds>(end - start).count();
	return genetic(n, foundSolutions, popNum+1, totalTime); // Continues until 5 solutions is found, previous is for total number of generation counter 
}
int main() {
	srand(time(0));
	int n = 8;
	int count = 0;
	double totalTime = 0;
	cout << "Hill Climb Solutions\n";
	for (int i = 0; i < 100; i++) {
		vector<int> initial = generateState(n);
		auto start = chrono::high_resolution_clock::now();
		pair<vector<int>, int> state = hillClimb(initial, n);
		auto end = chrono::high_resolution_clock::now();
		totalTime += chrono::duration_cast<chrono::microseconds>(end - start).count();
		if (state.second == 0) {
			count++;
			cout << "Solution found at " << i << "/100 tries\n";
			if (count <= 3) {
				printState(state.first, n);
			}
		}
	}
	cout << "\nHill Climb Percentage: " << count << "%\n";
	cout << "Average Runtime: " << (totalTime / 100) / 1000 << "ms\n";
	cout << "\nGenetic Solutions (Generation up to 1000)\n";
	pair<double, int> geneticData = genetic(n); // Total time / Iterations-1
	cout << "\nAverage Runtime: " << geneticData.first/(geneticData.second+1) << "s\n";
	return 0;
}