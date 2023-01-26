def smith_waterman(seq1, seq2, match_score=1, mismatch_score=-1, gap_penalty=-1):
    m, n = len(seq1), len(seq2)
    # Create the rolling array
    rolling_array = [[0] * (n + 1) for _ in range(m+1)]
    # Initialize the maximum score and position
    max_score, max_pos = 0, (0, 0)
    for i in range(1, m + 1):
        for j in range(1, n + 1):
            # Calculate the score for the current position
            diagonal_score = rolling_array[i-1][j-1] + (match_score if seq1[i - 1] == seq2[j - 1] else mismatch_score)
            left_score = rolling_array[i][j-1] + gap_penalty
            up_score = rolling_array[i-1][j] + gap_penalty
            rolling_array[i][j] = max(0, diagonal_score, left_score, up_score)
            if rolling_array[i][j] > max_score:
                max_score = rolling_array[i][j]
                max_pos = (i, j)
    return rolling_array, max_score, max_pos

def traceback(seq1, seq2, rolling_array, max_pos, match_score=1, mismatch_score=-1, gap_penalty=-1):
    i, j = max_pos
    align1, align2 = "", ""
    while i > 0 and j > 0:
        current_score = rolling_array[i][j]
        diagonal_score = rolling_array[i-1][j-1] + (match_score if seq1[i-1] == seq2[j-1] else mismatch_score)
        up_score = rolling_array[i-1][j] + gap_penalty
        left_score = rolling_array[i][j-1] + gap_penalty
        if current_score == diagonal_score:
            align1 = seq1[i-1] + align1
            align2 = seq2[j-1] + align2
            i -= 1
            j -= 1
        elif current_score == up_score:
            align1 = seq1[i-1] + align1
            align2 = "-" + align2
            i -= 1
        elif current_score == left_score:
            align1 = "-" + align1
            align2 = seq2[j-1] + align2
            j -= 1
        else:
            break
    return align1, align2

def traverse(seq1, seq2, rolling_array, match_score=1, mismatch_score=-1, gap_penalty=-1):
    align1, align2 = [''], ['']
    queue = [(len(seq1), len(seq2))]
    visited = set()
    while queue:
        i, j = queue.pop(0)
        
        # Skip if cell is already visited
        if (i, j) in visited:
            continue
        visited.add((i, j))
        
        # End of path, append the alignments
        if rolling_array[i][j] == 0:
            align1.append("".join(reversed(align1[-1])))
            align2.append("".join(reversed(align2[-1])))
            
            # Remove '-' from the alignment
            align1[-1] = align1[-1].replace("-", "")
            align2[-1] = align2[-1].replace("-", "")
            
            # remove the last element
            align1.pop(-2)
            align2.pop(-2)
            continue
        
        # If match, append letters and go diagonal
        if i > 0 and j > 0 and rolling_array[i][j] == rolling_array[i - 1][j - 1] + (match_score if seq1[i - 1] == seq2[j - 1] else mismatch_score):
            align1.append(seq1[i - 1] + align1[-1])
            align2.append(seq2[j - 1] + align2[-1])
            queue.append((i - 1, j - 1))
        
        # If insertion, append '-' in second sequence and go up
        elif i > 0 and rolling_array[i][j] == rolling_array[i - 1][j] + gap_penalty:
            align1.append(seq1[i - 1] + align1[-1])
            align2.append("-" + align2[-1])
            queue.append((i - 1, j))
        
        # If deletion, append '-' in first sequence and go left
        elif j > 0 and rolling_array[i][j] == rolling_array[i][j - 1] + gap_penalty:
            align1.append("-" + align1[-1])
            align2.append(seq2[j - 1] + align2[-1])
            queue.append((i, j - 1))
    return align1, align2


s1 = 'ATCGA'
s2 = 'AACCCG'
match = 1
mismatch = -1
gap = -2
rolling_array, max_score, max_pos = smith_waterman(s1, s2, match, mismatch, gap)
align1, align2 = traceback(s1, s2, rolling_array, max_pos, match, mismatch, gap)
print(f'Max score: {max_score} at {max_pos}')
print('Optimal Alignment: ')
print(align1)
print(align2)

print('All possible cases: ')
align1_all, align2_all = traverse(s1, s2, rolling_array, match, mismatch, gap)
print(align1_all)
print('\n')
print(align2_all)



