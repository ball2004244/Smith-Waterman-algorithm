'''
Original version of Space Optimized Smith-Waterman algorithm
'''
def findMax(DPmap: dict) -> int:
    return max(DPmap.values(), default=float('-inf'))


def smithWaterman(s1: str, s2: str, match: int, mismatch: int, open_gap: int, gap: int) -> dict:
    DPmap = {}

    for i in range(1, len(s1)+1):
        for j in range(1, len(s2)+1):
            if s1[i-1] == s2[j-1]:
                score = match
                score += DPmap.get((i-1, j-1), 0)
            else:
                score = mismatch
                score += DPmap.get((i-1, j-1), 0)
            above = gap + DPmap.get((i-1, j), 0)
            left = gap + DPmap.get((i, j-1), 0)

            if max(score, above, left) > 0:
                DPmap[(i, j)] = max(score, above, left)

    return DPmap


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


def traceBack(DPmap: dict, s1: str, s2: str, match: int = 1, mismatch: int = -1, open_gap: int = -10, gap: int = -2):
    num = 0
    align1 = align2 = ""
    max_val = findMax(DPmap)

    for pos, score in DPmap.items():
        if score != max_val:
            continue

        i, j = pos
        print("----------")
        print("Case", num+1)
        num += 1

        align1 = ""
        align2 = ""
        count_match = count_mismatch = count_gap = opening_gap = 0

        while i > 0 and j > 0 and score:
            diagonal = above = left = 0
            if s1[i-1] == s2[j-1]:
                if (i-1, j-1) in DPmap:
                    diagonal = DPmap.get((i-1, j-1), 0) + match
                elif (i-1, j-1) in DPmap:
                    diagonal = DPmap.get((i-1, j-1), 0) + mismatch

            left = DPmap.get((i, j-1), 0) + gap
            above = DPmap.get((i-1, j), 0) + gap

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
                if opening_gap == 0:
                    opening_gap += 1
                count_gap += 1
            elif left == score:
                align1 = "-" + align1
                align2 = s2[j - 1] + align2
                j -= 1
                if opening_gap == 0:
                    opening_gap += 1
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
        print("Number of opening gaps:", opening_gap)


if __name__ == '__main__':
    s1 = input("s1: ")
    s2 = input("s2: ")

    match = int(input('match: '))
    mismatch = int(input('mismatch: '))
    open_gap = int(input('open gap: '))
    gap = int(input('gap: '))

    DPmap = smithWaterman(s1, s2, match, mismatch, open_gap, gap)
    printMap(DPmap, s1, s2)
    traceBack(DPmap, s1, s2)
