#ifndef COMMAND
#define COMMAND

#include "header.h"

int find_command(char* command, char* cmd[]);
void ls(NODE* root, NODE *cwd, char* pathname);
void pwd(NODE *root, NODE *cwd);
char* find_path(NODE *root, NODE *cwd, char *abs_path, NODE *parent, int level);
void mkdir(NODE* root, NODE* cwd, char* pathname);
NODE* find(NODE *current, char *name, char* flag);
void print_structure(NODE* node, int level);
void add_from_path(NODE* root, char* pathname, char* flag);
void add_relative(NODE* cwd, char *pathname, char* flag);
void cd(NODE* root, NODE** cwd, char* pathname);
void creat(NODE* root, NODE* cwd, char* filename);
void rmdir(NODE* root, NODE* cwd, char* pathname);
void rm(NODE* root, NODE* cwd, char* filename);
void remove_node(NODE* node);
void get_node_path(NODE* node, char* abs_path);
void save(NODE* root, FILE* outfile);
void create_node(NODE* node, char* name, char type);
void reload(NODE* root, FILE* infile);
void destroy_filesystem(NODE* root);

#endif