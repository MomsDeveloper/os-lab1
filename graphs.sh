./profiling/run_parallel_profiling.sh ./bin/ema-sort-int ./graph/ema-sort-int/1 1 1
./profiling/run_parallel_profiling.sh ./bin/dedup ./graph/dedup/1 1 1 20000

./profiling/run_parallel_profiling.sh ./bin/ema-sort-int ./graph/ema-sort-int/2 2 1
./profiling/run_parallel_profiling.sh ./bin/dedup ./graph/dedup/2 2 1 20000

./profiling/run_parallel_profiling.sh ./bin/ema-sort-int ./graph/ema-sort-int/4 4 1
./profiling/run_parallel_profiling.sh ./bin/dedup ./graph/dedup/4 4 1 20000

./profiling/run_parallel_profiling.sh ./bin/ema-sort-int ./graph/ema-sort-int/8 8 1
./profiling/run_parallel_profiling.sh ./bin/dedup ./graph/dedup/8 8 1 20000

./profiling/run_parallel_profiling.sh ./bin/ema-sort-int ./graph/ema-sort-int/16 16 1
./profiling/run_parallel_profiling.sh ./bin/dedup ./graph/dedup/16 16 1 20000

./profiling/run_parallel_profiling.sh ./bin/ema-sort-int ./graph/ema-sort-int/32 32 1
./profiling/run_parallel_profiling.sh ./bin/dedup ./graph/dedup/32 32 1 20000

./profiling/run_parallel_profiling.sh ./bin/ema-sort-int ./graph/ema-sort-int/64 64 1
./profiling/run_parallel_profiling.sh ./bin/dedup ./graph/dedup/64 64 1 20000