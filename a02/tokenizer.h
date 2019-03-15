/**
 * This contains the linked list struct and the forward declarations of the
 * tokenizer functions to be used in command.c.
 */

struct Token {
    char *val; // the string, null terminated
    struct Token *next;
};

struct Token *getcmds();
void pprint();
void ffree();
int parse();
