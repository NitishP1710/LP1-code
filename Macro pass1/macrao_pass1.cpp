#include <iostream>
#include <bits/stdc++.h>
using namespace std;

struct macroName
{
    string name;
    int positionalParam;
    int keywordParam;
    int kpdptr;
    int mdtptr;
};
struct parameterName
{
    int position;
    string parameter;
};

struct keyValue
{
    int pointer;
    string name;
    string value;
};
struct defination
{
    int ptr; // address
    string instruction;
    string token1;
    string token2;
};

vector<pair<string, vector<parameterName>>> parameterNameTable; //{macroname->parameterNameTable}
vector<keyValue> keyValueParameterTable;
vector<macroName> macroNameTable;
vector<defination> macroDefination;
int countPP(string token)
{
    int count = 0;
    for (char c : token)
    {
        if (c == '&')
        {
            count++;
        }
    }
    return count;
}
int countKP(string token)
{
    int count = 0;
    for (char c : token)
    {
        if (c == '=')
        {
            count++;
        }
    }
    return count;
}
void process(string input)
{
    ifstream fin;
    fin.open("input.txt");
    if (!fin)
    {
        cout << "NO input" << endl;
    }
    string label, opcode, token1, token2;
    bool isMacro = false; // current opcode is MACRO
    string currentMacroname = "";
    int KPDPTR = 1;
    int MDTPTR = 1;
    while (!fin.eof())
    {
        fin >> label >> opcode >> token1 >> token2;
        if (opcode == "MACRO")
        {
            isMacro = true; // skip this line by setting isMACRO true
            continue;
        }
        if (opcode == "MEND")
        {
            defination d;
            d.instruction = "MEND";
            d.ptr = MDTPTR;
            d.token1 = "";
            d.token2 = "";
            MDTPTR++;
            macroDefination.push_back(d);
            continue;
        }
        if (isMacro)
        {
            // fill the macro name table
            // token1 have all position parameter
            // token2 have all keyword parameter
            vector<parameterName> currentmacroParameterTable;
            currentMacroname = opcode;
            macroName M;
            M.name = currentMacroname;
            int pp = countPP(token1);
            int kp = countKP(token2);
            M.positionalParam = pp;
            M.keywordParam = kp;
            M.kpdptr = KPDPTR;
            M.mdtptr = MDTPTR;
            macroNameTable.push_back(M);
            // fill parameter name table
            int index = 0;
            string para = "";
            for (char c : token1)
            {
                if (c == '&' && index == 0)
                {
                    index++;
                    continue; // first and no matter
                }

                if (c != '&')
                {
                    para += c;
                }

                else
                {
                    parameterName P;
                    P.parameter = para;
                    para = "";
                    P.position = index;
                    index++;
                    currentmacroParameterTable.push_back(P);
                }
            }
            if (para != "")
            {
                parameterName P;
                P.parameter = para;
                para = "";
                P.position = index;
                index++;
                currentmacroParameterTable.push_back(P);
            }
            if (token2 != "NAN")
            {
                parameterName P;
                string s1;
                int pos = token2.find('=');
                if (pos != string::npos)
                {
                    s1 = token2.substr(1, pos - 1);
                }
                P.parameter = s1;
                P.position = index;
                currentmacroParameterTable.push_back(P);
            }
            parameterNameTable.push_back({currentMacroname, currentmacroParameterTable});
            // currently considering only one key value parameter in code
            string s1, s2;
            int pos = token2.find('=');
            if (pos != string::npos)
            {
                s1 = token2.substr(1, pos - 1); // skip '&'
                s2 = token2.substr(pos + 1);
            }
            keyValue k;
            k.pointer = KPDPTR;
            KPDPTR++;
            k.name = s1;
            k.value = s2;
            keyValueParameterTable.push_back(k);
            isMacro = false;
            continue;
        }
        int ind = -1;
        // find actual index of pntable for current macro name
        for (int i = 0; i < parameterNameTable.size(); i++)
        {
            if (parameterNameTable[i].first == currentMacroname)
            {
                ind = i;
                break;
            }
        }
        if(opcode=="START"){
            break;
        }
        if (ind != -1)
        {
            vector<parameterName> currentPNTab = parameterNameTable[ind].second;
            defination D;
            D.instruction = opcode;
            int token1_ind = -1;
            int token2_ind = -1;
            for (auto &entry : currentPNTab)
            {
                if (entry.parameter == token1.substr(1))
                {
                    token1_ind = entry.position;
                }
                if (entry.parameter == token2.substr(1))
                {
                    token2_ind = entry.position;
                }
            }
            D.ptr = MDTPTR;
            MDTPTR++;
            D.token1 = "(P " + to_string(token1_ind) + ")";
            D.token2 = "(P " + to_string(token2_ind) + ")";
            macroDefination.push_back(D);
        }
    }
}

int main()
{
    process("input.txt");
    cout << "\n=== Macro Name Table (MNT) ===\n";
    cout << "Name\tPP\tKP\tKPD_PTR\tMDT_PTR\n";
    for (auto &m : macroNameTable)
    {
        cout << m.name << "\t" << m.positionalParam << "\t" << m.keywordParam
             << "\t" << m.kpdptr << "\t" << m.mdtptr << endl;
    }

    cout << "\n=== Parameter Name Table (PNTAB) ===\n";
    for (auto &entry : parameterNameTable)
    {
        cout << "Macro: " << entry.first << endl;
        for (auto &p : entry.second)
            cout << "\t" << p.position << " -> " << p.parameter << endl;
    }

    cout << "\n=== Keyword Parameter Table (KPTAB) ===\n";
    cout << "Ptr\tName\tDefaultValue\n";
    for (auto &kv : keyValueParameterTable)
        cout << kv.pointer << "\t" << kv.name << "\t" << kv.value << endl;

    cout << "\n=== Macro Definition Table (MDT) ===\n";
    cout << "Ptr\tInstruction\tOp1\tOp2\n";
    for (auto &d : macroDefination)
        cout << d.ptr << "\t" << d.instruction << "\t" << d.token1 << "\t" << d.token2 << endl;
}
