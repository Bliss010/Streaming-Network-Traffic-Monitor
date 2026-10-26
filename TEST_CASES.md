# Test Cases: Streaming Network Monitor

Prepared by Bliss (Member C). All test files are in `tests/test_data/`.
Every file uses the same format as the real dataset sample:

```
timestamp,src_ip,dst_ip,dst_port,protocol,bytes,label
```

A **device** is a `src_ip`. A **connection** is a (`src_ip` → `dst_ip`) pair. A line counts as **broken** if it is empty, has the wrong number of fields, has an empty IP, has text where a number should be, has a port outside 0–65535, or has negative bytes.

> Note for the parser: if the last window is not full when the file ends, `processStream()` still runs `detectSpike()` on that partial window. Several tests below depend on this.

---

## Normal inputs

| ID | Test | File | Settings | Expected result |
|---|---|---|---|---|
| TC01 | Exact counts on a small known file | `tc01_small_known.csv` (20 records) | mode = exact, k = 3 | Counts: 10.0.0.1 = 8, 10.0.0.2 = 6, 10.0.0.3 = 4, 10.0.0.4 = 2. Top-3: 10.0.0.1, 10.0.0.2, 10.0.0.3 in that order. Distinct connections = 10 |
| TC02 | CMS never undercounts | `tc02_random_10000.csv` | mode = both, memory = 64 KB | For every device, `cmsEstimate(dev) ≥ hashCount(dev)` |
| TC03 | Bloom filter "new" is always right | `tc02_random_10000.csv` | mode = both, memory = 64 KB | Every time `bloomContains()` returns false, `hashHasPair()` also returns false |
| TC04 | Spike detection catches one attacker | `tc04_spike.csv` (200 records) | window = 200, threshold = 50 | Exactly 1 alert: device 10.0.0.99 with 100 new connections. The 5 normal devices (10.0.0.11–15) each have 2 new connections and no alert |
| TC05 | Alerts match attack labels | `tc04_spike.csv` | window = 200, threshold = 50 | `matchLabels()`: 1 true positive, 0 false positives, 0 missed attacks. Precision = 1.0, recall = 1.0 |
| TC06 | Results file is written | `tc01_small_known.csv` | k = 3, then menu option 10 | `results.csv` is created with the 3 top devices and their counts |

## Boundary inputs

| ID | Test | File | Settings | Expected result |
|---|---|---|---|---|
| TC07 | Completely empty file | `tc05_empty.csv` (0 bytes) | defaults | Message "No records"; no crash; returns to menu |
| TC08 | Header only, no data | `tc06_header_only.csv` | defaults | Message "No records"; no crash; empty results file |
| TC09 | One record | `tc07_one_record.csv` | k = 10 | Count of 10.0.0.7 = 1; top-k shows only that device; 1 new connection |
| TC10 | k larger than number of devices | `tc08_five_devices.csv` (15 records) | k = 100 | Shows all 5 devices: 10.0.0.25 = 5, .24 = 4, .23 = 3, .22 = 2, .21 = 1. No error |
| TC11 | k = 1 | `tc08_five_devices.csv` | k = 1 | Shows only 10.0.0.25 = 5 |
| TC12 | Tiny memory | `tc02_random_10000.csv` | memory = 1 KB | Program finishes without crashing. CMS error is much higher than at 64 KB, but TC02 still holds (no undercounts) |
| TC13 | Threshold of 0 | `tc01_small_known.csv` | window = 20, threshold = 0 | All 4 devices are flagged: 10.0.0.1 (4 new), 10.0.0.2 (3), 10.0.0.3 (2), 10.0.0.4 (1) |

## Invalid inputs

| ID | Test | Input | Expected result |
|---|---|---|---|
| TC14 | Missing file | Type `nofile.csv` at menu option 1 | Message "File not found"; returns to menu |
| TC15 | Broken lines | `tc11_broken_lines.csv` (12 data lines) | 5 valid, 7 broken lines skipped and counted. Counts: 10.0.0.31 = 3, 10.0.0.32 = 1, 10.0.0.33 = 1. Device 10.0.0.34 never appears (both its lines are broken) |
| TC16 | Letters at the menu | Type `abc` | Message "Invalid choice"; menu shown again |
| TC17 | Out-of-range menu number | Type `42` or `-1` | Message "Invalid choice"; menu shown again |
| TC18 | Negative k | Option 4, type `-5` | Rejected with a message; k stays at its old value |
| TC19 | Zero memory | Option 3, type `0` | Rejected with a message; memory stays at its old value |
| TC20 | Run before opening a file | Option 6 before option 1 | Message "Open a file first"; no crash |

---

## Which lines in `tc11_broken_lines.csv` are broken

| Line | Problem | Valid? |
|---|---|---|
| 1 | — | Yes |
| 2 | — | Yes |
| 3 | Missing fields | No |
| 4 | Text (`abc`) in bytes | No |
| 5 | Empty line | No |
| 6 | — | Yes |
| 7 | Empty src_ip | No |
| 8 | Port 99999 is out of range | No |
| 9 | — | Yes |
| 10 | Too many fields | No |
| 11 | — | Yes |
| 12 | Negative bytes | No |
