#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>

// linked list
struct Token {
    char *val; // the string, null terminated
    struct Token *next;
} *head, *tail;

int parse() {
    char *input = readline("> "); // free when finished
    if (!input) { // ctrl+d
        free(input);
        printf("\n");
        return 0;
    }
    /* worst case: every char is a special character
       adding one \0 for each char = 2*len(input) */
    char *output = calloc(2, strlen(input));

    head = malloc(sizeof(struct Token)); // the current token
    char *p = input; // used to iterate through input
    char *q = output; // ditto for output
    head->val = output;
    head->next = NULL;
    tail = head;
    // move past inital whitespace
    while (isspace(*p)) {
        p++;
    }

    do {
        if (isspace(*p)) { // whitespace
            *q++ = '\0'; // end token
            // move to start of next token
            while (isspace(*p)) {
                    p++;
            }
            // start next token
            tail->next = malloc(sizeof(struct Token));
            tail = tail->next;
            tail->val = q;
        } else {
            switch (*p) { // current char
                case '\\': // add the next char
                    *q++ = *++p;
                    if (*p) {
                        p++;
                    }
                    break;
                case '\'': // contents are part of the current token
                    p++;
                    while (*p != '\0' && *p != '\'') {
                        *q++ = *p++; // accept next character
                    }
                    if (*p) {
                        p++;
                    }
                    break;
                case '\"': // same as above but allowing back-slash
                    p++;
                    while (*p != '\0' && *p != '\"') {
                        if (*p == '\\') {
                            *q++ = *++p;
                            if (*p) {
                                p++;
                            }
                        } else {
                            *q++ = *p++; // accept next character
                        }
                    }
                    if (*p) {
                        p++;
                    }
                    break;
                // these special chars do not get included in new token (in instruction example but never explicitly stated)
                case '<':
                case '>':
                    *q++ = '\0'; // end token
                    // start next token
                    tail->next = malloc(sizeof(struct Token));
                    tail = tail->next;
                    tail->val = q;
                    *q++ = *p++; // accept next character
                    *q++ = '\0'; // end token
                    // start next token
                    tail->next = malloc(sizeof(struct Token));
                    tail = tail->next;
                    tail->val = q;
                    break;
                // special chars
                case '|':
                case ';':
                case '&':
                    *q++ = '\0'; // end token
                    // start next token
                    tail->next = malloc(sizeof(struct Token));
                    tail = tail->next;
                    tail->val = q;
                // everything else
                default:
                    *q++ = *p++; // accept next character
            }
        }
    } while (*p); // not null
    *q = '\0';
    tail->next = NULL; // end last token

    // print the linked list
    if (*head->val) {
        printf("[");
        tail = head;
        if (*tail->val) {
            printf("{%s}", tail->val);
            tail = tail->next;
        }
        while (tail) { // while next != null
            if (*tail->val) {
                printf(",{%s}", tail->val);
            }
            tail = tail->next;
        }
        printf("]\n\n");
    }

    // sharing is caring, don't hog memory
    free(input);
    free(output);
    // free each token starting from head
    tail = head;
    while (tail->next) {
        head = tail;
        tail = tail->next;
        free(head);
    }
    return 1;
}

int main() {
    while (parse());
    return 0;
}
