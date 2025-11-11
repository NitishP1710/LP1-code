#include <bits/stdc++.h>
using namespace std;

struct macroName {
    string name;
    int positionalParam;
    int keywordParam;
    int kpdptr;
    int mdtptr;
};
struct parameterName {
    int position;
    string parameter;
};
struct keyValue {
    int pointer;
    string name;
    string value;
};
struct defination {
    int ptr; // address
    string instruction;
    string token1;
    string token2;
};
struct actual {
    int position;
    string value;
};

vector<pair<string, vector<parameterName>>> PNTTAB;
vector<defination> MDTAB;
vector<macroName> MNTAB;
vector<keyValue> KPDTAB;
vector<pair<string,vector<actual>>> APTAB;

void createApt(){
    vector<actual> v;
    string name=PNTTAB[0].first;
    vector<parameterName> v=PNTTAB[0].second;
    actual a,b;
    a.position=1;
    a.value=v[0].value;

}
void parseMDT() {
    ifstream fin("mdt.txt");
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        string token = "";
        vector<string> current;
        for (char c : line) {
            if (c != ',' && c != '(' && c != ')')
                token += c;
            else {
                if (!token.empty()) current.push_back(token);
                token = "";
            }
        }
        if (!token.empty()) current.push_back(token);

        defination d;
        d.ptr = stoi(current[0]);
        d.instruction = current.size() > 1 ? current[1] : "";
        d.token1 = current.size() > 2 ? current[2] : "";
        d.token2 = current.size() > 3 ? current[3] : "";
        MDTAB.push_back(d);
    }
}

void parseMNT() {
    ifstream fin("mnt.txt");
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        string token = "";
        vector<string> current;
        for (char c : line) {
            if (c != ',' && c != '(' && c != ')')
                token += c;
            else {
                if (!token.empty()) current.push_back(token);
                token = "";
            }
        }
        if (!token.empty()) current.push_back(token);

        if (current.size() < 5) continue;

        macroName m;
        m.name = current[0];
        m.positionalParam = stoi(current[1]);
        m.keywordParam = stoi(current[2]);
        m.kpdptr = stoi(current[3]);
        m.mdtptr = stoi(current[4]);
        MNTAB.push_back(m);
    }
}
void parseKPT() {
    ifstream fin("kevalue.txt");
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        string token = "";
        vector<string> current;

        for (char c : line) {
            if (c != ',' && c != '(' && c != ')')
                token += c;
            else {
                if (!token.empty()) {
                    current.push_back(token);
                    token = "";
                }
            }
        }
        if (!token.empty()) current.push_back(token);  
        if (current.size() < 2) continue;
        keyValue k;
        k.pointer = stoi(current[0]);
        k.name = current.size() > 1 ? current[1] : "";
        k.value = current.size() > 2 ? current[2] : "";
        KPDTAB.push_back(k);
    }
}
void parsepnt() {
    ifstream fin("pntab.txt");
    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        string token = "";
        vector<string> current;
        for (char c : line) {
            if (c != ',' && c != '(' && c != ')')
                token += c;
            else {
                if (!token.empty()) current.push_back(token);
                token = "";
            }
        }
        if (!token.empty()) current.push_back(token);

        if (current.size() < 2) continue;

        string currentMacro = current[0];
        vector<parameterName> v;
        for (int i = 1; i < current.size(); i++) {
            parameterName p;
            p.parameter = current[i];
            p.position = i;
            v.push_back(p);
        }
        PNTTAB.push_back({currentMacro, v});
    }
}

// ------------------- PRINT -------------------
void printMDT() {
    cout << "\n--- MDTAB ---\n";
    cout << "PTR\tINSTRUCTION\tTOKEN1\tTOKEN2\n";
    for (auto &d : MDTAB)
        cout << d.ptr << "\t" << d.instruction << "\t\t" << d.token1 << "\t" << d.token2 << endl;
}

void printMNT() {
    cout << "\n--- MNTAB ---\n";
    cout << "NAME\tPP\tKP\tKPD_PTR\tMDT_PTR\n";
    for (auto &m : MNTAB)
        cout << m.name << "\t" << m.positionalParam << "\t" << m.keywordParam << "\t" << m.kpdptr << "\t" << m.mdtptr << endl;
}

void printKPT() {
    cout << "\n--- KPDTAB ---\n";
    cout << "PTR\tNAME\tVALUE\n";
    for (auto &k : KPDTAB)
        cout << k.pointer << "\t" << k.name << "\t" << k.value << endl;
}

void printPNT() {
    cout << "\n--- PNTTAB ---\n";
    for (auto &macro : PNTTAB) {
        cout << "Macro: " << macro.first << endl;
        cout << "Pos\tParameter\n";
        for (auto &p : macro.second)
            cout << p.position << "\t" << p.parameter << endl;
    }
}

// ------------------- MAIN -------------------

int main() {
    parseMDT();
    parseMNT();
    parseKPT();
    parsepnt();

    printMDT();
    printMNT();
    printKPT();
    printPNT();

    return 0;
}
