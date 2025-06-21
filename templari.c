#include <stdio.h>
#include <stdlib.h>

#include <string.h>

#include <sys/stat.h>
#include <sys/types.h>

char *templariPath;

char *getTemplariPath();
int useFuzzyPicker();
int displayHelp();
int getPathState(char *path);
int createTemplate(char *path);

void removeTrailingNewLine(char *input);
void removeTrailingSlash(char *input);


int main(int argc, char *argv[]) {
    templariPath = getTemplariPath();
    if (!strcmp(templariPath, "")) {
        return 1;
    }

    if (argc == 1)
        return useFuzzyPicker();
    if (argc < 2)
        return 1;

    if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "--help"))
        return displayHelp();

    return createTemplate(argv[1]);
}

char *getTemplariPath() {
    char *path = malloc(sizeof(char) * 255);
    sprintf(path, "%s/.config/templari", getenv("HOME"));
    mkdir(path, 0755);

    if (getPathState(path) != 0) {
        printf("Templari path not found at %s\n", path);
        path = "";
    }

    return path;
}

int useFuzzyPicker() {
    FILE *fp;
    char path[1024] = "";
    char fzfCommand[200];

    sprintf(
        fzfCommand,
        "find %s -maxdepth 1 -type d -not -path %s | fzf",
        templariPath,
        templariPath
    );

    fp = popen(fzfCommand, "r");
    if (fp == NULL) {
        printf("Failed to run command\n" );
        return 1;
    }

    while (fgets(path, sizeof(path), fp) != NULL) {
        printf("found %s", path);
    }

    pclose(fp);

    if (!strcmp(path,""))
        return 1;

    char command[1024];
    removeTrailingNewLine(path);
    removeTrailingSlash(path);
    sprintf(command, "cp -r %s/* ./", path);
    system(command);

    return 0;
}

int displayHelp() {
    printf("======= Templari (beta) =======\n");
    printf("- templari: open template picker\n");
    printf("- templari -h: open help\n");
    printf("- templari <path>: create template from path\n");

    return 0;
}

int getPathState(char *path) {
    struct stat st = {0};
    return stat(path, &st);
}

int createTemplate(char *path) {
    removeTrailingSlash(path);

    struct stat st = {0};

    if (stat(path, &st) != 0) {
        printf("Failed to create template from %s\n", path);
        path = "";
        return 1;
    }

    char *templateName = malloc(sizeof(char) * 255);
    printf("Template name: ");

    fgets(templateName, 255, stdin);
    removeTrailingNewLine(templateName);
    removeTrailingSlash(templateName);

    printf("%s\n", templateName);
    printf("%s\n", path);

    char *command = malloc(sizeof(char) * 255);

    sprintf(command, "mkdir %s/\"%s\"", templariPath, templateName);
    system(command);

    if (S_ISDIR(st.st_mode))
        sprintf(command, "cp -r \"%s\"/* %s/\"%s\"/", path, templariPath, templateName);
    else
        sprintf(command, "cp -r \"%s\" %s/\"%s\"/", path, templariPath, templateName);

    printf("running %s\n", command);
    system(command);

    /* remove .git */
    sprintf(command, "rm -rf %s/\"%s\"/.git", templariPath, templateName);



    return 0;
}

void removeTrailingNewLine(char *input) {
    if ((strlen(input) > 0) && (input[strlen (input) - 1] == '\n'))
        input[strlen (input) - 1] = '\0';
}

void removeTrailingSlash(char *input) {
    if ((strlen(input) > 0) && (input[strlen (input) - 1] == '/'))
        input[strlen (input) - 1] = '\0';
}
