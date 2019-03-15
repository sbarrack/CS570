typedef struct Token Token;

struct Token {
    char *val; // the string, null terminated
    Token *next;
};

Token *getcmds();
void pprint();
void ffree();
int parse();
