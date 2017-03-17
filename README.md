## Introduction

This project is a part of [Artificial Intelligence Course](http://www.cse.iitd.ac.in/~mausam/courses/csl333/spring2015/) taught at IIT Delhi by Pf. Mausam.
In this assignment a bayesian network was learnt. A bayesian network was given in a .bif format. The code parses the bif file and makes the graph and expectation maximisation was coded in cpp to estimate the CPT Tables. Soft EM was used with smoothing.  

For detailed problem description , look the assignment pdf.

The gen.cpp file generates a records according to CPT mentioned in gold\_alarm.bif file. The records have atmost one missing value .

The run.sh runs the executable . The executable takes two arguments , the first one - the bif file of the network and missing CPT values, and the second data file containing the records from which CPT tables can be estimated. The estimated CPT tables are given as output in solved\_alarm.bif file. 
