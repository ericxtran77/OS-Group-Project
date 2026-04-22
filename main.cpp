// ============================================================
// CPSC 351-08: Operating Systems Concepts - Spring 2026
// Story: Disneyland Ride Queue Scheduling
// Group Members: Eric Tran, Ernesto Gomez, Marinah Rubalcaba,
//               Yash Patel, Aiden Hosanna
// ============================================================

#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <iomanip>
#include <climits>
using namespace std;

// Represents a guest group waiting in line
struct Guest {
    string name;
    int arrivalTime;
    int burstTime;
    int priority;
    int remainingTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
};

// Prints a simple Gantt chart
void printGantt(vector<pair<string, int>>& timeline) {
    cout << "\nGantt Chart:\n ";
    for (auto& slot : timeline) cout << "------";
    cout << "\n|";
    for (auto& slot : timeline) cout << " " << setw(3) << slot.first << " |";
    cout << "\n ";
    for (auto& slot : timeline) cout << "------";
    cout << "\n0";
    for (auto& slot : timeline) cout << setw(6) << slot.second;
    cout << "\n";
}

// Prints results table with averages
void printResults(vector<Guest>& guests) {
    float totalTAT = 0, totalWT = 0;

    cout << "\n" << left
         << setw(8)  << "Group"
         << setw(10) << "Arrival"
         << setw(10) << "Burst"
         << setw(14) << "Completion"
         << setw(14) << "Turnaround"
         << setw(10) << "Waiting" << "\n";
    cout << string(66, '-') << "\n";

    for (auto& g : guests) {
        cout << left
             << setw(8)  << g.name
             << setw(10) << g.arrivalTime
             << setw(10) << g.burstTime
             << setw(14) << g.completionTime
             << setw(14) << g.turnaroundTime
             << setw(10) << g.waitingTime << "\n";
        totalTAT += g.turnaroundTime;
        totalWT  += g.waitingTime;
    }

    int n = guests.size();
    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << totalTAT / n << "\n";
    cout << "Average Waiting Time:    " << fixed << setprecision(2) << totalWT  / n << "\n";
}

// FCFS: guests are served in the order they arrive
void fcfs(vector<Guest> guests) {
    cout << "\n========================================\n";
    cout << " FCFS - First Come, First Served\n";
    cout << " Guests are served in order of arrival\n";
    cout << "========================================\n";

    sort(guests.begin(), guests.end(), [](Guest a, Guest b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;
    vector<pair<string, int>> timeline;

    for (auto& g : guests) {
        if (currentTime < g.arrivalTime)
            currentTime = g.arrivalTime;

        currentTime += g.burstTime;
        g.completionTime  = currentTime;
        g.turnaroundTime  = g.completionTime - g.arrivalTime;
        g.waitingTime     = g.turnaroundTime - g.burstTime;
        timeline.push_back({g.name, currentTime});
    }

    printGantt(timeline);
    printResults(guests);
}

// SJF: guest with the shortest burst time goes next
void sjf(vector<Guest> guests) {
    cout << "\n========================================\n";
    cout << " SJF - Shortest Job First\n";
    cout << " Shortest ride duration boards first\n";
    cout << "========================================\n";

    int n = guests.size();
    int currentTime = 0, completed = 0;
    vector<bool> done(n, false);
    vector<pair<string, int>> timeline;

    while (completed < n) {
        int shortest = -1;
        int minBurst = INT_MAX;

        for (int i = 0; i < n; i++) {
            if (!done[i] && guests[i].arrivalTime <= currentTime) {
                if (guests[i].burstTime < minBurst) {
                    minBurst = guests[i].burstTime;
                    shortest = i;
                }
            }
        }

        if (shortest == -1) {
            currentTime++;
            continue;
        }

        currentTime += guests[shortest].burstTime;
        guests[shortest].completionTime  = currentTime;
        guests[shortest].turnaroundTime  = currentTime - guests[shortest].arrivalTime;
        guests[shortest].waitingTime     = guests[shortest].turnaroundTime - guests[shortest].burstTime;
        done[shortest] = true;
        completed++;

        timeline.push_back({guests[shortest].name, currentTime});
    }

    printGantt(timeline);
    printResults(guests);
}

// Round Robin: each guest gets a fixed time slice
void roundRobin(vector<Guest> guests, int quantum) {
    cout << "\n========================================\n";
    cout << " Round Robin (Quantum = " << quantum << ")\n";
    cout << " Each guest group gets equal time slots\n";
    cout << "========================================\n";

    int n = guests.size();
    for (auto& g : guests)
        g.remainingTime = g.burstTime;

    queue<int> readyQueue;
    vector<bool> inQueue(n, false);
    vector<pair<string, int>> timeline;

    int currentTime = 0, completed = 0, contextSwitches = 0;

    for (int i = 0; i < n; i++) {
        if (guests[i].arrivalTime == 0) {
            readyQueue.push(i);
            inQueue[i] = true;
        }
    }

    while (completed < n) {
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

        int idx = readyQueue.front();
        readyQueue.pop();

        int timeSlice = min(quantum, guests[idx].remainingTime);
        guests[idx].remainingTime -= timeSlice;
        currentTime += timeSlice;
        contextSwitches++;

        timeline.push_back({guests[idx].name, currentTime});

        for (int i = 0; i < n; i++) {
            if (!inQueue[i] && guests[i].arrivalTime <= currentTime && guests[i].remainingTime > 0) {
                readyQueue.push(i);
                inQueue[i] = true;
            }
        }

        if (guests[idx].remainingTime > 0) {
            readyQueue.push(idx);
        } else {
            guests[idx].completionTime  = currentTime;
            guests[idx].turnaroundTime  = currentTime - guests[idx].arrivalTime;
            guests[idx].waitingTime     = guests[idx].turnaroundTime - guests[idx].burstTime;
            completed++;
        }
    }

    printGantt(timeline);
    printResults(guests);
    cout << "Total Context Switches: " << contextSwitches << "\n";
}

int main() {
    cout << "============================================================\n";
    cout << "  Disneyland Ride Queue - CPU Scheduling Simulation\n";
    cout << "  CPSC 351-08 | Spring 2026\n";
    cout << "============================================================\n";

    // Guest groups: name, arrivalTime, burstTime, priority, (rest start at 0)
    vector<Guest> guests = {
        {"G1", 0, 6, 2, 0, 0, 0, 0},
        {"G2", 1, 3, 1, 0, 0, 0, 0},
        {"G3", 2, 8, 3, 0, 0, 0, 0},
        {"G4", 3, 4, 2, 0, 0, 0, 0},
        {"G5", 4, 2, 1, 0, 0, 0, 0}
    };

    int quantum = 3;

    fcfs(guests);
    sjf(guests);
    roundRobin(guests, quantum);

    cout << "\n============================================================\n";
    cout << "  Simulation Complete\n";
    cout << "============================================================\n";

    return 0;
}
