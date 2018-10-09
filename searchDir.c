
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "sorter.h"

// int main(int argc, char *argv[] )
// {
//     printf("Directory scan\n");
//     dirwalk("../hw0/agtm",0);
//     //printf("done.\n");
//     // if (endsWith ("data.csv", ".csv")){
//     //     printf("success!!");
//     // }

//     return(0);
// }

//recursively travserses a directory and prints subdirectories
void dirwalk(char *dir,char *out, char *colToSort, int depth){
    DIR *dp;
    char str[80]; 
    char name[80]; 
    struct dirent *entry;
    struct stat statbuf;

    if((dp = opendir(dir)) == NULL) {
        fprintf(stderr,"Error: cannot open directory: %s\n",dir);
        exit(EXIT_FAILURE);
    }

    chdir(dir);

    while((entry = readdir(dp)) != NULL){
        lstat(entry->d_name,&statbuf);
        if(S_ISDIR(statbuf.st_mode)) { //ITS A DIRECTORY
            /* Found a directory , but ignore . and .. */
            if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
                continue;

            printf("%*s %s/\n",depth,"",entry->d_name); //FORK HERE AND RECURSE
            /*funtion is called recursively at a new indent level */
            dirwalk(entry->d_name,out,colToSort,depth + 4);
        }
        else if(S_ISREG(statbuf.st_mode)){ //ITS A FILE, FORK TO SORT FILE
            //if (endsWith (entry->d_name, ".csv"))
                printf("%*s %s \n",depth,"",entry->d_name);   
            strcpy(str,entry->d_name);
            strcpy(name,str); 
        }

        //printf("\nFile name is %s\n",name);
    }

        chdir("..");
        closedir(dp);
}


//checks if a file ends with a certain string
// (in our case we need to check if it ends with a .csv
int endsWith (char *str, char *end) {
    size_t slen = strlen (str);
    size_t elen = strlen (end);
    if (slen < elen)
        return 0;
    return (strcmp (&(str[slen-elen]), end) == 0);
}


//FORK THE PROCESS!!!!!
/*

METHOD FOR FORKING

*/

