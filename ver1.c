#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <ctype.h>

int err_count = 0;
int line_num = 0;


int get_error_num(char *ptr) {
    char *ptr_copy = ptr;
    int error_num = -1;
    char *error_ptr;
    char error[4] = "\0";
    for (int i = 0; i < 2; i++) {
        error_ptr = strstr(ptr_copy, "\"");
        if (error_ptr != NULL) {
            ptr_copy = error_ptr + 1;
        }
    }
    if (error_ptr != NULL) {
        while (!isdigit(*error_ptr)) {
            error_ptr++;
        }
        for (int i = 0; i < 3; i++) {
            error[i] = *(error_ptr + i);
        }
        error[3] = '\0';
        error_num = atoi(error);
    }
    return error_num;
}

void request_parse(FILE *fptr, char *str, int error_num) {
    err_count++;
    char *beginning = strstr(str, "\"") + 1;
    fprintf(fptr, "%d", err_count);
    fputs(". Line: ", fptr);
    fprintf(fptr, "%d", line_num);
    fputc(' ', fptr);
    while (*beginning != '"') {
        fputc(*beginning, fptr);
        beginning++;
    }
    fputs(" Error: ", fptr);
    fprintf(fptr, "%d", error_num);
    fputc('\n', fptr);
}


int main(int argc, char *argv[]) {
    char str[512] = "\0";
    FILE *ptr = NULL;
    FILE *wptr = NULL;
    if (argc >= 2) {
        ptr = fopen(argv[1], "r");
        wptr = fopen(argv[2], "r");
    }
    if (ptr == NULL) {
        fprintf(stderr, "The program has encountered a problem whilst trying to open a '.log' file\nError code:%d",
                errno);
    } else if (wptr == NULL) {
        fprintf(stderr, "The program has encountered a problem whilst trying to open an output file\n");
    } else {
        while (fgets(str, 512, ptr) != NULL) {
            line_num++;
            int i = get_error_num(str);
            if (i >= 500 && i < 600) {
                request_parse(wptr, str, i);
            }
        }
        printf("The results have been successfully parsed to the designated file.");
    }
    return 0;
}