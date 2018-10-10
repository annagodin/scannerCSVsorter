#ifndef SORTER_H     
#define SORTER_H


typedef struct CSVrecord {
  char* sortVal;
  char** data;
  int numCols;
  struct CSVrecord *next;
} CSVrecord;

typedef struct hNode{
	char* data;
	struct hNode *next;
}hNode;


//in searchDir
void dirwalk(char *dir,char *out, char *colToSort, int depth);
int endsWith (char *str, char *end);
int numProc(char *dir);


//in sorter.c
char* stripNewLineChar (char* token,int tokLen);
char* stripFirstChar (char* token, int tokLen);
char* stripLastChar (char* token);
char* trimWhiteSpace(char* token);
void printAllRecords (CSVrecord * frontRec);
void printRecNode(CSVrecord *rec);
void addRecToEnd(CSVrecord** head, CSVrecord *node);

//in mergesort.c
char* toLowerCase (char* str);
int is_digit(char*str);
struct CSVrecord* SortedMerge(CSVrecord* a, CSVrecord* b);
int compareFields(char* a, char*b);
void FrontBackSplit(CSVrecord* source, CSVrecord** frontRef, CSVrecord** backRef);
void mergesort(CSVrecord** headRef);



#endif
