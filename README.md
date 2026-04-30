# Disneyland Ride Queue Simulation using CPU Scheduling Algorithms

## Storyline:
Imagine you are visiting the happiest place on Earth on a busy day, surrounded by long lines of people itching to get on the next attraction. Park guests arrive at rides throughout the day, with each guest waiting for their turn to experience the ride they’re waiting for. In this scenario, each guest or group of guests represents a process in a scheduling system. The arrival time would equate to when a guest joins the line, and the burst time represents how long the ride takes for that group of guests. Although all guests are waiting for the same ride, the time required to serve each group may vary due to factors like group size, loading time, and any other operational delays like the ride breaking down. Since rides have a limited capacity of guests of which they can operate with, it is essential that Disneyland, the theme park in this particular proposal, must determine  how to manage their wait queues for attractions in the most efficient manner possible, as to not upset their guests which pay a hefty amount of money to be waiting most of the day. 
Scheduling plays an important role in ensuring two crucial factors: fairness and efficiency. If park guests are served strictly in their arrival time, some guests might experience severe delays. If shorter rides are prioritized, the overall wait times of the park may improve, but some guests might still end up waiting longer. Rotating access between guests can be a way to improve fairness when it comes to guests’ waiting time, but efficiency may be a significant tradeoff. By applying three different scheduling algorithms, FCFS, SJF, and Round Robin, we are able to analyze how each approach will impact waiting time, turnaround time, and overall guest experience, which highlights the trade-offs between fairness, efficiency, and responsiveness. 

## Algorithms:
We selected First Come First Serve, Shortest Job First, and Round Robin because each represents a different approach to CPU scheduling and highlights key trade offs between fairness, efficiency, and responsiveness. 
FCFS was chosen as a baseline for comparison because it is simple and ensures fairness by serving guests in the order in which they have entered the line. In the context of Disneyland, a theme park, this reflects the traditional standby line where guests aren’t allowed to skip ahead and must wait for all the guests in front of them. However, this approach can lead to longer wait times when a group that requires a longer ride duration is at the front of the queue. All guests behind them must wait, even if their ride times are much shorter. This demonstrates the trade off between fairness and efficiency, as maintaining the strict order of FCFS may increase overall waiting time.
Shortest Job First was selected because it improves system efficiency by prioritizing guests with shorter ride durations. In this case, groups which can be served faster would be allowed to go first, which would reduce the average waiting time, and increase the amount of guests passing through. However, the shortest job first can also result in starvation, where guests with longer ride durations will repeatedly be delayed if shorter groups continue to arrive. This highlights the trade off between efficiency and fairness, as optimizing for faster service can lead to unfair delays for certain guests, causing dissatisfaction.
Round Robin was chosen as well because it introduces the concept of time-sharing, where each guest will receive an equal fixed amount of time before the queue system will move on to the next guest. In the scenario of theme parks, this would be interpreted as rotating access to the ride among groups/guests, ensuring that no guest monopolizes the system which would ensure fairness and responsiveness from guests, as all guests would be given the same opportunities to ride the attraction. However, this approach could reduce efficiency because of the overhead from having to switch between guests, and if the fixed time slot is already small due to the amount of guests waiting in the queue. 
Together, these three algorithms provide a balanced comparison of different scheduling strategies, which allows us to analyze how each approach will affect factors like waiting time, turnaround time, and also the overall guest experience. If the guests are unhappy, they are unlikely to return and more likely to return to a competitor. 

## Input:
| Process | Arrival | Burst   | Priority |
|-------- |-------- |---------|----------|
|    G1   |   0     |    6    |     2    |
|    G2   |   1     |    3    |     1    |
|    G3   |   2     |    8    |     3    |
|    G4   |   3     |    4    |     2    |
|    G5   |   4     |    2    |     1    |

Quantum (RR) = 3

## Output:

  Disneyland Ride Queue - CPU Scheduling Simulation
  CPSC 351-08 | Spring 2026



 ## FCFS - First Come, First Served
 Guests are served in order of arrival


Scheduling Order:
```
| G1 | G2 | G3 | G4 | G5 |
0    6    9    17   21   23
```


| Process | Arrival | Burst | Completion | Turnaround | Waiting |
|--------|--------|------|-----------|------------|---------|
| G1     | 0      | 6    | 6         | 6          | 0       |
| G2     | 1      | 3    | 9         | 8          | 5       |
| G3     | 2      | 8    | 17        | 15         | 7       |
| G4     | 3      | 4    | 21        | 18         | 14      |
| G5     | 4      | 2    | 23        | 19         | 17      |

Average Turnaround Time: 13.20
Average Waiting Time:    8.60

 ## SJF - Shortest Job First
 Shortest ride duration boards first


Scheduling Order:
```
| G1 | G5 | G2 | G4 | G3 |
0    6    8    11   15   23
```

| Process | Arrival | Burst | Completion | Turnaround | Waiting |
|--------|--------|------|-----------|------------|---------|
| G1     | 0      | 6    | 6         | 6          | 0       |
| G2     | 1      | 3    | 11        | 10         | 7       |
| G3     | 2      | 8    | 23        | 21         | 13      |
| G4     | 3      | 4    | 15        | 12         | 8       |
| G5     | 4      | 2    | 8         | 4          | 2       |

Average Turnaround Time: 10.60
Average Waiting Time:    6.00


## Round Robin (Quantum = 3)
Each guest group gets equal time slots

Scheduling Order:
```
| G1 | G2 | G3 | G4 | G1 | G5 | G3 | G4 | G3 |
0    3    6    9    12   15   17   20   21   23
```
| Process | Arrival | Burst | Completion | Turnaround | Waiting |
|--------|--------|------|-----------|------------|---------|
| G1     | 0      | 6    | 15        | 15         | 9       |
| G2     | 1      | 3    | 6         | 5          | 2       |
| G3     | 2      | 8    | 23        | 21         | 13      |
| G4     | 3      | 4    | 21        | 18         | 14      |
| G5     | 4      | 2    | 17        | 13         | 11      |


Average Turnaround Time: 14.40

Average Waiting Time:    9.80

Total Context Switches (Including the Initial Dispatch): 9


Simulation Complete

## Results Summary

| Algorithm | Avg Waiting Time | Avg Turnaround Time |
|----------|-----------------|---------------------|
| FCFS     | 8.60            | 13.20               |
| SJF      | 6.00            | 10.60               |
| RR       | 9.80            | 14.40               |
## Key Insights

SJF achieved the best efficiency with the lowest average waiting and turnaround times. 
FCFS ensured fairness by serving guests in arrival order, but resulted in longer waits. 
Round Robin balanced fairness and responsiveness but introduced overhead, increasing overall times. 
These results show that no single scheduling algorithm is optimal in all situations.
In an environment where efficiency is the primary goal, SJF may be preferred. In scenarios like a theme park where fairness and the customer’s experience are essential, FCFS and Round Robin may be more appropriate to keep the status quo, and not experimenting especially with such a large theme park like Disneyland, dubbed as the “happiest place on Earth.” 

## How To Run The Project?
Clone this repository using GitHub. The easiest way is to click the green "code" button, 
and then copy the URL under the local tab, in the HTTPS. After you've cloned the repository, if you are
using Windows, head into your terminal/command prompt. Make sure you are in the same directory in your terminal 
that actually has the project's files, specifcally main.cpp.
Then, in your terminal, using a g++ compiler, enter "g++ main.cpp -o program"
and then to execute the file, enter ".\program" and the code will run and 
give each algorithm's outputs.
