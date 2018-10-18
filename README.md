# MapReduce

```Problem```
Using C++ only, implement an object providing the programmer with a parallel google mapreduce pattern. The user must be able to provide two filenames (an input and an output file) the mapper (function<vector<pair<Tkey,Tvalue>>(string)>) processing lines of the file, the reducer (function<Tvalue(Tvalue,Tvalue)>) and a method void compute(void) to compute the results in the output file out of the lines of the input file.


```Arguments``` "input.txt output.txt nw"

