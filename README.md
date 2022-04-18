# Homework4

To implement a simple job scheduler that executes non-interactive jobs (e.g., jobs that 
do not have direct user interaction, jobs that can run in the background).  

# Description

In this project a simple job scheduler is implemented that will execute non-
interactive jobs (for example, jobs that do not have direct user interaction, jobs that can 
run in the background).  
At any given time, only P jobs will be executing, and P is provided as an argument 
to the program.  
If there are more than P jobs submitted, then these additional jobs must wait until one 
of the P executing jobs are completed.  
P is typically the number of cores that are available on a system 
and are executing one process per core and if there are more jobs than the 
available number of cores these jobs will wait for one of the processes to complete.  
When program is launched with P as the command-line argument, the program 
will print a prompt and wait for the user to enter commands.  
As the jobs are non-interactive, the output and error streams from the job will be 
written to separate files - <jobid>.out and <jobid>.err, where <jobid> is the appropriate 
job id that is associated with a job.  
 
 
# Getting started
  
These instructions will give you a copy of the project up and running on your local machine for development and testing purposes.
  
# Prerequisites
  
Requirements for the software and other tools to build, test and push
1. gcc - GNU Compiler Collection
2. Text editor

# Installing

Download the required files to a directory and open it in the terminal, enter the command 'make' to run the makefile. Run the executable with P value as command line argument. 
        
# Running the tests

functionality of the program:

In the main function, init_jobs() is called which takes the input continuously from the user and if command entered is `<submit> <unix-command>`, inint_jobs() tokenizes the command line argument and adds each job to the queue. Further, the command line arguments are checked and a execute_main_thread thread is created. In execute_main_thread, we run the thread execute_job if count of job_queue is > 0 i.e if there are any jobs available to start, and at the same time checking if current_jobs_running_count < P. In execute_jobs, we run each job and output and error streams will be written to seperate files and update the status of each job, decrement the current_jobs_running_count after execution. When showjobs command is entered, show_jobs method is called with job_no as the parameter, it displays all the jobs that are in waiting or running state. For the 'submithistory' command, show_history() method is being called which displays all the jobs that are completed. 
  
Output:
```
make
./homework4 4
Enter Command: submit ls
Enter Command: submit ps
Enter Command: submit ls
Enter Command: submit ls
Enter Command: submithistory
jobid	command	starttime               	endtime                 	status
0	ls	Sun Apr 17 14:42:54 2022	Sun Apr 17 14:42:54 2022	SUCCESS
1	ps	Sun Apr 17 14:42:59 2022	Sun Apr 17 14:42:59 2022	SUCCESS
2	ls	Sun Apr 17 14:43:02 2022	Sun Apr 17 14:43:02 2022	SUCCESS
3	ls	Sun Apr 17 14:43:06 2022	Sun Apr 17 14:43:06 2022	SUCCESS
Enter Command: submit /home/harivamshi/Desktop/Lab_09/hw1 2000
Enter Command: submit /home/harivamshi/Desktop/Lab_09/hw1 2000
Enter Command: submit /home/harivamshi/Desktop/Lab_09/hw1 2000
Enter Command: showjobs
jobid	command                                 	status
0	/home/harivamshi/Desktop/Lab_09/hw1 2000	RUNNING
1	/home/harivamshi/Desktop/Lab_09/hw1 2000	RUNNING
2	/home/harivamshi/Desktop/Lab_09/hw1 2000	RUNNING
Enter Command: showjobs
jobid	command                                 	status
0	/home/harivamshi/Desktop/Lab_09/hw1 2000	RUNNING
1	/home/harivamshi/Desktop/Lab_09/hw1 2000	RUNNING
2	/home/harivamshi/Desktop/Lab_09/hw1 2000	RUNNING
Enter Command: submit /home/harivamshi/Desktop/Lab_09/hw1
Enter Command: submit /home/harivamshi/Desktop/Lab_09/hw1 2000
Enter Command: submit /home/harivamshi/Desktop/Lab_09/hw1 2000
Enter Command: submit /home/harivamshi/Desktop/Lab_09/hw1 2000
Enter Command: showjobs
jobid	command                                 	status
1	/home/harivamshi/Desktop/Lab_09/hw1 2000	RUNNING
5	/home/harivamshi/Desktop/Lab_09/hw1 2000	RUNNING
6	/home/harivamshi/Desktop/Lab_09/hw1 2000	RUNNING
Enter Command: showjobs
jobid	command                                 	status
1	/home/harivamshi/Desktop/Lab_09/hw1 2000	RUNNING
5	/home/harivamshi/Desktop/Lab_09/hw1 2000	RUNNING
6	/home/harivamshi/Desktop/Lab_09/hw1 2000	RUNNING
Enter Command: submithistory
jobid	command                                 	starttime               	endtime                 	status
0	/home/harivamshi/Desktop/Lab_09/hw1 2000	Sun Apr 17 15:09:43 2022	Sun Apr 17 15:10:22 2022	SUCCESS
2	/home/harivamshi/Desktop/Lab_09/hw1 2000	Sun Apr 17 15:09:54 2022	Sun Apr 17 15:10:16 2022	SUCCESS
3	/home/harivamshi/Desktop/Lab_09/hw1     	Sun Apr 17 15:10:18 2022	Sun Apr 17 15:10:18 2022	FAILED
4	/home/harivamshi/Desktop/Lab_09/hw1 2000	Sun Apr 17 15:10:26 2022	Sun Apr 17 15:10:27 2022	SUCCESS
6	/home/harivamshi/Desktop/Lab_09/hw1 2000	Sun Apr 17 15:10:36 2022	Sun Apr 17 15:11:00 2022	SUCCESS
Enter Command: showjobs
jobid	command	status
Enter Command: submithistory
jobid	command                                 	starttime               	endtime                 	status
0	/home/harivamshi/Desktop/Lab_09/hw1 2000	Sun Apr 17 15:09:43 2022	Sun Apr 17 15:10:22 2022	SUCCESS
1	/home/harivamshi/Desktop/Lab_09/hw1 2000	Sun Apr 17 15:09:48 2022	Sun Apr 17 15:11:09 2022	SUCCESS
2	/home/harivamshi/Desktop/Lab_09/hw1 2000	Sun Apr 17 15:09:54 2022	Sun Apr 17 15:10:16 2022	SUCCESS
3	/home/harivamshi/Desktop/Lab_09/hw1     	Sun Apr 17 15:10:18 2022	Sun Apr 17 15:10:18 2022	FAILED
4	/home/harivamshi/Desktop/Lab_09/hw1 2000	Sun Apr 17 15:10:26 2022	Sun Apr 17 15:10:27 2022	SUCCESS
5	/home/harivamshi/Desktop/Lab_09/hw1 2000	Sun Apr 17 15:10:31 2022	Sun Apr 17 15:11:05 2022	SUCCESS
6	/home/harivamshi/Desktop/Lab_09/hw1 2000	Sun Apr 17 15:10:36 2022	Sun Apr 17 15:11:00 2022	SUCCESS
Enter Command: submit /home/harivamshi/Desktop/Lab_09/hw1 3000
Enter Command: submit /home/harivamshi/Desktop/Lab_09/hw1 3000
Enter Command: submit /home/harivamshi/Desktop/Lab_09/hw1 3000
Enter Command: submit /home/harivamshi/Desktop/Lab_09/hw1 3000
Enter Command: submit /home/harivamshi/Desktop/Lab_09/hw1 3000
Enter Command: showjobs
jobid	command                                 	status
7	/home/harivamshi/Desktop/Lab_09/hw1 3000	RUNNING
8	/home/harivamshi/Desktop/Lab_09/hw1 3000	RUNNING
9	/home/harivamshi/Desktop/Lab_09/hw1 3000	RUNNING
10	/home/harivamshi/Desktop/Lab_09/hw1 3000	RUNNING
11	/home/harivamshi/Desktop/Lab_09/hw1 3000	WAITING

```


  

# Author
  
Contributors: 
  
Harivamshi Diddiga   
Sri Vastava Rangaraju Naga Venkata

# Acknowledgement

I thank my TA's and Dr. Mahmut Unan for helping me learn everyday. 

