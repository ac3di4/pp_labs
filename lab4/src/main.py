# This script compiles main.c with different number of threads
# and collects data to data.csv file
# format: worst,best,average
import os
import subprocess
import csv
import sys

# important constants
RUNS_PER_THREADS = 5
TESTING_THREADS = range(2, 16, 2)

# compile with threads
def compile(threads, schedule, chunk_size):
    cmd = "gcc main.c"
    if threads > 1:
        cmd += " -fopenmp -DTHREADS=" + str(threads) + " -DSCHEDULE=" + schedule
        if chunk_size:
            cmd += " -Dchunk_size=" + str(chunk_size)
    cmd += " -o main"
    os.system(cmd)

# capture worst, best and average
def run():
    data = []
    for _ in range(RUNS_PER_THREADS):
        proc = subprocess.run(["./main"], capture_output=True, text=True)
        data.append(float(proc.stdout))
    
    return sum(data) / len(data) 

def main():
    if len(sys.argv) < 3:
        print("Specify output file, schedule and chunk size")

    out = sys.argv[1]
    schedule = sys.argv[2]
    try:
        chunk_size = sys.argv[3]
    except:
        chunk_size = None

    with open(out, "w") as file:
        writer = csv.writer(file)
        writer.writerow(["Threads", "Average time (ms)"])
        for threads in TESTING_THREADS:
            print("testing threads =", threads, "chunk size = ", chunk_size)
            compile(threads, schedule, chunk_size)
            result = ["{:.3f}".format(run())]
            writer.writerow([str(threads)] + result)

if __name__ == "__main__":
    main()