#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "dshlib.h"

/*
 *  build_cmd_list
 *    cmd_line:     the command line from the user
 *    clist *:      pointer to clist structure to be populated
 *
 *  This function builds the command_list_t structure passed by the caller
 *  It does this by first splitting the cmd_line into commands by spltting
 *  the string based on any pipe characters '|'.  It then traverses each
 *  command.  For each command (a substring of cmd_line), it then parses
 *  that command by taking the first token as the executable name, and
 *  then the remaining tokens as the arguments.
 *
 *  NOTE your implementation should be able to handle properly removing
 *  leading and trailing spaces!
 *
 *  errors returned:
 *
 *    OK:                      No Error
 *    ERR_TOO_MANY_COMMANDS:   There is a limit of CMD_MAX (see dshlib.h)
 *                             commands.
 *    ERR_CMD_OR_ARGS_TOO_BIG: One of the commands provided by the user
 *                             was larger than allowed, either the
 *                             executable name, or the arg string.
 *
 *  Standard Library Functions You Might Want To Consider Using
 *      memset(), strcmp(), strcpy(), strtok(), strlen(), strchr()
 */
int build_cmd_list(char *cmd_line, command_list_t *clist)
{
    int i = 0, j = 0, k = 0;
    char *comms; 
    char *inrem;
    char *exrem;
    comms = strtok_r(cmd_line, PIPE_STRING, &exrem);
    while(comms != NULL) {
        if(*comms == ' ') {
            comms++;
        }
        i++;
        strcpy(&(clist->commands[i-1].args[0]), "\0");
        j = 0;
        k = 0;
        char *this_comm = strtok_r(comms, " ", &inrem);
        while(this_comm != NULL) {
            int str_len = strlen(this_comm);
            if(j == 0) {
                strcpy(clist->commands[i-1].exe, this_comm);
                j += str_len;
                strcpy(&(clist->commands[i-1].exe[j]), "\0");
            } else {
                strcpy(&(clist->commands[i-1].args[k]), this_comm);
                k += str_len;
                strcpy(&(clist->commands[i-1].args[k]), " ");
                k++;
            }
            this_comm = strtok_r(NULL, " ", &inrem);
        }
        strcpy(&(clist->commands[i-1].args[k-1]), "\0");
        if ((strlen(clist->commands[i-1].args) > EXE_MAX) || (strlen(clist->commands[i-1].exe) > ARG_MAX)) {
            return ERR_CMD_OR_ARGS_TOO_BIG;
        }
        comms = strtok_r(NULL, PIPE_STRING, &exrem);
    }
    if (i > CMD_MAX) {
        return ERR_TOO_MANY_COMMANDS;
    }
    clist->num = i;
    return OK;
}