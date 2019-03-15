/**
 * This is MUSH!
 */

#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdio.h>
#include <string.h>
#include "tokenizer.h"

// for printing errors
#define printe(args...) fprintf(stderr, args)

struct Token *head, *tail, *temp; // a different, local linked list

char in[256], *argv[100];
void execute_commands(struct Token *list) {
    if (list) {
        // # of remaining commands, will never exceed 1
        int cmds = 1;
        head = list;
        tail = head;

        do { // while there's still tokens
        
            // if we still have cmds left
            if (!strcmp(tail->val, "pwd") && cmds > 0) {
                cmds--;
                if (getcwd(in, 256)) {
                    printf("%s\n", in);
                } else {
                    printe("Unable to obtain current directory\n");
                }
                if (tail->next) {
                    tail = tail->next;
                } else { // return if there's no more tokens
                    return;
                }

            } else if (!strcmp(tail->val, "cd") && cmds > 0) {
                cmds--;
                if (tail->next) { // >= 1 argument
                    tail = tail->next;
                    if (tail->next) {
                        temp = tail;
                        tail = tail->next;
                        // cd arg ; cmd == 1 argument
                        if (*tail->val == ';' || *tail->val == '|') {
                            if (chdir(temp->val)) {
                                printe("%s%s", "Directory does not exist",
                                " or is not accessible\n");
                            }
                        } else { // cd arg cmd > 1 argument
                            printe("Accepts exactly one argument\n");
                            return;
                        }
                    } else { // cd arg == 1 argument
                        if (chdir(tail->val)) {
                            printe("%s%s", "Directory does not exist or ",
                            "is not accessible\n");
                        }
                        return;
                    }
                } else { // cd < 1 argument
                    printe("Accepts exactly one argument\n");
                    return;
                }

            } else {
                char *p = tail->val;
                // checks if the operator was escaped into the token
                if (!*++p) {
                    switch (*tail->val) { // operators to strip
                        case '<':
                        case '>':
                            if (tail->next) { // strips one token (if there)
                                tail = tail->next;
                            }
                        case '&':
                            if (tail->next) {
                                tail = tail->next;
                            }
                            printe("%s%s", "IO redirection and ",
                            "background not implemented\n");
                            break;
                        case '|':
                            printe("Pipe not implemented\n");
                        case ';':
                            if (tail->next) {
                                // allows mush to process the next cmd token
                                cmds = 1;
                                tail = tail->next;
                            }
                    }
                // if it was not an operator, it was an exe file
                } else if (cmds > 0) {
                    cmds--;
                    int i = 0;
                    // build argument list argv
                    do { // until no more args
                        argv[i++] = tail->val; // add arg
                        if (tail->next) {
                            tail = tail->next;
                        } else {
                            argv[i] = NULL; // end args
                            break;
                        }

                        p = tail->val;
                        if (!*++p) {
                            switch (*tail->val) {
                                // stop if we hit one of these chars
                                case '<':
                                case '>':
                                case '&':
                                case '|':
                                case ';':
                                    argv[i] = NULL;
                                    i = 100; // stop while-loop
                            }
                        }
                    } while (i < 100);

                    // fork child process
                    pid_t pid = fork();
                    if (pid == 0) { // good to go
                        if (execvp(argv[0], argv) == -1) {
                            printe("Unable to execute %s\n", argv[0]);
                            exit(0); // kill child process
                        }
                    } else if (pid > 0) { // need to wait
                        i = wait(0);
                        if (i == -1) {
                            printe("Process exited with error\n");
                        } else {
                            printf("Process exited successfully\n");
                        }
                    } else { // failed, should never happen
                        printe("Unable to spawn program\n");
                    }
                    // in the event that the exe was called as the last token
                    if (!tail->next) {
                        // set tail, will be NULL, breaks the while-loop
                        tail = tail->next;
                    }
                }
            }
        } while (tail);
    }
}

int main() {
    while (parse()) { // no magic numbers this time, see tokenizer.c
        execute_commands(getcmds());
        // pprint();
        ffree();
    }
    return 0;
}
