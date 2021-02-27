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

    //DEBUG: Print a graph after each voter to visually see preferences.
    // printf("\nPreferences: \n");
    // for (int i = 0; i < candidate_count; i++)
    // {
    //     for (int j = 0; j < candidate_count; j++)
    //     {
    //       printf("%i ", preferences[i][j]);
    //     }
    //     printf("\n");
    // }

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



    //DEBUG: Print all the pairs
    //In this format: C0: 3 vs C1: 1
    // printf("All Pairs: \n");
    // for (int i = 0; i < candidate_count; i++)
    // {
    //     printf("C%i: %i vs C%i: %i\n", pairs[i].winner, preferences[pairs[i].winner][pairs[i].loser], pairs[i].loser, preferences[pairs[i].loser][pairs[i].winner]);
    // }



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

    //DEBUG: Print Strength of pairs
    // printf("\nPairs Strength: \n");

    // for (int i = 0; i < pair_count; i++)
    // {
    //     int n = preferences[pairs[i].winner][pairs[i].loser] - preferences[pairs[i].loser][pairs[i].winner];
    //     printf("C%i-C%i: n: %i\n", pairs[i].winner, pairs[i].loser, n);
    // }
    // return;
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

    //NOT working code_________________________________________________________________________________________________________

    //cycle through the pairs
    // for (int i = 0; i < pair_count; i++)
    // {
    //     if (!cycle(pairs[i].winner, pairs[i].loser))
    //     {
    //         locked[pairs[i].winner][pairs[i].loser] = true;
    //     }
    // }
    // return;

    // int winner;
    // int loser;

    // while (winner != -1 && winner != loser)
    // {
    //     bool found = false;

    //     for (int i = 0; i < candidate_count; i++)
    //     {
    //         if (locked[i][winner])
    //         {
    //             found = true;
    //             winner = i;
    //         }
    //     }

    //     if (!found)
    //     {
    //         winner = -1;
    //     }


    // }

    // if (winner == loser)
    // {
    //     return true;
    // }
    // return false;

    //NOT working code END_________________________________________________________________________________________________________



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

/* Log

TIDEMAN

Here we go again!

    // //print out the graph for debugging
    // for (int i = 0; i < pair_count; i++)
    // {
    //     printf("i %i: ", i);

    //         for(int j = 0; j < candidate_count; j++)
    //         {
    //             printf("%i ", preferences[i][j]);
    //         }
    //         printf("\n");
    // }
    // printf("\n");

Okay, I'm trying to figure out how to print a graph to match the
voter preferences it should look like this

Votes

alice
charlie
bob

alice
charlie
bob

charlie
alice
bob

bob
alice
charlie


candidates
alice   bob     charlie

preferences
0       3       3
1       0       1
1       3       0

alice vs alice: 0
alice vs bob: 3
alice vs charlie: 3

bob vs alice: 1
bob vs bob: 0
bob vs charlie: 1

charlie vs alice: 1
charlie vs bob: 3
charlie vs charlie: 0

Okay so if I continue to write this out the array should go like
this:

alice vs alice =
[0] [0]

then alice vs bob:

[0] [1]

Then alice vs charlie:

[0] [2]

or in other words [i][j], where i is the first preference,
and j is the second preference.

i is also based on the number of candadites, so in the case
there are 3 candidates i is 0, 1, and 2.

j is also based on the number of candidates, because it must be
betwen 0, and 2.

So I think I would need two for loops.

ok so if I write the following code:

    //cycle trough all the voters
    for (int i = 0; i < candidate_count; i++)
    {
        //cycle through all the candidates
        for (int j = 0; j < candidate_count; j++)
        {
            //update the global preferences array to add the current
            //voters preferences[i][j], candidate i, over candidate j.
            printf("rank: %i ", ranks[i]);
        }
        printf("\n");
    }
    printf("\n");

I get:
./tideman a b c
Number of voters: 1
Rank 1: a
Rank 2: c
Rank 3: b
rank: 0 rank: 0 rank: 0
rank: 2 rank: 2 rank: 2
rank: 1 rank: 1 rank: 1

So it's kinda working but I need to format it a bit.

Format looks good now:

./tideman a b c
Number of voters: 2
Rank 1: a
Rank 2: b
Rank 3: c

Preferences:
0 0 0
1 1 1
2 2 2

Right, so I got my graph to work and to print out the correct terms

I went for

0-0 0-1 0-2 ect...

So that's what I have to set the preferences to.
I'm just not sure again, how to set the double array to the right
values.

for example, it should do:
preferences[i][j]

preferences[0][0]
preferences[0][1]
preferences[0][2]

this would mean that the voter prefers alice vs alice,
alice over bob, and alice over charlie.

Then, I'll want to print out a graph to make sure my program is corret.

what it should be doing is starting with the first preference, which is i

so preference[i][j]
if i is the first loop I want it to compare the first preference
to all the other candiadtes, creating these pairs. After the first
preference it should then compare all the seond prefererence against
the remaning candidates.

This is what my graph is printing right now:

./tideman a b c
Number of voters: 1
Rank 1: a
Rank 2: b
Rank 3: c

Preferences:
0-0 0-1 0-2
1-0 1-1 1-2
2-0 2-1 2-2

The first rank is alice, so alice is tied with alice,
alice beats bob 0-1
alice beat charlie 0-2

Then the 2nd and thrid rows are not working correctly.

Well it's kind of working, it's working in the sense that it is comparing
0 to 0 or alice vs alice, alice versus bob, and alice versus charlie
bob vs alice and so on.

But I need to get to the next step, I need to compare alice vs bob
and see how wins, then record the result. The winner of all those
pairs should be the correct way to make the graph recording the winner

so with just one voter who votes

1: a
2: b
3: c

it should say:

0 1 1
0 0 1
0 0 0

in other words,

a vs a = 0
a vs b = a wins so a 1
a vs c = a wins so a 1


Okay I'm going to take a step back and just print the ranks.

 ./tideman a b c
Number of voters: 1
Rank 1: a
Rank 2: b
Rank 3: c

Preferences:
0
1
2

 ./tideman a b c
Number of voters: 1
Rank 1: c
Rank 2: a
Rank 3: b

Preferences:
2
0
1

A simple program, but it shows how the ranks are being recorded.

so if I stick with the first one where they voted for a, b, and c.

a is their first preference.

This is stored in rank[i] or rank[0]
I then want to compare rank[0] to rank 1  and 2.

That would look like:
0 over 0, 0 over 1, 0 over 2

since I only have one voter, I would just change preferences to:

preferences[0][0]
preferences[0][1]
preferences[0][2]

prefrences is "represents the nubmer of voters who prefer
candidate i over j" so this should be incremented each time. (I think.)

Let me see if I can do this.

So I did this:

    printf("\nPreferences: \n");

    //cycle trough all the voters
    for (int i = 0; i < candidate_count; i++)
    {
        //cycle through all the candidates
        for (int j = 0; j < candidate_count; j++)
        {
            //update the global preferences array to add the current
            //voters preferences[i][j], candidate i, over candidate j.
            preferences[ranks[i]][ranks[j]]++;
            printf("%i ", preferences[i][j]);
        }
    }

And got this:

./tideman a b c
Number of voters: 1
Rank 1: a
Rank 2: b
Rank 3: c

Preferences:
1 1 1 1 1 1 1 1 1

So what happened here?
I guess it ran the loop 9 times and recored the preferences nine times.

let me add a break real quick.

Right, that's better,

 ./tideman a b c
Number of voters: 1
Rank 1: a
Rank 2: b
Rank 3: c

Preferences:
1 1 1
1 1 1
1 1 1

So it's recording the preferences, and printing them out in the right
format, but it's not printing the correct value.

It should print 0 or 1 based on if the preference is better thatn the other

maybe I need an if statment? if the preference is equal to the cord, then
it will increment, otherwise it won't?

I created a simple if statement

   printf("\nPreferences: \n");

    //cycle trough all the voters
    for (int i = 0; i < candidate_count; i++)
    {
        //cycle through all the candidates
        for (int j = 0; j < candidate_count; j++)
        {
            //update the global preferences array to add the current
            //voters preferences[i][j], candidate i, over candidate j.
            if (preferences[ranks[i]][ranks[j]] == preferences[i][j])
            {
                preferences[ranks[i]][ranks[j]]++;
            }
            printf("%i ", preferences[i][j]);
        }
        printf("\n");
    }

And got this:

./tideman a b c
Number of voters: 1
Rank 1: a
Rank 2: b
Rank 3: c

Preferences:
1 1 1
1 1 1
1 1 1

So in this case the if statment is always true. I want it to be false when there is a loss.

so the first is
0-0, the second is 0-1,

but my ranks should be

0-0, then 0-1, then 0-2.

So if I change the loop to != it then records all zeros. So that means it works in both true and false
cases, I just need the right if statment to flip it from true to false.

Preferences:
0 0 0
0 0 0
0 0 0

So what am I trying to check for?

I'm trying to check if one candidate's preference beats anotehr candidate.

for example 0-0, alice vs alice is 0
but when I do alice vs bob, since alice was the first preference, bob loses. so a vs b should be 1.

How do I know that alice beats bob? Well alice is the first preference, bob is the second.

in other words rank[0] is 0 (for alice)
and rank[1] is 1 (for bob)
so alice wins because she is rank[0].

How do I write this as an if statement?

The preference statment is saying voter 1 prefers alice over bob, because preference[0][1]

so the first number, 0-0 needs to be 0, the second number because 0-1, needs to be 1,

This should be easy to do, what am I missing?

Okay I got it:

 ./tideman a b c
Number of voters: 1
Rank 1: a
Rank 2: b
Rank 3: c

Preferences:
0 1 1
0 0 1
0 0 0

 //cycle through all the candidates
    for (int i = 0; i < candidate_count; i++)
    {
        //cycle through next candidate
        for (int j = i + 1; j < candidate_count; j++)
        {
            //update the global preferences array to add the current
            //voters preferences[i][j], candidate i, over candidate j.
            preferences[ranks[i]][ranks[j]]++;
        }
    }

I went back through the walk through and realized that the array ranks is given:

[3, 0, 4, 1, 2]

As described,

3 is prefered over 0, 4, 1, 2
0 over 4, 1, 2
4 over 1, 2
1 over 2,
and 2 isn't prefered over any.

So what my nested for looped needed to do was record i, which is was, but then j wasn't the same as
i but it should be +1. So in the case above preferences[i][j] would be 3, 0 or preferences[3][0].

After that I can get rid of my if statement and go back to just adding the ++.

Then I created a sperate for loop to record the voter preferences.

Now my graph will add up the votes everytime a person votes so if I do a second voter:

Rank 1: c
Rank 2: a
Rank 3: b

Preferences:
0 2 1
0 0 1
1 1 0

It add the second votes to the first! Now, a is beating b twice, and c is now beat a one, and b once.

LOG: 2:32

Okay, I have the preferences working now to make the add_pairs work.

first off I'm going to manual do an sample election to see what
I should get, I'll use the example in the walk through

preferences
0 3 1
1 0 2
3 2 0

0 vs 1 (A vs B) = 3 to 1
2 vs 0 (C vs A) = 3 to 1
1 vs 2 (B vs C) = 2 to 2

The third pair can't be included because there is no winner/loser.

pair.winner = 0
pair.loser = 1

pairs[0].winner = 0
pairs[0].loser = 1

pairs[0] = 0 - 1 this refers to the winner and loser.

I'm guessing what I'm thinking is I want something like this:

All Pairs:
Pair: 0-1
Pair: 1-2
Pair: 0-2

This way it's printing all the possible pairs.

Log 2-25-21

I think I've made a breakthrough in thinking of the array like a mailbox.
I've watched the array video a few times, so the preference[i][j]
is essentiall a coordinate system, where I compare 1,0 to 0,1. But
the mailbox 0-1 has a number it in 3, and 1-0 has 1 in it.
So by looping through each mailbox I can check the letter and compare
it to another. Now, I have a working add_pair loop!

Next is to sort pairs in order.




*/
