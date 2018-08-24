#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <string.h>
#include <unistd.h>

#define LINE_SIZE 4*1024
#define TITLE_SIZE 4*128
#define CONTENT_SIZE 4*48*1024

typedef struct chapter{
    char *title;
    char *content;
}chapter;

void chapter_to_file(chapter*, const char*);
char* trime(char*);
void useage();

int main(int argc, char* argv[]){

    char *file=malloc(sizeof(char)*64); // input file, for example test.txt
    char *output_dir=malloc(sizeof(char)*64); // output directory, for example ./results/
    char *title_pattern=malloc(sizeof(char)*TITLE_SIZE); // chapter title pattern
    char *line=malloc(sizeof(char)*LINE_SIZE);

    strcpy(title_pattern, "^ *第[零一二三四五六七八九十百千0123456789]*章 .*\n$");
    strcpy(output_dir, "./results/");

    const size_t nmatch = 1;
    regmatch_t pmatch[1];
    int status;

    int opt=getopt(argc, argv, "i:o:r:");
    while(opt!=-1){
        switch(opt){
            case 'i':
                strcpy(file, optarg);
                break;
            case 'o':
                memset(output_dir, 0, 64*sizeof(char));
                strcpy(output_dir, optarg);
                output_dir[strlen(output_dir)-1]=='/'?output_dir:strcat(output_dir,"/");
                break;
            case 'r':
                memset(title_pattern, 0, 64*sizeof(char));
                strcpy(title_pattern, optarg);
                break;
        }
        opt=getopt(argc, argv, "i:o:r:");
    }

    if(!strcmp(file, "")){
        useage();
        exit(0);
    }

    FILE* input_file=fopen(file, "r");

    if(input_file == NULL){
        printf("Failed to open %s\n", file);
        exit(0);
    }

    if(access(output_dir, 4)!=0){
        printf("%s doesn't exists or permission denied!\n", output_dir);
        exit(0);
    }

    regex_t reg;
    regcomp(&reg, title_pattern, REG_EXTENDED);

    chapter* chapter=malloc(sizeof(chapter));
    chapter->title=malloc(sizeof(char)*TITLE_SIZE);
    chapter->content=malloc(sizeof(char)*CONTENT_SIZE);

    strcpy(chapter->title, "start\n");
    strcpy(chapter->content, strcat(file, "\n"));
    while(!feof(input_file)){
        status = regexec(&reg, line, nmatch, pmatch, 0);
        switch(status){
            case 0:
                chapter_to_file(chapter, output_dir);
                memset(chapter->title, 0, TITLE_SIZE);
                memset(chapter->content, 0, CONTENT_SIZE);
                trime(strcat(chapter->title, line));
                break;
            case REG_NOMATCH:
                if(CONTENT_SIZE-strlen(chapter->content)*4<=strlen(line)*4){
                    printf("ERR at %s\n", chapter->title);
                    printf("The size is too large for just one chapter, maybe your title pattern is incorrect. Please check it!\n");
                    printf("    For Chinese novel, you can use regular expression: \"^ *第[零一二三四五六七八九十百千0123456789]*章 .*\\n$\"\n");
                    printf("    For English novel, you can use regular expression: \"^ *[cC][hH][aA][pP][tT][eE][rR] *[0123456789]*.* \\n$\"\n");
                    exit(0);
                }
                strcat(chapter->content, line);
                break;
            default:
                printf("Match ERR\n");
                break;
        }
        memset(line, 0, LINE_SIZE);
        fgets(line, LINE_SIZE, input_file);
    }
    chapter_to_file(chapter, output_dir);
    regfree(&reg);
    fclose(input_file);

    free(file);
    free(output_dir);
    free(title_pattern);
    free(line);
    return 0;
}

void chapter_to_file(chapter* chapter, const char* output_dir){
    char* filepath =malloc(sizeof(char)*LINE_SIZE);
    strcpy(filepath, output_dir);
    trime(strcat(filepath, chapter->title));
    strcat(filepath, ".txt");
    FILE* output = fopen(filepath, "w");
    if(output==NULL){
        printf("File %s open err.\n", filepath);
        exit(0);
    }
    fputs(chapter->title, output);
    fputs("\n", output);
    fputs(chapter->content, output);
    fclose(output);
}

char* trime(char* str){
    char *p=NULL;
    p=str;
    while(p[0]==' ' || p[0]=='\t' || p[0]=='\r' || p[0]=='\n')
        p++;
    while(p[strlen(p)-1]==' ' || p[strlen(p)-1]=='\t'  || p[strlen(p)-1]=='\r' || p[strlen(p)-1]=='\n')
        p[strlen(p)-1]='\0';
    memcpy(str, p, strlen(str)); // make sure str start position doesn't change
    return str;
}

void useage(){
    printf("chapter-split: used to split novel to chapters.\n");
    printf("  args:\n");
    printf("  -i: specify input file\n");
    printf("  -o: specify output directory\n");
    printf("      default ./results/, but it should exists before execute\n");
    printf("  -r: specify title regular expression\n");
    printf("      default \"^ *第[零一二三四五六七八九十百千0123456789]*章 .*\\n$\"\n");
    printf("      English novel \"^ *[cC][hH][aA][pP][tT][eE][rR] *[0123456789]*.* \\n$\"\n");
}
