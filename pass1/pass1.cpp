#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <map>
#include <iomanip>
using namespace std;

// --- Global Data Structures ---
map<string, pair<string, int>> MOT = {
    {"STOP", {"IS", 0}}, {"ADD", {"IS", 1}}, {"SUB", {"IS", 2}},
    {"MULT", {"IS", 3}}, {"MOVER", {"IS", 4}}, {"MOVEM", {"IS", 5}},
    {"COMP", {"IS", 6}}, {"BC", {"IS", 7}}, {"DIV", {"IS", 8}},
    {"READ", {"IS", 9}}, {"PRINT", {"IS", 10}}
};

map<string, pair<string, int>> AD = {
    {"START", {"AD", 1}}, {"END", {"AD", 2}},
    {"ORIGIN", {"AD", 3}}, {"EQU", {"AD", 4}},
    {"LTORG", {"AD", 5}}
};

map<string, pair<string, int>> DL = {
    {"DC", {"DL", 1}}, {"DS", {"DL", 2}}
};

map<string, int> REG = { {"AREG", 1}, {"BREG", 2}, {"CREG", 3}, {"DREG", 4} };
map<string, int> CC = { {"LT", 1}, {"LE", 2}, {"EQ", 3}, {"GT", 4}, {"GE", 5}, {"ANY", 6} };

struct Symbol { string name; int addr; bool defined = false; };
struct Literal { string lit; int addr; };

vector<Symbol> symtab;
vector<Literal> littab;
vector<int> pooltab;

int LC = 0;
int literalPoolStart = 0;

// --- Utility Functions ---
int searchSymbol(string sym) {
    for (int i = 0; i < (int)symtab.size(); i++)
        if (symtab[i].name == sym) return i;
    return -1;
}

int searchLiteral(string lit) {
    for (int i = 0; i < (int)littab.size(); i++)
        if (littab[i].lit == lit) return i;
    return -1;
}

// --- Process Literals ---
void processLiterals(ofstream &icFile) {
    if ((int)littab.size() > literalPoolStart)
        pooltab.push_back(literalPoolStart + 1);

    for (int i = literalPoolStart; i < (int)littab.size(); i++) {
        if (littab[i].addr == -1) {
            icFile << LC << " (DL,01) (C,"
                   << littab[i].lit.substr(2, littab[i].lit.length() - 3)
                   << ")\n";
            littab[i].addr = LC;
            LC++;
        }
    }
    literalPoolStart = littab.size();
}

// --- Process Line ---
void processLine(vector<string> tokens, ofstream &icFile) {
    if (tokens.empty()) return;

    // START
    if (tokens[0] == "START") {
        LC = stoi(tokens[1]);
        icFile << LC << " (AD,01) (C," << tokens[1] << ")\n";
        return;
    }

    int idx = 0;
    string potentialLabel = tokens[0];

    // Label Handling
    if (MOT.count(potentialLabel) == 0 && DL.count(potentialLabel) == 0 && AD.count(potentialLabel) == 0) {
        int pos = searchSymbol(potentialLabel);
        if (pos == -1) {
            symtab.push_back({potentialLabel, LC, false});
            pos = symtab.size() - 1;
        }
        if (symtab[pos].defined) {
            cerr << "Error: Duplicate label definition: " << potentialLabel << endl;
        } else {
            symtab[pos].addr = LC;
            symtab[pos].defined = true;
        }
        idx = 1;
    }

    if ((int)tokens.size() <= idx) return;
    string op = tokens[idx];

    // --- Assembler Directives ---
    if (AD.count(op)) {
        if (op == "END") {
            processLiterals(icFile);
            icFile << "(AD,02)\n";
        }
        else if (op == "LTORG") {
            icFile << "(AD,05)\n";
            processLiterals(icFile);
        }
        else if (op == "ORIGIN") {
            string expr = tokens[idx + 1];
            int newLC = 0;

            size_t plusPos = expr.find('+');
            size_t minusPos = expr.find('-');

            if (plusPos != string::npos || minusPos != string::npos) {
                string sym = expr.substr(0, (plusPos != string::npos) ? plusPos : minusPos);
                int symIndex = searchSymbol(sym);

                int baseAddr = 0;
                if (symIndex != -1 && symtab[symIndex].addr != -1)
                    baseAddr = symtab[symIndex].addr;
                else
                    baseAddr = LC; // fallback to current LC if not found

                int offset = stoi(expr.substr((plusPos != string::npos) ? plusPos + 1 : minusPos + 1));
                if (minusPos != string::npos) offset = -offset;

                newLC = baseAddr + offset;
            } else {
                newLC = stoi(expr);
            }

            icFile << "(AD,03) (C," << newLC << ")\n";
            LC = newLC;
        }
        else if (op == "EQU") {
            int symPos = searchSymbol(tokens[idx - 1]);
            if (symPos != -1) {
                int equValue = stoi(tokens[idx + 1]);
                symtab[symPos].addr = equValue;
                icFile << "(AD,04) (C," << equValue << ")\n";
            }
        }
        return;
    }

    int currentLC = LC;

    // --- Imperative Statements ---
    if (MOT.count(op)) {
        icFile << currentLC << " (IS," << setw(2) << setfill('0') << MOT[op].second << ") ";

        for (int i = idx + 1; i < (int)tokens.size(); i++) {
            if (REG.count(tokens[i])) {
                icFile << "(R," << REG[tokens[i]] << ") ";
            }
            else if (CC.count(tokens[i])) {
                icFile << "(CC," << CC[tokens[i]] << ") ";
            }
            else if (tokens[i][0] == '=') {
                int litIndex = searchLiteral(tokens[i]);
                if (litIndex == -1) {
                    littab.push_back({tokens[i], -1});
                    litIndex = littab.size() - 1;
                }
                icFile << "(L," << litIndex + 1 << ") ";
            }
            else {
                int pos = searchSymbol(tokens[i]);
                if (pos == -1) {
                    symtab.push_back({tokens[i], -1, false});
                    pos = symtab.size() - 1;
                }
                icFile << "(S," << pos + 1 << ") ";
            }
        }
        icFile << "\n";
        LC++;
    }

    // --- Declarative Statements ---
    else if (DL.count(op)) {
        if (op == "DS") {
            int size = stoi(tokens[idx + 1]);
            icFile << currentLC << " (DL,02) (C," << size << ")\n";
            LC += size;
        }
        else if (op == "DC") {
            string val = tokens[idx + 1];
            if (val.front() == '\'' && val.back() == '\'')
                val = val.substr(1, val.size() - 2);
            icFile << currentLC << " (DL,01) (C," << val << ")\n";
            LC++;
        }
    }
}

// --- Main ---
int main() {
    ifstream inFile("input.txt");
    ofstream icFile("intermediate.txt");
    ofstream symFile("symtab.txt");
    ofstream litFile("littab.txt");
    ofstream poolFile("pooltab.txt");

    if (!inFile) {
        cerr << "Error: input.txt not found!\n";
        return 1;
    }

    string line;
    while (getline(inFile, line)) {
        stringstream ss(line);
        vector<string> tokens;
        string word;
        while (ss >> word) {
            if (word.back() == ',') word.pop_back();
            tokens.push_back(word);
        }
        processLine(tokens, icFile);
    }

    if ((int)littab.size() > literalPoolStart)
        pooltab.push_back(literalPoolStart + 1);

    symFile << "Index\tSymbol\tAddress\n";
    for (int i = 0; i < (int)symtab.size(); i++)
        symFile << i + 1 << "\t" << symtab[i].name << "\t" << symtab[i].addr << "\n";

    litFile << "Index\tLiteral\tAddress\n";
    for (int i = 0; i < (int)littab.size(); i++)
        litFile << i + 1 << "\t" << littab[i].lit << "\t" << littab[i].addr << "\n";

    poolFile << "Pool#\tStartIndex\n";
    for (int i = 0; i < (int)pooltab.size(); i++)
        poolFile << i + 1 << "\t" << pooltab[i] << "\n";

    cout << "Pass 1 completed successfully.\n";
    cout << "Generated: intermediate.txt, symtab.txt, littab.txt, pooltab.txt\n";
    return 0;
}
