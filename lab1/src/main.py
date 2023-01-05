# This script compiles main.c with different number of threads
# and collects data to data.csv file
# format: worst,best,average
import os
import subprocess
import csv
import sys

# important constants
RUNS_PER_THREADS = 10
THREADS_LIMIT = 16

# compile with threads
def compile(threads):
    if threads <= 1:
        os.system("gcc main.c -o main")
    else:
        os.system("gcc threaded.c -fopenmp -DTHREADS=" + str(threads) + " -o main")

def compile_opt(threads):
    if threads <= 1:
        os.system("gcc main.c -O3 -o main")
    else:
        os.system("gcc threaded.c -O3 -fopenmp -DTHREADS=" + str(threads) + " -o main")

# capture worst, best and average
def run():
    data = []
    for _ in range(RUNS_PER_THREADS):
        proc = subprocess.run(["./main"], capture_output=True, text=True)
        data.append(float(proc.stdout))
    
    worst = data[0]
    best = data[0]
    s = 0
    for val in data[1:]:
        if val > worst:
            worst = val
        if val < best:
            best = val
        s += val
    return (worst, best, s / len(data)) 

def main():
    if len(sys.argv) < 2 or sys.argv[1] != 'opt':
        comp = compile
    else:
        comp = compile_opt
    
    if len(sys.argv) >= 3:
        file = sys.argv[2]
    else:
        file = "data.csv"
    
    with open(file, "w") as data:
        writer = csv.writer(data)
        writer.writerow(["Threads", "Worst (ms)", "Best (ms)", "Average (ms)"])
        for threads in range(1, THREADS_LIMIT):
            print("Testing threads =", threads)
            comp(threads)
            
            writer.writerow([str(threads)] + ["{:.3f}".format(val) for val in run()])

if __name__ == "__main__":
    main()