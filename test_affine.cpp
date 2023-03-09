#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

using namespace std;

/*
    SMITH WATERMAN BEYER Algorithm (Space Optimized + Affine Gap Penalty)
*/

// find max score inside a map
int findMax(map<pair<int, int>, int> DPmap)
{
    int max_score = INT_MIN;
    for (auto item : DPmap)
        max_score = max(max_score, item.second);
    
    return max_score;
}

// create a map with pairs to store (i, j) instead of orignial 2D table
map<pair<int, int>, int> smithWaterman(map<pair<int, int>, bool> &gap_extend, string s1, string s2, int match, int mismatch, int open_gap, int gap)
{
    int score = 0;
    int above = 0;
    int left = 0;
    map<pair<int, int>, int> DPmap;
    DPmap[{0, 0}] = 0; // base case, (0, 0) is the position in the matrix

    // base case
    gap_extend[{0, 0}] = false;

    // initialize first row and column
    for (int i = 1; i < s1.length() + 1; i++)
    {
        DPmap[{i, 0}] = open_gap + gap * i;
        gap_extend[{i, 0}] = false;
    }

    for (int j = 1; j < s2.length() + 1; j++)
    {
        DPmap[{0, j}] = open_gap + gap * j;
        gap_extend[{0, j}] = false;
    }

    for (int i = 1; i < s1.length() + 1; i++)
    {
        for (int j = 1; j < s2.length() + 1; j++)
        {
            if (s1[i - 1] == s2[j - 1])
                score = match;
            else
                score = mismatch;

            if (DPmap.count({i - 1, j - 1}))
                score += DPmap[{i - 1, j - 1}];

            above = gap;
            if (DPmap.count({i - 1, j}))
                above += DPmap[{i - 1, j}];

            if (gap_extend[{i - 1, j}] != true)
                above += open_gap;

            left = gap;

            if (DPmap.count({i, j - 1}))
                left += DPmap[{i, j - 1}];

            if (gap_extend[{i, j - 1}] != true)
                left += open_gap;

            DPmap[{i, j}] = max({score, above, left});
            if (DPmap[{i, j}]== above || DPmap[{i, j}] == left)
                gap_extend[{i, j}] = true;
            else gap_extend[{i, j}] = false;
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

// void traceBack(map<pair<int, int>, int> DPmap, map<pair<int, int>, bool> gap_extend, string s1, string s2, int match = 1, int mismatch = -1, int open_gap = -3, int gap = -2)
// {
//     int count_match = 0, count_mismatch = 0, count_gap = 0;
//     string align1 = "", align2 = "";
//     int i = s1.length(), j = s2.length(); // start traceback from the bottom right corner


//         cout << "----------" << endl;

//         align1 = "";
//         align2 = "";
//         count_match = 0;
//         count_mismatch = 0;
//         count_gap = 0;

//         while (i > 0 && j > 0)
//         {
//             int diagonal = 0, above = 0, left = 0;
//             if (s1[i - 1] == s2[j - 1])
//                 diagonal = match;
//             else 
//                 diagonal = mismatch;

//             if (DPmap.count({i - 1, j - 1}))
//                 diagonal += DPmap[{i - 1, j - 1}];

//             left = gap;
//             if (DPmap.count({i, j - 1}))
//                 left += DPmap[{i, j - 1}];
            
//             if (gap_extend[{i, j - 1}] != true)
//                 left += open_gap;

//             above = gap;
//             if (DPmap.count({i - 1, j}))
//                 above += DPmap[{i - 1, j}];

//             if (gap_extend[{i - 1, j}] != true)
//                 above += open_gap;

//             int score = max({diagonal, above, left});

//             if (diagonal == score)
//             {
//                 align1 = s1[--i] + align1;
//                 align2 = s2[--j] + align2;
//                 count_match++;
//             }
//             else if (above == score)
//             {
//                 align1 = s1[--i] + align1;
//                 align2 = "-" + align2;
//                 count_gap++;
//             }
//             else if (left == score)
//             {
//                 align1 = "-" + align1;
//                 align2 = s2[--j] + align2;
//                 count_gap++;
//             }
//             else
//             {
//                 align1 = s1[--i] + align1;
//                 align2 = s2[--j] + align2;
//                 count_mismatch++;
//             }
//         }

//         cout << "Align 1: " << align1 << endl;
//         cout << "Align 2: " << align2 << endl;
//         cout << "Match: " << count_match << " | ";
//         cout << "Mismatch: " << count_mismatch << " | ";
//         cout << "Gap: " << count_gap << " | ";
//         cout << endl;
//     }

void traceBack(map<pair<int, int>, int> DPmap, string s1, string s2, int i, int j, int match, int mismatch, int open_gap, int gap, vector<pair<int, int>> &path, vector<vector<pair<int, int>>> &allPaths)
{
    if (i == 0 || j == 0)
    {
        allPaths.push_back(path);
        return;
    }

    int diagonal = 0, above = 0, left = 0;
    if (s1[i - 1] == s2[j - 1])
        diagonal = match;
    else 
        diagonal = mismatch;

    if (DPmap.count({i - 1, j - 1}))
        diagonal += DPmap[{i - 1, j - 1}];

    left = gap;
    if (DPmap.count({i, j - 1}))
        left += DPmap[{i, j - 1}];

    above = gap;
    if (DPmap.count({i - 1, j}))
        above += DPmap[{i - 1, j}];

    int score = max({diagonal, above, left});

    if (diagonal == score)
    {
        path.push_back({i - 1, j - 1});
        traceBack(DPmap, s1, s2, i - 1, j - 1, match, mismatch, open_gap, gap, path, allPaths);
        path.pop_back();
    }
    if (above == score)
    {
        path.push_back({i - 1, j});
        traceBack(DPmap, s1, s2, i - 1, j, match, mismatch, open_gap, gap, path, allPaths);
        path.pop_back();
    }
    if (left == score)
    {
        path.push_back({i, j - 1});
        traceBack(DPmap, s1, s2, i, j - 1, match, mismatch, open_gap, gap, path, allPaths);
        path.pop_back();
    }
}


int main()
{
    // string s1 = "ACATGCTACACGTATCCGATACCCCGTAACCGATAACGATACACAGACCTCGTACGCTTGCTACAACGTACTCTATAACCGAGAACGATTGACATGCCTCGTACACATGCTACACGTACTCCGAT";
    // string s2 = "ACATGCGACACTACTCCGATACCCCGTAACCGATAACGATACAGAGACCTCGTACGCTTGCTAATAACCGAGAACGATTGACATTCCTCGTACAGCTACACGTACTCCGAT";
    string s1, s2;
    int max_val = INT_MIN;
    int match_val, mismatch_val, open_gap, gap;
    map<pair<int, int>, bool> gap_extend;

    vector<pair<int, int>> path;
    vector<vector<pair<int, int>>> allPaths;

    cout << "Smith-Waterman-Beyer Algorithm" << endl;
    cout << "1st string: ";
    cin >> s1;

    cout << "2nd string: ";
    cin >> s2;

    cout << "Match: ";
    cin >> match_val;

    cout << "Mismatch: ";
    cin >> mismatch_val;

    cout << "Open gap: ";
    cin >> open_gap;

    cout << "Gap: ";
    cin >> gap;

    map<pair<int, int>, int> DPmap = smithWaterman(gap_extend, s1, s2, match_val, mismatch_val, open_gap, gap);
    printMap(DPmap, s1, s2);
    traceBack(DPmap, s1, s2, s1.length(), s2.length(), match_val, mismatch_val, open_gap, gap, path, allPaths);
    // traceBack(DPmap, gap_extend, s1, s2, match_val, mismatch_val, gap);

    for (auto path : allPaths)
    {
        string align1 = "", align2 = "";
        int count_match = 0, count_mismatch = 0, count_gap = 0;
        int i = s1.length(), j = s2.length();
        for (auto p : path)
        {
            if (p.first == i - 1 && p.second == j - 1)
            {
                align1 = s1[--i] + align1;
                align2 = s2[--j] + align2;
                count_match++;
            }
            else if (p.first == i - 1 && p.second == j)
            {
                align1 = s1[--i] + align1;
                align2 = "-" + align2;
                count_gap++;
            }
            else if (p.first == i && p.second == j - 1)
            {
                align1 = "-" + align1;
                align2 = s2[--j] + align2;
                count_gap++;
            }
        }
        cout << "Align 1: " << align1 << endl;
        cout << "Align 2: " << align2 << endl;
        cout << "Match: " << count_match << " | ";
        cout << "Mismatch: " << count_mismatch << " | ";
        cout << "Gap: " << count_gap << " | ";
        cout << endl;
    }
    return 0;
}