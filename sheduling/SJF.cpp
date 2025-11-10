#include <iostream>
#include <bits/stdc++.h>

using namespace std;
class Field
{
public:
    string job;
    float arrivalTime;
    float runTime;
    float remainingTime;
    float startTime;
    float finishTime;
    float turnAroundTime;
    bool done;
    int executionSequence;
    Field()
    {
        this->job = "";
        this->arrivalTime = 0.0;
        this->runTime = 0.0;
        this->remainingTime = 0.0;
        this->startTime = -1.0;
        this->finishTime = 0.0;
        this->turnAroundTime = 0.0;
        this->done = false;
        this->executionSequence = 0;
    }

    void input(int i)
    {
        cout << "Enter a job " << i << " :" << endl;
        cin >> job;
        cout << "Enter arrival time of job " << i << " :" << endl;
        cin >> arrivalTime;
        cout << "Enter runTime of job " << i << " :" << endl;
        cin >> runTime;
        remainingTime = runTime;
    }
};

void process(vector<Field> &table, vector<bool> &complete) {
    int n = table.size();
    int completed = 0;
    int currentTime = 0;
    int executed = 0;

    while (completed < n) {
        int idx = -1;
        float minTime = FLT_MAX;

        // find process with shortest remaining time that has arrived
        for (int i = 0; i < n; i++) {
            if (!complete[i] && table[i].arrivalTime <= currentTime && table[i].remainingTime < minTime && table[i].remainingTime > 0) {
                minTime = table[i].remainingTime;
                idx = i;
            }
        }

        // CPU idle if no process ready
        if (idx == -1) {
            currentTime++;
            continue;
        }

        if (table[idx].startTime == -1)
            table[idx].startTime = currentTime;

        // run process for 1 unit of time
        table[idx].remainingTime -= 1;

        if (table[idx].remainingTime == 0) {
            table[idx].finishTime = currentTime + 1;
            complete[idx] = true;
            completed++;
            executed++;
            table[idx].executionSequence = executed;
        }

        currentTime++;
    }

    // calculate turnaround time
    for (int i = 0; i < n; i++)
        table[i].turnAroundTime = table[i].finishTime - table[i].arrivalTime;
}
\
void output(vector<Field> &table)
{
    int n = table.size();
    cout << "index"<< "  "<< "JobName"<< "  "<< "arrivalTime"<< "  "<< "StartTime"<< "  "<< "Runtime"
         << "  "<< "finishTime"<< "  " << "TurnAroundTime" << "  "<< "ExecutionSequence"<< endl;
    for (int i = 0; i < n; i++)
    {
        Field p = table[i];
        cout << i << "       ";
        cout << p.job << "       ";
        cout << p.arrivalTime << "           ";
        cout << p.startTime << "           ";
        cout << p.runTime << "         ";
        cout << p.finishTime << "            ";
        cout << p.turnAroundTime<<"                ";
        cout << p.executionSequence;
        cout << endl;
    }
}

int main()
{
    int number_Of_Task;
    cout << "Enter a number of Task :";
    cin >> number_Of_Task;
    vector<Field> table(number_Of_Task);
    vector<bool> complete(number_Of_Task, false);
    for (int i = 0; i < number_Of_Task; i++)
    {
        Field f;
        f.input(i);
        table[i] = f;
    }
    process(table, complete);
    output(table);
}