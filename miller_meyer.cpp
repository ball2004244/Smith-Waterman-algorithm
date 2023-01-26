#include <iostream>
#include <string>
#include <vector>
#include <map>

using namespace std;

// function to return the max value from 4 integers
int getMax(int a, int b, int c, int d)
{
    return max(max(a, b), max(c, d));
}

int calculateScore(string s1, string s2, int s1_start, int s1_end, int s2_start, int s2_end, map<pair<int, int>, int> &boundary, int match_val, int mismatch_val, int gap)
{
    int score = 0;
    for (int i = s1_start; i < s1_end; i++)
    {
        for (int j = s2_start; j < s2_end; j++)
        {
            // check if current cell is on the boundary
            if (i == s1_start || i == s1_end - 1 || j == s2_start || j == s2_end - 1)
            {
                int score_temp = 0;
                if (s1[i] == s2[j])
                    score_temp = boundary[{i - 1, j - 1}] + match_val;
                else
                    score_temp = getMax(boundary[{i - 1, j - 1}] + mismatch_val, boundary[{i - 1, j}] + gap, boundary[{i, j - 1}] + gap, 0);

                // update boundary cell
                boundary[{i, j}] = max(boundary[{i, j}], score_temp);
                score = max(score, score_temp);
            }
        }
    }
    return score;
}

// function to traceback the alignment
void traceback(string s1, string s2, map<pair<int, int>, int> &boundary, int n, int m, int match_val, int mismatch_val, int gap)
{
    string align1 = "", align2 = "";
    int i = n - 1, j = m - 1;
    while (i >= 0 && j >= 0)
    {
        // if current cell is not on the boundary, move to next block
        if (i > 0 && j > 0 && boundary.find({i, j}) == boundary.end())
        {
            i--;
            j--;
            continue;
        }

        int score = boundary[{i, j}];
        if (score == boundary[{i - 1, j - 1}] + (s1[i - 1] == s2[j - 1] ? match_val : mismatch_val)) {
            align1 = s1[--i] + align1;
            align2 = s2[--j] + align2;
        }
        else if (score == boundary[{i - 1, j}] - gap)
        {
            align1 = s1[--i] + align1;
            align2 = "-" + align2;
        }
        else if (score == boundary[{i, j - 1}] - gap)
        {
            align1 = "-" + align1;
            align2 = s2[--j] + align2;
        }
        else
        {
            align1 = s1[--i] + align1;
            align2 = s2[--j] + align2;
        }

        cout << "Alignment:" << endl;
        cout << align1 << endl;
        cout << align2 << endl;
    }
}

void alignSequences(string s1, string s2, int blockSize, int match_val = 3, int mismatch_val = -3, int gap = 2)
{
    map<pair<int, int>, int> boundary;
    int n = s1.length() / blockSize + 1;
    int m = s2.length() / blockSize + 1;
    for (int i = 0; i < n; i++)
    {
        for (int j = 0; j < m; j++)
        {
            int s1_start = i * blockSize;
            int s1_end = min((i + 1) * blockSize, (int)s1.length());
            int s2_start = j * blockSize;
            int s2_end = min((j + 1) * blockSize, (int)s2.length());
            if (boundary.find({i, j}) != boundary.end())
                continue;
            int score = calculateScore(s1, s2, s1_start, s1_end, s2_start, s2_end, boundary, match_val, mismatch_val, gap);
        }
    }
    traceback(s1, s2, boundary, n, m, match_val, mismatch_val, gap);
}

int main()
{
    string s1 = "ATCG";
    string s2 = "ADGGAD";
    int block_size = 2;
    int match = 1, mismatch = -1, gap = 2;

    alignSequences(s1, s2, block_size, match, mismatch, gap);
    return 0;
}
