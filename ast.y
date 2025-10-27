%{
    #include <stdio.h>
    #include <stdlib.h>
    #include <string.h>
    #include "y.tab.h"
    int yylex(void);            
    int yyerror(const char *s); 

    /* ===== AST Structure ===== */
    typedef struct AST {
        char *nodeType;
        int value;
        struct AST *left, *right;
    } AST;

    AST* makeNode(char *type, AST *l, AST *r) {
        AST *node = (AST*) malloc(sizeof(AST));
        node->nodeType = strdup(type);
        node->value = 0;
        node->left = l;
        node->right = r;
        return node;
    }

    AST* makeNodeNum(int val) {
        AST *node = (AST*) malloc(sizeof(AST));
        node->nodeType = strdup("NUM");
        node->value = val;
        node->left = node->right = NULL;
        return node;
    }

    void printAST(AST *root, int level) {
        if (!root) return;
        for (int i = 0; i < level; i++) printf("  ");
        if (strcmp(root->nodeType, "NUM") == 0)
            printf("%s(%d)\n", root->nodeType, root->value);
        else
            printf("%s\n", root->nodeType);
        printAST(root->left, level + 1);
        printAST(root->right, level + 1);
    }
%}

%union {
    int num;
    struct AST *node;
}

%token <num> NUMBER
%type <node> expr
%left '+' '-'
%left '*' '/'
%right UMINUS

%%
start:
      /* empty */
    | start expr '\n' {
        printf("\nAbstract Syntax Tree:\n");
        printAST($2, 0);
        printf("\n");
      }
    ;

expr:
      expr '+' expr { $$ = makeNode("+", $1, $3); }
    | expr '-' expr { $$ = makeNode("-", $1, $3); }
    | expr '*' expr { $$ = makeNode("*", $1, $3); }
    | expr '/' expr { $$ = makeNode("/", $1, $3); }
    | '-' expr %prec UMINUS { $$ = makeNode("NEG", $2, NULL); }
    | '(' expr ')' { $$ = $2; }
    | NUMBER { $$ = makeNodeNum($1); }
    ;
%%

int main() {
    printf("Enter arithmetic expressions (one per line, Ctrl+D to end)\n");
    yyparse();
    return 0;
}

int yyerror(const char *s) {
    fprintf(stderr, "Error: %s\n", s);
    return 0;
}

