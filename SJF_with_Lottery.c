
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX_JOBS 20                     // Maximum number of jobs that can be read from input file
#define MAX_DISTRIBUTION 10             // Maximum number of cpu_bursts and i/o a job can have
#define MAX_VALUE 10000
#define MAX_LOTTERY_NUM 1000            


// input file
// Process    Arrival_Time      Execution_Time         Distribution
// 1            0                   7                   3+5+3+5+1
// 2            2                   9                   5+2+4
// 3            4                   6                   6
// 4            6                   8                   4+1+4


typedef struct {
    int job_id;                                 // name of the job
    int arrival_time;                           // arival time of the job
    int Execution_time;                         // total Execution time given
    int execution_time;                         // total Execution time remaining
    int distribution[MAX_DISTRIBUTION];         // total number of io and cpu_burst
    int current_ind;                            // acts as an index for the current cpu_burst in distribution array
    int current_cpu_burst;                      // current cpu_burst -- we only use this in available_jobs
    int io_time;                                // stores the current i/o time of the job and tells if this job is in i/o or not
    bool completed;                             // tells if the job has completed its execution
    int total_io_time;
    int completion_time;
    int turnaround_time;
    int waiting_time;
} job;


int main(int argc, char **argv){

    srand(time(0));

    // checking if input file is specified
    if (argc != 2) {                                
        fprintf(stderr, "Usage: %s input_file\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    FILE *fp = fopen(argv[1], "a+");
    if (fp == NULL) {
        fprintf(stderr, "Error: cannot open file '%s'\n", argv[1]);
        exit(EXIT_FAILURE);
    }

    // Read jobs from input file
    int num_jobs = 0;
    job jobs[MAX_JOBS];
    char line[100];                                         // 100 - specifies the maximum line length 
    fgets(line, sizeof(line), fp);                          // to get the headers out of the loop
    while (fgets(line, sizeof(line), fp) != NULL) {

        // printf(line);
        char *token = strtok(line, " ");
        jobs[num_jobs].job_id = atoi(token);
        printf("J%d ", jobs[num_jobs].job_id);

        token = strtok(NULL, " ");
        jobs[num_jobs].arrival_time = atoi(token);
        printf("%d ", jobs[num_jobs].arrival_time);

        token = strtok(NULL, " ");
        jobs[num_jobs].execution_time = atoi(token);
        jobs[num_jobs].Execution_time = atoi(token);
        printf("%d ", jobs[num_jobs].execution_time);
        

        token = strtok(NULL, " ");   
        int num_dist = 0; 
        char *dist_list = strtok(token, "+");
        while (dist_list!=NULL && num_dist < MAX_DISTRIBUTION) {                       
            jobs[num_jobs].distribution[num_dist] = atoi(dist_list);
            num_dist++;
            dist_list = strtok(NULL, "+");
        }

        printf("  ");
        for (int i=0; i<num_dist; i++){
            printf("%d ",jobs[num_jobs].distribution[i] );
        }

        jobs[num_jobs].io_time = 0;
        jobs[num_jobs].completed = false;
        jobs[num_jobs].current_ind = 0;
        jobs[num_jobs].total_io_time = 0;

        num_jobs++;
        printf("\n");
    }

    fclose(fp);


    const int total_jobs = num_jobs;
    int current_time = 0;
    int previous_execution_time = -1;
    job available_jobs[total_jobs];
    int previous_executed_job_id = -1;
    int total_idle_time = 0;

    while(num_jobs > 0){

        int num_available_jobs = 0;
        int highest_cpu_burst = 0;                          // used for the getting the token range
        int least_cpu_burst = MAX_VALUE;

        // iterate over all jobs and add all the jobs which are available at current time
        for (int i = 0; i < total_jobs; i++) {
            if (jobs[i].completed == false){

                // reduce the io_time if the job is in i/o i.e io_time updation for other jobs except the previous executed job as it finished its cpu_burst
                if (jobs[i].io_time != 0) {
                    if (jobs[i].job_id != previous_executed_job_id){
                        if (jobs[i].io_time <= previous_execution_time){
                            jobs[i].io_time = 0;
                            jobs[i].current_ind++;                                              // this now points to the next cpu_burst 
                        }else{
                            jobs[i].io_time -= previous_execution_time;
                        }
                    }
                }

                // add jobs to available jobs if the jobs arrival time is less than current time and the job is not in i/o
                if (jobs[i].arrival_time <= current_time){
                    if (jobs[i].io_time == 0){
                                            
                        available_jobs[num_available_jobs] = jobs[i];

                        // manage the current cpu_burst
                        int current_cpu_burst = jobs[i].distribution[jobs[i].current_ind];
                        available_jobs[num_available_jobs].current_cpu_burst = current_cpu_burst;
                        if (current_cpu_burst > highest_cpu_burst){
                            highest_cpu_burst = current_cpu_burst;
                        }
                        if (current_cpu_burst < least_cpu_burst){
                            least_cpu_burst = current_cpu_burst;
                        }

                        num_available_jobs++;
                    
                    }
                }
            }
        }

        printf("----------------------------------------------------------------------------------------------------");

        printf("\nThe avilable jobs with their current cpu bursts are ");
        // printf("%d", num_available_jobs);
        for (int i=0; i<num_available_jobs;i++){
            printf(" (J%d , %d) ",available_jobs[i].job_id, available_jobs[i].current_cpu_burst );
        }
       

        // for idle time cases  -- available jobs are 0
        if (num_available_jobs == 0){      
            printf("None\n");
            int idle_time = 0;
            int next_min_arrival = MAX_VALUE;
            int least_io_time = MAX_VALUE;
            // iterate over all the jobs and find the next least arrival time or next least io time from the current time
            for (int i = 0; (i < total_jobs); i++) {
                if (jobs[i].completed == false){
                    if (jobs[i].arrival_time > current_time){
                        if(jobs[i].arrival_time < next_min_arrival){
                            next_min_arrival = jobs[i].arrival_time;
                        }
                    }
                    if (jobs[i].io_time > 0){
                        if(jobs[i].io_time < least_io_time){
                            least_io_time = jobs[i].io_time;
                        }
                    }
                }
            }
            if (next_min_arrival < least_io_time){
                idle_time = next_min_arrival;
            }else{
                idle_time = least_io_time;
            }
            printf("current time - %d,         CPU Idle for %d,          next job at %d\n", current_time, idle_time, current_time + idle_time);
            current_time += idle_time;
            total_idle_time += idle_time;
            previous_execution_time = idle_time;
            previous_executed_job_id = -1;                                                  // as no job has executed in this time
            continue;
        }


        // if the avilable jobs are not 0
        int job_ind;                                                                        // gives us the index of the job to be executed in the available_jobs
        if (num_available_jobs > 1){

            // assigning token ranges to available jobs based on priority/weights
            int weights[num_available_jobs];
            int sum = 0;
            int temp = highest_cpu_burst + least_cpu_burst;
            // we get a changed order of the priorities/weights  =>  input-[1,4,5,3,5] output-[5,2,1,3,1]
            for (int i=0; i<num_available_jobs; i++){     
                // printf(" (J%d , %d) ", available_jobs[i].job_id, available_jobs[i].current_cpu_burst);                     
                weights[i] = temp - available_jobs[i].current_cpu_burst;
                sum += weights[i];
            }

            //the token array stores the first value in the range of its lottery tickets 
            int token[num_available_jobs];
            token[0] = 1;
            int temp_sum = weights[0];
            printf("\ntoken ranges are : (%d-", token[0]);
            for (int i=1; i<num_available_jobs; i++){
                token[i] = temp_sum * MAX_LOTTERY_NUM / sum;
                temp_sum += weights[i];
                printf("%d)   (%d-", token[i], token[i]);
            } 
            printf("%d)\n",MAX_LOTTERY_NUM);


            // picking a random lottery ticket
            int ticket = ((rand()+1)*MAX_LOTTERY_NUM/(RAND_MAX));
            printf("The random ticket is %d, So the job ",ticket);

            // selecting the job that has the current lottery number in its range
            for (int i=0; i<num_available_jobs-1; i++){               
                if ((token[i] <= ticket) && (ticket < token[i+1]) ){
                    job_ind = i;
                }
                if (token[num_available_jobs-1] <= ticket && ticket <= MAX_LOTTERY_NUM){
                    job_ind = num_available_jobs-1;
                }
            }

        }else{
            job_ind = 0;                                // if there is only one job in the available_jobs
            printf("\nNo need of lottery, ");
        }



        // executing the choosen job from the jobs
        for (int i=0; i<total_jobs; i++){
            if (jobs[i].job_id == available_jobs[job_ind].job_id ){

                printf("(J%d, %d) will execute.\n", jobs[i].job_id, available_jobs[job_ind].current_cpu_burst);
                jobs[i].execution_time -= available_jobs[job_ind].current_cpu_burst;


                int updated_current_time = current_time + available_jobs[job_ind].current_cpu_burst;
                printf("current time - %d,     J%d is executing,      finished its cpu burst at %d\n", current_time, jobs[i].job_id, updated_current_time );
                current_time = updated_current_time;
                previous_execution_time = available_jobs[job_ind].current_cpu_burst;

                // remove the jobs that have completed their execution
                if (jobs[i].execution_time == 0){                       
                    num_jobs--;
                    jobs[i].completed = true;
                    jobs[i].completion_time = current_time;
                    jobs[i].turnaround_time = current_time - jobs[i].arrival_time;
                    printf("\nJ%d has finished its execution completely.\n\n", jobs[i].job_id);
                }

                // after job's cpu burst if it is not entirely completed it will go for i/o 
                previous_executed_job_id = jobs[i].job_id;
                jobs[i].current_ind++;                                                          // this points to the i/o, next to the executed cpu_burst
                jobs[i].io_time = jobs[i].distribution[jobs[i].current_ind];
                if (jobs[i].execution_time != 0){
                    jobs[i].total_io_time += jobs[i].io_time;
                }

                break;
            
            }
        }
    }
    

    printf("\n-----------------------------------------------------------------------------------------------------------\n");
    
    int sum_waiting_time;
    printf(" job   arrival_time   execution_time   i/o_time   completion_time   waiting_time\n");
    for (int i=0; i<total_jobs;i++){
        jobs[i].waiting_time = jobs[i].turnaround_time - jobs[i].Execution_time - jobs[i].total_io_time;
        sum_waiting_time +=jobs[i].waiting_time;
        printf(" J%d         %d            %d             %d            %d               %d\n", jobs[i].job_id, jobs[i].arrival_time, jobs[i].Execution_time, jobs[i].total_io_time, jobs[i].completion_time, jobs[i].waiting_time );
    }
    int avg_waiting_time = sum_waiting_time/total_jobs;
    printf("Average waiting time is %d\n", avg_waiting_time);
    printf("Total idle time is %d\n", total_idle_time);

    return 0;
 
    
}


