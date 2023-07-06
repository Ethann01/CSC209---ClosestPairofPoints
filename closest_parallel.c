#include "closest_parallel.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/wait.h>

int curr_depth = 0;

double _closest_parallel(struct Point P[], size_t n, int pdmax, int *pcount)
{
    static int num_forks = 0;
    qsort(P, n, sizeof(struct Point), compare_x);
    if(pdmax == 0 || n <= 3){
        return _closest_serial(P, n);
    }
    int mid = n/2;
    double dl;
    double dr;
    double r1;
    double r2;
    int fd[2];
    if(pipe(fd) == -1){
        fprintf(stderr,"Creating a pipe failed.");
        exit(1);
    }
    int r = fork();
    if(r == -1){
        fprintf(stderr,"Creating a child process failed.");
        exit(1);
    }
    if(r == 0){
        if(close(fd[0]) == -1){
            fprintf(stderr,"Closing file descriptor failed.");
            exit(1);
        }
        dl = _closest_parallel(P, mid, pdmax - 1, pcount);
        if(write(fd[1], &dl, sizeof(dl)) == -1){
            fprintf(stderr,"Writing to pipe failed.");
            exit(1);
        }
        if(close(fd[1]) == -1){
            fprintf(stderr,"Closing file descriptor failed.");
            exit(1);
        }
        curr_depth ++;
        exit(num_forks);
    }
    if(r > 0){
        int fd2[2];
        if(pipe(fd2) == -1){
            fprintf(stderr,"Creating a pipe failed.");
            exit(1);
        }
        int s = fork();
        if(s == -1){
            fprintf(stderr,"Creating a child process failed.");
            exit(1);
        }
        if(s == 0){
            if(close(fd2[0]) == -1){
                fprintf(stderr,"Closing file descriptor failed.");
                exit(1);
            }
            dr = _closest_parallel(&P[n/2], n - mid, pdmax - 1, pcount);
            if(write(fd2[1], &dr, sizeof(dr)) == -1){
                fprintf(stderr,"Writing to pipe failed.");
                exit(1);
            }
            if(close(fd2[1]) == -1){
                fprintf(stderr,"Closing file descriptor failed.");
                exit(1);
            }
            curr_depth ++;
            exit(num_forks);
        }
        if(s > 0){
            num_forks += 2;
            if(close(fd[1]) == -1){
                fprintf(stderr,"Closing file descriptor failed.");
                exit(1);
            }
            if(close(fd2[1]) == -1){
                fprintf(stderr,"Closing file descriptor failed.");
                exit(1);
            }
            int stat;
            if(wait(&stat) == -1){
                fprintf(stderr,"Wait() failed.");
                exit(1);
            }
            int stat2;
            if(wait(&stat2) == -1){
                fprintf(stderr,"Wait() failed.");
                exit(1);
            }
            num_forks += WEXITSTATUS(stat) + WEXITSTATUS(stat2);
            if(read(fd[0], &r1, sizeof(dl)) == -1){
                fprintf(stderr,"Reading from pipe failed.");
                exit(1);
            }
            if(read(fd2[0], &r2, sizeof(dr)) == -1){
                fprintf(stderr,"Writing to pipe failed.");
                exit(1);
            }
            if(close(fd[0]) == -1){
                fprintf(stderr,"Closing file descriptor failed.");
                exit(1);
            }
            if(close(fd2[0]) == -1){
                fprintf(stderr,"Closing file descriptor failed.");
                exit(1);
            }
        }
    }
    *pcount = num_forks;
    if(r1 < r2){
        return combine_lr(P, n, P[mid], r1);
    }
    return combine_lr(P, n, P[mid], r2);
}

double closest_parallel(struct Point P[], size_t n, int pdmax, int *pcount)
{
    qsort(P, n, sizeof(struct Point), compare_x);
    return _closest_parallel(P, n, pdmax, pcount);
}