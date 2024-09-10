#include "header.h"
#include "commands.h"

NODE *root; 
NODE *cwd;

// Global Commands
char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm", "reload","save", "quit", 0 };


int initialize() {
	root = (NODE *)malloc(sizeof(NODE));
	strcpy(root->name, "/");
	root->parent = root;
	root->sibling = 0;
	root->child = 0;
	root->type = 'D';
	cwd = root;
	// other steps as needed
	
	printf("Filesystem initialized!\n");
}

int main() {
	initialize();
	// other initialization as needed

	while(1) {
		// Get user command
		printf("\nEnter Command:\n");
		char command[100], line[100], path[100];
		for (int i = 0; i < 100; i++)
			line[i] = '\0';
		fgets(line, 100, stdin);

		// Find command
		switch(find_command(command)) {
			case 0: //mkdir
				mkdir(path);
				break;
			case 1: //rmdir
				break;
			case 2: //ls
				break;
			case 3: //cd
				break;
			case 4: //pwd
				break;
			case 5: //creat
				break;
			case 6: //rm
				break;
			case 7: //reload
				break;
			case 8: //save
				break;
			case 9: //quit
				break;
			default:
				printf("\nCommand Not Found!\n");
				break;
		}
	}
}

