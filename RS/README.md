# Random forest implementation

Our goal here was to implement random forests that could be used for authorship attribution, breaking down to a multi-class classification problem.

## What's in here?

### Encapsulating the data

The classes column, matrix, probamap and dataset are used to encapsulate the data input from a .txt or .csv file containing real numbers. The dataset class combines the others to prepare the data to be used to create decision trees.

### Random trees

The class node defines an interface for leafnode and splitnode and are both encapsulated in the randomtree class that has a node called root as only attribute. Randomtree implements decision-making based on a dataset.

### Random forests

The class randomforest defines a forest and has methods to grow trees from a given dataset, to be used in the context of binary classification

### Multi-class classifier

The class multiclass_rs is the final layer of the program and generalizes the binary random forests by creating several 1 vs. all classifiers to solve a multiclass classification problem.

### Tests

The classes testDataPrep and testRandom implement unit tests on binary forests.

## How to run it

### Run tests

In order to run the tests, first comment out the main in main.cpp then de-comment the main at the end of one of the test files. testRandom.cpp uses the files data.data and seq.csv so make sure the paths indicated are correct for you.

### Run the multiclass classifier

From main.cpp, define how many trees per forest you need as well as the number of classes the data you have has. Input the corret paths for train and test and run. The method run_random_forest called will train a random forest on train and classify the data you have in test and return a list of predictions. To watch the performance of the forest on a labeled testing set, make sure test contains the labels and de-comment the line calling run_random_forest_test and run.
The data should have features starting from the thrid column on and classes in the second column for the random forest algorithm to operate smoothly.
