# Streaming Network Traffic Monitor

A C++ streaming monitor that detects unusual network activity using exact and approximate data structures.

## Overview

The program reads network records one at a time and identifies the most active devices, detects new vs. repeated connections, and flags devices that suddenly make many new connections. It compares exact and approximate data structures on memory, speed, and error.

## Data Structures

| Structure | Type | Purpose |
|---|---|---|
| Hash table | Exact | Per-device counts and connection tracking |
| Min-heap | Exact | Top-k most active devices |
| Count-Min Sketch | Approximate | Per-device count estimates in fixed memory |
| Bloom filter | Approximate | Seen-before checks in fixed memory |

All structures are implemented from scratch.

## Build

```
g++ -std=c++17 -Wall -O2 -o monitor src/*.cpp
```

## Run

```
./monitor
```

## Test

```
g++ -std=c++17 -Wall -o run_tests tests/*.cpp
./run_tests
```


## Run Instructions

```
./monitor
```

On Windows, run `monitor.exe`. Then use the menu to open a data file, choose settings, run the stream, and save results to CSV.

