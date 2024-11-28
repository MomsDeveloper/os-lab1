./profiling/run_parallel_profiling.sh ./bin/ema-sort-int ./profiling/ema-sort-int/parallel_fast_ordinary 8 1
./profiling/run_parallel_profiling.sh ./bin/dedup ./profiling/dedup/parallel_fast_ordinary 4 1 40000
./profiling/run_parallel_profiling.sh ./bin/complex ./profiling/complex/parallel_fast_ordinary 8 1 35000

