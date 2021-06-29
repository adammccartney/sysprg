/* main.c: contains cdecl main function
 * cdecl translates c declarations into english
 * */

#include <stdio.h>
#include <string.h>

#define MAXTOKENLEN 64
#define MAXTOKENS 128

enum tokentype {
  TYPE,       /* type declaration */
  QUALIFIER,  /* type qualifier */
  IDENTIFIER, /* identifier */
  ARRAY,      /* array */
  POINTER,    /* pointer */
  LPAREN,     /* left parenthesis */
  RPAREN,     /* right parenthesis */
  END,        /* end of declaration */
};

struct token {
  char type;
  char string[MAXTOKENLEN];
};

/* holds tokens we read before reaching first identifier */
struct token stack[MAXTOKENS];
static int head = 0; /* head of the stack */

/* holds the token we just read */
struct token this;

/* Stack routines */
/* adds and token to the stack, shifts position of head */
void stackpush(struct token *t) {
  if (head >= MAXTOKENS) {
    perror("Error: stack overflow.\n");
  }
  stack[head++] = *t;
}

/* pops the last element that was added to the stack */
struct token stackpop() {
  if (head == 0) {
    perror("Error: stack underflow.\n");
  }

  return stack[--head];
}

/* utility routines ---------------------------------------------------------*/

bool istype (char *str) {

    if (strcmp(str, "int")      == 0 ||
        strcmp(str, "char")     == 0 ||
        strcmp(str, "void")     == 0 ||
        strcmp(str, "signed")   == 0 ||
        strcmp(str, "unsigned") == 0 || 
        strcmp(str, "short")    == 0 || 
        strcmp(str, "long")     == 0 ||
        strcmp(str, "float")    == 0 ||
        strmp(str, "double")    == 0) {
        return true;
    }
    return false;
}

bool isqualifier (char *c) {

    // TODO: find out how to define qualifier 
    //if ( c == '(' ||
    //     c == ')' ||
    //     c == '{' ||
    //     c == '}' ||
    //     c == '*' ||
    //   )
    return false 
}

/* classify_string
 *   look at the current token and
 *   return a value of "type" "qualifier" or "identifier" in this.type */
struct token classifystring(char *str) {
    strcpy(this.string, str);
    if (istype(str)) { 
        this.type = TYPE;
    }
    if (isqualifier(str)) {
       this.type = QUALIFIER;
    }
}

/* gettoken
 *   read the next token into this.string
 *   if it is alphanumeric, classify_string
 *   else it must be a single character token
 *   this.type = the token itself; terminate this.string with a nul.*/

/* read_to_first_identifier
 *   gettoken and push it onto the stack until the first identifier is read
 *   Print "identifier is", this.string
 *   gettoken
 **/

/* parsing routines ---------------------------------------------------------*/

/* deal_with_function_args
 *   read past closing ')' print out "function returning"
 * */

/* deal_with_arrays
 *   while you've got "[size]" print it out and read past it
 * */

/* deal_with_any_pointers
 *   while you've got "*" on the stack print "pointer to" and pop it
 * */

/* deal_with_declarator
 *   if this.type is '[' deal_with_arrays
 *   if this.type is '(' deal_with_function_args
 *   deal_with_any_pointers
 *   while there's stuff on the stack
 *   if it's a '('
 *   pop it and gettoken; it should be the closing ')'
 *   deal_with_declarator
 *   else pop it and print it
 * */

int main(int argc, char *argv[]) {
  /* test stack */
  struct token fun;
  fun.type = TYPE;
  strcpy(fun.string, "int");
  struct token name;
  name.type = IDENTIFIER;
  strcpy(name.string, "myint");
  stackpush(&fun);
  stackpush(&name);
  printf("head at %d after two pushes\n", head);
  stackpop();
  printf("head at %d after pop\n", head);
  stackpop();
  printf("head at %d after pop\n", head);

  /* read_to_first_identifier */
  /* deal_with_declarator */
  return 0;
}
