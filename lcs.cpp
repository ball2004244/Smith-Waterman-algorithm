#include <iostream>
#include <string>
#include <vector>

using namespace std;

// create a DP table
void createTable(vector<vector<int>> &DPtable, string s1, string s2)
{

    for (int i = 0; i <= s1.length(); i++)
        DPtable[i][0] = 0;

    for (int j = 0; j <= s2.length(); j++)
        DPtable[0][j] = 0;

    for (int i = 1; i <= s1.length(); i++)
        for (int j = 1; j <= s2.length(); j++)
        {
            if (s1[i - 1] == s2[j - 1])
                DPtable[i][j] = DPtable[i - 1][j - 1] + 1;
            else
                DPtable[i][j] = max(DPtable[i][j - 1], DPtable[i - 1][j]);
        }
}

// Find 1 longest common sequence
string LCS(vector<vector<int>> DPtable, string s1, string s2, int i, int j)
{

    if (i == 0 || j == 0)
        return "";

    if (s1[i - 1] == s2[j - 1])
        return LCS(DPtable, s1, s2, i - 1, j - 1) + s1[i - 1];

    if (DPtable[i][j - 1] > DPtable[i - 1][j])
        return LCS(DPtable, s1, s2, i, j - 1);

    return LCS(DPtable, s1, s2, i - 1, j);
}

// Find all LCS if they have same length
vector<string> find_all_LCS(vector<vector<int>> DPtable, string s1, string s2, int i, int j) {
    vector<string> all_lcs;
    if (i == 0 || j == 0) {
        all_lcs.push_back("");
        return all_lcs;
    }

    if (s1[i - 1] == s2[j - 1]) {
        vector<string> lcs = find_all_LCS(DPtable, s1, s2, i - 1, j - 1);
        for (string x : lcs)
            all_lcs.push_back(x + s1[i - 1]);
    } else {
        if (DPtable[i][j - 1] >= DPtable[i - 1][j]) {
            vector<string> lcs = find_all_LCS(DPtable, s1, s2, i, j - 1);
            for (string x : lcs)
                all_lcs.push_back(x);
        }
        if (DPtable[i - 1][j] >= DPtable[i][j - 1]) {
            vector<string> lcs = find_all_LCS(DPtable, s1, s2, i - 1, j);
            for (string x : lcs)
                all_lcs.push_back(x);
        }
    }
    return all_lcs;
}

void test() {
    //TEST CASES:
    vector<vector<string>> test = {{"abcdefg", "abcdefg"}, {"abcdefg", "abxeg"}, {"abcdefg", "efg"}, {"abc", "def"}, {"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz", "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"}};
    vector<string> expect = {"abcdefg", "abeg", "efg", "", "abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"};
    int i = 0;

    for (auto item : test)
    {
        vector<vector<int>> DPtable(item[0].length() + 1, vector<int>(item[1].length() + 1));
        createTable(DPtable, item[0], item[1]);

        if (LCS(DPtable, item[0], item[1], item[0].length(), item[1].length()) == expect[i])
            cout << "Matched ";
        else
            cout << "Unmatched ";
        ++i;

        vector<string> all_lcs = find_all_LCS(DPtable, item[0], item[1], item[0].length(), item[1].length());
        for (auto x : all_lcs) cout << x << ' ';
        cout << endl;
    }
}


int main()
{
    string s1 = "ACATGCTACACGTATCCGATACCCCGTAACCGATAACGATACACAGACCTCGTACGCTTGCTACAACGTACTCTATAACCGAGAACGATTGACATGCCTCGTACACATGCTACACGTACTCCGAT";
    string s2 = "ACATGCGACACTACTCCGATACCCCGTAACCGATAACGATACAGAGACCTCGTACGCTTGCTAATAACCGAGAACGATTGACATTCCTCGTACAGCTACACGTACTCCGAT";
    
    vector<vector<int>> DPtable(s1.length() + 1, vector<int>(s2.length() + 1));

    createTable(DPtable, s1, s2);
    cout << s1.length() << ' ' << s2.length() << endl;
    cout << LCS(DPtable, s1, s2, s1.length(), s2.length());
    
    return 0;
}