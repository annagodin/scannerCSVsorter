

#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "sorter.h"
#include <errno.h>
//-c blah -d boo -o haii


int numProcesses=0;


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

//writes CSV to a file
void writeCSV (CSVrecord *frontRec, FILE *sorted){
	CSVrecord *ptr = frontRec;
	//ptr=ptr->next;
	while(ptr!=NULL){
		int i;
		for(i=0;i<ptr->numCols; i++){		
			if(ptr->data[i]==NULL){
				fprintf(sorted,"");
			}else {
				fprintf(sorted,"%s",ptr->data[i]);
			}
			
			if(i<ptr->numCols-1){
				fprintf(sorted,",");
			}
		}
		fprintf(sorted,"\n");
		ptr=ptr->next;
	}

}

//sort function that takes in a file, col to sort, filename, and outputDir
//writes to a new file
void sort(FILE *file, char *colToSort, char* fileName, char *outputDir){
	//if the specified output directory does not have a slash at the end - add one
	if (strcmp(outputDir,"")!=0&&outputDir[strlen(outputDir)-1]!='/'){
		char appendSlash[strlen(outputDir)+2];
		strcpy(appendSlash,outputDir);
		strcat(appendSlash,"/");
		outputDir=malloc((strlen(outputDir)+2)*sizeof(char));
		strcpy(outputDir,appendSlash);
	}
	//printf("outputDir is %s\n",outputDir);


	int sortPos=-1;
	char* str;
	str = (char*)malloc(sizeof(char)*900); //string buffer
	
	char* token;
	token = (char*)malloc(sizeof(char)*200);

	//get headers
	fgets(str, 900, file);


	str=stripNewLineChar(str,strlen(str));

   	char* rest = (char*)malloc(sizeof(char)*800);
   	rest = str;
   	
   	hNode *headersFront = NULL;
   	int count = 0;

   	//tokenizes the headers
   	while ((token = strsep(&rest, ",")) != NULL){
	
        	//loads headers into array
        	char* data = malloc((strlen(token)+1)*sizeof(char));
        
        	hNode *node = malloc(sizeof(hNode));
        	if (token[strlen(token)-1] == '\n'){
        		//printf("stripping newline"); 
				token=stripNewLineChar(token,strlen(token));
		    } 
        	strcpy(data,token);
        	node->data=data;
        	addhNodeToEnd(&headersFront, node);
  	
        	//finds col pos to sort by
        	if(strcmp(token,colToSort)==0){
        		sortPos=count;       	
        	}
        	
        	count++;
       }

       if(sortPos==-1){
       		printf("ERROR: Column specified is not in the CSV that is being processed\n");
       		exit(EXIT_FAILURE);
       }

      



   //sets the number of columns
   int numCols = count;
   




   //pointer to the front of LL
   CSVrecord * frontRec = NULL;
    
	int i=0;
	
	while(fgets(str,900,file)!=NULL){ //EACH ITERATION IS READING ONE LINE	
		CSVrecord *record = malloc(sizeof(CSVrecord));
		record->next=NULL;
		record->data=malloc(numCols*sizeof(char*)); 
		
		count=0;
					
		char* parseStr = (char*)malloc((strlen(str)+1)*sizeof(char));
		parseStr=str;
			//printf("some testing shit\n");
			int index = 0;
			while ((token = strsep(&parseStr, ",")) != NULL) {
				if (token[strlen(token)-1] == '\n'){ 
					token=stripNewLineChar(token,strlen(token));
				} 			
				//QUOTE CASE if theres a quote at the beginning of a token aka theres a COMMA within the field
		    	if(token[0]=='"'){
		    		//first token in quote
		    		char* append = (char*)malloc(strlen(parseStr)*sizeof(char));	
		    		strcpy(append, token);		
	    			if (token[strlen(token)-1] == '\n'){ 
						token=stripNewLineChar(token,strlen(token));
					}  	//end first token in quote			
					
					//following tokens in quote;
					int counting=1;					
		    		do{		    			 				    			
		    			token = strsep(&parseStr, ",");	
		    			if (token==NULL){
		    				break;
		    			} 
		    			append[strlen(append)]=',';		    			
		    			if (token[strlen(token)-1] == '\n'){ 
							token=stripNewLineChar(token,strlen(token));
						}								 
						counting++;   
						strcat(append,token);				    		
		    		} while (searchForQuote(token)==0); 
		   			
		   			token = append;		   				   				   					   					   			
		    	} //END QUOTE CASE
		    	
		    	//empty field
		    	if (strcmp(token,"")==0){		    		
		    		token = NULL;
		    	}		    					
				
				//*****TOKEN LOADED INTO A STRUCT
				if(index==sortPos){
					if (token==NULL){
						record->sortVal=NULL;					
					} else{
						record->sortVal=malloc(strlen(token)*sizeof(char));
						strcpy(record->sortVal,token);
					}
				}
				if(token!=NULL){
					record->data[index] = malloc(strlen(token)*sizeof(char));
					strcpy(record->data[index], token);
				} else {
					record->data[index]=NULL;
				}				
				index++;				
		  	 } //END LINE (RECORD)
			
			record->numCols=numCols;
			//ADD RECORD TO LL HERE		
			addRecToEnd(&frontRec,record);			
			//HERE THE RECORD SHOULD BE COMPLETE
					
		i++;
	} //END FILE
	
	//sorts the damn LL
	mergesort(&frontRec);
	
	//length of the name of the sorted file
	int lengthSorted = strlen(outputDir)+strlen(fileName)+strlen(colToSort)+10;
	char sortedFileName[lengthSorted];
	
	//trim the .csv off the file
	char* trimmedFileName=malloc(strlen(fileName)*sizeof(char));
	strcpy(trimmedFileName,fileName);
	trimmedFileName[strlen(fileName)-4]='\0';

	printf("trimmed file name is %s\n",trimmedFileName);
	char* extension = ".csv";
	
	//if output directory specified, add slash after that dir, if not then no slash
	if(strcmp(outputDir,"")!=0)
		snprintf(sortedFileName, lengthSorted, "%s/%s-sorted-%s%s", outputDir, trimmedFileName, colToSort, extension);
	else
		snprintf(sortedFileName, lengthSorted, "%s-sorted-%s%s", trimmedFileName, colToSort, extension);
	
	printf("sorted file name:\t%s\n",sortedFileName);
	
	//creates new file with the sorted file name
	FILE *sorted;
	sorted=fopen(sortedFileName, "w");


   	int c=0;
   	//prints headers
   	hNode *ptr = headersFront;
   	while (ptr!=NULL){
   		fprintf(sorted,"%s",ptr->data);
   		ptr=ptr->next;
   		if(c<numCols-1){
   			fprintf(sorted,",");
   		}
   		c++;
   	}
   	fprintf(sorted, "\n");	

	writeCSV(frontRec,sorted);


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
	if(argc<3){
		printf("Error, not enough arguments!\n");
		exit(EXIT_FAILURE);
	}

	if(strcmp(argv[1], "-c") == 0){
		colToSort = (char*)malloc(strlen(argv[2])*sizeof(char)+1);
		strcpy(colToSort,argv[2]);
	} else {
		printf("Error, invalid flag!\n");
		exit(EXIT_FAILURE);
	}


	if(argc==5){ //only 2 flags 
		if(strcmp(argv[3], "-d") == 0) {
			hasDir=1;
			hasOut=0;
			searchDir = (char*)malloc(strlen(argv[4])*sizeof(char)+1);
			strcpy(searchDir,argv[4]);
		} else if (strcmp(argv[3], "-o") == 0) {
			hasDir=0;
			hasOut=1;
			outputDir = (char*)malloc(strlen(argv[4])*sizeof(char)+1);
			strcpy(outputDir,argv[4]);
		} else {
			printf("Error: Incorrect parameters\n");
			exit(EXIT_FAILURE);
		}
	} else if (argc==7){ //all flags present
		if(strcmp(argv[3], "-d") == 0) {
			searchDir = (char*)malloc(strlen(argv[4])*sizeof(char)+1);
			strcpy(searchDir,argv[4]);
			hasDir=1;
		} else {
			printf("Error: Incorrect parameters for 2nd argument\n");
			exit(EXIT_FAILURE);
		}

		if (strcmp(argv[5],"-o")==0){
			outputDir = (char*)malloc(strlen(argv[6])*sizeof(char)+1);
			strcpy(outputDir,argv[6]);
			hasOut=1;
		} else {
			printf("Error: Incorrect parameters for 3rd argument\n");
			exit(EXIT_FAILURE);
		}
	} else if (argc>7||argc==4||argc==6){
		if(argc>7)
			printf("Error: Too many parameters\n");
		else
			printf("Error: Incorrent arguments\n");
		exit(EXIT_FAILURE);
	}
	

	printf("colToSort:\t%s\n",colToSort);
	if(hasDir)
		printf("searchDir:\t%s\n",searchDir);
	if(hasOut)
		printf("outputDir:\t%s\n",outputDir);

	//dirwalk("../../../spring18",0);
	
	FILE *file = fopen("small.csv", "r");
	if (file==0){
		printf("ERROR: %s\n", strerror(errno));
		exit(EXIT_FAILURE);
	}
	
	sort(file, colToSort, "small.csv", "testDir");

	// if(hasDir == 1 && hasOut == 0) { //-d 
	// 	dirwalk(searchDir, "", colToSort,0);
	// } else if(hasDir  == 1 && hasOut == 1)	{ //-d and -o
	// 	dirwalk(searchDir, outputDir, colToSort,0);
	// } else if(hasDir  == 0 && hasOut == 1)	{ //-o
	// 	dirwalk("./", outputDir, colToSort,0);
	// } else { //neither 
	// 	dirwalk("./", "", colToSort,0);
	// }


	//dirwalk("../../hw0",0);

	return 0;
} //-----------------------------------ENDMAIN