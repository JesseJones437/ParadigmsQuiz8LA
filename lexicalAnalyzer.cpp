#include <stdio.h>
#include <ctype.h>
#include <string.h>

/* Global declarations */
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
FILE *in_fp;

/* Function declarations */
void addChar();
void getChar();
void getNonBlank();
int lex();

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define PREPROCCESS 2
#define INCLUDE_FILE 3
#define COMMENT 4
#define UNKNOWN 99

/* Token codes */
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26
#define LESS_THAN 31
#define GREATER_THAN 32
#define PERIOD 33
#define LEFT_CURLY 34
#define RIGHT_CURLY 35
#define LEFT_HARD 36
#define RIGHT_HARD 37
#define COMMA 38
#define SEMI_COLON 39
#define COLON 40
#define QUOTE 41
#define MOD_OP 42
#define BACK_SLASH 43
#define KEYWORD 88

/*************************************************************/
/* main driver */
int main() {
    /* Open the input data file and process its contents */
    if ((in_fp = fopen("front.in", "r")) == NULL)
        printf("ERROR - cannot open front.in \n");
    else {
        getChar();
        do {
            lex();
        } while (nextToken != EOF);
    }
    return 0;
}

/**************************************************************************
/* lookup - a function to lookup operators and parentheses
                            and return the token */
int lookup(char ch) {
    switch (ch) {
        case '(':
            addChar();
            nextToken = LEFT_PAREN;
            break;
        case ')':
            addChar();
            nextToken = RIGHT_PAREN;
            break;
        case '+':
            addChar();
            nextToken = ADD_OP;
            break;
        case '-':
            addChar();
            nextToken = SUB_OP;
            break;
        case '*':
            addChar();
            nextToken = MULT_OP;
            break;
        case '/':
            addChar();
            nextToken = DIV_OP;
            break;     
        case '<':
            addChar();
            nextToken = LESS_THAN;
            break;
        case '>':
            addChar();
            nextToken = GREATER_THAN;
            break;
        case '.':
            addChar();
            nextToken = PERIOD;
            break;
        case '{':
            addChar();
            nextToken = LEFT_CURLY;
            break;
        case '}':
            addChar();
            nextToken = RIGHT_CURLY;
            break;
        case '[':
            addChar();
            nextToken = LEFT_HARD;
            break;
        case ']':
            addChar();
            nextToken = RIGHT_HARD;
            break;
        case '=':
            addChar();
            nextToken = ASSIGN_OP;
            break;
        case ',':
            addChar();
            nextToken = COMMA;
            break;
        case ';':
            addChar();
            nextToken = SEMI_COLON;
            break;
        case ':':
            addChar();
            nextToken = COLON;
            break;
        case '\"':
            addChar();
            nextToken = QUOTE;
            break;
        case '%':
            addChar();
            nextToken = MOD_OP;
            break;
        case '\\':
            addChar();
            nextToken = BACK_SLASH;
            break;
        default:
            addChar();
            nextToken = EOF;
            break;
    }
    return nextToken;
}

/**************************************************************************
/* lookup - a function to lookup operators and parentheses
                            and return the token */
int keywordlookup() {
    if (strcmp(lexeme, "include") == 0)
        nextToken = KEYWORD;
    else if (strcmp(lexeme, "int") == 0)
        nextToken = KEYWORD;
    else if (strcmp(lexeme, "for") == 0)
        nextToken = KEYWORD;
    else if (strcmp(lexeme, "if") == 0)
        nextToken = KEYWORD;
    else if (strcmp(lexeme, "return") == 0)
        nextToken = KEYWORD;
    return nextToken;
}

/*************************************************************************
/* addChar - a function to add nextChar to lexeme */
void addChar() {
    if (lexLen <= 98) {
        lexeme[lexLen++] = nextChar;
        lexeme[lexLen] = 0;
    }
    else
        printf("Error - lexeme is too long \n");
}

/*************************************************************************
/* getChar - a function to get the next character of
                            input and determine its character class */
void getChar() {
    if ((nextChar = getc(in_fp)) != EOF) {
        char tmp = getc(in_fp);
        if (nextChar == '#')
            charClass = PREPROCCESS;
        else if (nextChar == '<' && !isspace(tmp) )
            charClass = INCLUDE_FILE;
        else if (nextChar == '/' && tmp == '/' )
            charClass = COMMENT;
        else if (isalpha(nextChar))
            charClass = LETTER;
        else if (isdigit(nextChar))
            charClass = DIGIT;
        else charClass = UNKNOWN;
        
        ungetc(tmp, in_fp);
    }
    else
        charClass = EOF;
}

/*****************************************************************/
/* getNonBlank - a function to call getChar until it
                                    returns a non-whitespace character */
void getNonBlank() {
    while (isspace(nextChar))
        getChar();
}

/************************************************************/
/* lex - a simple lexical analyzer for arithmetic
                            expressions */
int lex() {
    lexLen = 0;
    getNonBlank();
      bool flag = true;
    switch (charClass) {
        /* Parse identifiers */
        case LETTER:
            addChar();
            getChar();
            while (charClass == LETTER || charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = IDENT;
            nextToken = keywordlookup();
            break;
        /* Parse integer literals */
        case DIGIT:
            addChar();
            getChar();
            while (charClass == DIGIT) {
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;
        /* PREPROCCESS */
        case PREPROCCESS: 
            addChar();
            getChar();
            while (charClass == LETTER) {
                addChar();
                getChar();
            }
            nextToken = PREPROCCESS;
            break;
        /* include file <file> */
        case INCLUDE_FILE: 
            addChar();
            getChar();
            while (flag){
                addChar();
                if (nextChar == '>')
                    flag = false;
                getChar();
            }
            nextToken = INCLUDE_FILE;
            break;
        case COMMENT: 
            addChar();
            getChar();
            while (nextChar != '\n'){
                addChar();
                getChar();
            }
            nextToken = COMMENT;
            break;
        /* Parentheses and operators */
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;
        /* EOF */
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    } /* End of switch */
    printf("Next token is: %d, Next lexeme is %s\n",
           nextToken, lexeme);
    return nextToken;
} /* End of function lex */