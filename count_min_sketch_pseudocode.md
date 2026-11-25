# Count-Min Sketch: Pseudocode (Bliss)

Variables:
- d = number of rows (depth), w = counters per row (width)
- M = memory budget in bytes, each counter is 4 bytes, so w = floor(M / (4 * d))
- table[0..d-1][0..w-1] = counters, all starting at 0
- seed[0..d-1] = a different hash seed for each row
- N = total number of updates (records) in the stream

```
HASH(key, seed):                      // FNV-1a with a seed
    h <- 2166136261 XOR seed
    for each character c in key:
        h <- h XOR c
        h <- (h * 16777619) mod 2^32
    mix h (shift and multiply) so similar keys spread out
    return h

CMS_INIT(memoryKB, d):
    if memoryKB <= 0 or d <= 0: return FAIL
    w <- floor(memoryKB * 1024 / (4 * d))
    if w < 1: return FAIL
    create table[d][w], all set to 0
    for r <- 0 to d-1: seed[r] <- different constant for each row
    return OK

CMS_UPDATE(key, amount):              // standard update
    for r <- 0 to d-1:
        col <- HASH(key, seed[r]) mod w
        table[r][col] <- table[r][col] + amount

CMS_ESTIMATE(key):
    best <- infinity
    for r <- 0 to d-1:
        col <- HASH(key, seed[r]) mod w
        best <- min(best, table[r][col])
    return best

CMS_UPDATE_CONSERVATIVE(key, amount): // improved variant
    target <- CMS_ESTIMATE(key) + amount
    for r <- 0 to d-1:
        col <- HASH(key, seed[r]) mod w
        if table[r][col] < target:
            table[r][col] <- target   // raise only counters that are too low
```

## Complexity

| Operation | Best case | Worst case | Notes |
|---|---|---|---|
| HASH | Θ(L) | Θ(L) | L = length of the key (an IP string, at most 15 characters, so effectively constant) |
| CMS_UPDATE | Θ(d) | Θ(d) | one counter per row |
| CMS_ESTIMATE | Θ(d) | Θ(d) | reads one counter per row |
| CMS_UPDATE_CONSERVATIVE | Θ(d) | Θ(d) | one estimate (d reads) plus d writes |
| Whole stream of N records | Θ(N · d) | Θ(N · d) | independent of how many distinct devices there are |
| Space | Θ(d · w) | Θ(d · w) | fixed by the memory budget, does not grow with traffic |

## Error guarantee
- The estimate is never smaller than the true count (it can only overcount).
- With w = ⌈e/ε⌉ and d = ⌈ln(1/δ)⌉, the estimate is at most the true count + εN with probability at least 1 − δ.
- Conservative update never gives a larger estimate than the standard update.
