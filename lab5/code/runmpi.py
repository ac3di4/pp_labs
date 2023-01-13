import os, sys, subprocess

MAX_PROCS = 6
RUNS_PER_PROC = 20

def run(procs, run_id):
    proc = subprocess.run(["mpirun", "-c", str(procs), "-mca",\
    "opal_warn_on_missing_libcuda", "0", "main", str(run_id)], capture_output=True, text=True)
    return float(proc.stdout)

os.system("mpicc mpi_main.c -o main")

print("Threads,Worst (ms),Best (ms),Avg (ms)")

for procs in range(MAX_PROCS):
    sum = 0
    worst = 0
    best = 100000
    for run_id in range(RUNS_PER_PROC):
        time = run(procs + 1, run_id)
        if time > worst:
            worst = time
        if time < best:
            best = time
        sum += time
    
    print(procs + 1, worst, best, "{:.3f}".format(sum / RUNS_PER_PROC), sep=',')
