# Dynamic-Round-Robin-Algorithm

With reference to the "An Efficient Dynamic Round Robin Algorithm for CPU scheduling" IEEE paper by Muhammad Umar Farooq, Aamna Shakoor & Abu Bakar Siddique from the University of Engineering and Technology, Lahore, this is my own implementation of the proposed Dynamic Round Robin Algorithm. 

The performance of the Round Robin algorithm fairly depends on the chosen time quantum. A larger quantum leads to FCFS algorithm while a shorter quantum algorithm causes more context switches and thus a large overhead. So, the efficiency of Round Robin depends on how cleverly the time qunatum is selected. One of the problems with Round Robin algorithm with unclever time quantum is that sometimes a process has to wait for all other processes even if its remaining execution time is much smaller.

# Solution: 
The Dynamic Round Robin approach was to select an elastic dynamic quantum that will allow a process to execute completely if its remaining execution time is less than or equal to 0.2th of its actual time. First, the maximum burst time is found from all the available processes in the ready queue. Then take a proportion of this time to set the time quantum. As a rule of thumb, the time quantum is selected as 0.8th fraction of the maximum burst time. Now the scheduler assigns the CPU to all the processes in ready queue with burst time less than the time quantum while larger ones are kept hold on. As soon as all the smaller processes complete their execution, the time quantum is set equal to maximum burst time.
