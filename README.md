# runoff
Each voter has their preferred candidates ranked and stored in a 2D array. If the first placed candidate, considering voter's first options, doesn't own more than half of all votes, the election continues. The minimum number of votes is taken as reference. If all participants have had these number of votes, the election ends as a tie. Otherwise, every candidate with the minimum number of votes, considering first options, is eliminated from the election. After elimination, another tabulation of votes is mad this loop continues until the winner(s) is declared. At each tabulation, it is considered the first non-eliminated candidate preference of each voter. 
