
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>
#include <sys/stat.h>
#include <stdlib.h>
#include "sorter.h"
#include <errno.h>










// //recursively travserses a directory and prints subdirectories
// void dirwalk(char *dir,char *out, char *colToSort){
//     printf("hai\n");
//     DIR *dp;
//     struct dirent *entry;
//     struct stat statbuf;
//     int status1, status2;
//     int pid1, pid2;

//     if((dp = opendir(dir)) == NULL) {
//         fprintf(stderr,"Error: cannot open directory: %s\n",dir);
//         exit(EXIT_FAILURE);
//     }

//     chdir(dir);
//     printf("ha2\n");
//     while((entry = readdir(dp)) != NULL){
//         lstat(entry->d_name,&statbuf);
//         if(S_ISDIR(statbuf.st_mode)) { //ITS A DIRECTORY
//             /* Found a directory , but ignore . and .. */
//             if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0)
//                 continue;
//             printf("Found directory, [%s]\n",entry->d_name);
//             //printf("%*s %s/\n",depth,"",entry->d_name); //FORK HERE AND RECURSE
//             pid1=fork();
//             if(pid1==0){ //child
//                 //recurse here
//                 /*funtion is called recursively at a new indent level */
//                 dirwalk(entry->d_name,out,colToSort);
//                 exit(0);
//             } else { //parent
//                 printf("PID1:\t\t\t%d\n",pid1);
//                 waitpid(pid1, &status1, WUNTRACED);
//                 //np += WEXITSTATUS(status1);
//                 // if(retpid>0){
//                 //  numP++;
//                 //  np++;
//                 // }
//             }

//            printf("hai3\n");
            
//         }
//         else if(S_ISREG(statbuf.st_mode)){ //ITS A FILE, FORK TO SORT FILE
//             if (endsWith (entry->d_name, ".csv")){
//                 printf("found a csv, [%s] in dir [%s]:\t%d\n",entry->d_name, dir);
//                // printf("%*s %s \n",depth,"",entry->d_name); 
//                 pid2=fork();
//                 if(pid2==0){ //child
                    
//                     FILE *file = fopen(entry->d_name, "r");
//                     if (file==0){
//                         printf("ERROR: %s\n", strerror(errno));
//                         exit(EXIT_FAILURE);
//                     }
//                     sort(file, colToSort, entry->d_name, out);
//                     exit(0);
//                 } else { //parent
//                     printf("PID2:\t\t\t%d\n",pid2);
//                      waitpid(pid2, &status2, WUNTRACED);
//                     // WEXITSTATUS(status2);
//                     // if(retpid>0){
//                     //  numP++;
//                     //  np++;
//                     // }
//                  }
            
//             }

//         }

//         chdir("..");
//         closedir(dp);
//     }
// }


// //checks if a file ends with a certain string
// // (in our case we need to check if it ends with a .csv
// int endsWith (char *str, char *end) {
//     size_t slen = strlen (str);
//     size_t elen = strlen (end);
//     if (slen < elen)
//         return 0;
//     return (strcmp (&(str[slen-elen]), end) == 0);
// }

// //counts the number of processes that this directory search will require
// int numProc(char *dir){
//     int numP=0;
//     DIR *dp;
//     char str[80]; 
//     char name[80]; 
//     struct dirent *entry;
//     struct stat statbuf;

//     if((dp = opendir(dir)) == NULL) {
//         fprintf(stderr,"Error: cannot open directory: %s\n",dir);
//         exit(EXIT_FAILURE);
//     }

//     chdir(dir);

//     while((entry = readdir(dp)) != NULL){
//         lstat(entry->d_name,&statbuf);
//         if(S_ISDIR(statbuf.st_mode)) { //ITS A DIRECTORY
//             /* Found a directory , but ignore . and .. */
//             if(strcmp(".",entry->d_name) == 0 || strcmp("..",entry->d_name) == 0 || strcmp(".git",entry->d_name) == 0)
//                 continue;
//             numP++;
//             //printf("nump bc found directory, [%s]:\t%d\n",entry->d_name,numP);
//             /*funtion is called recursively at a new indent level */
//             numP += numProc(entry->d_name);
//             //printf("nump after recursing %s:\t%d\n",entry->d_name,numP);
//         }
//         else if(S_ISREG(statbuf.st_mode)){ //ITS A FILE, FORK TO SORT FILE
//             if (endsWith (entry->d_name, ".csv")){
//                 numP++;
//                 //printf("found a csv, [%s] in dir [%s] nump:\t%d\n",entry->d_name, dir,numP);
//             }  
//             strcpy(str,entry->d_name);
//             strcpy(name,str); 
//         }
//     }

//         chdir("..");
//         closedir(dp);
//         return numP;
// }


