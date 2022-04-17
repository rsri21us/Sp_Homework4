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

void *execute_job(void *arg){
    int fdout, fderr;

    current_jobs_running_count++;


    struct JOB* job = (struct JOB*) arg;

    char file_name[4096];
    sprintf(file_name,"%d.out",job->jobid);
    if ((fdout = open(file_name, O_CREAT | O_WRONLY, 0755)) == -1) { 
        printf("Error opening file for output\n");
        exit(-1); 
    } 

    char err_file_name[4096];
    sprintf(err_file_name,"%d.err",job->jobid);
    if ((fderr = open(err_file_name, O_CREAT | O_WRONLY, 0755)) == -1) { 
        printf("Error opening file for error\n");
        exit(-1); 
    } 
    // printf("execute_job");

    time(&t);
    strcpy(job->job_start_time,strtok(ctime(&t),"\n"));
    // printf("start time:%s",job->job_start_time);
    // job->job_start_time = ;
    
    // job->job_start_time = gettime();
    

    job->status = RUNNING;
    pid_t pid = fork();
    
    if(pid == 0){

        dup2(fdout, STDOUT_FILENO);
        dup2(fderr,STDERR_FILENO);
        
        execvp(job->commands[0],job->commands);
        perror("exec");
	    exit(-1);

    }else if(pid > 0){

        int status;
        wait(&status);
        job->status = COMPLETED;
        time(&t);
        strcpy(job->job_end_time, strtok(ctime(&t),"\n"));
        if(WIFEXITED(status))
        {
            if(WEXITSTATUS(status) == 0)
            {
                job->status = COMPLETED;
            }
            else
            {
                job->status = ERROR;
            }
        }
        else
        {
            job->status = ERROR;
        }

    }else{
        perror("fork");
        exit(EXIT_FAILURE);
    }
    current_jobs_running_count--;


    return (NULL);
}


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


void show_history(int n){
    int i;
    int max_command_len = 0;
    int max_starttime_len = 0;
    int max_endtime_len = 0;
    char job_status[8];

    char jobid_str[100];
    for(i=0;i<n;i++){
        if((jobs[i].status == COMPLETED) || (jobs[i].status == ERROR)){
            if(max_command_len < strlen(jobs[i].command))
                max_command_len = strlen(jobs[i].command);
            if(max_starttime_len < strlen(jobs[i].job_start_time))
                max_starttime_len = strlen(jobs[i].job_start_time);
            // printf("%s",jobs[i].job_end_time);
            if(max_endtime_len < strlen(jobs[i].job_end_time))
                max_endtime_len = strlen(jobs[i].job_end_time);
        }
    }
    printf("jobid\t%*s\t%*s\t%*s\tstatus\n",-max_command_len,"command",-max_starttime_len,"starttime",-max_endtime_len,"endtime");
    for(i=0;i<n;i++){
        if((jobs[i].status == COMPLETED) || (jobs[i].status == ERROR)){
            // printf("%s %s",jobs[i].job_start_time,jobs[i].job_end_time);
            sprintf(jobid_str,"%d",jobs[i].jobid);
            if(jobs[i].status == COMPLETED){
                strcpy(job_status,"SUCCESS");
            }
            else if(jobs[i].status == ERROR){
                strcpy(job_status,"FAILED");
            }
            printf("%d\t%*s\t%s\t%s\t%s\n",jobs[i].jobid,-max_command_len,jobs[i].command,jobs[i].job_start_time,jobs[i].job_end_time,job_status);
        }
    }
}
void show_jobs(int n){
    int i;
    int max_command_len = 0;
    char job_status[8];

    char jobid_str[100];
    for(i=0;i<n;i++){
        if((jobs[i].status == RUNNING) || (jobs[i].status == WAITING)){
            if(max_command_len < strlen(jobs[i].command))
                max_command_len = strlen(jobs[i].command);
        }
    }
    printf("jobid\t%*s\tstatus\n",-max_command_len,"command");
    for(i=0;i<n;i++){
        if((jobs[i].status == RUNNING) || (jobs[i].status == WAITING)){
            sprintf(jobid_str,"%d",jobs[i].jobid);
            if(jobs[i].status == RUNNING){
                strcpy(job_status,"RUNNING");
            }
            else if(jobs[i].status == WAITING){
                strcpy(job_status,"WAITING");
            }
            printf("%d\t%*s\t%s\n",jobs[i].jobid,-max_command_len,jobs[i].command,job_status);
        }
    }
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
