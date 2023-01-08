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
def compile():
    os.system("mpicc main.c -o main")

def compile_opt():
    os.system("mpicc main.c -O3 -o main")

# capture worst, best and average
def run(threads):
    data = []
    for _ in range(RUNS_PER_THREADS):
        proc = subprocess.run(["mpirun", "main", "-c", str(threads), "-mca",\
            "opal_warn_on_missing_libcuda", "0"], capture_output=True, text=True)
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
    comp()
    
    if len(sys.argv) >= 3:
        file = sys.argv[2]
    else:
        file = "data.csv"
    
    with open(file, "w") as data:
        writer = csv.writer(data)
        writer.writerow(["Threads", "Worst (ms)", "Best (ms)", "Average (ms)"])
        for threads in range(1, THREADS_LIMIT):
            print("Testing threads =", threads)
            
            writer.writerow([str(threads)] + \
                ["{:.3f}".format(val) for val in run(threads)])

if __name__ == "__main__":
    main()
