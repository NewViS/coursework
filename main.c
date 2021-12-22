#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define msize 80
#define MAXLEN 1024
#define word_size 20
#define int_size 10

void read_line(char* s, FILE* is, int max_len){
    char c;
    int i;

    i=0;
    while(((c=fgetc(is))!='\n')&&(i<max_len-1)&&(c!=EOF)){
        s[i]=c;
        i++;
    }
    s[i]='\0';
}

int is_consist(char* string, char** substring, int num_substr){
    int k, j, i;
    for(i=0; i<num_substr; i++) {
        k=0;
        for (j = 0; string[j]; j++) {
            if (tolower(string[j]) == tolower(substring[i][k])) {
                if(!k) {    /* is it word start */
                    if (!j) {
                        k++;
                    } else if (!isalnum(string[j - 1])) {
                        k++;
                    }
                } else{
                    k++;
                }
                if (!substring[i][k]){  /*is it word end*/
                    if(j==strlen(string)-1){
                        return 1;
                    }else if(!isalnum(string[j+1])){
                        return 1;
                    }
                }
            }else k=0;
        }
    }
    return 0;
}

int read_int(){
    char line[int_size];
    read_line(line, stdin, int_size);
    return abs((int)strtol(line, NULL, int_size));
}

int remove_lines(char** str, char** res, int num_lines){
    char **path;
    int num_words, i, res_lines;

    puts("Enter the number of key words:");
    num_words=read_int();
    path = malloc(msize * sizeof(char*));
    for(i=0; i<num_words; i++){
        path[i] = malloc(word_size * sizeof(char));
    }

    if(path!=NULL){
        puts("Enter the key words:");
        for(i =0; i<num_words;i++) {
            read_line(path[i], stdin, msize);
        }

        if(!num_words){
            res=str;
            res_lines=num_lines;
        }else{
            res_lines=0;
            for(i=0; i<num_lines; i++){
                if(!is_consist(str[i], path, num_words)){
                    res[res_lines]=str[i];
                    res_lines++;
                }
            }
        }

        puts("Result:");
        for(i=0; i<res_lines; i++){
            puts(res[i]);
        }

        for(i=0; i<num_words; i++){
            free(path[i]);
        }
        free(path);

    }else{
        puts("No memory allocated for path");
        return num_lines;
    }

    return res_lines;
}

int main() {
    int type_of_enter, num_lines, i, n, res_lines, flag_filename, flag_number;

    char **str, **res, *filename, *str1;

    FILE *df;
    flag_filename = 0;
    puts("What will be the input method?\nEnter\n1 if you want to use console\n2 if you want to use txt");
    type_of_enter = read_int();

    switch (type_of_enter) {
        case 1: { /*console*/
            puts("Enter the number of lines:");
            num_lines=read_int();

            str = malloc(num_lines * sizeof(char *));
            for (i = 0; i < num_lines; i++) {
                str[i] = malloc(msize * sizeof(char));
            }

            res = malloc(num_lines * sizeof(char *));
            for (i = 0; i < num_lines; i++) {
                res[i] = malloc(msize * sizeof(char));
            }

            if (str != NULL && res != NULL) {
                puts("Enter your text:");
                for (i = 0; i < num_lines; i++) {
                    read_line(str[i], stdin, msize);
                }
            flag_number=1;
            } else {
                puts("No memory allocated");
                flag_number=0;
            }
            break;
        }
        case 2: {   /*txt*/
            filename = malloc(msize * sizeof(char));
            str1 = malloc(MAXLEN * sizeof(char));

            if (filename != NULL && str1 != NULL) {
                puts("Enter name of your file:");
                read_line(filename, stdin, msize);
                if (strlen(filename) < msize - 1 - 4 - 4) { /* msize - '\0' - '_new' - '.txt' */
                    sprintf(filename, "%s.txt", filename);
                    df = fopen(filename, "r");

                    if (df != NULL) {
                        n = 0;
                        num_lines=0;

                        while (fgets(str1, MAXLEN, df) != NULL) n++;    /*count lines in file */

                        rewind(df);
                        str = (char **) malloc(n * sizeof(char *));
                        res = (char **) malloc(n * sizeof(char *));
                        if (str != NULL && res != NULL) {
                            for (i = 0, num_lines = 0; i < n; i++) {
                                str[i] = (char *) malloc(MAXLEN * sizeof(char));
                                res[i] = (char *) malloc(MAXLEN * sizeof(char));
                                if (str[i] != NULL) {
                                    num_lines++;
                                    read_line(str[i], df, MAXLEN);
                                } else {
                                    i = n;
                                    puts("Error at string allocation! Array not completed!");
                                }
                            }
                            puts("----------------");
                            puts("Results of processing:");
                            for (i = 0; i < num_lines; i++) {
                                printf("[%d]: %s\n", i, str[i]);
                            }
                            flag_number=1;
                        } else puts("No memory allocated for string or result");
                    } else {
                        flag_number = 0;
                        perror("Data error");
                    }
                } else {
                    puts("Too long filename");
                    flag_filename = 1;
                }
                free(str1);
            } else {
                puts("No memory allocated for filename or buffer");
                flag_number=0;
            }
            break;
        }
        default:
            puts("Wrong number");
            flag_number=0;
    }

    if(str!=NULL && res!=NULL && flag_number && num_lines) {
        res_lines = remove_lines(str, res, num_lines);

        if (type_of_enter == 2 && !flag_filename) {
            fclose(df);
            filename[strlen(filename) - 4] = '\0';
            sprintf(filename, "%s_new.txt", filename);

            df = fopen(filename, "w");

            if (df != NULL) {
                for (i = 0; i < res_lines; i++) {
                    fprintf(df, "%s\n", res[i]);
                }
            } else perror("Data error");
            fclose(df);
            free(filename);
        }

        for (i = 0; i < num_lines; i++) {
            free(str[i]);
        }

        free(str);
        free(res);
    }
    return 0;
}