#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

// Max voters and candidates
#define MAX_VOTERS 100
#define MAX_CANDIDATES 9

// preferences[i][j] is jth preference for voter i
int preferences[MAX_VOTERS][MAX_CANDIDATES];

// Candidates have name, vote count, eliminated status
typedef struct
{
    char* name;
    int votes;
    bool eliminated;
} candidate;

// Array of candidates
candidate candidates[MAX_CANDIDATES];
candidate sorted[MAX_CANDIDATES];

// Numbers of voters and candidates
int voter_count;
int candidate_count;

// Function prototypes
bool vote(int voter, int rank, char* name);
void tabulate(void);
bool print_winner(void);
int find_min(void);
bool is_tie(int min);
void eliminate(int min);

int main(int argc, char* argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: runoff [candidate ...]\n");
        return 1;
    }

    candidate_count = argc - 1;
    if (candidate_count > MAX_CANDIDATES)
    {
        printf("Maximum number of candidates is %i\n", MAX_CANDIDATES);
        return 2;
    }

    printf("Number of voters: ");
    scanf("%i", &voter_count);
        if (voter_count > MAX_VOTERS)
    {
        printf("Maximum number of voters is %i\n", MAX_VOTERS);
        return 3;
    }

    // Populate array of candidates
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i].name = argv[i + 1];
        candidates[i].votes = 0;
        candidates[i].eliminated = false;
    }

    // Keep querying for votes
    char* name = malloc(20 * sizeof(char));
    if (name == NULL)
    {
        return 1;
    }
    for (int i = 0; i < voter_count; i++)
    {
        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            printf("Rank %i: ", j + 1);
            scanf("%s", name);

            // Record vote, unless it's invalid
            if (!vote(i, j, name))
            {
                printf("Invalid vote.\n");
                return 4;
            }
        }

        printf("\n");
    }

    // Keep holding runoffs until winner exists
    while (true)
    {
        // Calculate votes given remaining candidates
        tabulate();

        // Populate sorted array
        for (int i = 0; i < candidate_count; i++)
        {
            sorted[i].name = candidates[i].name;
            sorted[i].votes = candidates[i].votes;
            sorted[i].eliminated = candidates[i].eliminated;
        }
        // Bubble sort
        for (int j = 0; j < (candidate_count - 1); j++)
        {
            for (int i = 0; i < (candidate_count - 1 - j); i++)
            {
                if (sorted[i].votes > sorted[i + 1].votes)
                {
                    int x = sorted[i].votes;
                    char* y = sorted[i].name;
                    bool e = sorted[i].eliminated;
                    sorted[i].votes = sorted[i + 1].votes;
                    sorted[i].name = sorted[i + 1].name;
                    sorted[i].eliminated = sorted[i + 1].eliminated;
                    sorted[i + 1].votes = x;
                    sorted[i + 1].name = y;
                    sorted[i + 1].eliminated = e;
                }
            }
        }

        // Check if election has been won
        bool won = print_winner();
        if (won)
        {
            break;
        }

        // Eliminate last-place candidates
        int min = find_min();
        bool tie = is_tie(min);

        // If tie, everyone wins
        if (tie)
        {
            for (int i = 0; i < candidate_count; i++)
            {
                if (!candidates[i].eliminated)
                {
                    printf("%s\n", candidates[i].name);
                }
            }
            break;
        }

        // Eliminate anyone with minimum number of votes
        eliminate(min);

        // Reset vote counts back to zero
        for (int i = 0; i < candidate_count; i++)
        {
            candidates[i].votes = 0;
        }
    }
    free(name);
    return 0;
}

// Record preference if vote is valid
bool vote(int voter, int rank, char* name)
{
    for (int i = 0; i < candidate_count; i++)
    {
        if (strcmp(name, candidates[i].name) == 0)
        {
            preferences[voter][rank] = i;
            return true;
        }
    }
    return false;
}

// Tabulate votes for non-eliminated candidates
void tabulate(void)
{
    for (int i = 0; i < voter_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            if (candidates[preferences[i][j]].eliminated == false)
            {
                for (int k = 0; k < candidate_count; k++)
                {
                    if (strcmp(candidates[preferences[i][j]].name, candidates[k].name) == 0)
                    {
                        candidates[k].votes += 1;
                    }
                }
                break;
            }
        }
    }
    return;
}

// Print the winner of the election, if there is one
bool print_winner(void)
{
    if (sorted[candidate_count - 1].votes > (voter_count / 2))
    {
        printf("%s\n", sorted[candidate_count - 1].name);
        return true;
    }
    return false;
}

// Return the minimum number of votes any remaining candidate has
int find_min(void)
{
    int min = 0;
    for (int j = 0; j < candidate_count; j++)
    {
        if (sorted[j].eliminated == false)
        {
            min = sorted[j].votes;
            break;
        }
    }
    return min;
}

// Return true if the election is tied between all candidates, false otherwise
bool is_tie(int min)
{
    int sum = 0;
    int t = 0;
    for (int j = 0; j < candidate_count; j++)
    {
        if (sorted[j].eliminated == false)
        {
            t += 1;
            if (sorted[j].votes == min)
            {
                sum += 1;
            }
        }
    }
    if (sum == t)
    {
        return true;
    }
    return false;
}

// Eliminate the candidate (or candidates) in last place
void eliminate(int min)
{
    for (int j = 0; j < candidate_count; j++)
    {
        if (candidates[j].votes == min)
        {
            candidates[j].eliminated = true;
        }
    }
    return;
}
