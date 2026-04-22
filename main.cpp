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
... (138 lines left)