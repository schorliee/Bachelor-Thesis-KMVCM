# KMVCM

## Motivation
Nowadays, data analyses are essential for continuous data collection in distributed systems as they can provide a better understanding of data and systems and these can thus be enhanced accordingly. Among other things, set operations are needed for such analyses to show the relation between data. 

## Theoretical Approach
To apply the set operations "union" and "intersection" on frequency data, e.g. on data stream, a combination of the two probabilistic data structures "Count-Min Sketch" and "K-Minimum Values Sketch" is developed. Here attention is paid to the accuracy of the results of the operations in relation to the depth and width of the Count-Min Sketch and the K-Minimum Values Sketch size. To calculate the accuracy, the four different functions "Minimum", "Median", "Average" and "Maximum" are used, in order to choose the best function for the respective operations. 

## Experimental Set-up
The data used for this experiment is taken from the data set "[adult](https://archive.ics.uci.edu/ml/datasets/adult)", which presents the database of the 1994 Census. In order to distinguish each entries from each other, the data set was modified by adding the new coloumn "ID" which gives each entry its own unique ID. Every citizen (represented now by an ID) is categorised by specific characteristics such as age, education or occupation whereas in this experiment, we inspect only 15 characteristics. 

When saving all the data into our database, it is for example for statistical reasons the interest to know how many people have a bachelor's degree. Instead of saving all the data of the people, we map the data in the KMV-based Count-Min Sketch and here is how it works:
1. Hash the characteristics and map it into the field in the Count-Min Sketch
2. Hash the ID and map it into the KMV Sketch of the mapped field in step 1

As mentioned before, it is possible to execute the set operations "union" and "intersection", e.g. it is in interest to know how many people have a bachelor's degree _and_ are married. This is how the set operations work: 
1. Mark the two KMVs of the fields in the Count-Min Sketch which are mapped for characteristics 1 and 2
2. Depending on the operation, apply _union_ or _intersection_ 

_Note: In order to understand how union and intersection in KMV Sketches work, please get acknowledged with the paper [Counting Distinct Elements in a Data Stream](http://cs.haifa.ac.il/~ilan/randomized_algorithms/bar-yosef_jayram.pdf) by Ziv Bar-Yossef, T. S. Jayram, Ravi Kumar, D. Sivakumar, and Luca Trevisan._

## Results
This approach allows for a smaller discrepancy of results in the "Point Query" and "Union" requests, whereas the combined data structure is not suited for intersection operations due to an error rate of at least 82.5 %. The error rate in Point Query and Union is below 5 %.


## How to run
There is a makefile which compiles and runs the code and deletes the output automatically. 
- *make all* - compile and run the code
- *make clean* - delete the compiled program and all its output



