# CS342-OperatingSystems-Project1
## Part A
A multi-process application that  generates a value histogram for the values sitting in a set of input ascii text files, one value per line is developed. Values can be an integers or real numbers.

### To run Part A
```
$ make
$ Format ./phistogram minvalue maxvalue bincount N file1 ... fileN outfile
$ Example ./phistogram 1 100 5 3 file1 file2 file3 outfile
```
## Part B
In this part, the same application described in part A using threads is developed. For each input file, there is a separate worker thread. Each worker thread generates the histogram of the corresponding input file values into a global data structure in memory. Then the main thread reads those histograms from these structures and generates a single histogram and will print that out to the output file.

### To run Part B
```
$ make
$ Format ./thistogram minvalue maxvalue bincount N file1 ... fileN outfile
$ Example ./thistogram 1 100 5 3 file1 file2 file3 outfile
```
