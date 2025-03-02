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
    char *freeing_p = cmd_buff;
    command_list_t buff = {0};
    char path[4095];



    while (1)
    {
        printf("%s", SH_PROMPT);
        if (fgets(cmd_buff, ARG_MAX, stdin) == NULL)
        {
            printf("\n");
            break;
        }  
        
        rc = build_cmd_list(cmd_buff, &buff);
        if (rc != 0) {
            return rc;
        }


        if (strcmp(buff.commands[0].argv[0], "") == 0) {
            printf(CMD_WARN_NO_CMD);
        } else if (strcmp(buff.commands[0].argv[0], EXIT_CMD) == 0) {
            free(freeing_p);
            exit(0);
        } else if (strcmp(buff.commands[0].argv[0], "cd") == 0) {
            if (buff.commands[0].argv[1] != NULL) {
                getcwd(path, sizeof(path));
                strcat(path, "/");
                strcat(path, buff.commands[0].argv[1]);
                rc = chdir(path);
                if (rc < 0) {
                    return ERR_CMD_ARGS_BAD;
                }
            }
        } else if (strcmp(buff.commands[0].argv[0], "rc") == 0) {
            if (ecode != 0) {
                printf("%d\n", ecode);
            } else {
                printf("%d\n", rc);
            }
        } else {
            int pipes[buff.num - 1][2];  // Array of pipes
            pid_t pids[buff.num];        // Array to store process IDs

            // Create all necessary pipes
            for (int i = 0; i < buff.num - 1; i++) {
                if (pipe(pipes[i]) == -1) {
                    perror("pipe");
                    exit(EXIT_FAILURE);
                }
            }

            // Create processes for each command
            for (int i = 0; i < buff.num; i++) {
                pids[i] = fork();
                if (pids[i] == -1) {
                    perror("fork");
                    exit(EXIT_FAILURE);
                }

                if (pids[i] == 0) {  // Child process
                    // Set up input pipe for all except first process
                    if (i > 0) {
                        dup2(pipes[i-1][0], STDIN_FILENO);
                    }

                    // Set up output pipe for all except last process
                    if (i < buff.num - 1) {
                        dup2(pipes[i][1], STDOUT_FILENO);
                    }

                    // Close all pipe ends in child
                    for (int j = 0; j < buff.num - 1; j++) {
                        close(pipes[j][0]);
                        close(pipes[j][1]);
                    }

                    // Execute command
                    rc = execvp(buff.commands[i].argv[0], buff.commands[i].argv);
                    exit(errno);
                }
            }

            // Parent process: close all pipe ends
            for (int i = 0; i < buff.num - 1; i++) {
                close(pipes[i][0]);
                close(pipes[i][1]);
            }

            int status;
            for (int i = 0; i < buff.num; i++) {
                waitpid(pids[i], &status, 0);
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
        for (int l = 0; l < buff.num; l++) {
            for (int j = 0; j < buff.commands[l].argc; j++) {
                free(buff.commands[l].argv[j]);
                buff.commands[l].argv[j] = NULL;
            }
        }
            

    }
    return OK;
}

int build_cmd_list(char *cmd_line, command_list_t *buff) {

    cmd_line[strcspn(cmd_line, "\n")] = '\0';

    int space_state = 0;
    int arg_state = 1;
    int quote_state = 1;
    int cnt = 0;
    int dest = 0;
    int cmd_num = 0;
    int temp;
    int piped = 1;
        
    while(*cmd_line != '\0') 
    {
        if (*cmd_line == ' ')
        {
            if (arg_state == 0) 
            {
                space_state = 0;
                arg_state = 1;
            }
            if(quote_state == 0) {
                temp = strcspn(cmd_line+1, "\"")+1;
                if (space_state == 0)
                {
                    buff->commands[cmd_num].argv[dest] = malloc(ARG_MAX);
                    if (buff->commands[cmd_num].argv[dest] == NULL) {
                        return ERR_MEMORY;
                    }
                    space_state = 1;
                    cnt++;
                    memcpy(buff->commands[cmd_num].argv[dest], cmd_line, temp*sizeof(char));
                    dest++;
                }
                quote_state = 1;
            }
        } else if (*cmd_line == '"') {
            quote_state = 0;
        } else if (*cmd_line == '|') {
            space_state = 0;
            arg_state = 1;
            quote_state = 1;
            cnt = 0;
            cmd_num++;
            dest = 0;
            piped = 0;
        } else {
            if(quote_state == 0) {
                temp = strcspn(cmd_line+1, "\"")+1;
                if (space_state == 0)
                {
                    buff->commands[cmd_num].argv[dest] = malloc(ARG_MAX);
                    if (buff->commands[cmd_num].argv[dest] == NULL) {
                        return ERR_MEMORY;
                    }
                    space_state = 1;
                    cnt++;
                    memcpy(buff->commands[cmd_num].argv[dest], cmd_line, temp*sizeof(char));
                    dest++;
                }
                quote_state = 1;
            } else {
                temp = strcspn(cmd_line, " ");
                if (space_state == 0) {
                    buff->commands[cmd_num].argv[dest] = malloc(ARG_MAX);
                    if (buff->commands[cmd_num].argv[dest] == NULL) {
                        return ERR_MEMORY;
                    }
                    space_state = 1;
                    arg_state = 0;
                    memcpy(buff->commands[cmd_num].argv[dest], cmd_line, temp*sizeof(char));
                    buff->commands[cmd_num].argv[dest][temp] = '\0';
                    dest++;
                }
            }
        }
        cmd_line++;
        if(piped == 0) {
            cmd_line++;
            piped = 1;
        }
    }
    buff->commands[cmd_num].argc = cnt;
    buff->num = cmd_num+1;
    if(buff->commands[0].argv[0] == NULL) {
        buff->commands[0].argv[0] = malloc(sizeof(char));
        strcpy(buff->commands[cmd_num].argv[0], "");
    }
    return OK;
}