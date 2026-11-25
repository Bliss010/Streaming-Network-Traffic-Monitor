# Exact frequency counting — Iteration 3 component
Compile: `g++ -std=c++11 -Wall -Wextra -pedantic exact.cpp test_exact.cpp -o test_exact`
Run: `./test_exact`

Integration: include exact.h and compile exact.cpp with the team application. Create a HashTable instance and call hashUpdate(record.src_ip) once per valid record. For ranking, allocate an Entry array of at least min(k, uniqueCount()) elements for positive k, and call topK(k, output, length). Results are descending by count, then ascending by string key. Negative or zero k returns no results; the menu should reject invalid k and retain the old setting. Do not compile test_exact.cpp with the team's main.cpp (both define main).

Design: custom chained hash table and custom array min-heap, no standard container classes. The heap is rebuilt on request from final exact counts, an alternative explicitly allowed in Iteration 2. heapUpdateTopK expects each distinct key only once per heap build, not once per stream record. CSV parsing, connection-pair tracking, menu integration and approximate structures are not tested here. The table uses fixed bucket count; performance degrades with long chains. Counts use int. Tests use synthetic source strings rather than the team's CSV files.

Complexity: with n records, m distinct sources, B buckets and r=min(k,m), expected counting time O(n(1+m/B)) under uniform hashing, worst case O(nm). Top-k time O(B+m log(r+1)), including extraction; space O(B+m+r), assuming bounded key length. A bounded load factor gives expected O(n) counting.

Bug reproduction: removing the tie condition in worse() caused the tie test to fail when B arrived before A in the heap scan. Restoring the condition makes A win equal counts. See bug_before.txt and test_results.txt.
