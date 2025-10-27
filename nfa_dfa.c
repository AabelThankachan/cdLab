#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define MAX_STATES 10
#define MAX_SYMBOLS 10
#define MAX_DFA_STATES 1024 // 2^MAX_STATES
int nStates, nSymbols;
char symbols[MAX_SYMBOLS];
int nfaTrans[MAX_STATES][MAX_SYMBOLS][MAX_STATES];
int nfaTransCount[MAX_STATES][MAX_SYMBOLS];
int dfaTrans[MAX_DFA_STATES][MAX_SYMBOLS];
int dfaFinal[MAX_DFA_STATES];
int stateMap[MAX_DFA_STATES];
int queue[MAX_DFA_STATES];
int front, rear;
int findState(int subset, int stateCount) {
 for (int i = 0; i < stateCount; i++) {
 	if (stateMap[i] == subset)
 		return i;
	}
	return -1;
}
void printSubset(int subset) {
 printf("{ ");
 for (int i = 0; i < nStates; i++) {
 	if (subset & (1 << i))
 		printf("q%d ", i);
 }
 printf("}");
}
int main() {
 printf("Enter number of NFA states: ");
 scanf("%d", &nStates);
 printf("Enter number of symbols: ");
 scanf("%d", &nSymbols);
 printf("Enter symbols: ");
 for (int i = 0; i < nSymbols; i++)
 	scanf(" %c", &symbols[i]);
 printf("Enter NFA transitions:\n");
 for (int i = 0; i < nStates; i++) {
 	for (int j = 0; j < nSymbols; j++) {
		printf("Number of transitions from q%d with %c: ", i, symbols[j]);
 	 	scanf("%d", &nfaTransCount[i][j]);
	 	for (int k = 0; k < nfaTransCount[i][j]; k++) {
	 		scanf("%d", &nfaTrans[i][j][k]);
	 	}
 	}	
 }
 int nFinal;
 int nfaFinal[MAX_STATES];
 memset(nfaFinal, 0, sizeof(nfaFinal));
 printf("Enter number of final states: ");
 scanf("%d", &nFinal);
 printf("Enter final states: ");
 for (int i = 0; i < nFinal; i++) {
	 int fs;
	 scanf("%d", &fs);
	 nfaFinal[fs] = 1;
 }
 memset(dfaFinal, 0, sizeof(dfaFinal));
 memset(dfaTrans, -1, sizeof(dfaTrans));
 front = rear = 0;
 int startSubset = 1 << 0;
 stateMap[0] = startSubset;
 queue[rear++] = 0;
 int dfaStateCount = 1;
 for (int i = 0; i < nStates; i++) {
 	if ((startSubset & (1 << i)) && nfaFinal[i]) {
		 dfaFinal[0] = 1;
		 break;
 	}
 }
 while (front < rear) {
	int current = queue[front++];
	int currentSubset = stateMap[current];
 	for (int sym = 0; sym < nSymbols; sym++) {
		 int newSubset = 0;
		 for (int st = 0; st < nStates; st++) {
 			if (currentSubset & (1 << st)) {
 				for (int k = 0; k < nfaTransCount[st][sym]; k++) {
 					newSubset |= (1 << nfaTrans[st][sym][k]);
 				}
 			}
 		 }
 		 if (newSubset == 0) {
			 dfaTrans[current][sym] = -1; // no transition
		 } else {
			 int idx = findState(newSubset, dfaStateCount);
			 if (idx == -1) {
				idx = dfaStateCount++;
				stateMap[idx] = newSubset;
				queue[rear++] = idx;
				// Check if new DFA state is final
				for (int i = 0; i < nStates; i++) {
 					if ((newSubset & (1 << i)) && nfaFinal[i]) {
						dfaFinal[idx] = 1;
						break;
 					}
 				}
 			}
 			dfaTrans[current][sym] = idx;
 		}
 	}
 }
 printf("\nDFA states and transitions:\n");
 for (int i = 0; i < dfaStateCount; i++) {
	 printf("DFA state %d: ", i);
	 printSubset(stateMap[i]);
	 if (dfaFinal[i])
 		printf(" [Final]");
 	 printf("\n");
 	 for (int sym = 0; sym < nSymbols; sym++) {
		 int t = dfaTrans[i][sym];
		 if (t != -1) {
 			printf(" δ(%d, %c) = %d\n", i, symbols[sym], t);
 		 }
	 }
 }
 return 0;
}
