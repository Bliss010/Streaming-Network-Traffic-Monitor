# Streaming Network Traffic Monitor

## Project Description

A menu-driven C++ program that processes network traffic records as a stream. It identifies the most active devices, detects new and repeated connections, and flags devices with a sudden spike in new connections. Exact data structures (hash table, min-heap) are compared with approximate ones (Count-Min Sketch, Bloom filter) on memory, speed, and error.

## Build Instructions

Requires g++ with C++17 support.

```
g++ -std=c++17 -Wall -O2 -o monitor src/*.cpp
```

## Run Instructions

```
./monitor
```

On Windows, run `monitor.exe`. Use the menu to load a data file, configure settings, process the stream, and export results to CSV.

## Current Progress

**Iteration 1: Complete**
- Program design, pseudocode, and testing plan

**Iteration 2: In Progress**
- Stream parser, hash table, and min-heap
- Count-Min Sketch and Bloom filter
- Spike detection, benchmarking, and CSV export

A C++ streaming monitor that detects unusual network activity using exact and approximate data structures.

## Data Structures

| Structure | Type | Purpose |
|---|---|---|
| Hash table | Exact | Per-device counts and connection tracking |
| Min-heap | Exact | Top-k most active devices |
| Count-Min Sketch | Approximate | Per-device count estimates in fixed memory |
| Bloom filter | Approximate | Seen-before checks in fixed memory |

All structures are implemented from scratch.



On Windows, run `monitor.exe`. Then use the menu to open a data file, choose settings, run the stream, and save results to CSV.

