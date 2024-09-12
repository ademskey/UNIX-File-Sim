#include "header.h"
#include "commands.h"

// Global Variables
NODE *root; 
NODE *cwd;

// Global Commands list
char *cmd[] = {"mkdir", "rmdir", "ls", "cd", "pwd", "creat", "rm", "reload","save", "quit", 0 };

int initialize() 
{
	root = (NODE *)malloc(sizeof(NODE));

	if (!root) {
        // Check if malloc failed
        printf("Memory allocation failed!\n");
        return 1; // Non-zero return value for error
    }

	strcpy(root->name, "/");
	root->parent = root;
	root->sibling = 0;
	root->child = 0;
	root->type = 'D';
	cwd = root;
	printf("Filesystem initialized!\n");
	return 0;
}

int main() {
	// Initialize variables and filestystem
	int init, cmd_idx;
	char usr_input[130], command[30], argument[100], lastname[30],prefix[100] = "fssim_";;
	init = initialize();

	if(init == 0){
		while(cmd_idx != 9) {
			// Clear the argument string
			argument[0] = '\0';

			printf("\nEnter Command:");

			// Get user input
			if (fgets(usr_input, sizeof(usr_input), stdin) != NULL){
				// Remove the newline character from the input
				usr_input[strcspn(usr_input, "\n")] = 0;
				// Split the input into the command and argument using sscanf
				sscanf(usr_input, "%s %49[^\n]", command, argument);
			}

			cmd_idx = find_command(command, cmd);

			switch(cmd_idx) {
				case 0:
					//mkdir
					mkdir(root, cwd, argument);
					break;
				case 1:
					//rmdir
					rmdir(root, cwd, argument);
					break;
				case 2:
					//ls
					ls(root,cwd, argument);
					break;
				case 3:
					//cd
					cd(root, &cwd, argument);
					break;
				case 4:
					//pwd
					pwd(root, cwd);
					break;
				case 5:
					//create
					creat(root, cwd, argument);
					break;
				case 6:
					//remove
					rm(root, cwd, argument);
					break;
				case 7:
					//reload
					if (strcmp(argument, "") == 0) {
						printf("Please provide a file name\n");
						break;
					}
					FILE* infile = fopen(argument, "r");
					reload(root, infile);
					fclose(infile);
					break;
			case 8: 
				//save
				printf("Enter File Name: ");
				
				if (fgets(argument, sizeof(argument), stdin) != NULL) {
					argument[strcspn(argument, "\n")] = 0; // Remove newline character (caused loop issue)
					printf("Saving to %s\n", argument);
					FILE *outfile = fopen(argument, "w");
					if (outfile) {
						save(root, outfile);
					} 
					fclose(outfile);
				}
				else {
					printf("Failed to open file %s for writing\n", argument);
				}
				break;
			case 9:
				//quit
				printf("Enter Last Name: ");

				if (fgets(lastname, sizeof(lastname), stdin) != NULL) 
				{
					lastname[strcspn(lastname, "\n")] = 0; // remove newline character 

					// Format filename
					strcat(prefix, lastname);
					strcat(prefix, ".txt");
					printf("Saving to %s\n", prefix);
					printf("Exiting Simulation\n");

					FILE *quitfile = fopen(prefix, "w");
					if (quitfile) {
						save(root, quitfile);
						fclose(quitfile);
						destroy_filesystem(root);
					} 
					else {
						printf("Failed to open file %s for writing\n", prefix);
					}
				} 
				else {
					printf("Failed to read last name\n");
				}
				
				break;

				default:
					printf("\nCommand Not Found!\n");
					break;
				}
			}
			printf("See you later!\n");
	}
	else {
		printf("Error initializing filesystem!\n");
		return 1;
	}
	return 0;
}

