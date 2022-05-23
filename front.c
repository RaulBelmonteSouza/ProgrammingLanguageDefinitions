/* A lexical analyzer and syntax for simple math expressions */
#include<stdio.h>
#include<ctype.h>

/* global declarations */

/* vars */
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp, *fopen();

/* functions */
void addChar();
void getChar();
void getNonBlank();
int lex();

/* characters classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* TOKENS */
#define INTEGER_LITERAL 10
#define IDENTIFIER 11
#define ASSIGN_OPERATOR 20
#define ADD_OPERATOR 21
#define SUB_OPERATOR 22
#define MULT_OPERATOR 23
#define DIV_OPERATOR 24
#define LEFT_PARENTHESE 25
#define RIGHT_PARENTHESE 26

/* main function */

int main() {
    /* open data file and process the content */
    if ((in_fp = fopen("front.in", "r")) == NULL) {
        printf("ERROR - cannot open front.in \n");
    } else {
        getChar();
        do {
            lex();
        } while (nextToken != EOF);
    }
    if (fclose(in_fp)) {
        printf("END");
    }
       
}

/* process the operators and paranthese and return their token */
int lookup(char ch) {
    switch (ch) {
    case '(' :
        addChar();
        nextToken = LEFT_PARENTHESE; 
        break;

    case ')' :
        addChar();
        nextToken = RIGHT_PARENTHESE;
        break;

    case '+':
        addChar();
        nextToken = ADD_OPERATOR;
        break;

    case '-':
        addChar();
        nextToken = SUB_OPERATOR;
        break;

    case '*':
        addChar();
        nextToken = MULT_OPERATOR;
        break;

    case '/':
        addChar();
        nextToken = DIV_OPERATOR;
        break;
    
    default:
        addChar();
        nextToken = EOF;
        break;
    }
    
    return nextToken;
}

/* add nextChar to the lexeme */
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    } else {
        printf("ERROR - Lexeme is too long. \n");
    }
}

void getChar() {
    nextChar = getc(in_fp);
    if(nextChar != EOF) {
        if (isalpha(nextChar))
            charClass = LETTER;
        else if(isdigit(nextChar))
            charClass = DIGIT;
        else charClass = UNKNOWN;
    } else 
        charClass = EOF;
}

/* calls getChar until nextChar returns a non blank space */
void getNonBlank() {
    while (isspace(nextChar)) {
        getChar();
    }
}

/* lex analyzer for math expressions */
int lex() {
    lexLen = 0;
    getNonBlank();
    
    switch (charClass) {
        // recognizing identifiers
        case LETTER: 
            addChar();
            getChar();
            while(charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENTIFIER;
            break;
        
        // recognizing integer literals
        case DIGIT:
            addChar();
            getChar();
            while(charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INTEGER_LITERAL;
            break;

        // recognizing parentheses and operators
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;
        
        //end file
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    }

    printf("Next token is %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
}