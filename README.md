# 3SAT_experiment

- Name: Elijah Jedidiah Yohannan
- Email: ejyohannan@crimson.ua.edu

## 3SAT.cpp

This is the main program that contains the brute force and hueristic methods. We can compile it by running the following in the command line:


    g++ -std=c++11 -Wall -o 3sat 3sat.cpp


This will produce an output file called 3sat. If you do not wish to compile the cpp file ourself, this output file has already been generated for you.
To run the file, you need to run the following in the command line, where "input_file_name" is the name of the input file:


    ./3sat input_file_name.dat


If you write an incorrect file name, you will get an error message.
The following are examples of valid files that I have provided:


    simple.dat
    hard.dat
    intractable.dat
    input1.dat
    input2.dat


We will here use simple.dat as an example. If we run the appropriate command, we will recieve the following output and prompt.


    ./3sat simple.dat

    Current Input (number of variables = 4):
    ((-x1 ∨ x2 ∨ x3)
      ∧  (x1 ∨ -x2 ∨ x3)
      ∧  (-x1 ∨ x2 ∨ x4))

    Which method would you like to use to solve the 3SAT problem?
    1. Brute Force
    2. Greedy Hueristic
    3. Exit

    Please enter your choice: 

Here we see that the user is shown the values from the input file in formula form. You can look at the contents of simple.dat and verify that this is the correct.
Then the user is prompted to enter a number from 1 to 3. Entering an invalid input (or entering '3') will cause the program to exit and terminate gracefully.

