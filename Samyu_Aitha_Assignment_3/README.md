# EECS 348 Assignment 3

Author: Shashank Aitha. Confirm this matches your enrollment name; update the folder and source/PDF author if necessary.

Included: final commented source, locally compiled executable, analysis PDF containing both raw GenAI codes, sample input/output, and Makefile.

## Required before submitting
1. Replace the highlighted placeholder in Section C of the analysis PDF with the exact prompt actually used for both Gemini and Claude. Confirm the stated access methods are accurate.
2. Rebuild on Cycle. The included executable was compiled locally and is NOT verified as a Cycle server executable. In this folder on Cycle, run:

```bash
g++ -std=c++17 -Wall -Wextra -pedantic assignment3.cpp -o assignment3
./assignment3 sample_test.txt
```

Or run `make clean`, `make`, and `make test`.
3. Push this folder (including the Cycle-built executable and completed analysis) to a PUBLIC repository named Fall2026_EECS348_Assignment_3.
4. Submit the repository URL to Canvas.

## Run
```bash
./assignment3 test.txt
```
With no argument, the program looks for input.txt.

The official sample was compiled and checked locally. Cycle verification remains necessary.
