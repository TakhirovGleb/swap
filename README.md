*This project has been created as part of the 42 curriculum by <login1>, <login2>.*

# Push_swap

## Description

Sort distinct signed 32-bit integers using two stacks and only the eleven
Push_swap operations. The first input number is the top of stack A. Stack B
starts empty. The output sorts A in ascending order and leaves B empty.

This implementation follows `en.subject.pdf`, version 1.2: four selectable
strategies, an initial disorder measurement, optional benchmark output, and a
bonus checker. It favors small, readable functions over elaborate optimizations.
It does not claim to find the mathematically shortest sequence for every input.

## Instructions

Requires a C compiler and Make. No external C library is needed.

```sh
make
./push_swap 4 67 3 87 23
./push_swap --simple "4 67 3 87 23"
./push_swap --medium 4 67 3 87 23
./push_swap --complex 4 67 3 87 23
./push_swap --adaptive --bench 4 67 3 87 23
make bonus
./push_swap 4 67 3 87 23 | ./checker 4 67 3 87 23
```

`--adaptive` is the default. One strategy flag and one `--bench` flag are
accepted, in any argument position. Duplicate or conflicting flags are errors.
Numbers may be separate arguments, whitespace-separated strings, or a mixture.
Empty numeric arguments, duplicates, malformed numbers and integer overflow
produce `Error\n` on stderr and a nonzero exit status. With no arguments both
programs exit silently. A strategy or benchmark flag alone represents an empty
stack. The checker accepts numbers only, without strategy or benchmark flags.

Operations go to stdout. `--bench` reports the original disorder with two decimal
places, the strategy and its operation complexity, the total operation count,
and all eleven individual counters to stderr. An already sorted input emits no
operations. The checker reads newline-terminated instructions until EOF and
prints `OK` or `KO`; invalid instructions, including an incomplete final line,
produce `Error` on stderr.

```sh
make clean                 # remove object files
make fclean                # also remove executables
make re                    # rebuild push_swap
make all bonus
python3 tests/test.py       # independent simulator and regression tests
norminette include src
```

## Algorithms and complexity

Before any moves, each value is replaced by its rank: the number of smaller
input values. Ranks preserve order and map arbitrary integers to `0..n-1`.
Duplicates are rejected before rank conversion. Disorder is the number of
inverted pairs divided by `n*(n-1)/2`; it is zero for fewer than two values.
Rank conversion, duplicate detection and disorder calculation take O(n²) CPU
time and generate no stack operations.

The bounds below count **generated Push_swap operations**, as required by the
subject. They are not bounds on total C execution time. Searching arrays for
minima and maxima may require O(n²) CPU time even with fewer emitted moves.
Stack operations themselves use circular arrays and take constant CPU time.

| Selector | Method | Operation upper bound |
| --- | --- | --- |
| `--simple` | Repeated minimum extraction | O(n²) |
| `--medium` | Fixed rank chunks, width ceil(sqrt(n)) | O(n sqrt(n)) |
| `--complex` | Stable binary LSD radix sort | O(n log(n)) |
| `--adaptive` | Choose a method from the initial disorder | Depends on regime |

**Simple:** rotate the smallest remaining value to the top via the shorter
direction and push it to B. Sort the last three values, then push B back to A.
Each extraction uses at most O(n) rotations; n extractions give O(n²) moves.
An already sorted remainder needs no further extraction.

**Medium:** process consecutive rank chunks from smallest to largest. For each
chunk, rotate A forward until a matching value is on top, then push it to B.
Move values in the lower half of that chunk to the bottom of B. Each chunk
requires at most one traversal of the remaining A, giving O(n²/k) moves for
chunk width k. Finally rotate B's maximum to the top via the shorter direction
and push it back to A. Each chunk occupies two runs around the earlier chunks:
its upper half at the top and lower half at the bottom. Extracting its maxima
costs O(k²) within those runs plus at most O(n) to cross the earlier chunks once.
Across all chunks this gives O(nk + n²/k). Choosing k = ceil(sqrt(n)) gives
O(n sqrt(n)) operations. This fixed-chunk design keeps the operation bound
explicit, without relying on a favorable random input distribution.

**Complex:** for each bit of the ranks, push zero-bit values to B and rotate
one-bit values in A. Push all of B back. Each pass is a stable binary partition,
so after processing all bits A is sorted. There are ceil(log2(n)) passes of at
most 2n operations each. This gives O(n log(n)) moves for every permutation.

Inputs of at most five values use the same small minimum-extraction routine in
all modes. This constant-size base case does not change any asymptotic bound.

**Adaptive:** select once, before moving anything:

- disorder < 0.2: simple, O(n²) moves;
- 0.2 <= disorder < 0.5: medium, O(n sqrt(n)) moves;
- disorder >= 0.5: complex, O(n log(n)) moves.

These thresholds are prescribed by the subject. Low disorder uses the simplest
method with an early sorted-remainder exit. Intermediate disorder uses chunks.
High disorder uses radix's predictable upper bound. `--bench` names both the
adaptive selector and its selected internal method.

All methods use O(n) integer storage, constant extra sorting workspace, and no
recursion. More precisely, allocated stack capacity is bounded by the input text
length L, so memory is O(L + n); with ordinary integer tokens this is O(n).
Operations are streamed and are not stored in memory. The baseline simple mode
is intentionally quadratic and is not intended to meet the large-input
performance thresholds. Benchmark thresholds apply to the default adaptive run.

## Source map

- `src/parse.c`, `src/rank.c`: validate, rank and measure the original input.
- `src/stack.c`, `src/names.c`: execute and emit the eleven instructions.
- `src/sort.c`: small cases, simple extraction and radix.
- `src/medium.c`: chunk sorting.
- `src/main.c`, `src/bench.c`: strategy selection and statistics.
- `src/checker_bonus.c`: read and execute instructions without emitting them.
- `tests/test.py`: independent Python model, exhaustive small permutations,
  seeded random inputs, edge cases and benchmark checks.

## Contributors

The subject requires exactly two learners. Replace the two login placeholders in
the first line and the table below with real logins and an accurate account of
work performed before submitting. Replace the generic `student` C file headers
with the appropriate authors as well. No human contributions are assumed here.

| Learner | Actual contribution |
| --- | --- |
| `<login1>` | To be completed by the learner |
| `<login2>` | To be completed by the learner |

Both learners must understand every strategy and attend the defense.

## Resources

- The supplied `en.subject.pdf`: project rules, thresholds and benchmark limits.
- Thomas H. Cormen et al., *Introduction to Algorithms*: inversions, selection
  sorting, radix sorting, and asymptotic analysis.
- Donald E. Knuth, *The Art of Computer Programming*, Volume 3: sorting methods.
- Local manual pages: `man 2 read`, `man 2 write`, `man 3 malloc`, `man 3 free`.

AI assistance was used to read the subject, design and implement the C sources,
Makefile, checker and Python tests, write this documentation, and run checks.
The learners must review, understand and take responsibility for this code;
the authorship placeholders deliberately do not attribute that work to them.

## Validation results

On the seeded suite in `tests/test.py`, 1,364 sorting cases passed, together with
input-error, checker and benchmark checks. The worst adaptive operation counts
among 30 random samples per size were 1,084 for 100 numbers and 7,487 for 500.
These meet the subject's good-performance targets, but not its excellent targets.
Norminette passed all C and header files. Valgrind reported no errors or leaks
across 29 runs covering all strategies, checker execution and error paths.
A second `make all bonus` performed no compilation or relinking.
