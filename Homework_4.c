#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <time.h>
#include "queue.h"
#define WAITING 0
#define RUNNING 1
#define COMPLETED 2
#define ERROR 3

struct JOB jobs[1000];
queue *job_queue;
int P;

int current_jobs_running_count = 0;

time_t t;

void *execute_main_thread(void *arg){
    while(1){
        if(job_queue->count > 0){
            if(current_jobs_running_count < P){
                pthread_t tid;

                struct JOB* job = queue_delete(job_queue);  
                pthread_create(&tid,NULL,execute_job,job);
            }
        }
    }

    return (NULL);
}

void init_jobs(){
    int job_no = 0;
    while(1){
        printf("Enter Command: ");
        char *line;
        size_t maxlen = 1000;
        int n = getline(&line,&maxlen,stdin);
        if(n > 0){
            if(strstr(line,"submithistory") != NULL){
                show_history(job_no);
                
            }
            else if(strstr(line,"showjobs") != NULL){
                show_jobs(job_no);
                
            }
            else{
                int j = 0;
                char *tok = strtok(line," ");
                strcpy(jobs[job_no].command,"");
                while(tok != NULL){
                    if(strcmp(tok,"submit") != 0){
                        
                        jobs[job_no].commands[j] = tok;
                        strcat(jobs[job_no].command,tok);
                        strcat(jobs[job_no].command," ");
                        j++;
                    }
                    tok = strtok(NULL, " ");
                }
                jobs[job_no].jobid = job_no;
                jobs[job_no].status = WAITING;
                jobs[job_no].commands[j-1] = strtok(jobs[job_no].commands[j-1],"\n");
                jobs[job_no].commands[j] = NULL;
                strcpy(jobs[job_no].command,strtok(jobs[job_no].command,"\n"));

                queue_insert(job_queue,jobs+job_no);
                job_no++;
            }
        }
    }
        
}

int main(int argc, char **argv){
    
    job_queue = queue_init(1000);
    pthread_t mtid;

    if(argc!=2){
        printf("Usage: submit <no_of_jobs> || submithistory || showjobs\n");
        exit(-1);
    }
    P = atoi(argv[1]);
    pthread_create(&mtid,NULL,execute_main_thread,NULL);

    init_jobs();

    return 0;
}
