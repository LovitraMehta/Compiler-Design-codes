#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_PRODUCTIONS 10
#define MAX_TERMINALS 10
#define MAX_VARIABLES 10
#define MAX_RULE_LENGTH 20

char grammar[MAX_PRODUCTIONS][MAX_RULE_LENGTH]; // Grammar rules
char first[MAX_VARIABLES][MAX_TERMINALS]; // FIRST set
char follow[MAX_VARIABLES][MAX_TERMINALS]; // FOLLOW set
int numVariables, numTerminals, numProductions;
char variables[MAX_VARIABLES]; // Variables (non-terminals)
char terminals[MAX_TERMINALS];  // Terminals

// Function prototypes
void computeFirst();
void computeFollow();
void addFirst(char var, char symbol);
void addFollow(char var, char symbol);
int isTerminal(char c);
int isVariable(char c);
void printSets();
void constructParsingTable();

// Helper function to find index of a variable
int findVariableIndex(char var) {
    for (int i = 0; i < numVariables; i++) {
        if (variables[i] == var) {
            return i;
        }
    }
    return -1;
}

// Helper function to find index of a terminal
int findTerminalIndex(char term) {
    for (int i = 0; i < numTerminals; i++) {
        if (terminals[i] == term) {
            return i;
        }
    }
    return -1;
}

// Main function
int main() {
    // Example grammar
    printf("Enter number of variables (non-terminals): ");
    scanf("%d", &numVariables);
    printf("Enter number of terminals: ");
    scanf("%d", &numTerminals);
    printf("Enter number of productions: ");
    scanf("%d", &numProductions);

    printf("Enter the variables: ");
    for (int i = 0; i < numVariables; i++) {
        scanf(" %c", &variables[i]);
    }

    printf("Enter the terminals: ");
    for (int i = 0; i < numTerminals; i++) {
        scanf(" %c", &terminals[i]);
    }

    printf("Enter the productions (one per line, e.g., A->aB | A->b):\n");
    for (int i = 0; i < numProductions; i++) {
        scanf(" %[^\n]s", grammar[i]);
    }

    // Step 1: Compute the FIRST sets
    computeFirst();

    // Step 2: Compute the FOLLOW sets
    computeFollow();

    // Step 3: Print FIRST and FOLLOW sets
    printSets();

    // Step 4: Construct and print the parsing table
    constructParsingTable();

    return 0;
}

// Compute FIRST sets
void computeFirst() {
    for (int i = 0; i < numVariables; i++) {
        first[i][0] = '\0'; // Initialize first set to empty
    }

    // Rule 1: If X -> aY, add 'a' to FIRST(X)
    for (int i = 0; i < numProductions; i++) {
        char lhs = grammar[i][0];  // Non-terminal (LHS)
        int lhsIndex = findVariableIndex(lhs);
        for (int j = 3; grammar[i][j] != '\0'; j++) {  // Loop through RHS of the production
            if (isTerminal(grammar[i][j])) {
                addFirst(lhs, grammar[i][j]);
                break;
            }
            if (isVariable(grammar[i][j])) {
                addFirst(lhs, grammar[i][j]);
                if (!strchr(first[findVariableIndex(grammar[i][j])], 'ε')) {
                    break;
                }
            }
        }
    }
}

// Compute FOLLOW sets
void computeFollow() {
    for (int i = 0; i < numVariables; i++) {
        follow[i][0] = '\0'; // Initialize FOLLOW sets to empty
    }
    follow[0][0] = '$'; // Start symbol gets the end of input symbol '$'

    // Rule 1: For each production, add FOLLOW(A) to FOLLOW(B) if A->...B is a part of the rule
    for (int i = 0; i < numProductions; i++) {
        char lhs = grammar[i][0];  // Non-terminal (LHS)
        int lhsIndex = findVariableIndex(lhs);
        for (int j = 3; grammar[i][j] != '\0'; j++) {
            if (isVariable(grammar[i][j])) {
                if (grammar[i][j+1] == '\0') { // B -> ... A
                    addFollow(grammar[i][j], follow[lhsIndex][0]);
                }
            }
        }
    }
}

// Add terminal or non-terminal to FIRST set
void addFirst(char var, char symbol) {
    int index = findVariableIndex(var);
    if (index != -1 && !strchr(first[index], symbol)) {
        strncat(first[index], &symbol, 1);
    }
}

// Add terminal or non-terminal to FOLLOW set
void addFollow(char var, char symbol) {
    int index = findVariableIndex(var);
    if (index != -1 && !strchr(follow[index], symbol)) {
        strncat(follow[index], &symbol, 1);
    }
}

// Check if the character is a terminal
int isTerminal(char c) {
    return !isupper(c); // Non-terminals are upper case
}

// Check if the character is a variable (non-terminal)
int isVariable(char c) {
    return isupper(c);
}

// Print FIRST and FOLLOW sets
void printSets() {
    printf("\nFIRST Sets:\n");
    for (int i = 0; i < numVariables; i++) {
        printf("FIRST(%c) = {", variables[i]);
        for (int j = 0; first[i][j] != '\0'; j++) {
            printf("%c", first[i][j]);
            if (first[i][j+1] != '\0') printf(", ");
        }
        printf("}\n");
    }

    printf("\nFOLLOW Sets:\n");
    for (int i = 0; i < numVariables; i++) {
        printf("FOLLOW(%c) = {", variables[i]);
        for (int j = 0; follow[i][j] != '\0'; j++) {
            printf("%c", follow[i][j]);
            if (follow[i][j+1] != '\0') printf(", ");
        }
        printf("}\n");
    }
}

// Construct and print the parsing table
void constructParsingTable() {
    char parsingTable[MAX_VARIABLES][MAX_TERMINALS];

    // Initialize table with null character
    for (int i = 0; i < numVariables; i++) {
        for (int j = 0; j < numTerminals; j++) {
            parsingTable[i][j] = '\0';
        }
    }

    // Fill parsing table
    for (int i = 0; i < numProductions; i++) {
        char lhs = grammar[i][0];  // Non-terminal (LHS)
        int lhsIndex = findVariableIndex(lhs);
        for (int j = 3; grammar[i][j] != '\0'; j++) {  // Loop through RHS of the production
            if (isTerminal(grammar[i][j])) {
                parsingTable[lhsIndex][findTerminalIndex(grammar[i][j])] = grammar[i][j];
            }
        }
    }

    // Print parsing table
    printf("\nPredictive Parsing Table:\n");
    for (int i = 0; i < numVariables; i++) {
        for (int j = 0; j < numTerminals; j++) {
            printf("%c -> %c ", variables[i], parsingTable[i][j]);
        }
        printf("\n");
    }
}
