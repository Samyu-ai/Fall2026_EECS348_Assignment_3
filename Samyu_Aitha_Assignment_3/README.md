# EECS 348 Assignment 3
Author: Samyu Aitha

## Description
An object-oriented C++ program that prioritizes CEO emails using a custom,
vector-backed MaxHeap. No pre-existing heap modules are used.

Priority is determined by sender category:
Boss > Subordinate > Peer > ImportantPerson > OtherPerson.
Within the same category, newer emails have higher priority.

## Files
- assignment3.cpp — final source code
- assignment3 — executable compiled on the Cycle server
- GenAI_Analysis_Assignment3.pdf — Gemini and Claude code comparison
- sample_test.txt — sample command file
- sample_expected.txt — expected sample output
- Makefile — compilation and testing commands

## Compile and Run
From the Samyu_Aitha_Assignment_3 folder:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic assignment3.cpp -o assignment3
./assignment3 sample_test.txt
```

To run another test file:

```bash
./assignment3 test.txt
```

## Sample Test
```bash
make test
```

No differences from `diff` means the sample test passed.

## Commands
- EMAIL: adds an email
- NEXT: displays the highest-priority email without removing it
- READ: removes the highest-priority email
- COUNT: displays the number of unread emails

## GenAI Contributions
Google Gemini and Anthropic Claude provided the initial code versions.
The final program uses Claude’s design as its baseline.
The accompanying analysis PDF describes the comparison and improvements.
