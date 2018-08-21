#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>

#define LINE_SIZE 4*1024
#define TITLE_SIZE 4*128
#define CONTENT_SIZE 4*20480

typedef struct chapter{
    char *title;
    char *content;
}chapter;

void printChapter(chapter*);
void chapterToFile(chapter*, const char*);

int main(int argc, char* argv[]){
    FILE* novel=fopen(argv[1], "r");
    char* line=malloc(sizeof(char)*4096);
    regex_t reg;
    const char* title_pattern="^ *第[零一二三四五六七八九十百千0123456789]*章 .*\n$";
    const char* pathDir="./results/";
    regcomp(&reg, title_pattern, REG_EXTENDED);

    const size_t nmatch = 1;
    regmatch_t pmatch[1];
    int status;

    if(argc==1){
        printf("Please specify a file.\n");
        printf("Usage: %s filename\n", argv[0]);
        exit(0);
    }
    if(novel == NULL){
        printf("Failed to open %s\n", argv[1]);
        exit(0);
    }

    chapter* chapter=malloc(sizeof(chapter));
    chapter->title=malloc(sizeof(char)*TITLE_SIZE);
    chapter->content=malloc(sizeof(char)*CONTENT_SIZE);

    strcpy(chapter->title, "start\n");
    strcpy(chapter->content, argv[1]);
    while(!feof(novel)){
        status = regexec(&reg, line, nmatch, pmatch, 0);
        switch(status){
            case 0:
                chapterToFile(chapter, pathDir);
                memset(chapter->title, 0, TITLE_SIZE);
                memset(chapter->content, 0, CONTENT_SIZE);
                strcpy(chapter->title, line);
                break;
            case REG_NOMATCH:
                strcat(chapter->content, line);
                break;
            default:
                printf("Match ERR\n");
                break;
        }
        memset(line, 0, LINE_SIZE);
        fgets(line, LINE_SIZE, novel);
    }
    chapterToFile(chapter, pathDir);
    regfree(&reg);
    fclose(novel);
    return 0;
}

void printChapter(chapter* chapter){
    printf("Title:\n%s\n", chapter->title);
    printf("Content:\n%s\n", chapter->content);
}

void chapterToFile(chapter* chapter, const char* pathDir){
    char* filepath =malloc(sizeof(char)*LINE_SIZE);
    strcpy(filepath, pathDir);
    strcat(filepath, chapter->title);
    filepath[strlen(filepath)-1]='\0';
    strcat(filepath, ".txt");
    FILE* output = fopen(filepath, "w");
    if(output==NULL)
        printf("File %s open err.\n", filepath);
    fputs(chapter->title, output);
    fputs(chapter->content, output);
    printf("%s\n", filepath);
    fclose(output);
}
