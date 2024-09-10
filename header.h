#include <stdio.h>            
#include <stdlib.h>      
#include <string.h>


typedef struct node {
	char  name[64];       // node's name string
	char  type;
	struct node *child, *sibling, *parent;
	// other fields if needed
} NODE;