

#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "sorter.h"
//-c blah -d boo -o haii





void freeLL(CSVrecord *frontRec){
	CSVrecord *curr = NULL;
	while ((curr=frontRec)!=NULL){
		frontRec=frontRec->next;
		free(curr);
	}
}

//converts a string to an integer
int stringToInt (char* str){
	
	int dec = 0, i, len;
	len = strlen(str);
	for(i=0; i<len; i++){
		dec = dec * 10 + ( str[i] - '0' );
	}
	return dec;
}

//strips new line character off a string
char* stripNewLineChar (char* token,int tokLen){
	token[tokLen-1]='\0';
	return token;
	/*char* replace = (char*)malloc((tokLen-1)*sizeof(char));
	int i;
	for (i=0; i<tokLen-1; i++){
		replace[i]=token[i];
	}

	return replace;*/
}

//strips last character
char* stripLastChar (char* token){
	char* replace = (char*)malloc((strlen(token))*sizeof(char));
	int i;	
	for (i=0; i<(strlen(token)-1); i++){
		replace[i]=token[i];
	}	
	return replace;
	
}

//strips the first character off a string
char* stripFirstChar (char* token, int tokLen){
	char* replace = (char*)malloc((tokLen-1)*sizeof(char));
	int i;
	int j=0;
	for (i=1; i<tokLen; i++){
		replace[j]=token[i];
		j++;
	}
	
	return replace;
}

//searches string for a quote
int searchForQuote (char* token){
	int len = strlen(token);
	int i;
	for (i=0; i<len; i++){
		if (token[i]=='"'){
			return 1;
		}
	}
	return 0;
}

//trims white spaces
char* trimWhiteSpace(char* token){
	int index, i;
	index=0;
	i=0;
	char* trimmed = malloc(sizeof(char)*strlen(token));
	//trim leading
	while (token[index] == ' '){
		index++;
	}
	for(i=0;i<strlen(token)-index;i++){
		trimmed[i]=token[index+i];
	}	
	//trim trailing
	index=strlen(trimmed)-1;
	while(trimmed[index] == ' '){
		index--;
	}	
	trimmed[index+1]='\0';	
    return trimmed;
}

//adds record node to end;
void addRecToEnd(CSVrecord** head, CSVrecord *node){
	CSVrecord *last = *head;
	
    if (*head == NULL){
       *head = node;
       return;
    }
    while (last->next != NULL)
        last = last->next;
 
    last->next = node;
    return;
}

void addhNodeToEnd(hNode** head, hNode *node){
	hNode *last = *head;
	
    if (*head == NULL){
       *head = node;
       return;
    }
    while (last->next != NULL)
        last = last->next;
 
    last->next = node;
    return;
}

//prints one Record Node
void printRecNode(CSVrecord *rec){
	printf("Record contents:\n");
	int i;
	printf("sortVal:\t%s\n",rec->sortVal);
	printf("numCols:\t%d\n",rec->numCols);
	printf("data values:\t");
	for(i=0;i<rec->numCols; i++){
			printf("'%s'\t",rec->data[i]);
	}
	printf("\n");
}

//prints entire linked list of recnodes
void printAllRecords(CSVrecord *frontRec){
	CSVrecord *ptr = frontRec;
	while (ptr!=NULL){
		printRecNode(ptr);
		printf("\n\n");
		ptr=ptr->next;
	}
	printf("\n");
}

//prints in csv format
void printCSV (CSVrecord *frontRec){
	CSVrecord *ptr = frontRec;
	//ptr=ptr->next;
	while(ptr!=NULL){
		int i;
		for(i=0;i<ptr->numCols; i++){		
			if(ptr->data[i]==NULL){
				printf("");
			}else {
				printf("%s",ptr->data[i]);
			}
			
			if(i<ptr->numCols-1){
				printf(",");
			}
		}
		printf("\n");
		ptr=ptr->next;
	}

}







int main(int argc, char *argv[] ){ //-----------------------MAIN---------
	
	int hasDir;
	int hasOut;
	char* searchDir;
	char* outputDir;
	char* colToSort;
	// argv[1] //"-c"
	// argv[2] //-c val
	// argv[3] //"-d"
	// argv[4] //-d val
	// argv[5] //"-o"
	// argv[6] //-o val

	//if no -d, only search current directory
	//if no -o, output to current directory
	if(argv[1]==NULL){
		printf("error, there is no column specified!\n");
		return 1;
	}

	
	colToSort = (char*)malloc(strlen(argv[2])*sizeof(char)+1);
	strcpy(colToSort,argv[2]);


	if(argc!=7){ //not all flags are given
		//check for -d and -o
		if(argv[3][1]=='d'){
			hasDir=1;
			hasOut=0;
			searchDir = (char*)malloc(strlen(argv[4])*sizeof(char)+1);
			strcpy(searchDir,argv[4]);
		} else if (argv[3][1]=='o'){
			hasOut=1;
			hasDir=0;
			outputDir = (char*)malloc(strlen(argv[4])*sizeof(char)+1);
			strcpy(outputDir,argv[4]);
		} else {
			hasOut=0;
			hasDir=0;
		}

	} else { //all flags present
		searchDir = (char*)malloc(strlen(argv[4])*sizeof(char)+1);
		strcpy(searchDir,argv[4]);
		hasDir=1;

		outputDir = (char*)malloc(strlen(argv[6])*sizeof(char)+1);
		strcpy(outputDir,argv[6]);
		hasOut=1;
	}

	

	printf("colToSort:\t%s\n",colToSort);
	if(hasDir)
		printf("searchDir:\t%s\n",searchDir);
	if(hasOut)
		printf("outputDir:\t%s\n",outputDir);

	//dirwalk("../../../spring18",0);

	return 0;
}