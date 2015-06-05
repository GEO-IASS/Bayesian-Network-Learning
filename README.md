In this assignment a bayesian network was learnt. A bayesian network was given in a .bif format. The code parses the bif file and makes the graph and expectation maximisation was coded in cpp to estimate the CPT Tables. Soft EM was used with smoothing.

For detailed problem description , look the pdf.

The gen.cpp file generates a records according to CPT mentioned in gold_alarm.bif file. The records have atmost one missing value .

The run.sh runs the executable . The executable takes two arguments , te first one - the bif file of the network and missing CPT values, and the second data file containing the records from which CPT tables can be estimated.
