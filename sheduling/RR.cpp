#include <bits/stdc++.h>
using namespace std;

class Field {
public:
    string job;
    float arrivalTime;
    float runTime;
    float remainingTime;
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
        remainingTime = 0;
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
        cout << "Enter run time of job " << job << ": ";
        cin >> runTime;
        remainingTime = runTime;
    }
};

void process(vector<Field> &table, float tq) {
    int n = table.size();
    sort(table.begin(), table.end(), [](const Field &a, const Field &b){
        if (a.arrivalTime == b.arrivalTime) return a.job < b.job; // stable order
        return a.arrivalTime < b.arrivalTime;
    });

    queue<int> q;
    vector<bool> inQ(n, false), done(n, false);
    int completed = 0, executed = 0;
    float time = table[0].arrivalTime;

    // seed queue with all processes that arrive at the earliest time
    for (int i = 0; i < n; i++) {
        if (table[i].arrivalTime == time) { q.push(i); inQ[i] = true; }
    }

    while (completed < n) {
        if (q.empty()) {
            // jump to next arrival
            float nextAt = FLT_MAX; int nextIdx = -1;
            for (int i = 0; i < n; i++)
                if (!done[i] && table[i].arrivalTime < nextAt) { nextAt = table[i].arrivalTime; nextIdx = i; }
            time = nextAt; q.push(nextIdx); inQ[nextIdx] = true;
        }

        int idx = q.front(); q.pop(); inQ[idx] = false;

        if (table[idx].startTime == -1) table[idx].startTime = time;

        float slice = min(tq, table[idx].remainingTime);
        table[idx].remainingTime -= slice;
        time += slice;

        // 1) enqueue ONLY newly arrived processes (skip the one that just ran)
        for (int i = 0; i < n; i++) {
            if (i == idx) continue;                      // <<< critical fix
            if (!done[i] && !inQ[i] && table[i].arrivalTime <= time) {
                q.push(i); inQ[i] = true;
            }
        }

        // 2) then, if current didn't finish, requeue it at the tail
        if (table[idx].remainingTime > 0) {
            q.push(idx); inQ[idx] = true;
        } else {
            done[idx] = true;
            table[idx].finishTime = time;
            table[idx].executionSequence = executed++;
            completed++;
        }
    }

    // metrics
    for (auto &p : table) {
        p.turnAroundTime = p.finishTime - p.arrivalTime;
        p.waitingTime    = p.turnAroundTime - p.runTime;
    }
}

void output(vector<Field> &table) {
    cout << "\n-------------------------------------------------------------------------------\n";
    cout << "Job\tAT\tBT\tST\tCT\tTAT\tWT\tSeq\n";
    cout << "-------------------------------------------------------------------------------\n";

    float totalTAT = 0, totalWT = 0;
    for (auto &p : table) {
        cout << p.job << "\t"
             << p.arrivalTime << "\t"
             << p.runTime << "\t"
             << p.startTime << "\t"
             << p.finishTime << "\t"
             << p.turnAroundTime << "\t"
             << p.waitingTime << "\t"
             << p.executionSequence << endl;

        totalTAT += p.turnAroundTime;
        totalWT += p.waitingTime;
    }

    cout << "-------------------------------------------------------------------------------\n";
    cout << fixed << setprecision(2);
    cout << "Average Turnaround Time = " << totalTAT / table.size() << endl;
    cout << "Average Waiting Time = " << totalWT / table.size() << endl;
}

int main() {
    int n;
    float tq;
    cout << "Enter number of tasks: ";
    cin >> n;
    cout << "Enter Time Quantum: ";
    cin >> tq;

    vector<Field> table(n);
    for (int i = 0; i < n; i++)
        table[i].input(i);

    process(table, tq);
    output(table);
}
