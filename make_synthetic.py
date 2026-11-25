"""Generate synthetic network traffic CSV files for testing.

Normal devices talk to a small set of servers again and again.
One scanner device contacts many new machines (label 1 = attack).
Usage: python tools/make_synthetic.py
"""
import random

HEADER = "timestamp,src_ip,dst_ip,dst_port,protocol,bytes,label"


def make(filename, n_records, n_devices=50, scanner_share=0.05, seed=42):
    rng = random.Random(seed)
    devices = [f"10.0.{i // 250}.{i % 250 + 1}" for i in range(n_devices)]
    servers = [f"172.16.0.{i}" for i in range(1, 21)]
    scanner = "10.9.9.9"
    t = 1700000000.0
    with open(filename, "w") as f:
        f.write(HEADER + "\n")
        for _ in range(n_records):
            t += rng.uniform(0.0005, 0.002)
            if rng.random() < scanner_share:
                dst = f"192.168.{rng.randint(0, 255)}.{rng.randint(1, 254)}"
                f.write(f"{t:.6f},{scanner},{dst},{rng.choice([22, 23, 80, 443])},6,60,1\n")
            else:
                src = devices[min(int(rng.paretovariate(1.2)) - 1, n_devices - 1)]
                dst = rng.choice(servers[:5]) if rng.random() < 0.8 else rng.choice(servers)
                f.write(f"{t:.6f},{src},{dst},{rng.choice([80, 443, 53])},6,{rng.randint(60, 1500)},0\n")


if __name__ == "__main__":
    for name, n in [("small", 1_000), ("medium", 10_000), ("large", 100_000)]:
        make(f"tests/test_data/synthetic_{name}.csv", n)
        print(f"Wrote tests/test_data/synthetic_{name}.csv ({n} records)")
