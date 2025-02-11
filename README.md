# Лабораторная работа №1

Вариант: 

* ForkKind: `vfork`
* Bench 1: `(ema-sort-int)` - Сортировка массива чисел во внешней памяти
* Bench 2: `(dedup)` - Дедупликация элементов в массиве

## Задание Часть 1. Запуск программ

Необходимо реализовать собственную оболочку командной строки - shell. Выбор ОС для реализации производится на усмотрение студента. Shell должен предоставлять пользователю возможность запускать программы на компьютере с переданными аргументами командной строки и после завершения программы показывать реальное время ее работы (подсчитать самостоятельно как «время завершения» – «время запуска»).

## Задание Часть 2. Мониторинг и профилирование

Разработать комплекс программ-нагрузчиков по варианту, заданному преподавателем. Каждый нагрузчик должен, как минимум, принимать параметр, который определяет количество повторений для алгоритма, указанного в задании. Программы должны нагружать вычислительную систему, дисковую подсистему или обе подсистемы сразу. Необходимо скомпилировать их без опций оптимизации компилятора.

Перед запуском нагрузчика, попробуйте оценить время работы вашей программы или ее результаты (если по варианту вам досталось измерение чего-либо). Постарайтесь обосновать свои предположения. Предположение можно сделать, основываясь на своем опыте, знаниях ОС и характеристиках используемого аппаратного обеспечения.

1. Запустите программу-нагрузчик и зафиксируйте метрики ее работы с помощью инструментов для профилирования. Сравните полученные результаты с ожидаемыми. Постарайтесь найти объяснение наблюдаемому.

2. Определите количество нагрузчиков, которое эффективно нагружает все ядра процессора на вашей системе. Как распределяются времена `USER%`, `SYS%`, `WAIT%`, а также реальное время выполнения нагрузчика? Какое количество переключений контекста (вынужденных и невынужденных) происходит при этом?

3. Увеличьте количество нагрузчиков вдвое, втрое, вчетверо. Как изменились времена, указанные на предыдущем шаге? Как ведет себя ваша система?

4. Объедините программы-нагрузчики в одну, реализованную при помощи потоков выполнения, чтобы один нагрузчик эффективно нагружал все ядра вашей системы. Как изменились времена для того же объема вычислений? Запустите одну, две, три таких программы.

5. Добавьте опции агрессивной оптимизации для компилятора. Как изменились времена? На сколько сократилось реальное время исполнения программы нагрузчика?

## Ограничения

Программа (комплекс программ) должна быть реализована на языке C, C++. Дочерние процессы должны быть созданы через заданные системные вызовы выбранной операционной системы, с обеспечением корректного запуска и завершения процессов. Запрещено использовать высокоуровневые абстракции над системными вызовами. Необходимо использовать, в случае Unix, процедуры `libc`.


## Выполнение

Часть 1. Запуск программ

<!-- 1. Реализована оболочка командной строки - add link to main file -->
1. Реализована оболочка командной строки - [shell.c](./src/shell.c). Программа запускает программы с переданными аргументами и показывает время работы программы.

Часть 2. Мониторинг и профилирование

1. Разработаны программы-нагрузчики:
    * [ema-sort-int.c](./src/ema-sort-int.c) - Сортировка массива чисел во внешней памяти
    * [dedup.c](./src/dedup.c) - Дедупликация элементов в массиве
    * [complex.c](./src/complex.c) - Комплексная программа-нагрузчик, объединяющая две предыдущие программы в одну, реализованную при помощи потоков выполнения

2. Предположения о свойствах программ-нагрузчиков:

Алгоритм `ema-sort-int`

- NlogN - кол-во i/o операций
- 1mb - размер файла
- 200 скорость чтения
- 200 скорость записи

- N = 256 * 1024 (кол-во чисел)
- k = 1024 (размер буфера)

$$NlogN * ((2 * 1mb / (k*200)) + (2 * 1mb / (k*200))) = 27.74s$$

Алгоритм `dedup`

 - N - кол-во элементов
 - (N + N*3 + (N*N/2)*3) - кол-во итераций
 - 3.2 ГГц - скорость процессора

$$(N + N*3 + (N*N/2)*3) / (3.2 * 10^9) = 4.915328s$$

3. Сравнительные хараетиристики для различного кол-ва нагрузчиков

Алгоритм `ema-sort-int`

| numb | cs   | task-clock (msec) | %usr  | %sys  | %iowait | %cpu | used Mem total | ins/per cycle |
|------|------|-------------------|-------|-------|---------|------|----------------|---------------|
| 1    | 1954 | 287.24           | 5.38  | 21.54 | 23.12   | 36.4 | 589            | 0.64          |
| 2    | 1984 | 304.89           | 9.63  | 45.99 | 38.50   | 27.3 | 592.5          | 0.61          |
| 4    | 2447 | 418.61           | 14.51 | 59.07 | 23.83   | 6.3  | 612            | 0.50          |
| 8    | 2122 | 332.83           | 14.95 | 64.43 | 20.62   | 6.6  | 622.2          | 0.61          |
| 16   | 1980 | 331.20           | 18.59 | 73.37 | 8.00    | 6.5  | 688.4          | 0.57          |
| 32   | 2019 | 340.76           | 20.92 | 77.04 | 0.51    | 2.5  | 773.8          | 0.55          |
| 64   | 1992 | 362.67           | 24.87 | 67.01 | 8.12    | 1.2  | 893.9          | 0.55          |


Алгоритм `dedup`

| numb | cs  | task-clock (msec) | %usr  | %sys  | %iowait | %cpu | used Mem total | ins/per cycle |
|------|-----|-------------------|-------|-------|---------|------|----------------|---------------|
| 1    | 16  | 605.48           | 33.51 | 18.56 | 0       | 100  | 590            | 0.91          |
| 2    | 37  | 617.94           | 64.50 | 35.50 | 0       | 100  | 592.3          | 0.90          |
| 4    | 343 | 648.24           | 97.50 | 2.00  | 0       | 46.2 | 612.9          | 0.90          |
| 8    | 356 | 663.57           | 96.52 | 2.99  | 0       | 15.4 | 620            | 0.89          |
| 16   | 416 | 664.55           | 97.99 | 2.00  | 0       | 12.3 | 648.5          | 0.90          |
| 32   | 364 | 671.88           | 98.00 | 1.99  | 0       | 6.3  | 693            | 0.89          |
| 64   | 383 | 654.65           | 98.51 | 1.49  | 0       | 3.2  | 797            | 0.90          |

Профилирование программы `ema-sort-int`:
Один нагрузчик
* [mpstat](./profiling/ema-sort-int/ordinary/mpstat.txt)
```
12:47:36 AM  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
12:47:37 AM  all    9.95    0.00    9.42   34.55    0.00    0.00    1.05    0.00    0.00   45.03
12:47:37 AM    0   14.89    0.00   11.70   47.87    0.00    0.00    1.06    0.00    0.00   24.47
12:47:37 AM    1    5.15    0.00    7.22   21.65    0.00    0.00    1.03    0.00    0.00   64.95
```
* [top](./profiling/ema-sort-int/ordinary/top.txt)
```
top - 00:47:28 up 17:30,  2 users,  load average: 0.50, 0.25, 2.52
Tasks: 126 total,   1 running, 125 sleeping,   0 stopped,   0 zombie
%Cpu(s): 12.0 us, 12.0 sy,  0.0 ni, 40.0 id, 32.0 wa,  0.0 hi,  4.0 si,  0.0 st 
MiB Mem :   1967.8 total,   1324.3 free,    605.8 used,    190.3 buff/cache     
MiB Swap:      0.0 total,      0.0 free,      0.0 used.   1362.0 avail Mem 

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
  52434 root      20   0    2684   1536   1408 D  25.0   0.1   0:00.05 ema-sor+
```

* [perf](./profiling/ema-sort-int/ordinary/perf_stat.txt)
```
# started on Fri Dec 13 00:47:09 2024


 Performance counter stats for './bin/ema-sort-int 1 ./profiling/ema-sort-int/ordinary/file.bin':

          2,912.20 msec task-clock                       #    0.526 CPUs utilized             
            19,034      context-switches                 #    6.536 K/sec                     
        67,696,289      cache-misses                     #   56.03% of all cache refs         
       120,818,147      cache-references                 #   41.487 M/sec                     
     1,441,719,776      instructions                     #    0.61  insn per cycle            
     2,369,010,459      cycles                           #    0.813 GHz                       
       641,736,061      L1-dcache-loads                  #  220.361 M/sec                     
                 0      L1-dcache-load-misses                                                 
   <not supported>      LLC-loads                                                             
   <not supported>      LLC-load-misses                                                       

       5.536177603 seconds time elapsed

       0.242781000 seconds user
       1.975013000 seconds sys


```
Количество нагрузчиков: 3
* [mpstat](./profiling/ema-sort-int/parallel_ordinary/mpstat.log)
```
06:46:16 PM  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
06:46:17 PM  all   16.49    0.00   42.27   38.66    0.00    0.00    0.00    0.00    0.00    2.58
06:46:17 PM    0   20.62    0.00   38.14   37.11    0.00    0.00    0.00    0.00    0.00    4.12
06:46:17 PM    1   12.37    0.00   46.39   40.21    0.00    0.00    0.00    0.00    0.00    1.03
```
* [top](./profiling/ema-sort-int/parallel_ordinary/top.log)
```
top - 18:46:05 up 11:28,  1 user,  load average: 0.64, 0.44, 0.31
Tasks: 136 total,   3 running, 133 sleeping,   0 stopped,   0 zombie
%Cpu(s): 16.1 us, 64.6 sy,  0.0 ni,  1.2 id, 17.5 wa,  0.0 hi,  0.5 si,  0.2 st 
MiB Mem :   1967.8 total,    306.2 free,    611.9 used,   1220.2 buff/cache     
MiB Swap:      0.0 total,      0.0 free,      0.0 used.   1355.9 avail Mem 

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
  12704 root      20   0    3084   1792   1408 R  36.1   0.1   0:03.09 ema-sor+
  12696 root      20   0    3084   1920   1408 R  34.4   0.1   0:02.98 ema-sor+
  12700 root      20   0    3084   1792   1408 D  32.5   0.1   0:02.99 ema-sor+
```
* [perf](./profiling/ema-sort-int/parallel_ordinary/instance_2/perf.log)
```
# started on Thu Dec 12 18:45:56 2024


 Performance counter stats for './bin/ema-sort-int 1 /root/os-lab1/profiling/ema-sort-int/parallel_ordinary/instance_2/file.bin':

          3,334.43 msec task-clock                       #    0.366 CPUs utilized             
            21,388      context-switches                 #    6.414 K/sec                     
        73,744,007      cache-misses                     #   54.20% of all cache refs         
       136,065,953      cache-references                 #   40.806 M/sec                     
     1,555,548,585      instructions                     #    0.57  insn per cycle            
     2,716,227,820      cycles                           #    0.815 GHz                       
       685,817,816      L1-dcache-loads                  #  205.678 M/sec                     
                 0      L1-dcache-load-misses                                                 
   <not supported>      LLC-loads                                                             
   <not supported>      LLC-load-misses                                                       

       9.111521773 seconds time elapsed

       0.273016000 seconds user
       2.794301000 seconds sys


```
Профилирование программы `dedup`:
Один нагрузчик
* [mpstat](./profiling/dedup/ordinary/mpstat.txt)
```
07:18:29 PM  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
07:18:30 PM  all   52.02    0.00    0.51    0.00    0.00    0.00    0.00    0.00    0.00   47.47
07:18:30 PM    0  100.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00
07:18:30 PM    1    3.06    0.00    1.02    0.00    0.00    0.00    0.00    0.00    0.00   95.92
```
* [top](./profiling/dedup/ordinary/top.txt)
```
top - 19:18:17 up 12:01,  1 user,  load average: 0.92, 0.36, 0.20
Tasks: 126 total,   2 running, 124 sleeping,   0 stopped,   0 zombie
%Cpu(s): 50.8 us,  0.5 sy,  0.0 ni, 48.7 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st 
MiB Mem :   1967.8 total,    311.6 free,    603.9 used,   1222.8 buff/cache     
MiB Swap:      0.0 total,      0.0 free,      0.0 used.   1363.9 avail Mem 

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
  14463 root      20   0    3084   1536   1152 R  99.7   0.1   0:06.23 dedup
    927 root      20   0  436896  37632  18048 S   0.3   1.9   1:08.41 fail2ba+
   3859 root      20   0   11.3g 110368  46848 S   0.3   5.5   0:45.55 node
```
* [perf](./profiling/dedup/ordinary/perf_stat.txt)
```
# started on Thu Dec 12 19:17:09 2024


 Performance counter stats for './bin/dedup 1 102400':

         16,130.41 msec task-clock                       #    0.996 CPUs utilized             
               287      context-switches                 #   17.792 /sec                      
        21,434,643      cache-misses                     #    7.10% of all cache refs         
       301,853,617      cache-references                 #   18.713 M/sec                     
    45,159,845,053      instructions                     #    0.89  insn per cycle            
    50,624,288,501      cycles                           #    3.138 GHz                       
    17,088,819,336      L1-dcache-loads                  #    1.059 G/sec                     
                 0      L1-dcache-load-misses                                                 
   <not supported>      LLC-loads                                                             
   <not supported>      LLC-load-misses                                                       

      16.202531519 seconds time elapsed

      16.074861000 seconds user
       0.038096000 seconds sys

```
Количество нагрузчиков: 2
* [mpstat](./profiling/dedup/parallel_ordinary/mpstat.log)
```
06:46:37 PM  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
06:46:38 PM  all   99.00    0.00    0.50    0.00    0.00    0.00    0.50    0.00    0.00    0.00
06:46:38 PM    0   98.02    0.00    0.99    0.00    0.00    0.00    0.99    0.00    0.00    0.00
06:46:38 PM    1  100.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00
```
* [top](./profiling/dedup/parallel_ordinary/top.log)
```
top - 18:46:27 up 11:29,  1 user,  load average: 1.50, 0.66, 0.39
Tasks: 133 total,   3 running, 130 sleeping,   0 stopped,   0 zombie
%Cpu(s): 79.2 us, 16.7 sy,  0.0 ni,  4.2 id,  0.0 wa,  0.0 hi,  0.0 si,  0.0 st 
MiB Mem :   1967.8 total,    305.5 free,    612.5 used,   1220.3 buff/cache     
MiB Swap:      0.0 total,      0.0 free,      0.0 used.   1355.3 avail Mem 

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
  12840 root      20   0    2844   1408   1152 R  91.7   0.1   0:00.19 dedup
  12841 root      20   0    2844   1280   1152 R  83.3   0.1   0:00.17 dedup
```
* [perf](./profiling/dedup/parallel_ordinary/instance_2/perf.log)
```
# started on Thu Dec 12 18:46:27 2024


 Performance counter stats for './bin/dedup 1 40000':

          2,496.36 msec task-clock                       #    0.934 CPUs utilized             
               310      context-switches                 #  124.181 /sec                      
         3,923,589      cache-misses                     #   10.43% of all cache refs         
        37,610,048      cache-references                 #   15.066 M/sec                     
     6,918,920,764      instructions                     #    0.90  insn per cycle            
     7,702,954,092      cycles                           #    3.086 GHz                       
     2,617,232,348      L1-dcache-loads                  #    1.048 G/sec                     
                 0      L1-dcache-load-misses                                                 
   <not supported>      LLC-loads                                                             
   <not supported>      LLC-load-misses                                                       

       2.673795618 seconds time elapsed

       2.462842000 seconds user
       0.018728000 seconds sys



```
Профилирование программы `complex`:
Один нагрузчик
* [mpstat](./profiling/complex/ordinary/mpstat.txt)
```
07:19:24 PM  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
07:19:25 PM  all   30.89    0.00    6.28   37.17    0.00    0.00    0.52    0.00    0.00   25.13
07:19:25 PM    0   51.52    0.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00   48.48
07:19:25 PM    1    8.70    0.00   13.04   77.17    0.00    0.00    1.09    0.00    0.00    0.00
```
* [top](./profiling/complex/ordinary/top.txt)
```
top - 19:19:19 up 12:02,  1 user,  load average: 1.36, 0.58, 0.29
Tasks: 128 total,   1 running, 127 sleeping,   0 stopped,   0 zombie
%Cpu(s): 43.4 us,  7.7 sy,  0.0 ni, 11.9 id, 36.9 wa,  0.0 hi,  0.0 si,  0.2 st 
MiB Mem :   1967.8 total,    305.4 free,    610.1 used,   1222.9 buff/cache     
MiB Swap:      0.0 total,      0.0 free,      0.0 used.   1357.7 avail Mem 

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
  14654 root      20   0   84772   2048   1536 S  94.4   0.1   0:03.10 complex
```
* [perf](./profiling/complex/ordinary/perf_stat.txt)
```
# started on Thu Dec 12 19:18:44 2024


 Performance counter stats for './bin/complex 1 40000 ./profiling/complex/ordinary/file1.bin':

          5,317.56 msec task-clock                       #    0.935 CPUs utilized             
            18,540      context-switches                 #    3.487 K/sec                     
        60,045,625      cache-misses                     #   43.14% of all cache refs         
       139,173,811      cache-references                 #   26.172 M/sec                     
     8,304,969,744      instructions                     #    0.84  insn per cycle            
     9,835,540,979      cycles                           #    1.850 GHz                       
     3,222,665,066      L1-dcache-loads                  #  606.042 M/sec                     
           617,396      L1-dcache-load-misses            #    0.02% of all L1-dcache accesses 
   <not supported>      LLC-loads                                                             
   <not supported>      LLC-load-misses                                                       

       5.687995009 seconds time elapsed

       2.775914000 seconds user
       1.856612000 seconds sys

```
Количество нагрузчиков: 2
* [mpstat](./profiling/complex/parallel_ordinary/mpstat.log)
```
06:46:45 PM  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
06:46:46 PM  all   35.42    0.00   40.10   18.23    0.00    2.08    0.00    0.00    0.00    4.17
06:46:46 PM    0   17.89    0.00   52.63   18.95    0.00    2.11    0.00    0.00    0.00    8.42
06:46:46 PM    1   52.58    0.00   27.84   17.53    0.00    2.06    0.00    0.00    0.00    0.00
```
* [top](./profiling/complex/parallel_ordinary/top.log)
```
top - 18:46:44 up 11:29,  1 user,  load average: 2.00, 0.81, 0.44
Tasks: 133 total,   1 running, 132 sleeping,   0 stopped,   0 zombie
%Cpu(s): 65.7 us, 33.7 sy,  0.0 ni,  0.2 id,  0.2 wa,  0.0 hi,  0.2 si,  0.2 st 
MiB Mem :   1967.8 total,    296.0 free,    623.2 used,   1219.2 buff/cache     
MiB Swap:      0.0 total,      0.0 free,      0.0 used.   1344.6 avail Mem 

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
  12973 root      20   0   84776   1664   1408 S  87.4   0.1   0:02.64 complex
  12974 root      20   0   84776   1664   1408 S  74.2   0.1   0:02.24 complex
```
* [perf](./profiling/complex/parallel_ordinary/instance_2/perf.log)
```
# started on Thu Dec 12 18:46:41 2024


 Performance counter stats for './bin/complex 1 40000 /root/os-lab1/profiling/complex/parallel_ordinary/instance_2/file1.bin':

          6,344.93 msec task-clock                       #    0.646 CPUs utilized             
            23,939      context-switches                 #    3.773 K/sec                     
        81,563,056      cache-misses                     #   45.33% of all cache refs         
       179,932,703      cache-references                 #   28.359 M/sec                     
     8,487,233,455      instructions                     #    0.80  insn per cycle            
    10,572,233,411      cycles                           #    1.666 GHz                       
     3,307,265,554      L1-dcache-loads                  #  521.245 M/sec                     
            24,093      L1-dcache-load-misses            #    0.00% of all L1-dcache accesses 
   <not supported>      LLC-loads                                                             
   <not supported>      LLC-load-misses                                                       

       9.815674007 seconds time elapsed

       3.044648000 seconds user
       2.883935000 seconds sys

```

Профилирование программы `ema-sort-int` с агрессивной оптимизацией:
Один нагрузчик
* [mpstat](./profiling/ema-sort-int/agressive_ordinary/mpstat.txt)
```
07:49:04 PM  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
07:49:05 PM  all   10.53    0.00   11.05   39.47    0.00    0.53    0.00    0.00    0.00   38.42
07:49:05 PM    0   15.31    0.00    9.18    1.02    0.00    0.00    0.00    0.00    0.00   74.49
07:49:05 PM    1    5.43    0.00   13.04   80.43    0.00    1.09    0.00    0.00    0.00    0.00
```
* [top](./profiling/ema-sort-int/agressive_ordinary/top.txt)
```
top - 19:49:00 up 12:31,  1 user,  load average: 0.52, 0.52, 0.30
Tasks: 127 total,   2 running, 125 sleeping,   0 stopped,   0 zombie
%Cpu(s): 10.0 us, 10.7 sy,  0.0 ni, 42.1 id, 36.7 wa,  0.0 hi,  0.2 si,  0.3 st 
MiB Mem :   1967.8 total,    278.5 free,    634.6 used,   1225.4 buff/cache     
MiB Swap:      0.0 total,      0.0 free,      0.0 used.   1333.2 avail Mem 

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
   3959 root      20   0   11.2g  59808  43264 S  24.3   3.0   2:46.50 node
  15842 root      20   0    3084   1792   1408 R  18.3   0.1   0:00.57 ema-sor+
```
* [perf](./profiling/ema-sort-int/agressive_ordinary/perf_stat.txt)
```
# started on Thu Dec 12 19:48:39 2024


 Performance counter stats for './bin/ema-sort-int 1 ./profiling/ema-sort-int/agressive_ordinary/file.bin':

          2,700.16 msec task-clock                       #    0.499 CPUs utilized             
            18,649      context-switches                 #    6.907 K/sec                     
        61,665,882      cache-misses                     #   54.04% of all cache refs         
       114,102,526      cache-references                 #   42.258 M/sec                     
       895,912,991      instructions                     #    0.51  insn per cycle            
     1,760,990,628      cycles                           #    0.652 GHz                       
       316,241,577      L1-dcache-loads                  #  117.120 M/sec                     
                 0      L1-dcache-load-misses                                                 
   <not supported>      LLC-loads                                                             
   <not supported>      LLC-load-misses                                                       

       5.415306172 seconds time elapsed

       0.085334000 seconds user
       1.954265000 seconds sys


```
Количество нагрузчиков: 2
* [mpstat](./profiling/ema-sort-int/agressive_parallel/mpstat.log)
```
09:16:31 PM  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
09:16:32 PM  all   10.31    0.00   50.52   39.18    0.00    0.00    0.00    0.00    0.00    0.00
09:16:32 PM    0    8.25    0.00   52.58   39.18    0.00    0.00    0.00    0.00    0.00    0.00
09:16:32 PM    1   12.37    0.00   48.45   39.18    0.00    0.00    0.00    0.00    0.00    0.00
```
* [top](./profiling/ema-sort-int/agressive_parallel/top.log)
```
top - 21:16:37 up 13:59,  2 users,  load average: 0.61, 0.55, 0.48
Tasks: 133 total,   1 running, 132 sleeping,   0 stopped,   0 zombie
%Cpu(s): 12.7 us, 61.9 sy,  0.0 ni,  0.3 id, 24.4 wa,  0.0 hi,  0.5 si,  0.2 st 
MiB Mem :   1967.8 total,    288.1 free,    618.2 used,   1232.3 buff/cache     
MiB Swap:      0.0 total,      0.0 free,      0.0 used.   1349.6 avail Mem 

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
  16952 root      20   0   11.2g  60784  43264 S  45.5   3.0   0:20.78 node
  19295 root      20   0    3088   1792   1408 D  33.3   0.1   0:02.90 ema-sor+
  19298 root      20   0    3088   1920   1408 D  32.7   0.1   0:02.80 ema-sor+
  19293 root      20   0    3088   1792   1408 D  30.4   0.1   0:02.63 ema-sor+
```
* [perf](./profiling/ema-sort-int/agressive_parallel/instance_2/perf.log)
```
# started on Thu Dec 12 21:16:28 2024


 Performance counter stats for './bin/ema-sort-int 1 /root/os-lab1/profiling/ema-sort-int/agressive_parallel/instance_2/file.bin':

          3,049.17 msec task-clock                       #    0.344 CPUs utilized             
            20,971      context-switches                 #    6.878 K/sec                     
        68,778,369      cache-misses                     #   53.00% of all cache refs         
       129,759,841      cache-references                 #   42.556 M/sec                     
     1,008,673,805      instructions                     #    0.49  insn per cycle            
     2,066,398,580      cycles                           #    0.678 GHz                       
       365,815,068      L1-dcache-loads                  #  119.972 M/sec                     
                 0      L1-dcache-load-misses                                                 
   <not supported>      LLC-loads                                                             
   <not supported>      LLC-load-misses                                                       

       8.864652136 seconds time elapsed

       0.083431000 seconds user
       2.575841000 seconds sys


```
Профилирование программы `dedup` с агрессивной оптимизацией 
Один нагрузчик (кол-во чисел 1024000)
* [mpstat](./profiling/dedup/agressive_ordinary/mpstat.txt)
```
11:28:54 PM  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
11:28:55 PM  all    6.03    0.00    3.52    0.00    0.00    0.00    0.00    0.00    0.00   90.45
11:28:55 PM    0    8.08    0.00    3.03    0.00    0.00    0.00    0.00    0.00    0.00   88.89
11:28:55 PM    1    4.00    0.00    4.00    0.00    0.00    0.00    0.00    0.00    0.00   92.00
```

* [perf](./profiling/dedup/agressive_ordinary/perf_stat.txt)
```
# started on Thu Dec 12 23:26:28 2024


 Performance counter stats for './bin/dedup 1 1024000':

             19.06 msec task-clock                       #    0.926 CPUs utilized             
                 2      context-switches                 #  104.959 /sec                      
            83,828      cache-misses                     #   10.53% of all cache refs         
           795,855      cache-references                 #   41.766 M/sec                     
        79,372,920      instructions                     #    1.47  insn per cycle            
        53,898,337      cycles                           #    2.829 GHz                       
        25,310,455      L1-dcache-loads                  #    1.328 G/sec                     
                 0      L1-dcache-load-misses                                                 
   <not supported>      LLC-loads                                                             
   <not supported>      LLC-load-misses                                                       

       0.020577966 seconds time elapsed

       0.013870000 seconds user
       0.006401000 seconds sys

```
Количество нагрузчиков: 2
* [mpstat](./profiling/dedup/agressive_parallel/mpstat.log)
```
09:17:01 PM  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
09:17:02 PM  all   15.58    0.00   84.42    0.00    0.00    0.00    0.00    0.00    0.00    0.00
09:17:02 PM    0   16.00    0.00   84.00    0.00    0.00    0.00    0.00    0.00    0.00    0.00
09:17:02 PM    1   15.15    0.00   84.85    0.00    0.00    0.00    0.00    0.00    0.00    0.00
```
* [top](./profiling/dedup/agressive_parallel/top.log)
```
top - 21:17:02 up 13:59,  2 users,  load average: 1.54, 0.78, 0.56
Tasks: 142 total,   3 running, 138 sleeping,   1 stopped,   0 zombie
%Cpu(s): 17.2 us, 82.5 sy,  0.0 ni,  0.0 id,  0.0 wa,  0.0 hi,  0.2 si,  0.2 st 
MiB Mem :   1967.8 total,    286.9 free,    619.2 used,   1232.5 buff/cache     
MiB Swap:      0.0 total,      0.0 free,      0.0 used.   1348.6 avail Mem 

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
  19444 root      20   0    5044   3072   2560 R  75.2   0.2   0:02.42 ltrace
  19445 root      20   0    5044   3072   2560 S  74.8   0.2   0:02.39 ltrace
  19447 root      20   0    2844   1280   1152 R  19.9   0.1   0:00.63 dedup
  19446 root      20   0    2844   1280   1152 t  19.5   0.1   0:00.62 dedup
```
* [perf](./profiling/dedup/agressive_parallel/instance_2/perf.log)
```
# started on Thu Dec 12 21:16:59 2024


 Performance counter stats for './bin/dedup 1 40000':

              1.69 msec task-clock                       #    0.656 CPUs utilized             
                 1      context-switches                 #  592.064 /sec                      
            32,601      cache-misses                     #   26.57% of all cache refs         
           122,692      cache-references                 #   72.642 M/sec                     
         4,109,096      instructions                     #    1.01  insn per cycle            
         4,078,397      cycles                           #    2.415 GHz                       
         1,428,594      L1-dcache-loads                  #  845.819 M/sec                     
                 0      L1-dcache-load-misses                                                 
   <not supported>      LLC-loads                                                             
   <not supported>      LLC-load-misses                                                       

       0.002575031 seconds time elapsed

       0.000000000 seconds user
       0.002544000 seconds sys

```
Профилирование программы `complex` с агрессивной оптимизацией:
Один нагрузчик
* [mpstat](./profiling/complex/agressive_ordinary/mpstat.txt)
```
07:50:08 PM  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
07:50:09 PM  all   10.16    0.00   10.16   39.57    0.00    0.00    0.00    0.00    0.00   40.11
07:50:09 PM    0    2.15    0.00   11.83   69.89    0.00    0.00    0.00    0.00    0.00   16.13
07:50:09 PM    1   18.09    0.00    8.51    9.57    0.00    0.00    0.00    0.00    0.00   63.83
```
* [top](./profiling/complex/agressive_ordinary/top.txt)
```
top - 19:50:00 up 12:32,  1 user,  load average: 0.85, 0.60, 0.34
Tasks: 126 total,   1 running, 125 sleeping,   0 stopped,   0 zombie
%Cpu(s): 65.2 us, 17.4 sy,  0.0 ni,  0.0 id, 17.4 wa,  0.0 hi,  0.0 si,  0.0 st 
MiB Mem :   1967.8 total,    295.0 free,    617.9 used,   1225.5 buff/cache     
MiB Swap:      0.0 total,      0.0 free,      0.0 used.   1349.8 avail Mem 

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
  16069 root      20   0   84772   1664   1408 S 100.0   0.1   0:00.27 complex
```
* [perf](./profiling/complex/agressive_ordinary/perf_stat.txt)
```
# started on Thu Dec 12 21:05:31 2024


 Performance counter stats for './bin/complex 1 40000 ./profiling/complex/agressive_ordinary/file1.bin':

          1,620.05 msec task-clock                       #    0.585 CPUs utilized             
             8,630      context-switches                 #    5.327 K/sec                     
        29,417,781      cache-misses                     #   36.41% of all cache refs         
        80,791,483      cache-references                 #   49.870 M/sec                     
     3,262,865,218      instructions                     #    1.60  insn per cycle            
     2,041,985,332      cycles                           #    1.260 GHz                       
     1,366,791,013      L1-dcache-loads                  #  843.673 M/sec                     
                 0      L1-dcache-load-misses                                                 
   <not supported>      LLC-loads                                                             
   <not supported>      LLC-load-misses                                                       

       2.770570896 seconds time elapsed

       0.439526000 seconds user
       0.863404000 seconds sys
```
Количество нагрузчиков: 2
* [mpstat](./profiling/complex/agressive_parallel/mpstat.log)
```
09:17:10 PM  CPU    %usr   %nice    %sys %iowait    %irq   %soft  %steal  %guest  %gnice   %idle
09:17:11 PM  all   11.54    0.00   46.70   30.22    0.00    3.30    0.00    0.00    0.00    8.24
09:17:11 PM    0   15.22    0.00   40.22   28.26    0.00    2.17    0.00    0.00    0.00   14.13
09:17:11 PM    1    7.78    0.00   53.33   32.22    0.00    4.44    0.00    0.00    0.00    2.22
```
* [top](./profiling/complex/agressive_parallel/top.log)
```
top - 21:17:08 up 13:59,  2 users,  load average: 1.76, 0.86, 0.59
Tasks: 146 total,   1 running, 145 sleeping,   0 stopped,   0 zombie
%Cpu(s): 29.2 us, 36.4 sy,  0.0 ni,  8.8 id, 23.1 wa,  0.0 hi,  2.4 si,  0.2 st 
MiB Mem :   1967.8 total,    269.5 free,    636.7 used,   1232.4 buff/cache     
MiB Swap:      0.0 total,      0.0 free,      0.0 used.   1331.1 avail Mem 

    PID USER      PR  NI    VIRT    RES    SHR S  %CPU  %MEM     TIME+ COMMAND
  19564 root      20   0   84776   1792   1536 S  40.2   0.1   0:01.21 complex
  19561 root      20   0   84776   1792   1536 S  39.2   0.1   0:01.18 complex
```
* [perf](./profiling/complex/agressive_parallel/instance_2/perf.log)
```
# started on Thu Dec 12 21:17:05 2024


 Performance counter stats for './bin/complex 1 40000 /root/os-lab1/profiling/complex/agressive_parallel/instance_2/file1.bin':

          3,444.87 msec task-clock                       #    0.414 CPUs utilized             
            20,599      context-switches                 #    5.980 K/sec                     
        70,141,002      cache-misses                     #   45.98% of all cache refs         
       152,530,671      cache-references                 #   44.278 M/sec                     
     3,838,651,482      instructions                     #    1.18  insn per cycle            
     3,251,344,467      cycles                           #    0.944 GHz                       
     1,578,065,648      L1-dcache-loads                  #  458.092 M/sec                     
            19,378      L1-dcache-load-misses            #    0.00% of all L1-dcache accesses 
   <not supported>      LLC-loads                                                             
   <not supported>      LLC-load-misses                                                       

       8.311256735 seconds time elapsed

       0.506209000 seconds user
       2.545617000 seconds sys
```


    