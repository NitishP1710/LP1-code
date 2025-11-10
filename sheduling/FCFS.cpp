#include <iostream>
#include <bits/stdc++.h>

using namespace std;
class Field
{
public:
    string job;
    float arrivalTime;
    float runTime;
    float startTime;
    float finishTime;
    float turnAroundTime;
    bool done;
    Field()
    {
        this->job = "";
        this->arrivalTime = 0.0;
        this->runTime = 0.0;
        this->startTime = 0.0;
        this->finishTime = 0.0;
        this->turnAroundTime = 0.0;
    }

    void input(int i)
    {
        cout << "Enter a job " << i << " :" << endl;
        cin >> job;
        cout << "Enter arrival time of job " << i << " :" << endl;
        cin >> arrivalTime;
        cout << "Enter runTime of job " << i << " :" << endl;
        cin >> runTime;
    }
};
void process(vector<Field> &table)
{
    int n = table.size();
    for (int i = 0; i < n; i++)
    {
        Field task = table[i];
        if (i == 0)
        {
            task.startTime = task.arrivalTime;
        }
        else
        {
            Field prevTask = table[i - 1];
            task.startTime = max(prevTask.finishTime, task.arrivalTime);
        }

        task.finishTime = task.startTime + task.runTime;
        task.turnAroundTime = task.finishTime - task.arrivalTime;

        table[i] = task;
    }
}

void output(vector<Field> &table)
{
    int n = table.size();
    cout << "index" << "  " << "JobName" << "  " << "arrivalTime" << "  " << "StartTime" << "  " << "Runtime" << "  " << "finishTime" << "  " << "TurnAroundTime" << endl;
    for (int i = 0; i < n; i++)
    {
        Field p = table[i];
        cout << i << "       ";
        cout << p.job << "       ";
        cout << p.arrivalTime << "           ";
        cout << p.startTime << "           ";
        cout << p.runTime << "         ";
        cout << p.finishTime << "            ";
        cout << p.turnAroundTime;
        cout << endl;
    }
}
int main()
{
    int number_Of_Task;
    cout << "Enter a niumber of Task :";
    cin >> number_Of_Task;
    vector<Field> table(number_Of_Task);
    for (int i = 0; i < number_Of_Task; i++)
    {
        Field f;
        f.input(i);
        table[i] = f;
    }
    process(table);
    output(table);
}