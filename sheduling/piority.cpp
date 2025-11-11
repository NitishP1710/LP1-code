#include <bits/stdc++.h>
using namespace std;

class Field {
public:
    string job;
    float arrivalTime;
    float runTime;
    int priority;
    float startTime;
    float finishTime;
    float turnAroundTime;
    float waitingTime;
    bool done;
    int executionSequence;

    Field() {
        job = "";
        arrivalTime = 0;
        runTime = 0;
        priority = 0;
        startTime = -1;
        finishTime = 0;
        turnAroundTime = 0;
        waitingTime = 0;
        done = false;
        executionSequence = 0;
    }

    void input(int i) {
        cout << "Enter job " << i + 1 << " name: ";
        cin >> job;
        cout << "Enter arrival time of job " << job << ": ";
        cin >> arrivalTime;
        cout << "Enter run time (burst time) of job " << job << ": ";
        cin >> runTime;
        cout << "Enter priority of job " << job << " (1 = highest): ";
        cin >> priority;
    }
};

void process(vector<Field> &table) {
    int n = table.size();
    int completed = 0;
    int executed = 0;
    float currentTime = 0;

    // Sort by arrival time first
    sort(table.begin(), table.end(), [](Field &a, Field &b) {
        return a.arrivalTime < b.arrivalTime;
    });

    while (completed < n) {
        int idx = -1;
        int minPriority = INT_MAX;

        // Find the process with highest priority (lowest priority number)
        // among those that have arrived
        for (int i = 0; i < n; i++) {
            if (!table[i].done && table[i].arrivalTime <= currentTime) {
                if (table[i].priority < minPriority) {
                    minPriority = table[i].priority;
                    idx = i;
                }
                // If same priority, choose the one that arrived earlier
                else if (table[i].priority == minPriority &&
                         table[i].arrivalTime < table[idx].arrivalTime) {
                    idx = i;
                }
            }
        }

        // If no process has arrived yet, jump forward in time
        if (idx == -1) {
            currentTime++;
            continue;
        }

        // Execute selected process completely
        table[idx].startTime = currentTime;
        currentTime += table[idx].runTime;
        table[idx].finishTime = currentTime;
        table[idx].done = true;
        executed++;
        table[idx].executionSequence = executed;
        completed++;
    }

    // Compute turnaround and waiting times
    for (auto &p : table) {
        p.turnAroundTime = p.finishTime - p.arrivalTime;
        p.waitingTime = p.turnAroundTime - p.runTime;
    }
}

void output(vector<Field> &table) {
    cout << "\n---------------------------------------------------------------------------------\n";
    cout << "Job\tAT\tBT\tPr\tST\tCT\tTAT\tWT\tSeq\n";
    cout << "---------------------------------------------------------------------------------\n";

    float totalTAT = 0, totalWT = 0;
    for (auto &p : table) {
        cout << p.job << "\t"
             << p.arrivalTime << "\t"
             << p.runTime << "\t"
             << p.priority << "\t"
             << p.startTime << "\t"
             << p.finishTime << "\t"
             << p.turnAroundTime << "\t"
             << p.waitingTime << "\t"
             << p.executionSequence << endl;
        totalTAT += p.turnAroundTime;
        totalWT += p.waitingTime;
    }

    cout << "---------------------------------------------------------------------------------\n";
    cout << fixed << setprecision(2);
    cout << "Average Turnaround Time = " << totalTAT / table.size() << endl;
    cout << "Average Waiting Time = " << totalWT / table.size() << endl;
}

int main() {
    int n;
    cout << "Enter number of tasks: ";
    cin >> n;

    vector<Field> table(n);
    for (int i = 0; i < n; i++)
        table[i].input(i);

    process(table);
    output(table);

    return 0;
}
