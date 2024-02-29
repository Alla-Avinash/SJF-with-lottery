# SJF-with-lottery
Shortest Job First with lottery Scheduling Algorithm - Operating Systems  
Written in C Language


## Description  

Scheduling algorithms are used in various computing systems and environments to efficiently manage and allocate resources, such as CPU time, memory, and I/O devices, among competing processes or tasks.In operating systems, scheduling algorithms are essential for efficiently managing the execution of multiple processes or threads on a single CPU. The primary goal of these algorithms is to maximize CPU utilization, minimize response time, ensure fairness, and balance system resources effectively.

There are different types of scheduling algorithms, this project is an implementation of the Shortest Job First(SJF) with lottery Scheduling Algorithm. This project aims to achieve the working of how a scheduling algorithm works and what factors to consider in its implementation.


## Features  

This algorithm exhibits a Non-preemptive behavior [a running process continues to execute until it voluntarily relinquishes the CPU (by going for I/O) or completes its execution]  

Algorithm logic and decision-making process:  
This algorithm gives a range of tickets (lottery numbers) for each available job at the decision making time, based on their burst times(the job with the least current CPU burst gets the highest range of tickets). Now a random number is picked from the alloted range of tickets, the job which has that random selected ticket will be executed next.

Handling of arrival times and burst times:    
The arrival times of the jobs/processes are already given to us from the input file. The algorithm always makes a decision if the CPU becomes idle, it checks the available jobs at that current time and allot a range of tickets based on their burst times (Current CPU burst time)



## Installation  

### Step 1: Clone the repository
You can clone this repository to your local system to run this algorithm on your pc. 

### Step 2: Compiling the C file  
Keep in mind that this algorithm is written in C, so make sure that your PC can support compiling C program files. Now compile the SFJ_with_Lottery.c file to a ".exe" file. 
  
### Step 3: Execution Steps  
After compiling the .c file to the corresponding .exe file, open the terminal and run the .exe file, by mentioning the file name of the compiled file along with the input.txt file path as an argument  
For example, open the current repository in the terminal and write the command  

~ SJF_with_Lottery.exe input.txt



## Usage  

### Input format
You will give a text file as an input to this scheduling algorithm, this file contain tabular data of the process/job number, their respective arrival time, total execution time(Total CPU burst time) and their distribution of CPU Bursts with I/O time(these are seperated with a '+' sign)
Take notE that a process always starts and ends with CPU bursts.

### Output Format  
You will get a detailed output of the execution process. The current available jobs at the decision making time(when CPU becomes Idle) will be displayed along with their alloted ticket ranges. You can also see the random ticket that is selected and what job is selected each time. 

After all the jobs have completed their execution you will get detailed information of their respective completion times and waiting times. YOu will also get a calculated value of the average waiting time and the total idle time.



## Example Output 

It is important to note that the same output might not be generated when you run this algorithm as this involes a random number and it might not match exactly, every time you run this algorithm you may find different outputs with different idle times and different avg. waiting times.
SJF_with_Lottery.exe input.txt


J1 0 7   3 5 3 5 1 
J2 2 9   5 2 4
J3 4 6   6
J4 6 8   4 1 4
/----------------------------------------------------------------------------------------------------
The avilable jobs with their current cpu bursts are  (J1 , 3)
No need of lottery, (J1, 3) will execute.
current time - 0,     J1 is executing,      finished its cpu burst at 3
/----------------------------------------------------------------------------------------------------
The avilable jobs with their current cpu bursts are  (J2 , 5)
No need of lottery, (J2, 5) will execute.
current time - 3,     J2 is executing,      finished its cpu burst at 8
/----------------------------------------------------------------------------------------------------
The avilable jobs with their current cpu bursts are  (J1 , 3)  (J3 , 6)  (J4 , 4)
token ranges are : (1-428)   (428-642)   (642-1000)
The random ticket is 89, So the job (J1, 3) will execute.
current time - 8,     J1 is executing,      finished its cpu burst at 11
/----------------------------------------------------------------------------------------------------
The avilable jobs with their current cpu bursts are  (J2 , 4)  (J3 , 6)  (J4 , 4)
token ranges are : (1-375)   (375-625)   (625-1000)
The random ticket is 945, So the job (J4, 4) will execute.
current time - 11,     J4 is executing,      finished its cpu burst at 15
/----------------------------------------------------------------------------------------------------
The avilable jobs with their current cpu bursts are  (J2 , 4)  (J3 , 6)
token ranges are : (1-600)   (600-1000)
The random ticket is 870, So the job (J3, 6) will execute.
current time - 15,     J3 is executing,      finished its cpu burst at 21

J3 has finished its execution completely.

/----------------------------------------------------------------------------------------------------
The avilable jobs with their current cpu bursts are  (J1 , 1)  (J2 , 4)  (J4 , 4)
token ranges are : (1-666)   (666-833)   (833-1000)
The random ticket is 997, So the job (J4, 4) will execute.
current time - 21,     J4 is executing,      finished its cpu burst at 25

J4 has finished its execution completely.

/----------------------------------------------------------------------------------------------------
The avilable jobs with their current cpu bursts are  (J1 , 1)  (J2 , 4)
token ranges are : (1-800)   (800-1000)
The random ticket is 270, So the job (J1, 1) will execute.
current time - 25,     J1 is executing,      finished its cpu burst at 26

J1 has finished its execution completely.

/----------------------------------------------------------------------------------------------------
The avilable jobs with their current cpu bursts are  (J2 , 4)
No need of lottery, (J2, 4) will execute.
current time - 26,     J2 is executing,      finished its cpu burst at 30

J2 has finished its execution completely.


/-----------------------------------------------------------------------------------------------------------
 job   arrival_time   execution_time   i/o_time   completion_time   waiting_time
 J1         0            7             10            26               9
 J2         2            9             2            30               17
 J3         4            6             0            21               11
 J4         6            8             1            25               10
Average waiting time is 11
Total idle time is 0


## License
This project is not licensed, but I would higly appreciate if you can give me credit in your references in case you find this useful.