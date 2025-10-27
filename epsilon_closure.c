#include <stdio.h>
#include <stdlib.h>
#define MAX_STATES 20
#define MAX_TRANSITIONS 100
struct Transition {
 int from;
 char symbol;
 int to;
};
int epsilonClosure[MAX_STATES][MAX_STATES];
int visited[MAX_STATES];
int n, t;
struct Transition transitions[MAX_TRANSITIONS];
void dfs(int state, int start) {
 visited[state] = 1;
 epsilonClosure[start][state] = 1;
 for (int i = 0; i < t; i++) {
 	if (transitions[i].from == state && transitions[i].symbol == 'e' && !visited[transitions[i].to]) {
 		dfs(transitions[i].to, start);
 	}
 }
}
int main() {
 printf("Enter number of states: ");
 scanf("%d", &n);
 printf("Enter number of transitions: ");
 scanf("%d", &t);
 printf("Enter transitions in format: from_state symbol to_state\n");
 printf("For epsilon transitions, enter symbol as 'e'\n");
 for (int i = 0; i < t; i++) {
 	scanf("%d %c %d", &transitions[i].from, &transitions[i].symbol, &transitions[i].to);
 }
 for (int i = 0; i < n; i++)
	for (int j = 0; j < n; j++)
 		epsilonClosure[i][j] = 0;
 for (int i = 0; i < n; i++) {
 	for (int k = 0; k < n; k++)
 		visited[k] = 0;
 	dfs(i, i);
 }
 printf("\nEpsilon closures of states:\n");
 for (int i = 0; i < n; i++) {
 	printf("Epsilon-closure(%d) = { ", i);
 	for (int j = 0; j < n; j++) {
 		if (epsilonClosure[i][j])
 			printf("%d ", j);
 	}
	 printf("}\n");
 }
 return 0;
}
