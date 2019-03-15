#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
#include "tokenizer.h"

Token *head, *tail, *temp; // linked list

#define END_INPUT 0
#define CONTINUE 1

// gets the linked list for command.c
Token *getcmds() {
    return head;
}

// print the linked list
void pprint() {
    if (*head->val) {
        printf("[");
        tail = head;
        do {
            printf("{%s}", tail->val);
            tail = tail->next;
        } while (tail);
        printf("]\n\n");
    }
}

char *input, *output;
// sharing is caring, don't hog memory
void ffree() {
    free(input);
    free(output);
    // free each token starting from head
    tail = head;
    do {
        head = tail;
        tail = tail->next;
        free(head);
    } while (tail);
}

// the tokenizer function
int parse() {
    input = readline("> "); // free when finished
    if (!input) { // ctrl+d
        free(input);
        printf("\n");
        return END_INPUT;
    }
    // worst case: every char is a special character adding one \0 for each char = 2*len(input)
    output = calloc(2, strlen(input));

    head = malloc(sizeof(Token)); // the current token
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
            tail->next = malloc(sizeof(Token));
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
                // these special chars do not get included in new token
                case '<':
                case '>':
                case '|':
                case ';':
                case '&':
                    *q++ = '\0'; // end token
                    // start next token
                    tail->next = malloc(sizeof(Token));
                    tail = tail->next;
                    tail->val = q;
                    *q++ = *p++; // accept next character
                    *q++ = '\0'; // end token
                    // start next token
                    tail->next = malloc(sizeof(Token));
                    tail = tail->next;
                    tail->val = q;
                    break;
                // everything else
                default:
                    *q++ = *p++; // accept next character
            }
        }
    } while (*p); // not null
    *q = '\0';
    tail->next = NULL; // end last token

    // remove any tokens with no values before the head
    while (!*head->val && head->next) {
        temp = head;
        head = head->next;
        free(temp);
    }
    tail = head;
    // remove the other tokens with no values
    while (tail->next) {
        temp = tail;
        tail = tail->next;
        if (!*tail->val) { // if there's no value
            if (tail->next) { // if there's another token
                temp->next = tail->next;
            } else { // else end the linked list
                temp->next = NULL;
            }
            free(tail); // remove the empty value
            tail = temp;
        }
    }

    return CONTINUE;
}
