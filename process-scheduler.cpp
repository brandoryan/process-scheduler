#include <iostream>
#include <fstream>
#include <vector>
#include <iterator>
#include <string>

using namespace std;

struct process {
    string pid;
    int burst_time;
    int wait_time = 0;
};

struct compPID { 
   bool operator()(process const &a, process const &b) {
       return a.pid < b.pid;
   }
};

struct compBT {
    bool operator()(process const &a, process const &b) {
       return a.burst_time < b.burst_time;
   }
};

int averageWaitTime(vector<process> processes) {
    // Computing the average wait time for all processes
    int avg_wait_time = 0;
    int total_wait_time = 0;

    for(vector<process>::iterator it = processes.begin(); it != processes.end(); ++it) {
        total_wait_time += it->wait_time;
    }
    //cout << total_wait_time << "  " << processes.size();
    avg_wait_time = total_wait_time / processes.size();

    return avg_wait_time;
}

int processesCompleted(vector<process> processes) {
    int total_burst_time = 0;
    for(vector<process>::iterator it = processes.begin(); it != processes.end(); ++it) {
        total_burst_time += it->burst_time;
    }
    
    if(total_burst_time == 0) {
        return true;
    }
    else {
        return false;
    }
}

int fcfs(vector<process> processes) {
    // First sorting by PID to see what process runs first since all arrive at 0
    sort(processes.begin(), processes.end(), compPID());
    while(!processesCompleted(processes)) {
        for(vector<process>::iterator it = processes.begin(); it != processes.end(); ++it) {
            // Add the burst time to the waiting times of the other processes
            for(vector<process>::iterator it2 = processes.begin(); it2 != processes.end(); ++it2) {
                if(it2->burst_time != 0 && it2->pid != it->pid) {
                    it2->wait_time+=it->burst_time;
                }
            }
            it->burst_time = 0;
        }
    }

    return averageWaitTime(processes);
}

int sjf(vector<process> processes) {
    // First sorting by shortest burst time to see what process runs first
    sort(processes.begin(), processes.end(), compBT());

    while(!processesCompleted(processes)) {
        for(vector<process>::iterator it = processes.begin(); it != processes.end(); ++it) {
            // Add the burst time to the waiting times of the other processes
            for(vector<process>::iterator it2 = processes.begin(); it2 != processes.end(); ++it2) {
                if(it2->burst_time != 0 && it2->pid != it->pid) {
                    it2->wait_time+=it->burst_time;
                }
            }
            it->burst_time = 0;
        }
    }

    return averageWaitTime(processes);
}

int rr(vector<process> processes) {

    int time_slice = 10;  //milliseconds
    vector<process>::iterator it = processes.begin();
    while(!processesCompleted(processes)) {
        /* Step Checking
        for(vector<process>::iterator it = processes.begin(); it != processes.end(); ++it) {
            cout << it->pid << ": " << it->burst_time << ", " << it->wait_time << endl;
        }
        */        
        for(vector<process>::iterator it = processes.begin(); it != processes.end(); ++it) {
            // If the time slice is more than the remaining burst time
            if(it->burst_time-time_slice <= 0 && !processesCompleted(processes)) {

                // Add the burst time to the waiting times of the other processes
                for(vector<process>::iterator it2 = processes.begin(); it2 != processes.end(); ++it2) {
                    if(it2->burst_time != 0 && it2->pid != it->pid) {
                        it2->wait_time+=it->burst_time;
                    }
                }

                // Remove the time slice from the current process
                it->burst_time = 0;
            }
            // If the burst time is bigger than the time slice
            else if(it->burst_time-time_slice > 0 && !processesCompleted(processes)){

                // Add the burst time to the wait time of the other processes
                for(vector<process>::iterator it2 = processes.begin(); it2 != processes.end(); ++it2) {
                    if(it2->burst_time != 0 && it2->pid != it->pid) {
                        it2->wait_time+=time_slice;
                    }
                }

                // Remove the time slice from the current process
                it->burst_time-=time_slice;
            }
        }
    }

    return averageWaitTime(processes);
}

int main() {
    ifstream infile("processes.txt");

    string a;
    int b;

    vector<process> processes;

    while(!infile.eof()) {
        infile >> a >> b;
        process addThis;
        addThis.pid = a;
        addThis.burst_time = b;
        processes.push_back(addThis);
    }
    
    int fcfs_avg_wait = fcfs(processes);
    int sjf_avg_wait = sjf(processes);
    int rr_avg_wait = rr(processes);
    
    cout << endl;
    cout << "Average wait time for FCFS: " << fcfs_avg_wait << " ms" << endl;
    cout << "Average wait time for SJF (non-preemptive): " << sjf_avg_wait << " ms" << endl;
    cout << "Average wait time for RR: " << rr_avg_wait << " ms" << endl << endl;

    if(fcfs_avg_wait < sjf_avg_wait && fcfs_avg_wait < rr_avg_wait) {
        cout << "Thus, the FCFS policy results the minimum average waiting time.\n\n";
    }
    else if(sjf_avg_wait < fcfs_avg_wait && sjf_avg_wait < rr_avg_wait) {
        cout << "Thus, the SJF policy results the minimum average waiting time.\n\n";
    }
    else {
        cout << "Thus, the RR policy results the minimum average waiting time.\n\n";
    }
    
    return 0;
}
