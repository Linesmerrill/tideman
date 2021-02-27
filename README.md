# tideman
Implement a program that runs a [Tideman](https://cs50.harvard.edu/x/2021/psets/3/tideman/) election

## How to setup

This project requires some environment variables if running from your local machine and also that you have the specific header file with the included functions.

Follow along with these installation steps [CS50 Installation Docs](https://cs50.readthedocs.io/libraries/cs50/c/?highlight=get_int#installation)

## How to compile

`$gcc tideman.c -lcs50`

## How to execute


`$./a.out Alice Bob Charlie`
```
Number of voters: 5
Rank 1: Alice
Rank 2: Charlie
Rank 3: Bob

Rank 1: Alice
Rank 2: Charlie
Rank 3: Bob

Rank 1: Bob
Rank 2: Charlie
Rank 3: Alice

Rank 1: Bob
Rank 2: Charlie
Rank 3: Alice

Rank 1: Charlie
Rank 2: Alice
Rank 3: Bob

Charlie
```
