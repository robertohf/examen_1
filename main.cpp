#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <iostream>
#include <stdlib.h>
#include "ast.h"

using namespace std;

extern FILE *yyin;
extern FILE *fp;

DIR *dir;
struct dirent *ent;

int yyparse();
void yyrestart(FILE *input_file);
bool execute(const char* argument);

StmtPtr root;

int main (int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Missing input file(s). %s \n", argv[0]);
        return 1;
    }
    
    bool parse_successfully = execute(argv[1]);
    if(!parse_successfully) {
        cout << "\033[1;32m\n--COMPILED SUCCESSFULLY--\033[0m" << endl;
    }
    else {
        cout << "\033[1;31m\n--COMPILED FAILED--\033[0m" << endl; 
    }
    return 0;
}

bool execute(const char* directory) {
    std::string dir_path = directory;
    std::string file_path;

    bool yyin_flag = true;
    bool parse_flag = false;

    Context ctx;
    if((dir = opendir(dir_path.c_str())) != NULL) {
        while((ent = readdir(dir)) != NULL) {
            if((strstr(ent->d_name, ".calc") != NULL)) {
                file_path.clear();
                file_path.append(dir_path);
                file_path.append(ent->d_name);

                fp = fopen(file_path.c_str(), "r");
                if(fp == NULL) {
                    printf("File Error. Null.\n");
                    return 1;
                }

                if (yyin_flag) {
                    yyin = fp;
                    yyin_flag = false;
                }
                
                parse_flag = yyparse();
                root->exec(ctx);

                yyrestart(yyin);
                fclose(fp);
                
                std::cout << std::endl;
            }
        }
        closedir(dir);
    }
    return parse_flag;
}