#include <iostream>
#include <vector>
using namespace std;

void bestFit(vector<int> blockSize, vector<int> processSize)
{
    int m = blockSize.size();
    int n = processSize.size();
    vector<int> allocation(n, -1);

    for (int i = 0; i < n; i++)
    {
        int bestIdx = -1;
        for (int j = 0; j < m; j++)
        {
            if (blockSize[j] >= processSize[i])
            {
                if (bestIdx == -1 || blockSize[j] < blockSize[bestIdx])
                {
                    bestIdx = j;
                }
            }
        }
        if (bestIdx != -1)
        {
            allocation[i] = bestIdx;
            blockSize[bestIdx] -= processSize[i];
        }
    }

    cout << "\nProcess No.\tProcess Size\tBlock No.\n";
    for (int i = 0; i < n; i++)
    {
        cout << " " << i + 1 << "\t\t" << processSize[i] << "\t\t";
        if (allocation[i] != -1)
            cout << allocation[i] + 1 << endl;
        else
            cout << "Not Allocated\n";
    }
}

void firstFit(vector<int> blockSize, vector<int> processSize)
{
    int m = blockSize.size();
    int n = processSize.size();
    vector<int> allocation(n, -1);

    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            if (blockSize[j] >= processSize[i])
            {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                break;
            }
        }
    }

    cout << "\nProcess No.\tProcess Size\tBlock No.\n";
    for (int i = 0; i < n; i++)
    {
        cout << " " << i + 1 << "\t\t" << processSize[i] << "\t\t";
        if (allocation[i] != -1)
            cout << allocation[i] + 1 << endl;
        else
            cout << "Not Allocated"<<endl;
    }
}

void nextFit(vector<int> blockSize, vector<int> processSize)
{
    int m = blockSize.size();
    int n = processSize.size();
    vector<int> allocation(n, -1);
    int j = 0; 

    for (int i = 0; i < n; i++)
    {
        int count = 0;
        while (count < m)
        {
            if (blockSize[j] >= processSize[i])
            {
                allocation[i] = j;
                blockSize[j] -= processSize[i];
                break;
            }
            j = (j + 1) % m;
            count++;
        }
    }

    cout << "\nProcess No.\tProcess Size\tBlock No.\n";
    for (int i = 0; i < n; i++)
    {
        cout << " " << i + 1 << "\t\t" << processSize[i] << "\t\t";
        if (allocation[i] != -1)
            cout << allocation[i] + 1 << "\n";
        else
            cout << "Not Allocated\n";
    }
}

void worstFit(vector<int> blockSize, vector<int> processSize)
{
    int m = blockSize.size();
    int n = processSize.size();
    vector<int> allocation(n, -1);

    for (int i = 0; i < n; i++)
    {
        int worstIdx = -1;
        for (int j = 0; j < m; j++)
        {
            if (blockSize[j] >= processSize[i])
            {
                if (worstIdx == -1 || blockSize[j] > blockSize[worstIdx])
                {
                    worstIdx = j;
                }
            }
        }
        if (worstIdx != -1)
        {
            allocation[i] = worstIdx;
            blockSize[worstIdx] -= processSize[i];
        }
    }

    cout << "\nProcess No.\tProcess Size\tBlock No.\n";
    for (int i = 0; i < n; i++)
    {
        cout << " " << i + 1 << "\t\t" << processSize[i] << "\t\t";
        if (allocation[i] != -1)
            cout << allocation[i] + 1 << "\n";
        else
            cout << "Not Allocated\n";
    }
}

int main()
{
    int m, n, choice;
    cout << "Enter number of memory blocks: ";
    cin >> m;
    cout << "Enter block sizes: "<<endl;
    vector<int> blockSize(m);
    for (int i = 0; i < m; i++)
    {
        cin >> blockSize[i];
    }

    cout << "Enter number of processes: ";
    cin >> n;
    cout << "Enter process sizes: "<<endl;
    vector<int> processSize(n);
    for (int i = 0; i < n; i++)
    {
        cin >> processSize[i];
    }

    do
    {
        cout << "\nMemory Allocation Strategies Menu:\n";
        cout << "1. First Fit"<<endl;
        cout << "2. Best Fit" << endl;
        cout << "3. Next Fit"<<endl;
        cout << "4. Worst Fit"<<endl;
        cout << "5. Exit"<<endl;
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
        case 1:
            firstFit(blockSize, processSize);
            break;
        case 2:
            bestFit(blockSize, processSize);
            break;
        case 3:
            nextFit(blockSize, processSize);
            break;
        case 4:
            worstFit(blockSize, processSize);
            break;
        case 5:
            cout << "Exiting program."<<endl;
            break;
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    } while (choice != 5);

    return 0;
}

// Enter number of memory blocks : 5 
// Enter block sizes : 100 500 200 300 600 
// Enter number of processes : 4 
// Enter process sizes : 212 417 112 426