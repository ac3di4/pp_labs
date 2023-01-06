# make csv comparacent
import csv
import sys

def main():
    if len(sys.argv) < 3:
        print(sys.argv[0], "input", "output")

    filein = open(sys.argv[1], "r")
    fileout = open(sys.argv[2], "w")
    reader = csv.reader(filein)
    writer = csv.writer(fileout)

    # skip header
    header = reader.__next__()
    writer.writerow([header[0], "Efficiency")

    # get first one
    first_avg = reader.__next__()[-1]
    # writer.writerow(["1", "100"])
    first_avg = float(first_avg)

    for row in reader:
        avg = float(row[-1])
        relative = "{:.3f}".format(100 * first_avg / avg)
        writer.writerow([row[0], relative])

    filein.close()
    fileout.close()
        

if __name__ == "__main__":
    main()