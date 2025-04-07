#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_EXPR_LEN 100

static char expr[MAX_EXPR_LEN];
static int tmp_counter = 0;

static inline void process_operand(const char* src, int start, int end, char* dest) {
    int idx = 0;
    for(int i = start; i <= end; i++) {
        if(src[i] != '$') dest[idx++] = src[i];
    }
    dest[idx] = '\0';
}

static inline char* generate_temp() {
    static char temp[4];
    snprintf(temp, sizeof(temp), "t%d", tmp_counter++);
    return temp;
}

void generate_code(const char* input) {
    char modified_expr[MAX_EXPR_LEN];
    strcpy(modified_expr, input);
    const char operators[] = {'/', '*', '+', '-'};
   
    for(int op = 0; op < 4; op++) {
        for(int i = 0; modified_expr[i]; i++) {
            if(modified_expr[i] == operators[op]) {
                char left[MAX_EXPR_LEN], right[MAX_EXPR_LEN];
                int left_start = i-1, right_end = i+1;
               
                while(left_start >= 0 && !strchr("+-*/", modified_expr[left_start]))
                    left_start--;
                process_operand(modified_expr, left_start+1, i-1, left);
               
                while(modified_expr[right_end] && !strchr("+-*/", modified_expr[right_end]))
                    right_end++;
                process_operand(modified_expr, i+1, right_end-1, right);
               
                const char* temp = generate_temp();
                printf("%s = %s %c %s\n", temp, left, operators[op], right);
               
                modified_expr[left_start+1] = '\0';
                strcat(modified_expr, temp);
                strcat(modified_expr, &modified_expr[right_end]);
                i = left_start + strlen(temp);
            }
        }
    }
}

int main() {
    printf("Enter expression: ");
    fgets(expr, MAX_EXPR_LEN, stdin);
    expr[strcspn(expr, "\n")] = '\0';
   
    printf("\nIntermediate Code:\n");
    generate_code(expr);
    return 0;
}
