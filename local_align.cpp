#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

/*
    SMITH WATERMAN Algorithm
*/

// function to return the max value from 4 integers
int getMax(int a, int b, int c, int d)
{
    return max(max(a, b), max(c, d));
}

// calculate the score for each cell in the DP table
int calculateScores(vector<vector<int>> &DPtable, string s1, string s2, int i, int j, int match_val = 3, int mismatch_val = -3, int gap = 2)
{
    int score = 0;
    if (s1[i - 1] == s2[j - 1])
        score = max(DPtable[i - 1][j - 1] + match_val, 0);
    else
        score = getMax(DPtable[i - 1][j - 1] + mismatch_val, DPtable[i - 1][j] + gap, DPtable[i][j - 1] + gap, 0);

    return score;
}

// create a DP table
void createTable(vector<vector<int>> &DPtable, int &max_val, string s1, string s2, int match_val = 3, int mismatch_val = -3, int gap = 2)
{
    for (int i = 0; i <= s1.length(); i++)
    {
        for (int j = 0; j <= s2.length(); j++)
        {
            int score = 0;
            if (i == 0 || j == 0)
            {
                DPtable[i][j] = score;
                continue;
            }

            score = calculateScores(DPtable, s1, s2, i, j, match_val, mismatch_val, gap);
            DPtable[i][j] = score;
            max_val = max(max_val, score);
        }
    }
}

void printTable(vector<vector<int>> DPtable, string s1, string s2)
{
    for (int i = 0; i <= s1.length(); i++)
    {
        if (i == 0)
        {
            cout << "    ";
            for (int j = 0; j <= s2.length(); j++)
                cout << s2[j] << ' ';
            cout << endl;
        }

        for (int j = 0; j <= s2.length(); j++)
        {
            if (j == 0)
                if (i == 0)
                    cout << "  ";
                else
                    cout << s1[i - 1] << ' ';

            cout << DPtable[i][j] << ' ';
        }
        cout << endl;
    }
}

// check if current cell is a match
bool isMatch(int score, int diagonal, char s1_char, char s2_char, int match_val)
{
    return score == diagonal + (s1_char == s2_char ? match_val : 0);
}

// check if current cell is a deletion
bool isDeletion(int score, int above, int gap)
{
    return score == above - gap;
}

// check if current cell is an insertion
bool isInsertion(int score, int left, int gap)
{
    return score == left - gap;
}

void traceBack(vector<vector<int>> DPtable, string s1, string s2, int i, int j, int num, int match_val = 3, int gap = 2)
{
    int match = 0, mismatch = 0, gap_count = 0, opening_gap = 0;
    string align1 = "", align2 = "";

    cout << "Case " << num << endl;
    // start traceback from the cell with highest score 
    while (i > 0 && j > 0)
    {
        int score = DPtable[i][j];

        if (score == 0)
            break;

        if (isMatch(score, DPtable[i - 1][j - 1], s1[i - 1], s2[j - 1], match_val))
        {
            align1 = s1[--i] + align1;
            align2 = s2[--j] + align2;
            match++;
        }
        else if (isDeletion(score, DPtable[i - 1][j], gap))
        {
            align1 = s1[--i] + align1;
            align2 = "-" + align2;
            if (opening_gap == 0)
                opening_gap++;
            gap_count++;
        }
        else if (isInsertion(score, DPtable[i][j - 1], gap))
        {
            align1 = "-" + align1;
            align2 = s2[--j] + align2;
            if (opening_gap == 0)
                opening_gap++;
        }
        else
        {
            align1 = s1[--i] + align1;
            align2 = s2[--j] + align2;
            mismatch++;
        }
    }

    cout << "Alig 1: " << align1 << endl;
    cout << "Align 2: " << align2 << endl;
    cout << "Match: " << match << " | ";
    cout << "Mismatch: " << mismatch << " | ";
    cout << "Gap: " << gap_count << " | ";
    cout << "Opening gap: " << opening_gap << endl;
    cout << endl;
}

void handleMultipleOutput(vector<vector<int>> DPtable, int max_val, string s1, string s2, int match_val = 3, int gap = 2) {
    // loop through table to get position of all cells with max_val
    vector<vector<int>> pos;
    int num = 0;
    for (int i = 0; i <= s1.length(); i++) {
        for (int j = 0; j <= s2.length(); j++) if (DPtable[i][j] == max_val) pos.push_back({i, j});
    }

    for (auto container : pos) {
        int i = container[0], j = container[1];
        traceBack(DPtable, s1, s2, i, j, ++num, match_val, gap);
    }
}

int main()
{
    // string s1 = "ACATGCTACACGTATCCGATACCCCGTAACCGATAACGATACACAGACCTCGTACGCTTGCTACAACGTACTCTATAACCGAGAACGATTGACATGCCTCGTACACATGCTACACGTACTCCGAT";
    // string s2 = "ACATGCGACACTACTCCGATACCCCGTAACCGATAACGATACAGAGACCTCGTACGCTTGCTAATAACCGAGAACGATTGACATTCCTCGTACAGCTACACGTACTCCGAT";
    string s1, s2;
    int max_val = INT_MIN;
    int match_val, mismatch_val, gap;

    cout << "Smith-Waterman Algorithm" << endl;
    cout << "1st string: ";
    cin >> s1;

    cout << "2nd string: ";
    cin >> s2;
    
    cout << "Match: ";
    cin >> match_val;

    cout << "Mismatch: ";
    cin >> mismatch_val;


    cout << "Gap: ";
    cin >> gap;
    
    cout << "----------" << endl;
    
    vector<vector<int>> DPtable(s1.length() + 1, vector<int>(s2.length() + 1));
    createTable(DPtable, max_val, s1, s2, match_val, mismatch_val, gap);
    // printTable(DPtable, s1, s2);
    handleMultipleOutput(DPtable, max_val, s1, s2, match_val, gap);

    return 0;
}