#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>
#include "dshlib.h"

/*
 * Implement your exec_local_cmd_loop function by building a loop that prompts the 
 * user for input.  Use the SH_PROMPT constant from dshlib.h and then
 * use fgets to accept user input.
 * 
 *      while(1){
 *        printf("%s", SH_PROMPT);
 *        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL){
 *           printf("\n");
 *           break;
 *        }
 *        //remove the trailing \n from cmd_buff
 *        cmd_buff[strcspn(cmd_buff,"\n")] = '\0';
 * 
 *        //IMPLEMENT THE REST OF THE REQUIREMENTS
 *      }
 * 
 *   Also, use the constants in the dshlib.h in this code.  
 *      SH_CMD_MAX              maximum buffer size for user input
 *      EXIT_CMD                constant that terminates the dsh program
 *      SH_PROMPT               the shell prompt
 *      OK                      the command was parsed properly
 *      WARN_NO_CMDS            the user command was empty
 *      ERR_TOO_MANY_COMMANDS   too many pipes used
 *      ERR_MEMORY              dynamic memory management failure
 * 
 *   errors returned
 *      OK                     No error
 *      ERR_MEMORY             Dynamic memory management failure
 *      WARN_NO_CMDS           No commands parsed
 *      ERR_TOO_MANY_COMMANDS  too many pipes used
 *   
 *   console messages
 *      CMD_WARN_NO_CMD        print on WARN_NO_CMDS
 *      CMD_ERR_PIPE_LIMIT     print on ERR_TOO_MANY_COMMANDS
 *      CMD_ERR_EXECUTE        print on execution failure of external command
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 1+)
 *      malloc(), free(), strlen(), fgets(), strcspn(), printf()
 * 
 *  Standard Library Functions You Might Want To Consider Using (assignment 2+)
 *      fork(), execvp(), exit(), chdir()
 */
int exec_local_cmd_loop()
{
    int rc = 0;
    int ecode;
    char *cmd_buff = malloc(SH_CMD_MAX * sizeof(char));
    cmd_buff_t buff = {0};
    buff._cmd_buffer = malloc(SH_CMD_MAX * sizeof(char));
    char *freeing_p = buff._cmd_buffer;
    char path[4095];



    while (1)
    {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }  
        
        rc = build_cmd_buff(cmd_buff, &buff);

        if (strcmp(buff.argv[0], "") == 0) {
            printf(CMD_WARN_NO_CMD);
        } else if (strcmp(buff.argv[0], EXIT_CMD) == 0) {
            free(freeing_p);
            exit(0);
        } else if (strcmp(buff.argv[0], "cd") == 0) {
            if (buff.argv[1] != NULL) {
                getcwd(path, sizeof(path));
                strcat(path, "/");
                strcat(path, buff.argv[1]);
                rc = chdir(path);
                if (rc < 0) {
                    return ERR_CMD_ARGS_BAD;
                }
            }
        } else if (strcmp(buff.argv[0], "rc") == 0) {
            if (ecode != 0) {
                printf("%d\n", ecode);
            } else {
                printf("%d\n", rc);
            }
        } else {
            pid_t child = fork();
            if (child < 0) {
                return ERR_EXEC_CMD;
            } else if (child == 0) {
                rc = execvp(buff.argv[0], buff.argv);
                exit(errno);
            } else {
                int status;
                waitpid(child, &status, 0);
                if (WIFEXITED(status)) {
                    ecode = WEXITSTATUS(status);
                    if (ecode == 2) {
                        printf("Command not found in PATH\n");
                    } else if (ecode == 13) {
                        printf("Permission Denied\n");
                    }
                } else {
                    return ERR_EXEC_CMD;
                }
            }
        }
        for (int j = 0; j < buff.argc; j++) {
            free(buff.argv[j]);
            buff.argv[j] = NULL;
        }

    }
    return OK;
}

int build_cmd_buff(char *cmd_line, cmd_buff_t *buff) {

    memcpy(buff->_cmd_buffer, cmd_line, SH_CMD_MAX*sizeof(char));

    buff->_cmd_buffer[strcspn(buff->_cmd_buffer, "\n")] = '\0';
        
    int space_state = 0;
    int arg_state = 1;
    int quote_state = 1;
    int cnt = 0;
    int dest = 0;
    int temp;
        
    while(*buff->_cmd_buffer != '\0') 
    {
        if (*buff->_cmd_buffer == ' ')
        {
            if (arg_state == 0) 
            {
                space_state = 0;
                arg_state = 1;
            }
            if(quote_state == 0) {
                temp = strcspn(buff->_cmd_buffer+1, "\"")+1;
                if (space_state == 0)
                {
                    buff->argv[dest] = malloc(ARG_MAX);
                    if (buff->argv[dest] == NULL) {
                        return ERR_MEMORY;
                    }
                    space_state = 1;
                    cnt++;
                    memcpy(buff->argv[dest], buff->_cmd_buffer, temp*sizeof(char));
                    dest++;
                }
                quote_state = 1;
            }
        } else if (*buff->_cmd_buffer == '"') {
            quote_state = 0;
        } else {
            if(quote_state == 0) {
                temp = strcspn(buff->_cmd_buffer+1, "\"")+1;
                if (space_state == 0)
                {
                    buff->argv[dest] = malloc(ARG_MAX);
                    if (buff->argv[dest] == NULL) {
                        return ERR_MEMORY;
                    }
                    space_state = 1;
                    cnt++;
                    memcpy(buff->argv[dest], buff->_cmd_buffer, temp*sizeof(char));
                    dest++;
                }
                quote_state = 1;
            } else {
                temp = strcspn(buff->_cmd_buffer, " ");
                if (space_state == 0) {
                    buff->argv[dest] = malloc(ARG_MAX);
                    if (buff->argv[dest] == NULL) {
                        return ERR_MEMORY;
                    }
                    space_state = 1;
                    arg_state = 0;
                    cnt++;
                    memcpy(buff->argv[dest], buff->_cmd_buffer, temp*sizeof(char));
                    buff->argv[dest][temp] = '\0';
                    dest++;
                }
            }
        }
        buff->_cmd_buffer++;
    }
    buff->argc = cnt;
    if(buff->argv[0] == NULL) {
        buff->argv[0] = malloc(sizeof(char));
        strcpy(buff->argv[0], "");
    }
    return OK;
}