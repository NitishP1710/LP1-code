#include <bits/stdc++.h>
using namespace std;

class Ring
{
    int n;                 // number of processes
    vector<int> processes; // process IDs
    int coordinator;       // current coordinator

public:
    void input()
    {
        cout << "Enter number of processes: ";
        cin >> n;

        processes.resize(n);
        cout << "Enter process IDs in ring order: ";
        for (int i = 0; i < n; i++)
            cin >> processes[i];

        coordinator = *max_element(processes.begin(), processes.end());
        cout << "Initial Coordinator is Process " << coordinator << endl;
    }

    void startElection(int initiator)
    {
        cout << "\nProcess " << initiator << " has detected coordinator failure!" << endl;
        cout << "Election initiated by Process " << initiator << endl;

        vector<int> electionList;
        int startIndex = find(processes.begin(), processes.end(), initiator) - processes.begin();
        int index = (startIndex + 1) % n;

        electionList.push_back(initiator);
        while (processes[index] != initiator)
        {
            electionList.push_back(processes[index]);
            index = (index + 1) % n;
        }

        coordinator = *max_element(electionList.begin(), electionList.end());
        cout << "Election message circulated: ";
        for (int id : electionList)
            cout << id << " ";
        cout << endl;

        cout << "Process " << coordinator << " becomes the new Coordinator." << endl;
        cout << "Coordinator message circulated around ring: COORDINATOR (" << coordinator << ")" << endl;
    }

    void failCoordinator()
    {
        cout << "\nEnter the failed coordinator ID: ";
        int failed;
        cin >> failed;

        if (failed != coordinator)
        {
            cout << "Error: Process " << failed << " is not the current coordinator!" << endl;
            return;
        }

        cout << "Coordinator Process " << coordinator << " has failed." << endl;
        processes.erase(remove(processes.begin(), processes.end(), failed), processes.end());
        n = processes.size();

        cout << "Enter process ID that will start election: ";
        int initiator;
        cin >> initiator;

        if (find(processes.begin(), processes.end(), initiator) == processes.end())
        {
            cout << "Invalid initiator process ID!" << endl;
            return;
        }

        startElection(initiator);
    }
};

int main()
{
    Ring r;
    r.input();
    r.failCoordinator();
    return 0;
}
