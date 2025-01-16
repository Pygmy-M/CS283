#include <stdio.h>
#include <string.h>
#include <stdlib.h>


#define BUFFER_SZ 50

//prototypes
void usage(char *);
void print_buff(char *, int);
int  setup_buff(char *, char *, int);

//prototypes for functions to handle required functionality
int  count_words(char *, int);
int  reverse(char *, int);
int  word_print(char *, int);
//add additional prototypes here


int setup_buff(char *buff, char *user_str, int len){
    int state_is_space = 0;
    int start = 1;
    char *tp = user_str;
    int len_of_str = 0;
    int cnt = 0;
    while (*tp != '\0') { //go until the end of the user_str
        if ((*tp == ' ') || (*tp == ' ') || (*tp == ' ')) { //check if *tp is whitespace
            if (start == 0) { 
                if (state_is_space == 0) { //if there is white space enter the "space" state
                    state_is_space = 1;
                }
            }
        } else {
            if (start == 1) {  //add the first non-whitespace char to buff. This will ignore adding 
                start = 0;     //whitespace from the start of user_str
                memset(buff+cnt, *tp, sizeof(char));
                cnt++;
            } else if (state_is_space == 1) { //if it's not the beginning of user_str, exit "space"
                len_of_str++;                 //state, print a ' ' and print the char encountered
                len_of_str++;
                if (len < len_of_str) {
                    return -1;
                }
                memset(buff+cnt, ' ', sizeof(char));
                state_is_space = 0;
                cnt++;
                memset(buff+cnt, *tp, sizeof(char));
                cnt++;
            } else if (state_is_space == 0) { //add any non-whitespace char encountered to buff
                len_of_str++;
                if (len < len_of_str) {
                    return -1;
                }
                memset(buff+cnt, *tp, sizeof(char));
                cnt++;
            }
        }
        
        tp++;
    }
    memset(buff+cnt, '.', len-(cnt*sizeof(char))); //fill the rest of buff with '.'
    return cnt; 
}

void print_buff(char *buff, int len){ 
    printf("Buffer:  [");
    for (int i=0; i<len; i++){
        putchar(*(buff+i));
    }
    putchar(']');
    putchar('\n');
}

void usage(char *exename){
    printf("usage: %s [-h|c|r|w|x] \"string\" [other args]\n", exename);

}

int count_words(char *buff, int str_len){
    int state_is_space = 1;
    int wcnt = 0;
    char *tp = buff;
    for (int i=0; i<str_len; i++) { //go until the end of our input
        if (*tp == ' ' || *tp == '\n' || *tp == '\t') { //check for whitespace
            if (state_is_space == 0) {
                state_is_space = 1;
            }  
        } else {
            if (state_is_space == 1) { //each time the function swaps from whitespace to char,
                state_is_space = 0;    //add 1 to word count
                wcnt++;
            }
        }
        tp++;
    }
    return wcnt;
}

int  reverse(char *buff, int str_len) {
    char temp[BUFFER_SZ];
    memcpy(temp, buff, str_len); //create a temp char array that is a copy of buff
    int cnt = 0;
    for (int i=0; i<str_len; i++) {                                //copy temp into buff backwards
        memset(((buff+str_len)-cnt-1), temp[cnt], sizeof(char));
        cnt++;
    }
    return 0;
}

int  word_print(char *buff, int str_len) {
    int state_is_space = 1;
    int start = 1;
    int wcnt = 0;
    int ccnt = 0;
    char *tp = buff;
    char *follow = buff;
    printf("Word Print\n----------\n");
    for (int i=0; i<str_len; i++) {
        if (*tp == ' ' || *tp == '\n' || *tp == '\t') {
            if (state_is_space == 0) {   //if we are entering whitespace, output the word along with its length
                state_is_space = 1;
                printf("%d. ", wcnt);
                while (follow != tp) { //print word
                    putchar(*follow);
                    follow++;
                }
                printf("(%d)\n", ccnt);
                ccnt = 0;
            }   else if (state_is_space == 1) {
                follow++;
            }
        } else {
            if (state_is_space == 1) { //while in whitespace, follow stays with tp
                state_is_space = 0;
                if (start == 0) {
                    follow++;
                }
                start = 0;
                wcnt++;
                ccnt++;
            } else if (state_is_space == 0) {
                ccnt++;
            }
        }
        tp++;
    }
    printf("%d. ", wcnt);
    while (follow != tp) {
        putchar(*follow);
        follow++;
    }
    printf("(%d)\n", ccnt);
    return wcnt;
}

int main(int argc, char *argv[]){

    char *buff;             //placehoder for the internal buffer
    char *input_string;     //holds the string provided by the user on cmd line
    char opt;               //used to capture user option from cmd line
    int  rc;                //used for return codes
    int  user_str_len;      //length of user supplied string

    //TODO:  #1. WHY IS THIS SAFE, aka what if arv[1] does not exist?
    //      This is safe because if arv[1] does not exist, instead of failing the program will
    //      output the proper usage and then exit.
    if ((argc < 2) || (*argv[1] != '-')){
        usage(argv[0]);
        exit(1);
    }

    opt = (char)*(argv[1]+1);   //get the option flag

    //handle the help flag and then exit normally
    if (opt == 'h'){
        usage(argv[0]);
        exit(0);
    }

    //WE NOW WILL HANDLE THE REQUIRED OPERATIONS

    //TODO:  #2 Document the purpose of the if statement below
    //      The if statement below checks if the arguments fit the proper format. If not, exit
    //      with error code 1.
    if (argc < 3){
        usage(argv[0]);
        exit(1);
    }

    input_string = argv[2]; //capture the user input string

    //TODO:  #3 Allocate space for the buffer using malloc and
    //          handle error if malloc fails by exiting with a 
    //          return code of 99
    // CODE GOES HERE FOR #3
    buff = malloc(BUFFER_SZ);
    if (buff == NULL) {
        printf("Error while allocating memory = %d\n", 2);
        exit(2);
    }
    user_str_len = setup_buff(buff, input_string, BUFFER_SZ);     //see todos
    if (user_str_len < 0){
        printf("Error setting up buffer, error = %d\n", user_str_len);
        exit(2);
    }

    switch (opt){
        case 'c':
            rc = count_words(buff, user_str_len); 
            if (rc < 0){
                printf("Error counting words, rc = %d", rc);
                exit(2);
            }
            printf("Word Count: %d\n", rc);
            break;

        case 'r':
            rc = reverse(buff, user_str_len); 
            if (rc < 0){
                printf("Error reversing, rc = %d", rc);
                exit(2);
            }
            break;
        
        case 'w':
            rc = word_print(buff, user_str_len);  
            if (rc < 0){
                printf("Error printing words, rc = %d", rc);
                exit(2);
            }
            printf("\nNumber of words returned: %d\n", rc);
            break;

        default:
            usage(argv[0]);
            exit(1);
    }

    //TODO:  #6 Dont forget to free your buffer before exiting
    
    print_buff(buff,BUFFER_SZ);
    free(buff);
    exit(0);
}

//TODO:  #7  Notice all of the helper functions provided in the 
//          starter take both the buffer as well as the length.  Why
//          do you think providing both the pointer and the length
//          is a good practice, after all we know from main() that 
//          the buff variable will have exactly 50 bytes?
//  
//          I think this is good practice because providing the length and pointer allow us
//          to easily work with the buffer without running into any errors. Effectively, we
//          give the function a value to use to keep itself within the bounds that we need it.