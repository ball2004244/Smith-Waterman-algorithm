'''
Modified version of Space Optimized Smith-Waterman algorithm,
take into account affine gap penalty, using 2 tables instead of 1
'''
def findMax(DPmap: dict) -> int:
    return max(DPmap.values(), default=float('-inf'))

def smithWaterman(s1: str, s2: str, match: int, mismatch: int, open_gap: int, gap: int) -> dict:
    # create table to store the scores
    DPmap = {}
    DPmap[(0, 0)] = 0 # base case, (0, 0) is the position in the matrix

    # create table to store the gap extend status
    gap_extend = {}
    gap_extend[(0, 0)] = False        

    # initialize first row and column
    for i in range(1, len(s1) + 1):
        DPmap[(i, 0)] = open_gap + gap * i
        gap_extend[(i, 0)] = False
    
    for j in range(1, len(s2) + 1):
        DPmap[(0, j)] = open_gap + gap * j
        gap_extend[(0, j)] = False

    # fill DPmap
    for i in range(1, len(s1) + 1):
        for j in range(1, len(s2) + 1):
            if s1[i - 1] == s2[j - 1]:
                score = match
                score += DPmap.get((i - 1, j - 1), 0)
            else:
                score = mismatch
                score += DPmap.get((i - 1, j - 1), 0)

            above = gap + DPmap.get((i - 1, j), 0)
            left = gap + DPmap.get((i, j - 1), 0)

            if gap_extend[(i - 1, j)] == False:
                above += open_gap
            if gap_extend[(i, j - 1)] == False:
                left += open_gap

            DPmap[(i, j)] = max(score, above, left)
            if DPmap[(i, j)] == above or DPmap[(i, j)] == left:
                gap_extend[(i, j)] = True
            else:
                gap_extend[(i, j)] = False

    return DPmap, gap_extend


def printMap(DPmap: dict, s1: str, s2: str):
    print("  ", end='')
    for i in range(len(s2)):
        print(s2[i], end=' ')
    print('')

    for i in range(1, len(s1)+1):
        print(s1[i-1], end=' ')
        for j in range(1, len(s2)+1):
            print(DPmap.get((i, j), 0), end=' ')
        print('')


def traceBack(DPmap: dict, gap_extend: dict, s1: str, s2: str, match: int = 1, mismatch: int = -1, open_gap: int = -10, gap: int = -2):
    num = 0
    align1 = align2 = ""
    # max_val = findMax(DPmap)

    for pos, score in DPmap.items():
        if pos != (len(s1), len(s2)):
            continue

        i, j = pos
        print("----------")
        print("Case", num + 1)
        num += 1

        align1 = ""
        align2 = ""
        count_match = count_mismatch = count_gap = 0

        while i > 0 and j > 0:
            diagonal = above = left = 0
            if s1[i-1] == s2[j-1]:
                diagonal = DPmap.get((i-1, j-1), 0) + match
            else:
                diagonal = DPmap.get((i-1, j-1), 0) + mismatch

            left = DPmap.get((i, j-1), 0) + gap
            above = DPmap.get((i-1, j), 0) + gap

            if gap_extend[(i, j-1)] == False:
                left += open_gap

            if gap_extend[(i-1, j)] == False:
                above += open_gap

            score = max(diagonal, above, left)

            if diagonal == score:
                align1 = s1[i-1] + align1
                align2 = s2[j-1] + align2
                count_match += 1
                i -= 1
                j -= 1

            elif above == score:
                align1 = s1[i - 1] + align1
                align2 = "-" + align2
                i -= 1
                count_gap += 1

            elif left == score:
                align1 = "-" + align1
                align2 = s2[j - 1] + align2
                j -= 1
                count_gap += 1
            else:
                align1 = s1[i - 1] + align1
                align2 = s2[j - 1] + align2
                i -= 1
                j -= 1
                count_mismatch += 1

        print("Alignment 1:", align1)
        print("Alignment 2:", align2)
        print("Number of matches:", count_match)
        print("Number of mismatches:", count_mismatch)
        print("Number of gaps:", count_gap)



if __name__ == '__main__':
    s1 = input("s1: ")
    s2 = input("s2: ")

    match = int(input('match: '))
    mismatch = int(input('mismatch: '))
    open_gap = int(input('open gap: '))
    gap = int(input('gap: '))

    DPmap, gap_extend = smithWaterman(s1, s2, match, mismatch, open_gap, gap)
    printMap(DPmap, s1, s2)
    traceBack(DPmap, gap_extend, s1, s2, match, mismatch, open_gap, gap)
