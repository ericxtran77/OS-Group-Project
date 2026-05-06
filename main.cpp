// ============================================================
// CPSC 351-08: Operating Systems Concepts - Spring 2026
// Story: Disneyland Ride Queue Scheduling
// Group Members: Eric Tran, Ernesto Gomez, Marinah Rubalcaba,
//               Yash Patel, Aiden Hosanna
// ============================================================

// These are libraries we need for input/output, data structures, and math
#include <iostream>      // allow use cout to print things
#include <vector>        // allow use of vectors (like a resizable array)
#include <algorithm>     // allow sort() and min()
#include <queue>         // allow the queue data structure used in Round Robin
#include <iomanip>       // allow format output with setw() for neat columns
#include <climits>       // allow INT_MAX, the largest possible int value
using namespace std;

// ============================================================
// GUEST STRUCT
// Holds all data for each guest group in the simulation.
// ============================================================
struct Guest {
    string name;          // Guest group label (G1, G2, etc.)
    int arrivalTime;      // When the group joins the line
    int burstTime;        // How long their ride + loading takes
    int priority;         // FastPass or disability access level
    int remainingTime;    // Tracks time left, used in Round Robin
    int completionTime;   // When they finish the ride
    int turnaroundTime;   // Total time from arrival to completion
    int waitingTime;      // Time spent waiting before getting on the ride
};

// ============================================================
// PRINT SCHEDULING ORDER
// Prints a text-based timeline showing the order guests were
// served and the time each one finished.
// ============================================================
void printSchedulingOrder(vector<pair<string, int>>& timeline) {
    cout << "\nScheduling Order:\n";

    // Print the top border of the timeline boxes
    cout << " ";
    for (auto& slot : timeline) cout << "------";
    cout << "\n|";

    // Print each guest group name inside their box
    for (auto& slot : timeline) cout << " " << setw(3) << slot.first << " |";

    // Print the bottom border
    cout << "\n ";
    for (auto& slot : timeline) cout << "------";

    // Print the time markers below each box (when each guest finished)
    cout << "\n0";
    for (auto& slot : timeline) cout << setw(6) << slot.second;
    cout << "\n";
}

// ============================================================
// PRINT RESULTS TABLE
// Prints each guest's completion, turnaround, and waiting time.
// Also calculates and prints the averages at the bottom.
// ============================================================
void printResults(vector<Guest>& guests) {
    float totalTAT = 0, totalWT = 0;

    // Print the table header row
    cout << "\n" << left
         << setw(8)  << "Group"
         << setw(10) << "Arrival"
         << setw(10) << "Burst"
         << setw(14) << "Completion"
         << setw(14) << "Turnaround"
         << setw(10) << "Waiting" << "\n";

    // Print a divider line under the header
    cout << string(66, '-') << "\n";

    // Loop through each guest and print their row of data
    for (auto& g : guests) {
        cout << left
             << setw(8)  << g.name
             << setw(10) << g.arrivalTime
             << setw(10) << g.burstTime
             << setw(14) << g.completionTime
             << setw(14) << g.turnaroundTime
             << setw(10) << g.waitingTime << "\n";

        // Add this guest's times to the running totals for averages
        totalTAT += g.turnaroundTime;
        totalWT  += g.waitingTime;
    }

    // Divide totals by number of guests to get averages
    int n = guests.size();
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << totalTAT / n << "\n";
    cout << "Average Waiting Time:    " << fixed << setprecision(2) << totalWT  / n << "\n";
}

// ============================================================
// ALGORITHM 1: FCFS - First Come, First Served
// Guests are served in arrival order, like a traditional
// standby line. Simple but a slow group at the front holds everyone up.
// ============================================================
void fcfs(vector<Guest> guests) {
    cout << "\n========================================\n";
    cout << " FCFS - First Come, First Served\n";
    cout << " Guests are served in order of arrival\n";
    cout << "========================================\n";

    // Sort all guests by arrival time so we process them in order
    sort(guests.begin(), guests.end(), [](Guest a, Guest b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;
    vector<pair<string, int>> timeline; // stores the order guests were served

    // Go through each guest one by one
    for (auto& g : guests) {

        // If the CPU is idle before this guest arrives, jump ahead to their arrival
        if (currentTime < g.arrivalTime)
            currentTime = g.arrivalTime;

        // Add the guest's burst time to the clock to simulate them riding
        currentTime += g.burstTime;

        // Calculate their metrics based on when they finished
        g.completionTime  = currentTime;
        g.turnaroundTime  = g.completionTime - g.arrivalTime; // total time in the system
        g.waitingTime     = g.turnaroundTime - g.burstTime;   // time spent just waiting

        // Record this guest and the time they finished for the timeline
        timeline.push_back({g.name, currentTime});
    }

    // Print the scheduling order and the results table
    printSchedulingOrder(timeline);
    printResults(guests);
}

// ============================================================
// ALGORITHM 2: SJF - Shortest Job First (Non-Preemptive)
// Guest with the shortest ride time goes next. Reduces average
// wait time but longer groups risk being skipped (starvation).
// ============================================================
void sjf(vector<Guest> guests) {
    cout << "\n========================================\n";
    cout << " SJF - Shortest Job First\n";
    cout << " Shortest ride duration boards first\n";
    cout << "========================================\n";

    int n = guests.size();
    int currentTime = 0, completed = 0;
    vector<bool> done(n, false);         // tracks which guests are finished
    vector<pair<string, int>> timeline;  // stores the order guests were served

    // Keep looping until all guests have been served
    while (completed < n) {
        int shortest = -1;       // index of the guest with shortest burst
        int minBurst = INT_MAX;  // start with a very large number to compare against

        // Search through all guests to find the shortest one that has already arrived
        for (int i = 0; i < n; i++) {
            if (!done[i] && guests[i].arrivalTime <= currentTime) {
                if (guests[i].burstTime < minBurst) {
                    minBurst = guests[i].burstTime;
                    shortest = i; // update the index of the shortest guest found
                }
            }
        }

        // If no guest has arrived yet, advance time by 1 and check again
        if (shortest == -1) {
            currentTime++;
            continue;
        }

        // Process the chosen guest by adding their burst time to the clock
        currentTime += guests[shortest].burstTime;

        // Calculate their metrics
        guests[shortest].completionTime  = currentTime;
        guests[shortest].turnaroundTime  = currentTime - guests[shortest].arrivalTime;
        guests[shortest].waitingTime     = guests[shortest].turnaroundTime - guests[shortest].burstTime;

        // Mark this guest as done and increase the completed count
        done[shortest] = true;
        completed++;

        // Record this guest in the timeline
        timeline.push_back({guests[shortest].name, currentTime});
    }

    // Print the scheduling order and the results table
    printSchedulingOrder(timeline);
    printResults(guests);
}

// ============================================================
// ALGORITHM 3: Round Robin
// Each guest gets a fixed time slice before moving to the next.
// Fair for all guests, but switching between groups adds overhead.
// ============================================================
void roundRobin(vector<Guest> guests, int quantum) {
    cout << "\n========================================\n";
    cout << " Round Robin (Quantum = " << quantum << ")\n";
    cout << " Each guest group gets equal time slots\n";
    cout << "========================================\n";

    int n = guests.size();

    // Set each guest's remaining time equal to their full burst time at the start
    for (auto& g : guests)
        g.remainingTime = g.burstTime;

    queue<int> readyQueue;           // holds the index of guests waiting to be served
    vector<bool> inQueue(n, false);  // tracks if a guest is already in the queue
    vector<pair<string, int>> timeline;

    int currentTime = 0, completed = 0, contextSwitches = 0;

    // Add any guests that arrive at time 0 to the queue right away
    for (int i = 0; i < n; i++) {
        if (guests[i].arrivalTime == 0) {
            readyQueue.push(i);
            inQueue[i] = true;
        }
    }

    // Keep looping until all guests are done
    while (completed < n) {

        // If the queue is empty, no one is ready yet
        // Jump the clock forward to the next guest's arrival time
        if (readyQueue.empty()) {
            for (int i = 0; i < n; i++) {
                if (!inQueue[i] && guests[i].remainingTime > 0) {
                    currentTime = guests[i].arrivalTime;
                    readyQueue.push(i);
                    inQueue[i] = true;
                    break;
                }
            }
        }

        // Take the next guest from the front of the queue
        int idx = readyQueue.front();
        readyQueue.pop();

        // Run this guest for either the full quantum or whatever time they have left
        int timeSlice = min(quantum, guests[idx].remainingTime);
        guests[idx].remainingTime -= timeSlice; // reduce their remaining time
        currentTime += timeSlice;               // advance the clock
        contextSwitches++;                      // count this as a context switch

        // Add this guest and their finish time to the timeline
        timeline.push_back({guests[idx].name, currentTime});

        // Check if any new guests have arrived during this time slice
        // If so, add them to the queue
        for (int i = 0; i < n; i++) {
            if (!inQueue[i] && guests[i].arrivalTime <= currentTime && guests[i].remainingTime > 0) {
                readyQueue.push(i);
                inQueue[i] = true;
            }
        }

        // If this guest still has time left, put them back in the queue
        if (guests[idx].remainingTime > 0) {
            readyQueue.push(idx);
        } else {
            // Guest is fully done — calculate their final metrics
            guests[idx].completionTime  = currentTime;
            guests[idx].turnaroundTime  = currentTime - guests[idx].arrivalTime;
            guests[idx].waitingTime     = guests[idx].turnaroundTime - guests[idx].burstTime;
            completed++;
        }
    }

    // Print the scheduling order, results table, and total context switches
    printSchedulingOrder(timeline);
    printResults(guests);
    cout << "Total Context Switches: " << contextSwitches << "\n";
}

// ============================================================
// MAIN
// Entry point. Defines guest groups and runs all three algorithms.
// ============================================================
int main() {
    cout << "============================================================\n";
    cout << "  Disneyland Ride Queue - CPU Scheduling Simulation\n";
    cout << "  CPSC 351-08 | Spring 2026\n";
    cout << "============================================================\n";

    // Define the guest groups with their arrival time, burst time, and priority
    // Format: name, arrivalTime, burstTime, priority, (remaining/CT/TAT/WT start at 0)
    vector<Guest> guests = {
        {"G1", 0, 6, 2, 0, 0, 0, 0},
        {"G2", 1, 3, 1, 0, 0, 0, 0},
        {"G3", 2, 8, 3, 0, 0, 0, 0},
        {"G4", 3, 4, 2, 0, 0, 0, 0},
        {"G5", 4, 2, 1, 0, 0, 0, 0}
    };

    // Time quantum for Round Robin — each guest gets 3 units of time per turn
    int quantum = 3;

    // Run all three algorithms using the same guest data
    fcfs(guests);
    sjf(guests);
    roundRobin(guests, quantum);

    cout << "\n============================================================\n";
    cout << "  Simulation Complete\n";
    cout << "============================================================\n";

    return 0;
}
