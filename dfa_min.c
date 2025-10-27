#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#define MAX_STATES 50
#define MAX_SYMBOLS 20
// Global variables for the DFA
int num_states, num_symbols;
char symbols[MAX_SYMBOLS];
int start_state;
int final_states[MAX_STATES];
int num_final_states;
int transitions[MAX_STATES][MAX_SYMBOLS];
// Partitions
int partition[MAX_STATES];
int group_count;
// --- Function Prototypes ---
void get_dfa_input();
void minimize_dfa();
void print_minimized_dfa();
bool is_final(int state);
int main() {
	get_dfa_input();
	minimize_dfa();
	return 0;
}
/**
* @brief Reads the DFA definition from the user.
*/
void get_dfa_input() {
 printf("Enter number of states: ");
 scanf("%d", &num_states);
 printf("Enter number of input symbols: ");
 scanf("%d", &num_symbols);
 printf("Enter the input symbols: ");
 for (int i = 0; i < num_symbols; i++) {
 	scanf(" %c", &symbols[i]);
 }
 printf("Enter start state (e.g., 0): ");
 scanf("%d", &start_state);
 printf("Enter number of final states: ");
 scanf("%d", &num_final_states);
 printf("Enter the final states: ");
 for (int i = 0; i < num_final_states; i++) {
 	scanf("%d", &final_states[i]);
 }
 printf("\nEnter the transitions (state symbol -> next_state):\n");
 for (int i = 0; i < num_states; i++) {
 	for (int j = 0; j < num_symbols; j++) {
		 printf("d(%d, %c) -> ", i, symbols[j]);
		 scanf("%d", &transitions[i][j]);
 	}
 }
}
/**
* @brief Helper function to check if a state is a final state.
*/
bool is_final(int state) {
	for (int i = 0; i < num_final_states; i++) {
 		if (final_states[i] == state) {
 			return true;
 		}
 	}
 	return false;
}
void minimize_dfa() {
 // --- 1. Initial Partition ---
 // Group 0: Non-final states
 // Group 1: Final states
 for (int i = 0; i < num_states; i++) {
 	partition[i] = is_final(i) ? 1 : 0;
 }
 group_count = 2;
 // --- 2. Refine Partitions ---
 bool changed;
 do {
	 changed = false;
	 int current_group_count = group_count;
 // For every pair of states (i, j)
 	 for (int i = 0; i < num_states; i++) {
 		for (int j = i + 1; j < num_states; j++) {

 // If they are in the same partition, check if they are distinguishable
 			if (partition[i] == partition[j]) {

				// Check their transitions for each symbol
				for (int k = 0; k < num_symbols; k++) {
					int next_state_i = transitions[i][k];
					int next_state_j = transitions[j][k];

 					if (partition[next_state_i] != partition[next_state_j]){

						partition[j] = current_group_count;
						changed = true;
						break; 
 					}
 				}
 			}
 		}
 	 }

	 // If any split occurred, update the total group count
	 if (changed) {
 		group_count = current_group_count + 1;
 	 }
 } while (changed);
 print_minimized_dfa();
}
void print_minimized_dfa() {
 int new_state_map[MAX_STATES];
 for(int i=0; i<group_count; i++) new_state_map[i] = -1;
 int min_num_states = 0;


 for (int i = 0; i < num_states; i++) {
 	if (new_state_map[partition[i]] == -1) {
 		new_state_map[partition[i]] = min_num_states++;
 	}
 }
 printf("\n--- Minimized DFA ---\n");
 printf("Number of states: %d\n", min_num_states);
 // Find and print the new start state
 int min_start_state = new_state_map[partition[start_state]];
 printf("Start state: %d\n", min_start_state);
 // Find and print the new final states
 printf("Final states: ");
 bool is_min_final[min_num_states];
 for(int i=0; i<min_num_states; i++) is_min_final[i] = false;
 for (int i = 0; i < num_states; i++) {
 	if (is_final(i)) {
 		is_min_final[new_state_map[partition[i]]] = true;
 	}
 }
 for(int i=0; i<min_num_states; i++) {
 	if (is_min_final[i]) printf("%d ", i);
 }
 printf("\n");
 // Print the new transitions
 printf("Transitions:\n");
 int min_transitions[min_num_states][num_symbols];

 // Create the new transition table
 for(int i = 0; i < num_states; i++) {
 	for(int j = 0; j < num_symbols; j++) {
		 int from_state = new_state_map[partition[i]];
		 int to_state = new_state_map[partition[transitions[i][j]]];
		 min_transitions[from_state][j] = to_state;
 	}
 }
 // Print it
 for(int i = 0; i < min_num_states; i++) {
 	for(int j = 0; j < num_symbols; j++) {
 		printf("d(%d, %c) -> %d\n", i, symbols[j], min_transitions[i][j]);
 	}
 }
}
