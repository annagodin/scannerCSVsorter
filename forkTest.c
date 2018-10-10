#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "sorter.h"
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>
int numProcesses;
int numP=0;

void forkwalk(char *dir);
void recSearch(int x);


int endsWith (char *str, char *end) {
    size_t slen = strlen (str);
    size_t elen = strlen (end);
    if (slen < elen)
        return 0;
    return (strcmp (&(str[slen-elen]), end) == 0);
}


int main(int argc, char *argv[] ){ 
	
	numProcesses=1;

	int init = getpid();
	printf("init is\t\t\t%d\n",init);
	//int x = 5;
	//recSearch(x);

	//printf("numProcesses:\t%d\n",numProcesses);
	forkwalk("./");
	printf("NUM PROCESSES BITCH:\t%d\n",numP);
}


void recSearch(int x){
	printf("begin recsearch\n");
	int status1;
	int status2;

	int pid1 = 0; 
	int pid2 = 0;

	while (x>0){
		printf("x is %d\n",x);
		printf("numProcesses currently:\t%d\n",numProcesses);
		if(x%2==0){ //file
			numProcesses++;
			printf("numprocs bc %d%%2==0:\t%d\n",x,numProcesses);
			int pid1 = fork();
			
			if(pid1==0){
				printf("FILE CHILD\n"); //child
				printf("sorting\n");
				printf("ok finished SORT, returning\n");
				exit(0);
			} 
			else {
				printf("FILE PARENT\n");//not child
				printf("PID1:\t\t\t%d\n",pid1);
				int retpid = waitpid(pid1, &status1, WUNTRACED);
				if(retpid>0){
					numProcesses++;
				}
			}
		} else { //dir
			numProcesses++;
			printf("numprocs bc %d%%2!=0:\t%d\n",x,numProcesses);
			int pid2 = fork();
			
			if(pid2==0){
				printf("DIR CHILD\n"); //child
				x=x-2;
				recSearch(x);
				printf("returning the DIRchild\n");
				exit(0);
			} 
			else {
				printf("DIR PARENT\n");//parent
				printf("PID2:\t\t\t%d\n",pid2);
				int retpid = waitpid(pid2, &status2, WUNTRACED);
				if(retpid>0){
					numProcesses++;
				}
			}
		}
		x--;
	}

	// if (pid1 != 0) {	
	// 	wait(0); 
	// }
	
	// if (pid2 != 0) {
	// 	while(counter > 0) { 
	// 		wait(0); 
	// 		counter--;
	// 	}
	// }
	return;
}


void forkwalk(char *dir){
    
    DIR *dp;
    struct dirent *entry;
    struct stat statbuf;
    int status1, status2;
    int pid1, pid2;
    if((dp = opendir(dir)) == NULL) {
        fprintf(stderr,"Error: cannot open directory: %s\n",dir);
        exit(EXIT_FAILURE);
    }

    chdir(dir);

    while((entry = readdir(dp)) != NULL){
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) { //ITS A DIRECTORY
            
            //Found a directory , but ignore . and .. 
            if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0 || strcmp(".git",entry->d_name) == 0)
                continue;

            numP++;
            printf("numP bc found directory, [%s]:\t%d\n",entry->d_name,numP);
            
            pid1=fork();
            if(pid1==0){ //child
            	//recurse here
            	 forkwalk(entry->d_name);
            	 exit(0);
            } else { //parent
            	printf("PID1:\t\t\t%d\n",pid1);
            	int retpid = waitpid(pid1, &status1, WUNTRACED);
				if(retpid>0){
					numP++;
				}
            }
           
            //printf("nump after recursing %s:\t%d\n",entry->d_name,numP);
        }
        else if(S_ISREG(statbuf.st_mode)){ //ITS A FILE, FORK TO SORT FILE
            if (endsWith (entry->d_name, ".csv")){ 
                numP++;
                printf("found a csv, [%s] in dir [%s] numP:\t%d\n",entry->d_name, dir,numP);
                pid2=fork();
                if(pid2==0){ //child
                	printf("will sort here\n");
                	exit(0);
                } else { //parent
                	printf("PID2:\t\t\t%d\n",pid2);
                	int retpid = waitpid(pid2, &status2, WUNTRACED);
					if(retpid>0){
						numP++;
					}
                }
                
            }  
        }
    }

        chdir("..");
        closedir(dp);
        
}


