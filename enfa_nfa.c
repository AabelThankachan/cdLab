#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX 10
#define EPSILON 'e'
int nStates, nSymbols;
char symbols[MAX];
int transitions[MAX][MAX][MAX];
int epsilonClosure[MAX][MAX];
int closureSize[MAX];
int finalStates[MAX];
int newFinalStates[MAX];
int isFinal[MAX];
int getSymbolIndex(char symbol) {
 for (int i = 0; i < nSymbols; i++)
 	if (symbols[i] == symbol)
 		return i;
 return -1;
}
void addToClosure(int state, int target) {
 for (int i = 0; i < closureSize[state]; i++)
 	if (epsilonClosure[state][i] == target)
 		return;
 	epsilonClosure[state][closureSize[state]++] = target;
}
void computeEpsilonClosure() {
 for (int i = 0; i < nStates; i++) {
	 closureSize[i] = 0;
	 addToClosure(i, i);
	 int idx = 0;
	 while (idx < closureSize[i]) {
		 int s = epsilonClosure[i][idx++];
		 for (int k = 0; k < MAX; k++) {
 			if (transitions[s][getSymbolIndex(EPSILON)][k])
 				addToClosure(i, k);
 		}
 	}
 }
}
void removeEpsilonTransitions() {
 for (int i = 0; i < nStates; i++) {
 	for (int s = 0; s < nSymbols; s++) {
		if (symbols[s] == EPSILON)
 			continue;
		int result[MAX] = {0};
		for (int c = 0; c < closureSize[i]; c++) {
 			int state = epsilonClosure[i][c];
 			for (int k = 0; k < MAX; k++) {
 				if (transitions[state][s][k]) {
					for (int c2 = 0; c2 < closureSize[k]; c2++) {
 						result[epsilonClosure[k][c2]] = 1;
 					}
 				}
 			}
 		}
 		for (int r = 0; r < nStates; r++) {
 			if (result[r])
 				transitions[i][s][r] = 1;
 		}
 	}
 }
 // Update final states
 for (int i = 0; i < nStates; i++) {
 	for (int j = 0; j < closureSize[i]; j++) {
 		if (isFinal[epsilonClosure[i][j]]) {
		 newFinalStates[i] = 1;
		 break;
 		}
 	}
 }
}
void printTransitions() {
 printf("\nNew Transitions (without ε):\n");
 for (int i = 0; i < nStates; i++) {
 	for (int s = 0; s < nSymbols; s++) {
 		if (symbols[s] == EPSILON)
 			continue;
		printf("δ(q%d, %c) -> { ", i, symbols[s]);
		for (int j = 0; j < nStates; j++) {
 			if (transitions[i][s][j])
 				printf("q%d ", j);
 		}
 		printf("}\n");
 	}
 }
 printf("New Final States: ");
 for (int i = 0; i < nStates; i++) {
 	if (newFinalStates[i])
 		printf("q%d ", i);
 }
 printf("\n");
}
int main() {
 int nFinal;
 printf("Enter number of states: ");
 scanf("%d", &nStates);
 printf("Enter number of symbols (include 'e' for epsilon): ");
 scanf("%d", &nSymbols);
 printf("Enter symbols (e for epsilon): ");
 for (int i = 0; i < nSymbols; i++)
 	scanf(" %c", &symbols[i]);
 printf("Enter transitions:\n");
 for (int i = 0; i < nStates; i++) {
 	for (int j = 0; j < nSymbols; j++) {
		 int count;
		 printf("Number of transitions from q%d with %c: ", i, symbols[j]);
		 scanf("%d", &count);
		 printf("Enter target states: ");
		 for (int k = 0; k < count; k++) {
			 int target;
			 scanf("%d", &target);
			 transitions[i][j][target] = 1;
 		}
 	}
 }
 printf("Enter number of final states: ");
 scanf("%d", &nFinal);
 printf("Enter final states: ");
 for (int i = 0; i < nFinal; i++) {
	 int fs;
	 scanf("%d", &fs);
	 isFinal[fs] = 1;
 }
 computeEpsilonClosure();
 removeEpsilonTransitions();
 printTransitions();
 return 0;
}
