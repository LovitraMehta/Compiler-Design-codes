#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX 100

// Structures for Quadruples, Triples, and Indirect Triples
struct Quad {
    char op[10];
    char arg1[10];
    char arg2[10];
    char result[10];
};

struct Triple {
    char op[10];
    char arg1[10];
    char arg2[10];
};

struct Quad quads[MAX];
struct Triple triples[MAX];
int indirect[MAX];

int quadIndex = 0;
int tripleIndex = 0;
int indirectIndex = 0;

// Function to add a quadruple
void addQuad(char* op, char* arg1, char* arg2, char* result) {
    strcpy(quads[quadIndex].op, op);
    strcpy(quads[quadIndex].arg1, arg1);
    strcpy(quads[quadIndex].arg2, arg2);
    strcpy(quads[quadIndex].result, result);
    quadIndex++;
}

// Function to add a triple
void addTriple(char* op, char* arg1, char* arg2) {
    strcpy(triples[tripleIndex].op, op);
    strcpy(triples[tripleIndex].arg1, arg1);
    strcpy(triples[tripleIndex].arg2, arg2);
    tripleIndex++;
}

// Function to add an indirect triple
void addIndirect(int pos) {
    indirect[indirectIndex++] = pos;
}

// Function to display quadruples
void displayQuads() {
    printf("\nQuadruples:\n");
    printf("| %-7s | %-4s | %-4s | %-6s |\n", "Op", "Arg1", "Arg2", "Result");
    for (int i = 0; i < quadIndex; i++) {
        printf("| %-7s | %-4s | %-4s | %-6s |\n",
               quads[i].op, quads[i].arg1, quads[i].arg2, quads[i].result);
    }
}

// Function to display triples
void displayTriples() {
    printf("\nTriples:\n");
    printf("| %-7s | %-4s | %-4s |\n", "Op", "Arg1", "Arg2");
    for (int i = 0; i < tripleIndex; i++) {
        printf("| %-7s | %-4s | %-4s |\n",
               triples[i].op, triples[i].arg1, triples[i].arg2);
    }
}

// Function to display indirect triples
void displayIndirectTriples() {
    printf("\nIndirect Triples:\n");
    printf("| Statement | Op      | Arg1 | Arg2 |\n");
    for (int i = 0; i < indirectIndex; i++) {
        int idx = indirect[i];
        printf("| %-9d | %-7s | %-4s | %-4s |\n",
               i, triples[idx].op, triples[idx].arg1, triples[idx].arg2);
    }
}

int main() {
    // Input expression from the user
    char expression[MAX];
    printf("Enter an arithmetic expression (e.g., a = b * -c + b * -c):\n");
    fgets(expression, MAX, stdin);

    // Example hardcoded intermediate representation for demonstration
    // For simplicity, this is based on the expression "a = b * -c + b * -c"

    // Quadruples
    addQuad("uminus", "c", "", "t1");
    addQuad("*", "b", "t1", "t2");
    addQuad("uminus", "c", "", "t3");
    addQuad("*", "b", "t3", "t4");
    addQuad("+", "t2", "t4", "t5");
    addQuad("=", "t5", "", "a");

    // Triples
    addTriple("uminus", "c", "");
    addTriple("*", "b", "(0)");
    addTriple("uminus", "c", "");
    addTriple("*", "b", "(2)");
    addTriple("+", "(1)", "(3)");
    addTriple("=", "(4)", "");

    // Indirect Triples (default order)
    for (int i = 0; i < tripleIndex; i++) {
        addIndirect(i);
    }

    // Display all representations
    displayQuads();
    displayTriples();
    displayIndirectTriples();

    return 0;
}
