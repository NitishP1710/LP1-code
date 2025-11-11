#include <bits/stdc++.h>
using namespace std;

vector<vector<string>> symboltable = {
    {"1", "219", "X"},
    {"2", "203", "L1"},
    {"3", "208", "NEXT"},
    {"4", "203", "BACK"}
};

vector<vector<string>> literalTable = {
    {"1", "206", "5"},
    {"2", "207", "2"},
    {"3", "211", "1"},
    {"4", "212", "2"},
    {"5", "220", "4"}
};

vector<string> tokenise(const string &line) {
    vector<string> tokens;
    stringstream ss(line);
    string word;
    while (ss >> word) { // automatically splits by whitespace
        tokens.push_back(word);
    }
    return tokens;
}

void solve() {
    fstream fin;
    fin.open("ic.txt");
    if (!fin.is_open()) {
        cout << "Error: cannot open ic.txt\n";
        return;
    }

    string line;
    vector<string> machineCode;

    while (getline(fin, line)) {
        string code = "";
        vector<string> tokens = tokenise(line);

        if (tokens.empty()) continue;

        if (tokens[0] == "-") {
            continue; // assembler directive not processed
        }

        if (tokens.size() == 2) {
            if (tokens[0] != "-") {
                code += tokens[0] + "00  0  000";
                machineCode.push_back(code);
            }
            continue;
        }

        if (tokens[1].substr(1, 2) == "IS") {
            string add = tokens[0];
            string instructioncode = tokens[1].substr(4, 2);
            string regNo = tokens[2];
            string lit_or_sym = tokens[3].substr(1, 1);
            string value = tokens[3].substr(3, tokens[3].size() - 4);

            string last = "";
            if (lit_or_sym == "S") {
                for (int i = 0; i < symboltable.size(); i++) {
                    if (symboltable[i][0] == value) {
                        last = symboltable[i][1];
                        break;
                    }
                }
            } else {
                for (int i = 0; i < literalTable.size(); i++) {
                    if (literalTable[i][0] == value) {
                        last = literalTable[i][1];
                        break;
                    }
                }
            }
            code += add + "+" + instructioncode + "+" + regNo + "+" + last;
            machineCode.push_back(code);
            continue;
        }

        if (tokens[1].substr(1, 2) == "DL") {
            string add = tokens[0];
            string value = tokens[2].substr(3, tokens[2].size() - 4);
            if (tokens[2].substr(1, 1) == "C") {
                code += add + "+" + "0  0   " + value;
            } else {
                code += add + "+" + "0  0   ****";
            }
            machineCode.push_back(code);
            continue;
        }
    }

    fin.close();

    for (int i = 0; i < machineCode.size(); i++) {
        cout << machineCode[i] << endl;
    }
}

int main() {
    solve();
    return 0;
}
