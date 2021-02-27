#include <cs50.h>
#include <stdio.h>
#include <string.h>

// Max number of candidates
#define MAX 9

// preferences[i][j] is number of voters who prefer i over j
int preferences[MAX][MAX];

// locked[i][j] means i is locked in over j
bool locked[MAX][MAX];

// Each pair has a winner, loser
typedef struct
{
    int winner;
    int loser;
}
pair;

// Array of candidates
string candidates[MAX];
pair pairs[MAX * (MAX - 1) / 2];

int pair_count;
int candidate_count;

// My Function prototype
bool cycle(int winner, int loser);

// Function prototypes
bool vote(int rank, string name, int ranks[]);
void record_preferences(int ranks[]);
void add_pairs(void);
void sort_pairs(void);
void lock_pairs(void);
void print_winner(void);

int main(int argc, string argv[])
{
    // Check for invalid usage
    if (argc < 2)
    {
        printf("Usage: tideman [candidate ...]\n");
        return 1;
    }

    // Populate array of candidates
    candidate_count = argc - 1;
    if (candidate_count > MAX)
    {
        printf("Maximum number of candidates is %i\n", MAX);
        return 2;
    }
    for (int i = 0; i < candidate_count; i++)
    {
        candidates[i] = argv[i + 1];
    }

    // Clear graph of locked in pairs
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = 0; j < candidate_count; j++)
        {
            locked[i][j] = false;
        }
    }

    pair_count = 0;
    int voter_count = get_int("Number of voters: ");

    // Query for votes
    for (int i = 0; i < voter_count; i++)
    {
        // ranks[i] is voter's ith preference
        int ranks[candidate_count];

        // Query for each rank
        for (int j = 0; j < candidate_count; j++)
        {
            string name = get_string("Rank %i: ", j + 1);

            if (!vote(j, name, ranks))
            {
                printf("Invalid vote.\n");
                return 3;
            }

        }

        record_preferences(ranks);

        printf("\n");
    }

    add_pairs();
    sort_pairs();
    lock_pairs();
    print_winner();
    return 0;
}

bool cycle(int winner, int loser)
{
    while (winner != -1 && winner != loser)
    {
        bool found = false;

        for (int i = 0; i < candidate_count; i++)
        {
            if (locked[i][winner])
            {
                found = true;
                winner = i; //alice
            }
        }

        if (!found)
        {
            winner = -1;
        }


    }

    if (winner == loser)
    {
        return true;
    }
    return false;
}




// Update ranks given a new vote
bool vote(int rank, string name, int ranks[])
{
    //loop through all the candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //if name is a match for the name of a valid candidte
        if (strcmp(candidates[i], name) == 0)
        {
            //update ranks array
            ranks[rank] = i;
            //return true
            return true;
        }
    }
    //otherwise return false
    return false;
}

// Update preferences given one voter's ranks
void record_preferences(int ranks[])
{

    // get ranks [0, 1, 2]
    //cycle through all the candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //cycle through next ranked candidate
        for (int j = i + 1; j < candidate_count; j++)
        {
            //update the global preferences array to add the current
            //voters preferences[i][j], candidate i, over candidate j.
            preferences[ranks[i]][ranks[j]]++;
        }
    }

    //return
    return;
}

// Record pairs of candidates where one is preferred over the other
void add_pairs(void)
{
    //add all pairs of candidates where one candidate is
    //preferred to the pairs array. A pair of candidates who are
    //tied (one is not preferred over the other) should not be
    //added to the array.

    //The function should update the global variale pair_counts
    //to be the number of pairs of candidates. (the pairs
    //should thus all be stored between pairs[0] and
    //pairs[pair_count - 1], inclusive).


    //Loop through the candidates in a nested for loop
    // I might need to use MAX here?
    for (int i = 0; i < candidate_count; i++)
    {
        for (int j = i + 1; j < candidate_count; j++)
        {
            // Set the preferences to variables or not...
            // int incumbent = preferences[i][j];
            // int challenger = preferences[j][i];

            //exclude ties
            if (preferences[i][j] != preferences[j][i])
            {
                //make a temporary data structure to to store winner/losers
                pair p;

                //compare votes
                if (preferences[i][j] > preferences[j][i])
                {
                    //if the fist candidate has more vote, record the winner first.
                    p.winner = i;
                    p.loser = j;
                }
                else
                {
                    //if the second candidates has more votes, record the winner first
                    p.winner = j;
                    p.loser = i;
                }

                //store the pair in the pairs[] and update the pair count
                pairs[pair_count++] = p;
            }
        }
    }

    //return to the main loop
    return;
}

// Sort pairs in decreasing order by strength of victory
void sort_pairs(void)
{
    // The function should sort the pairs array in decreasing order
    // of strength of victory, where strength of victory is defined
    // to be the number of voters who prefer the preferred candidate.
    // If multiple pairs have the same strenth of victory, you may
    //assume that the order does not matter.

    //compare the strength of one pair to another

    //cycle through the pairs
    for (int i = 0; i < pair_count; i++)
    {
        //create a variable to store the pair with the highest strength of victory
        int preferred = i;

        //get the first pairs strength
        int FirstPair = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];

        // Find pair with the highest strength of victory
        for (int j = i + 1; j < pair_count; j++)
        {
            //Temporarily store the next pair
            int SecondPair = preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner];

            //if the second pair is greater than the first
            if (SecondPair > FirstPair)
            {
                //set it as the new preffered
                preferred = j;
                //update it to the first pair
                FirstPair = preferences[pairs[j].winner][pairs[j].loser] - preferences[pairs[j].loser][pairs[j].winner];
            }
        }

        //store the preferred pair
        pair p = pairs[preferred];
        pairs[preferred] = pairs[i];
        pairs[i] = p;
    }
}

// Lock pairs into the candidate graph in order, without creating cycles
void lock_pairs(void)
{
    //working code
    for (int i = 0; i < pair_count; i++)
    {
        if (!cycle(pairs[i].winner, pairs[i].loser))
        {
            locked[pairs[i].winner][pairs[i].loser] = true;
        }
    }
    return;
}

// Print the winner of the election
void print_winner(void)
{
    //Cycle through all the candidates
    for (int i =  0; i < candidate_count; i++)
    {
        //create a booleon called source
        bool source = true;

        //cycle through candidates
        for (int j = 0; j < candidate_count; j++)
        {
            //if not locked in
            if (locked[j][i] == true)
            {
                //source is not found
                source = false;
                break;
            }
        }

        //if the source is found
        if (source)
        {
            printf("%s\n", candidates[i]);
            return;
        }
    }
    return;
}
