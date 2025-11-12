#include <bits/stdc++.h>
using namespace std;

struct MNT {
    string name;
    int pp, kp, kpdptr, mdtptr;
};
struct MDT {
    int index;
    string opcode, op1, op2;
};
struct KPT {
    int index;
    string parameter, value;
};
struct PNT {
    string macro;
    vector<string> params; // 1-indexed logically
};

vector<MNT> MNTAB;
vector<MDT> MDTAB;
vector<KPT> KPDTAB;
vector<PNT> PNTTAB;

string trim(string s) {
    s.erase(remove_if(s.begin(), s.end(), ::isspace), s.end());
    return s;
}

// ------------------- Parse tables -------------------

void parseMNT() {
    ifstream fin("mnt.txt");
    string name;
    string line;
    getline(fin, line); // header skip
    while (fin >> name) {
        MNT m;
        m.name = name;
        fin >> m.pp >> m.kp >> m.kpdptr >> m.mdtptr;
        MNTAB.push_back(m);
    }
    fin.close();
}

void parseMDT() {
    ifstream fin("mdt.txt");
    MDT m;
    while (fin >> m.index >> m.opcode >> m.op1 >> m.op2) {
        MDTAB.push_back(m);
    }
    fin.close();
}

void parseKPT() {
    ifstream fin("kevalue.txt");
    string line;
    getline(fin, line); // header skip
    while (true) {
        KPT k;
        if (!(fin >> k.index >> k.parameter >> k.value))
            break;
        KPDTAB.push_back(k);
    }
    fin.close();
}

void parsePNT() {
    ifstream fin("pntab.txt");
    string line;
    PNT current;
    while (getline(fin, line)) {
        if (line.empty()) continue;
        if (line[0] != '(') {  // macro name line
            if (!current.macro.empty()) {
                PNTTAB.push_back(current);
                current.params.clear();
            }
            current.macro = trim(line);
        } else {
            // remove ( and )
            line.erase(remove(line.begin(), line.end(), '('), line.end());
            line.erase(remove(line.begin(), line.end(), ')'), line.end());
            // format: 1-O
            size_t dash = line.find('-');
            if (dash != string::npos) {
                string param = line.substr(dash + 1);
                current.params.push_back(param);
            }
        }
    }
    if (!current.macro.empty())
        PNTTAB.push_back(current);
    fin.close();
}

// ------------------- Expansion logic -------------------

int findMNTIndex(string name) {
    for (int i = 0; i < MNTAB.size(); i++)
        if (MNTAB[i].name == name)
            return i;
    return -1;
}

PNT findPNT(string name) {
    for (auto &p : PNTTAB)
        if (p.macro == name)
            return p;
    return {};
}

void expandMacro(string name, vector<string> args, ofstream &fout) {
    int idx = findMNTIndex(name);
    if (idx == -1) return;

    MNT m = MNTAB[idx];
    PNT p = findPNT(name);

    // Build APTAB
    unordered_map<string, string> APTAB;

    // Positional params
    for (int i = 0; i < m.pp && i < args.size(); i++) {
        APTAB[p.params[i]] = args[i];
    }

    // Default keyword values
    int kpIndex = m.kpdptr - 1;
    for (int i = 0; i < m.kp; i++) {
        if (kpIndex + i < KPDTAB.size()) {
            APTAB[KPDTAB[kpIndex + i].parameter] = KPDTAB[kpIndex + i].value;
        }
    }

    // Keyword overrides from call
    for (string a : args) {
        size_t eq = a.find('=');
        if (eq != string::npos) {
            string key = a.substr(0, eq);
            string val = a.substr(eq + 1);
            APTAB[key] = val;
        }
    }

    // Expand MDT
    int i = m.mdtptr - 1;
    while (i < MDTAB.size()) {
        MDT d = MDTAB[i];
        if (d.opcode == "MEND") break;

        string op1 = d.op1, op2 = d.op2;
        // Replace (P-n)
        if (op1.find("(P-") != string::npos) {
            int pos = op1[3] - '0';
            op1 = APTAB[p.params[pos - 1]];
        }
        if (op2.find("(P-") != string::npos) {
            int pos = op2[3] - '0';
            op2 = APTAB[p.params[pos - 1]];
        }

        fout << d.opcode << " " << op1;
        if (op2 != "()") fout << ", " << op2;
        fout << "\n";

        i++;
    }
}

// ------------------- MAIN -------------------

int main() {
    parseMNT();
    parseMDT();
    parseKPT();
    parsePNT();

    ifstream fin("input.txt");
    ofstream fout("output.txt");

    string line;
    while (getline(fin, line)) {
        if (line.empty()) continue;

        stringstream ss(line);
        string first;
        ss >> first;

        int idx = findMNTIndex(first);
        if (idx == -1) {
            fout << line << "\n";  // not a macro
            continue;
        }

        string argsPart;
        getline(ss, argsPart);
        if (argsPart.empty()) { expandMacro(first, {}, fout); continue; }

        argsPart.erase(remove(argsPart.begin(), argsPart.end(), ' '), argsPart.end());
        vector<string> args;
        string temp;
        stringstream s2(argsPart);
        while (getline(s2, temp, ',')) args.push_back(temp);

        expandMacro(first, args, fout);
    }

    cout << "\nMacro Expansion Done! Check output.txt\n";
    return 0;
}
