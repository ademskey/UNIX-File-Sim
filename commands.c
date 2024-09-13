#include "header.h"
#include "commands.h"
#include <string.h>

int find_command(char command[], char* cmd[]){
    int i = 0;

    while(cmd[i]){
        if (strcmp(command, cmd[i]) == 0){
            return i;
        }
        i++;
    }
    return -1;
}

void ls(NODE* root ,NODE *cwd, char* pathname){

    NODE *current;

    // If pathname included
    if (strcmp(pathname, "") != 0){
        NODE *found;
        NODE* prev = root;
        current = root;
        char path[256];
        strcpy(path, pathname);
        char* token = strtok(path, "/");

        while (token != NULL){
            if (strcmp(token, "") != 0){
                found = find(current, token, "D");
                if (found && found->parent == prev){
                    current = found;
                }
                else{
                    printf("Error: Invalid path\n");
                    return;
                }
                token = strtok(NULL, "/");
                prev = current;
            }
        }
        if (found){
            current = found -> child;
            while(current){
                printf("%c: %s\n", current -> type, current->name);
                current = current->sibling;
            }
        }
    }
    else{ //no pathname (does local dir)
        current = cwd->child;
        while (current){
            printf("%c: %s\n",current -> type, current->name);
            current = current->sibling;
        }
    }
}

void pwd(NODE *root, NODE *cwd)
{
    // Initialize string to record path
	char abs_path[300] = "";

    // Check conditions for printing
	if (strcmp(cwd -> name, "/") != 0){
        // recursively build tree (will be bottom-up)
		find_path(root, cwd, abs_path, cwd->parent, 0);
		printf("%s\n", abs_path);
	}
	else{
		printf("/");
	}
}

char* find_path(NODE *root, NODE *cwd, char *abs_path, NODE *parent, int level)
{
    // Base case: return when null
    if (strcmp(cwd -> name, "/") == 0){
        return abs_path;
    }
    // Recursively call
    find_path(root, cwd -> parent, abs_path, parent->parent, level + 1);

    // Append the current working directory name to abs_path
    strcat(abs_path, "/");
    strcat(abs_path, cwd->name);

    return abs_path;
}

void mkdir(NODE* root, NODE* cwd, char* pathname){

    // Check if directory already exists
    if (find(root, pathname, "D") != NULL){
        printf("Directory already exists\n");
        return;
    }
    // Check for empty imput
    if (strcmp(pathname, "") == 0){ //no pathname
        printf("Please provide a directory name\n");
    }
    else if (pathname[0] == '/'){ //absolute path
        add_from_path(root, pathname, "D");
    }
    else {//relative path
        add_relative(cwd, pathname, "D");
    }
}

NODE* find(NODE *current, char *name, char* flag) {
    if (!current) {
        return NULL;
    }
    // Check for directory name
    if(strcmp(current->name, name) == 0) {
        return current;
    }
    // Recursively search in the current node's children
    NODE *found = find(current->child, name, flag);
    if (found) {
        return found;
    }
    return find(current->sibling, name, flag);
}

void print_structure(NODE* node, int level) {
    // Base Case (Return when null)
    if (!node) {
        return;
    }
    // Print indentation based on depth 
    for (int i = 0; i < level; i++) {
        printf("  ");
    }
    // Print the current node
    printf("%s (%c)\n", node->name, node->type);

    // Recursively print the rest of nodes
    print_structure(node->child, level + 1);
    print_structure(node->sibling, level);
}

void add_from_path(NODE* root, char* pathname, char* flag) {

    // root will always be present after initialization
    if (strcmp(pathname, root -> name) == 0){
        return;
    }

    // Initialize variables
    char path[256];
    strcpy(path, pathname);
    char* token = strtok(path, "/");
    NODE* current = root;

    // Traverse down from root to the parent of the new directory
    while (token != NULL) {
            if (strcmp(token, "") != 0) // If not at end
            {
                // Find the directory
                NODE* found = find(current, token, flag);
                if (found) {// Directory found, move to child
                    current = found;
                }
                else {
                    // Check if there is more to path
                    // This check detects an invalid path (if not found before end)
                    if (strtok(NULL, "/") != NULL) {
                        printf("Error: Invalid path\n");
                        return;
                    }
                    else {
                        // Directory not found, create it
                        NODE* newDir = (NODE *)malloc(sizeof(NODE));
                        if (!newDir) {
                            printf("Error: Memory allocation failed.\n");
                            return;
                        }
                
                        // Make new node based on flag type
                        if (*flag == 'D') {
                            // Initialize the new directory
                            strcpy(newDir->name, token);
                            newDir->type = 'D';
                            newDir->parent = current;
                            newDir->child = NULL;
                            newDir->sibling = NULL;
                        }
                        if (*flag == 'F') {
                            // Initialize the new file
                            strcpy(newDir->name, token);
                            newDir->type = 'F';
                            newDir->parent = current;
                            newDir->child = NULL;
                            newDir->sibling = NULL;
                        }
                        // Insert new node into the parent's children list
                        if (current->child == NULL) {
                            current->child = newDir;
                        } 
                        else {
                            NODE* sibling = current->child;
                            while (sibling->sibling) {
                                sibling = sibling->sibling;
                            }
                            sibling->sibling = newDir;
                        }
                        // Move to the newly created directory
                        current = newDir;
                        return;
                    }
                }
            }
        token = strtok(NULL, "/"); // Move to the next part of the path
    }
    printf("Error: Directory/File already exists\n");
}

void add_relative(NODE* cwd, char* pathname, char* flag) {
    // Create or find the directory under the current working directory
    add_from_path(cwd, pathname, flag);
}

void cd(NODE* root, NODE** cwd, char* pathname) {
    // If nothing entered go to root
    // Not sure which compare worked but one of them did
    if(pathname == NULL || strcmp(pathname, "") == 0) {
        *cwd = root;
        return;
    }
    // Check for absolute path
    if (pathname[0] == '/') {
        // if cd empty or just "/" go to root
        if (strcmp(pathname, "/") == 0) {
            *cwd = root;
            return;
        }
        // Navigate to last directory
        char *token = strtok(pathname, "/");
        char *last_token = NULL;
        NODE* probe = root;
        NODE* prev = root;

        // Check that path is good
        while (token != NULL) {
            last_token = token; // Update the last_token each time
            token = strtok(NULL, "/");

            // Check to make sure path is correct
            probe = find(probe, last_token, "D");
            if(!probe || probe->parent != prev){
                printf("Error: Path does not exist\n");
                return;
            }

            prev = probe;
        }
        // Find the directory to change to
        NODE* Dir = find(root, last_token, "D");

        if (Dir) {
            if(Dir->type == 'F'){
                printf("Error: Not a directory\n");
                return;
            }
            else{
            *cwd = Dir;
            }
        } 
        else {
            printf("Directory not found\n");
        }
        return;
    }
    // check for .. (back one dir)
    if (strcmp(pathname, "..") == 0) {
        if ((*cwd)->parent) {
            *cwd = (*cwd)->parent;
        }
        return;
    }
    else{
        //  relative path
        NODE *temp = (*cwd)->child;
        while (temp != NULL) {
            if (temp->type == 'D' && strcmp(temp->name, pathname) == 0) {
                *cwd = temp; 
                return;
            }
            temp = temp->sibling;
        }
        printf("Directory not found\n");
        return;
    }
}

void creat(NODE* root, NODE* cwd, char* filename) {
    // Check if file already exists
    if (find(root, filename, "F") != NULL) {
        printf("File already exists\n");
        return;
    }
    // Check for empty input
    if (strcmp(filename, "") == 0) {
        printf("Please provide a file name\n");
    } 
    if(filename[0] == '/') { //absolute path
        add_from_path(root, filename, "F");
    }
    else {  //relative path
        // Create the file under the current working directory
        add_relative(cwd, filename, "F");
    }
}

void rmdir(NODE* root, NODE* cwd, char* pathname) {
    // Check for empty input
    if (strcmp(pathname, "") == 0) {
        printf("Please provide a directory name\n");
    } 
    else {
        // Check for absolute path
        if (pathname[0] == '/') {

            // Grab the last directory name at the end of the path
            char *token = strtok(pathname, "/");
            char *last_token = NULL;
            NODE* probe = root;
            NODE* prev = root;

            // Check that path is good
            while (token != NULL) {
                last_token = token; // Update the last_token each time
                token = strtok(NULL, "/");

                // Check to make sure path is correct
                probe = find(probe, last_token, "D");
                if(!probe || probe->parent != prev){
                    printf("Error: Path does not exist\n");
                    return;
                }
                prev = probe;
            }
            // Find directory to remove
            NODE* dir = find(root, last_token, "D");
            if (dir) {
                // check if file
                if(dir->type =='F'){
                    printf("Error: Not a directory\n");
                    return;
                }
                // Check if is empty
                if (dir->child == NULL) {
                    remove_node(dir);
                } 
                else {
                    printf("Directory is not empty\n");
                }
            } 
            else {
                printf("Directory not found\n");
            }
        } 
        else {
            // Find the directory to remove
            NODE* dir = find(cwd, pathname, "D");

            // Make sure it is in the directory
            if (dir){ //If found
                if (dir  -> parent == cwd) { //if in relative path
                    if (dir->type == 'F') { //check for file
                        printf("Error: Not a directory\n");
                        return;
                    }
                    // Check if the directory is empty
                    if (dir->child == NULL) {
                        // Remove the directory
                        remove_node(dir);
                    } else {
                        printf("Directory is not empty\n");
                    }
                } else {
                    printf("Directory not found\n");
                }
            }
            else{
                printf("Can't find directory\n");
            }
        }
    }
}

void rm(NODE* root, NODE* cwd, char* filename){
    if (strcmp(filename, "") == 0) {
        printf("Please provide a file name\n");
    } 
    else {
        // Check for absolute path
        if (filename[0] == '/') {

            // Grab the last directory name at the end of the path
            char *token = strtok(filename, "/");
            char *last_token = token;
            NODE* probe = root;
            NODE* prev = root;

            while (token != NULL) {
                // Check to make sure path is correct
                probe = find(probe, token, "F");

                if(!probe || probe->parent != prev){
                    printf("Error: Invalid path\n");
                    return;
                }
                prev = probe;
                last_token = token; // Update the last_token each time
                token = strtok(NULL, "/");
            }

            // Find the file to remove
            NODE* file = find(root, last_token, "F");
            if(file->type =='D'){
                    printf("Error: Not a file\n");
                    return;
                }
            if (file) {
                remove_node(file);
            } 
            else {
                printf("File not found\n");
            }
        } 
        else { // Relative path
            // Find the file to remove
            NODE* file = find(cwd, filename, "F");
            if (file -> parent == cwd){
                if (file) {
                    if (file->type == 'D') {
                        printf("Error: Not a file\n");
                        return;
                    }
                    remove_node(file);
                } 
                else {
                    printf("File not found\n");
                }
            }
        }
    }
}

void remove_node(NODE* node) {
    // Remove the node from the parent's list
    NODE* parent = node->parent;
    NODE* current = parent->child;

    // Check if node is first child
    if (current == node) {
        parent->child = current->sibling;
    } 
    else {
        while (current->sibling != node) {
            current = current->sibling;
        }
        current->sibling = node->sibling;
    }
}

void get_node_path(NODE* node, char* abs_path) {
    if (node->parent != NULL && strcmp(node->name, "/") != 0) {
        get_node_path(node->parent, abs_path);

        // Append a slash if the parent is not the root
        if (strcmp(node->parent->name, "/") != 0) {
            strcat(abs_path, "/");
        }
    } 
    strcat(abs_path, node->name); // Append the current node's name to the path
}

void save(NODE* root, FILE* outfile) {
    if (!root) {
        return; // Base case
    }
    char abs_path[300] = "";
    get_node_path(root, abs_path);  // Construct the full path 
    fprintf(outfile, "%c %s\n", root->type, abs_path); //write full path to file

    // Recursive calls to children then siblings
    save(root->child, outfile);
    save(root->sibling, outfile);
}

void create_node(NODE* node, char* name, char type) {
    // Copy node and make blank initializations
    strcpy(node->name, name);
    node->type = type;
    node->child = NULL;
    node->sibling = NULL;
    node->parent = NULL;
}

void reload(NODE* root, FILE* infile) {
    char type;
    char path[500];
    // Loop through file and add nodes
    while (fscanf(infile, "%c %s\n", &type, path) != EOF) {
        add_from_path(root, path, &type);
    }
}

void destroy_filesystem(NODE* root) {
    if (!root) {
        return; 
    }
    // recursive calls
    destroy_filesystem(root->child);
    destroy_filesystem(root->sibling);
    free(root);
}