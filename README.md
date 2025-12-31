# Banking Hash Tables (C++)

Hash tables for bank account storage using only `std::vector`. Implemented collision strategies: separate chaining, linear probing, quadratic probing, cubic probing, a tuned competitive variant, and double hashing. Every strategy supports: create account, get balance, add transaction, delete account, check existence, and top‑K balances.

## Layout
- `include/` — base interface (`BaseClass`) and headers for each strategy.
- `src/` — implementations for chaining, linear/quadratic/cubic probing, competitive (tuned) probing, and double hashing.
- `tests/` — assert-based smoke test and a CLI harness.
- `tests/data/test2/` — sample command-driven testcase with expected output.
- `scripts/` — build/run script.

## Build & quick run
```bash
chmod +x scripts/run.sh   # once
./scripts/run.sh
```
This compiles with `-std=c++17 -O2`, runs the assert tests, builds the CLI harness, and diffs `tests/data/test2/in.txt` against `tests/data/test2/out.txt`.

## Manual CLI example
```bash
g++ -std=c++17 -O2 -Iinclude src/*.cpp tests/cli.cpp -o cli
./cli tests/data/test2/in.txt
```
Input (`tests/data/test2/in.txt`):
```
createAccount CDAD7786825_7990768648 648
createAccount DCDA7547234_9919615552 552
createAccount AACB1850545_7974534788 788
databaseSize
createAccount CDBD5250777_1124276711 711
createAccount ABCB8377155_0656808575 575
getTopK 1
createAccount CDDC3792028_3313680038 38
createAccount CBBA9574444_7470182321 321
createAccount DBCC4334636_8057544188 188
getTopK 3
createAccount BABD5741078_5109455304 304
createAccount BCBA7529510_0817878868 868
databaseSize
getTopK 1
addTransaction BCBA7529510_0817878868 -860
getBalance BCBA7529510_0817878868
getTopK 1
addTransaction DCDA7547234_9919615552 592
getTopK 5
deleteAccount DCDA7547234_9919615552
getTopK 4
databaseSize
```
Expected output (matches `tests/data/test2/out.txt`):
```
3
788
788 711 648
10
868
8
788
1144 788 711 648 575
788 711 648 575
9
```

## Collision strategies
- Separate chaining on `bankStorage2d`.
- Open addressing on `bankStorage1d` with linear, quadratic, or cubic probing.
- Competitive/tuned probing uses a type-aware rolling hash (letters/digits mixed with avalanche) to reduce clustering.
- Double hashing adds a secondary step size derived from the same hash family.

## Notes
- Only `std::vector` is used for storage.
- All strategies share the same interface; swap implementations without changing callers.
- Collision counters are available if you want to add a benchmark driver.
