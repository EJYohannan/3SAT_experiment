# 3SAT_experiment

- Name: Elijah Jedidiah Yohannan
- Email: ejyohannan@crimson.ua.edu
This README file primarily focuses on the <em>implementation</em> of the project. For an in-depth explanation of the project as a whole, it is recommended to read the PDF titled "EJY_3SAT_Paper.pdf".

## The Problem

We are solving the 3SAT problem, which, given a set of clauses in CNF such as
    (x1 || x2 || x3)
    &&(-x1 || -x2 || -x4)
    &&(-x2 || -x3 || x4)
asks whether there is some set of boolean values to assign to the variables which satisfies all clauses. In the particular solvers I wrote, the programs aimed to satisfy as many clauses as possible, regardless of whether the formula as a whole could be satisfied.


## 3SAT.cpp (general)

3SAT.cpp is the main program, containing the brute force and hueristic methods. We use C++11 and a GNU C++ compiler (GCC). We can compile by running the following in the command line:


    g++ -std=c++11 -Wall -o 3sat 3sat.cpp


This will produce an executable file called 3sat. If you do not wish to compile the cpp file ourself, this executable file has already been generated for you.
To run the file, you need to run the following in the command line, where "input_file_name" is the name of the input file:


    ./3sat input_file_name.dat


If you write an incorrect file name, you will get an error message.
The following are examples of valid files that I have provided:


    simple.dat
    simple2.dat
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

If '1' is entered, the brute force method will be run. To find the precise details of how this method works, look for the "bruteForce" function. The function, like all code in the program, has numerous comments which describe the procedure in detail.
To summarize, the brute force method tries every possible combination of values for the variables. Note the these variables are independent of the number of clauses. For instance, though simple.dat has 3 clauses, it has 4 variables–x1 to x4. This is denoted by the "n=4" at the top of the file. In this case, the combination of values "FFFF" is tried first, followed by "FFFT", "FFTF", and so forth. The procedure only terminates early only if every clause is satisfied by the current combination of values. This is precisely what happens here, as we get the following output in the terminal:


    Running brute force method...


            Current max satisfiable: 3
            Current max string: FFFF
            x1=F    x2=F    x3=F    x4=F 
            Number of iterations tested: 1


    Answer: 
    Number of clauses satisfied: 3
    Number of iterations: 1
    Values: FFFF


Note that only one iteration needed to be tested, as FFFF solves the formula. Under normal circumstances, we would need to go through 2^n possible combinations of values, were 'n' is the number of variables. Running this function will produce (or overwrite) a file called "simple_answer.dat", which in this case will contain the following contents:


    n=4
    -1 2 3
    1 -2 3
    -1 2 4
    $
    *Brute Force used*
    Answer: 
        Number of clauses satisfied: 3
        Number of iterations: 1
        Values: FFFF


Now consider running the Greedy Heuristic algorithm. Again, details can be found in the cpp code and its corresponding comments. 
To summarize, the Greedy Heuristic counts the number of occurrences of each variable, treating a negative and positive version of the same variable as distinct from each other. In this case, we start with the following:

   Var    Counts
    x1:    1
    x2:    2
    x3:    2
    x4:    1
   -x1:    2
   -x2:    1
   -x3:    0
   -x4:    0

In this example x2=T will be selected first, as it satisfies the maximum number of clauses (2) and is the first in the arbitrary order depicted above. Setting x2 to TRUE satifies the first and third clauses of the original formula, leaving us with the following:

    1 -2 3

The program reduces the counts of affected variables as appropriate, leaving us with the following counts:

   Var    Counts
    x1:    1
    x2:    R
    x3:    1
    x4:    0
   -x1:    0
   -x2:    R
   -x3:    0
   -x4:    0

Note the x2 (both positive and negative) is set to R (removed) because it is no longer accessible as no longer accessible as an option, as we have already decided to set it to TRUE.
From here, we choose x1=T next, which satisfies the maximum number of clauses. Execution of the program gives us the following sequence of outputs to the terminal:


    Which method would you like to use to solve the 3SAT problem?
    1. Brute Force
    2. Greedy Hueristic
    3. Exit

    Please enter your choice: 2
    Running hueristic method...
    Counts (R = removed): 
    x1:1    x2:2    x3:2    x4:1
    -x1:2   -x2:1   -x3:0   -x4:0
    Removing x2 = T
    Clause satisfied: (-x1, x2, x3)
    Clause satisfied: (-x1, x2, x4)

            Current max satisfiable: 2
            Current max string: XTXX
            x1=X    x2=T    x3=X    x4=X 
            Number of iterations tested: 1

    Counts (R = removed): 
    x1:1    x2:R    x3:1    x4:0
    -x1:0   -x2:R   -x3:0   -x4:0
    Removing x1 = T
    Clause satisfied: (x1, -x2, x3)

            Current max satisfiable: 3
            Current max string: TTXX
            x1=T    x2=T    x3=X    x4=X 
            Number of iterations tested: 2

    Answer: 
    Number of clauses satisfied: 3
    Number of iterations: 2
    Values: TTXX


This function, like the brute force, also creates or overrides a file called, in this case, simple_answer.dat, which now would contain the following contents:


n=4
-1 2 3
1 -2 3
-1 2 4
$
*Greedy Hueristic used*
Answer: 
   Number of clauses satisfied: 3
   Number of iterations: 2
   Values: TTXX


Note the the 'X' representing x3 and x4 means that it does not matter what the value of these are; whether TRUE or FALSE, they do not impact the number of clauses satisfied.


## 3SAT.cpp (hard input)

Now let us consider an input that is hard for the heuristic. I have devised such an input and placed it into the file "hard.dat". The contents are as follows:


    n=9
    1 4 5
    1 -4 5
    1 4 -5
    2 6 7
    2 -6 7
    2 6 -7
    3 8 9
    3 -8 9
    3 8 -9
    -1 -2 -3
    $


Recall that we will run the program on the file with the following terminal command:


    ./3sat hard.dat


If you run the 3sat executable on this file and select the brute force method, you will get the following output:


    Answer: 
        Number of clauses satisfied: 10
        Number of iterations: 64
        Values: FFFTTTTTT


Examine the clauses of the input file and see that the values stated here (x1 to x3 set to FALSE and x4 to x9 set to TRUE) do indeed satisfy all 10 clauses, and, by extension, the formula as a whole.
However, if we then run the same executable on the same file, but select the heuristic method, we will instead get the following output:

    
    Answer: 
        Number of clauses satisfied: 9
        Number of iterations: 4
        Values: TTTXXXXXX


Note that the number of clauses satisfied here is 9, which is less than 10. What went wrong?
The answer is that this input exploits the greedy property of my algorithm. Let's walk through the process step-by-step.
When we begin, we have the following counts


    Counts (R = removed): 
     x1:3    x2:3    x3:3    x4:2    x5:2    x6:2    x7:2    x8:2    x9:2
    -x1:1   -x2:1   -x3:1   -x4:1   -x5:1   -x6:1   -x7:1   -x8:1   -x9:1


Since x1 has the highest count (3) and happens to be first in the order depicted above, it will be removed first by setting its value to TRUE. This removes the first three clauses of the formula, leaving us with the following remaining formula and new set of counts:


    n=9
    2 6 7
    2 -6 7
    2 6 -7
    3 8 9
    3 -8 9
    3 8 -9
    -1 -2 -3
    $

    Counts (R = removed): 
     x1:R    x2:3    x3:3    x4:0    x5:0    x6:2    x7:2    x8:2    x9:2
    -x1:R   -x2:1   -x3:1   -x4:0   -x5:0   -x6:1   -x7:1   -x8:1   -x9:1


Note that we will no longer consider the possibility of x1=F, as we have greedily decided to set x1 to TRUE. Now, since x2 has the highest count (3), we will remove it next by setting it to TRUE. This removes the next three clauses of the formula, leaving us with the following remaining formula and new set of counts:


    n=9
    3 8 9
    3 -8 9
    3 8 -9
    -1 -2 -3
    $

    Counts (R = removed): 
     x1:R    x2:R    x3:3    x4:0    x5:0    x6:0    x7:0    x8:2    x9:2
    -x1:R   -x2:R   -x3:1   -x4:0   -x5:0   -x6:0   -x7:0   -x8:1   -x9:1


Now both x1 and x2 have been set to TRUE. Of the remaining variables, x3 has the highest count (3), so we likewise set it to TRUE and remove all clauses that it satisfies. This gives us the following remaining formula and counts:


    n=9
    -1 -2 -3
    $

    Counts (R = removed): 
     x1:R    x2:R    x3:R    x4:0    x5:0    x6:0    x7:0    x8:0    x9:0
    -x1:R   -x2:R   -x3:R   -x4:0   -x5:0   -x6:0   -x7:0   -x8:0   -x9:0


Note that, even though there is one more clause to satisfy, there is no way to satisfy them, as x1, x2, and x3 have already been set to TRUE; their negative values are not considered.
The program goes through one more iteration, and, upon discovering that no additional clauses have been satisfied by this further action, will terminate and display the final result. This example well displays the type of input that causes the heuristic to fail. Failure in this case, means failing to satisfy the maximum number of clauses, as shown by execution of the brute force method.




## 3SAT.cpp (intractable input)

Now let us consider an input that is intractable for the brute force method. We will use the input file named intractable.dat, which contains 218 clauses. 
Recall that we will run the program on the file with the following terminal command:


    ./3sat intractable.dat


More important than the number of clauses, however, is the number of variables, which in this case is 50 (x1, x2, ..., x49, x50). Selecting the heuristic  method, as we would expect, takes a little under 50 iterations, and produces the following output:


    Answer: 
        Number of clauses satisfied: 215
        Number of iterations: 46
        Values: TTFFTFFXTFFXTFFTFTTTTFTFXFFTTFTXFFFFXTFFTTFTTFTFTF


This is pretty good; 215 out of 218 clauses are satisfied.
Running the brute force method on this input, however, has a very different result. You will see that many, many lines of output to the terminal, each representing 1000 new combinations of variable-values tested. The output after about 20 seconds will look something like this:


        ------------ 5175000 iterations tested ------------
        ------------ 5176000 iterations tested ------------
        ------------ 5177000 iterations tested ------------
        ------------ 5178000 iterations tested ------------
        ------------ 5179000 iterations tested ------------
        ------------ 5180000 iterations tested ------------
        ------------ 5181000 iterations tested ------------
        ------------ 5182000 iterations tested ------------
        ------------ 5183000 iterations tested ------------
        ------------ 5184000 iterations tested ------------


You will need to terminate the program with 'control+c'. Every now and then, you will notice a different output quickly pass by, in the following format:


        ------------ 4194000 iterations tested ------------
        ------------ 4195000 iterations tested ------------


        Current max satisfiable: 209
        Current max string: FFFFFFFFFFFFFFFFFFFFFFFFFFFTFFFFFFFFFFFTFTFTFFTFFF
        x1=F    x2=F    x3=F    x4=F    x5=F    x6=F    x7=F    x8=F    x9=F    x10=F   x11=F   x12=F   x13=F   x14=F   x15=F   x16=F   x17=F   x18=F   x19=F   x20=F   x21=F   x22=F  x23=F    x24=F   x25=F   x26=F   x27=F   x28=T   x29=F   x30=F   x31=F   x32=F   x33=F   x34=F   x35=F   x36=F   x37=F   x38=F   x39=F   x40=T   x41=F   x42=T   x43=F   x44=T   x45=F  x46=F    x47=T   x48=F   x49=F   x50=F 
        Number of iterations tested: 4195657

        ------------ 4196000 iterations tested ------------
        ------------ 4197000 iterations tested ------------


These unique parts of the output demonstrate instances where a combination of values is found that satisfies a greater number of clauses than the previous max. However, this problem will go on and on, and will eventually need to be terminated.
Why is this? Recall that the maximum number of iterations is 2^n, where n is the number of variables. 2^50 is equal to 1.1258999e+15.

    (1.1258999e+15 total combinations)  /  ( (5184000 iterations) / (20.37 seconds) )   =   4424109009 seconds.
    (4424109009 seconds)  /  (31536000 seconds per year)  = 140 years!

So even though we were able to satisfy 209 clauses in only 20 seconds, the total time it would take to use the brute force method would be, at the maximum (testing all combinations), 140 years!
Thus, the input is intractable for the brute force method.




## 3sat_to_IS.cpp

This program is used to convert a 3sat input into a Independent Set input.
To compile the program, use the following in the command line:


    g++ -std=c++11 -Wall -o 3sat_to_IS 3SAT_to_IS.cpp


This will generate an executable called 3sat_to_IS, which can be executed in the following manner:


    ./3sat_to_IS simple.dat


Note that "simple.dat" can be replaced with any valid input file name. In case you do not wish to compile or run the program yourself, I have done it for you. In this case, the program has generated an output file called "simple_to_IS.dat".
The precise details of how this function works can be found by examining the code, which likewise contains extensive comments documenting the procedure of the program.
To obtain a high-level overview, you may consider visiting the following site from University of Illinois Urbana-Champaign:

    https://courses.engr.illinois.edu/cs374/fa2020/lec_prerec/23/23_2_0_0.pdf

The key slide that contains details about the procedure and a clear example of this reduction is shown in the image file Reduction1.png.
The following is my summary of the steps:

    1.	Consider the first clause. For each variable in the clause, create a new vertex. I give each vertex a unique ID (since all vertices are distinct from each other), but still label the vertex with an x-value (such as x1, x2, -x3, etc.).
    Create connections between each of these three clauses, forming an undirected triangle.
    2.	Repeat this process with every other clause, creating a total 3*c vertices, where c is the number of clauses.
    3.	Connect all vertices that have opposite labels, such as x1 and -x1.

Note that the input specied in this image is precisely the same as the input that I have used in the simple.dat file.
Note also that the output file generated by my program (simple_to_IS.dat) matches exactly with the output shown in this image.
Note also that each edge (x y 1) has a corresponding edge (y x 1), as each edge of the graph is undirected.
Note also that, in order to keep vertexes distrinct from each other, it was necessary to generate a new number/ID for each one, representing the its individual identity, as opposed to its x-value. The mappings can be seen at the bottom of the output file, which contains the following contents:


    9
    1 2 1
    2 1 1
    2 3 1
    3 2 1
    3 1 1
    1 3 1
    4 5 1
    5 4 1
    5 6 1
    6 5 1
    6 4 1
    4 6 1
    7 8 1
    8 7 1
    8 9 1
    9 8 1
    9 7 1
    7 9 1
    1 4 1
    2 5 1
    4 1 1
    4 7 1
    5 2 1
    5 8 1
    7 4 1
    8 5 1
    $

    Mappings:
    1 --> -x1
    2 --> x2
    3 --> x3
    4 --> x1
    5 --> -x2
    6 --> x3
    7 --> -x1
    8 --> x2
    9 --> x4


Though verification can be somewhat tedious, I can assure you that this output matches the expected output depicted in the slides from University of Illinois Urbana-Champaign.
To aid in verification, I have included an annotated version of the original graph in an image file called "Reduction1A.png". This image include vertex identities (1-9) next to each vertex.




## 3sat_to_VC.cpp

This program functions in a manner somewhat similar to that of 3sat_to_VC, except that it maps 3SAT problem to the Vertex Cover problem. Compilation and execution of the program can be done in the following manner:


    g++ -std=c++11 -Wall -o 3sat_to_VC 3SAT_to_VC.cpp

    ./3sat_to_VC simple2.dat


If you do not wish to do this, it has, again, been done for you. This generates an output file called "simple2_to_VC.dat", which produces the following output:


    13
    1 2 1
    2 1 1
    2 3 1
    3 2 1
    3 1 1
    1 3 1
    4 5 1
    5 4 1
    5 6 1
    6 5 1
    6 4 1
    4 6 1
    7 8 1
    8 7 1
    8 9 1
    9 8 1
    9 7 1
    7 9 1
    10 11 1
    11 10 1
    10 1 1
    1 10 1
    10 2 1
    2 10 1
    11 4 1
    4 11 1
    11 7 1
    7 11 1
    12 13 1
    13 12 1
    12 3 1
    3 12 1
    12 8 1
    8 12 1
    12 9 1
    9 12 1
    13 5 1
    5 13 1
    13 6 1
    6 13 1
    $

    Mappings:
    1 --> x1
    2 --> x1
    3 --> x2
    4 --> -x1
    5 --> -x2
    6 --> -x2
    7 --> -x1
    8 --> x2
    9 --> x2
    10 --> x1
    11 --> -x1
    12 --> x2
    13 --> -x2


Precise details of the procedure can be found in the code, which, like the other programs, contains extensive documentation of the procedure. The procedure is from the following site from McGill University:

    http://cgm.cs.mcgill.ca/~athens/cs507/Projects/2001/CW/npproof.html

The following is my summary of the steps:
    1.	Consider the first clause. For each variable in the clause, create a new vertex. I give each vertex a unique ID (since all vertices are distinct from each other), but label the vertex with an x-value (such as x1, x2, -x3, etc.).
    Create connections between each of these three clauses, forming an undirected triangle.
    2.	Repeat this process with every other clause, creating a total 3*c vertices, where c is the number of clauses.
    3.	Add a pair of vertices (positive and negative) for each x-value that appeared in the input, connected by a single edge. For instance, if we were working with x1 to x3, we would add (x1, -x1), (x2, -x2), and (x3, -x3).
    For each of these new vertices, connect them to all other vertices (in the triangle) that have the same value. For instance, connect the new -x1 to all instances of -x1 among the triangles.

An example 3SAT formula and corresponding Vertex Cover graph taken from the site is found in the image titled "Reduction2.png".
Note that the example 3SAT formula given in simple2.dat exactly matches the input found in simple2.dat.
Note further that the output found in simple2_to_VC.dat exactly matches the output graph found on the site (and in the image).
Though verification can be somewhat tedious, I can assure you that this output matches the expected output depicted in the slides from McGill University.
To aid in verification, I have included an annotated version of the original graph in an image file called "Reduction2A.png". This image include vertex identities (1-13) next to each vertex.