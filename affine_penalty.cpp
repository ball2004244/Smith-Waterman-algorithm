#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

/*
    SMITH WATERMAN Algorithm (Space Optimized + Affine Gap Penalty)
*/

// find max score inside a map
int findMax(map<pair<int, int>, int> DPmap)
{
    int max_score = INT_MIN;

    for (auto item : DPmap)
    {
        max_score = max(max_score, item.second);
    }
    return max_score;
}
// create a map with pairs to store (i, j) instead of orignial 2D table
map<pair<int, int>, int> smithWaterman(string s1, string s2, int match, int mismatch, int gap)
{
    int score = 0;
    int above = 0;
    int left = 0;
    map<pair<int, int>, int> DPmap;

    for (int i = 1; i < s1.length() + 1; i++)
    {
        for (int j = 1; j < s2.length() + 1; j++)
        {
            if (s1[i - 1] == s2[j - 1])
            {
                score = match;
                if (DPmap.count({i - 1, j - 1}))
                    score += DPmap[{i - 1, j - 1}];
            }
            else
            {
                score = mismatch;
                if (DPmap.count({i - 1, j - 1}))
                    score += DPmap[{i - 1, j - 1}];
            }
            above = gap;

            if (DPmap.count({i - 1, j}))
                above += DPmap[{i - 1, j}];

            left = gap;

            if (DPmap.count({i, j - 1}))
                left += DPmap[{i, j - 1}];

            if (max({score, above, left}) > 0)
                DPmap[{i, j}] = max({score, above, left});
        }
    }
    return DPmap;
}

void printMap(map<pair<int, int>, int> DPmap, string s1, string s2)
{
    cout << "  ";
    for (int i = 0; i < s2.length(); i++)
    {
        cout << s2[i] << " ";
    }
    cout << endl;

    for (int i = 1; i < s1.length() + 1; i++)
    {
        cout << s1[i - 1] << " ";
        for (int j = 1; j < s2.length() + 1; j++)
        {
            if (DPmap.count({i, j}) > 0)
            {
                cout << DPmap[{i, j}] << " ";
            }
            else
            {
                cout << 0 << " ";
            }
        }
        cout << endl;
    }
}

void traceBack(map<pair<int, int>, int> DPmap, string s1, string s2, int match = 1, int mismatch = -1, int gap = -2)
{
    int count_match = 0, count_mismatch = 0, count_gap = 0, opening_gap = 0;
    string align1 = "", align2 = "";
    int max_val = findMax(DPmap), num = 0;

    // start traceback from the cell with highest score
    for (auto item : DPmap)
    {
        auto pos = item.first;
        int score = item.second;
        if (score != max_val)
            continue;

        int i = pos.first, j = pos.second;
        cout << "----------" << endl;
        cout << "Case " << ++num << endl;

        align1 = "";
        align2 = "";
        count_match = 0;
        count_mismatch = 0;
        count_gap = 0;
        opening_gap = 0;

        while (i > 0 && j > 0 && score)
        {
            int diagonal = 0, above = 0, left = 0;
            if (s1[i - 1] == s2[j - 1])
                if (DPmap.count({i - 1, j - 1}))
                    diagonal = DPmap[{i - 1, j - 1}] + match;

                else if (DPmap.count({i - 1, j - 1}))
                    diagonal = DPmap[{i - 1, j - 1}] + mismatch;

            if (DPmap.count({i, j - 1}))
                left = DPmap[{i, j - 1}] + gap;
            if (DPmap.count({i - 1, j}))
                above = DPmap[{i - 1, j}] + gap;

            score = max({diagonal, above, left});
            if (diagonal == score)
            {
                align1 = s1[--i] + align1;
                align2 = s2[--j] + align2;
                count_match++;
            }
            else if (above == score)
            {
                align1 = s1[--i] + align1;
                align2 = "-" + align2;
                if (opening_gap == 0)
                    opening_gap++;
                count_gap++;
            }
            else if (left == score)
            {
                align1 = "-" + align1;
                align2 = s2[--j] + align2;
                if (opening_gap == 0)
                    opening_gap++;
                count_gap++;
            }
            else
            {
                align1 = s1[--i] + align1;
                align2 = s2[--j] + align2;
                count_mismatch++;
            }
        }

        cout << "Align 1: " << align1 << endl;
        cout << "Align 2: " << align2 << endl;
        cout << "Match: " << count_match << " | ";
        cout << "Mismatch: " << count_mismatch << " | ";
        cout << "Gap: " << count_gap << " | ";
        cout << "Opening gap: " << opening_gap << endl;
        cout << endl;
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

    map<pair<int, int>, int> DPmap = smithWaterman(s1, s2, match_val, mismatch_val, gap);

    traceBack(DPmap, s1, s2, match_val, mismatch_val, gap);
    return 0;
}