#include <bits/stdc++.h>
using namespace std;

class Bully
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
        cout << "Enter process IDs: ";
        for (int i = 0; i < n; i++)
            cin >> processes[i];

        coordinator = *max_element(processes.begin(), processes.end());
        cout << "Initial Coordinator is Process " << coordinator << endl;
    }

    void startElection(int initiator)
    {
        cout << "\nProcess " << initiator << " has detected coordinator failure!" << endl;
        cout << "Election initiated by Process " << initiator << endl;
        vector<int> higher;
        for (int p : processes)
        {
            if (p > initiator)
                higher.push_back(p);
        }

        if (higher.empty())
        {
            coordinator = initiator;
            cout << "No higher process found. Process " << initiator << " becomes the new Coordinator.\n";
            return;
        }
        cout << "Process " << initiator << " sends ELECTION to: ";
        for (int h : higher)
            cout << h << " ";
        cout << endl;

        cout << "Processes ";
        for (int h : higher)
            cout << h << " ";
        cout << "respond with OK." << endl;
        coordinator = *max_element(higher.begin(), higher.end());

        cout << "Process " << coordinator << " becomes the new Coordinator." << endl;
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
    Bully b;
    b.input();
    b.failCoordinator();
    return 0;
}
