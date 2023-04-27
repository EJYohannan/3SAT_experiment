// Name: Elijah Yohannan.
// Compile with: g++ -std=c++11 -Wall -o 3sat 3sat.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>

using namespace std;

class Clause
{
public:
    // True or False
    bool A_val;
    bool B_val;
    bool C_val;
    // Which variable we are talking about (ex. 1, 2, -4  --> A_id=1, B_id=2, C_id=4)
    int A_id;
    int B_id;
    int C_id;

    Clause(bool A_val, bool B_val, bool C_val, int A_id, int B_id, int C_id)
    {
        this->A_val = A_val;
        this->B_val = B_val;
        this->C_val = C_val;
        this->A_id = A_id;
        this->B_id = B_id;
        this->C_id = C_id;
    }
};

// Print out the entire formula (for testing purposes)
void printFormula(const std::vector<Clause> &clauses)
{
    std::cout << "(";
    for (int i = 0; i < clauses.size(); i++)
    {
        const Clause &clause = clauses[i];
        cout << "(";
        if (clause.A_val)
        {
            cout << "x" << clause.A_id;
        }
        else
        {
            cout << "-x" << clause.A_id;
        }
        cout << " ∨ ";
        if (clause.B_val)
        {
            cout << "x" << clause.B_id;
        }
        else
        {
            cout << "-x" << clause.B_id;
        }
        cout << " ∨ ";
        if (clause.C_val)
        {
            cout << "x" << clause.C_id;
        }
        else
        {
            cout << "-x" << clause.C_id;
        }
        cout << ")";
        if (i < clauses.size() - 1)
        {
            cout << "\n  ∧  ";
        }
    }
    cout << ")" << endl;
}

string bruteForce(const std::vector<Clause> &clause, int num_vars)
{
    int max_satisfiable = 0;           // Keep track of the max number of clauses satisfied
    bool curVals[num_vars];            // Keep track of the current values of the variables
    string curMaxString = "";          // Keep track of the current values of the variables as a string
    int num_iterations = 0;            // Keep track of the number of iterations
    for (int i = 0; i < num_vars; i++) // Set initial value of curMaxString to all F's
    {
        curMaxString += "F";
    }
    cout << "GOT HERE 1" << endl;
    cout << (int)pow(2.0, num_vars) << endl;
    // For all 2^num_var possible combinations of values
    for (long long i = 0; i < (long long)pow(2.0, num_vars); i++)
    {
        // Convert the current number to a binary string
        string binary = bitset<64>(i).to_string();
        // Get the last num_vars digits
        string last = binary.substr(binary.size() - num_vars);
        // Convert the string to a boolean array
        for (int j = 0; j < num_vars; j++)
        {
            curVals[j] = (last[j] == '1');
        }
        // Check if the current combination of values satisfies the formula
        int num_satisfiable = 0;
        for (int j = 0; j < clause.size(); j++)
        {
            const Clause &curClause = clause[j];
            // Check if the current clause is satisfied
            if ((curVals[curClause.A_id - 1] == curClause.A_val) ||
                (curVals[curClause.B_id - 1] == curClause.B_val) ||
                (curVals[curClause.C_id - 1] == curClause.C_val))
            {
                num_satisfiable++;
            }
        }
        // If the current combination of values satisfies more clauses than the
        // previous max, update the max
        if (num_satisfiable > max_satisfiable)
        {
            max_satisfiable = num_satisfiable;
            // Update the current max string
            curMaxString = "";
            for (int j = 0; j < num_vars; j++)
            {
                if (curVals[j])
                {
                    curMaxString += "T";
                }
                else
                {
                    curMaxString += "F";
                }
            }
            cout << "\n\n\tCurrent max satisfiable: " << max_satisfiable << endl;
            cout << "\tCurrent max string: " << curMaxString << endl;
            for (int j = 0; j < num_vars; j++)
            {
                cout << "\tx" << j + 1 << "=" << curMaxString[j] << " ";
            }
            cout << endl;
            cout << "\tNumber of iterations tested: " << i + 1 << endl
                 << endl;

            num_iterations = i + 1;
        }
        if ((i + 1) % 1000 == 0)
        { // Print out the current iteration every 1000 iterations
            cout << "\t------------ " << i + 1 << " iterations tested ------------" << endl;
        }
        // If the current combination of values satisfies all clauses, stop
        if (num_satisfiable == clause.size())
        {
            break;
        }
    }
    cout << endl;
    return curMaxString + " " + to_string(max_satisfiable) + " " + to_string(num_iterations);
}

// Idea:    Count the number of times each variable appears in the formula.
//          A negative variable is counted as different from a positive variable.
//          The variable that appears the most is the one that will be removed first.
//              Its count will be set to  -999999999.
//              Its positive or negative counterpart will be set to -999999998.
//              This is to prevent the variable from being removed again.
//              Each clause that was satisfied by the removed variable will be removed from the list.
//          The variable that appears the second most is the one that will be removed next.
//          The process is repeated until all clauses are satisfied.
string EJs_HueristicMethod(const std::vector<Clause> &clause, int num_vars)
{
    int counts[num_vars * 2]; // Keep track of the number of times each variable appears in the formula.
                              // Negative variables are counted as different from positive variables
                              // and are stored in the second half of the array.
    int max_satisfiable = 0;  // Keep track of the max number of clauses satisfied
    string curMaxString = ""; // Keep track of the current values of the variables as a string
    // Initialize the counts array
    for (int i = 0; i < num_vars * 2; i++)
    {
        counts[i] = 0;
    }
    // Count the number of times each variable appears in the formula. Negative variables are counted as different from positive variables.
    for (int i = 0; i < clause.size(); i++)
    {
        if (clause[i].A_val)
        { // If the variable is positive
            counts[clause[i].A_id - 1]++;
        }
        else
        { // If the variable is negative
            counts[clause[i].A_id - 1 + num_vars]++;
        }
        if (clause[i].B_val)
        { // If the variable is positive
            counts[clause[i].B_id - 1]++;
        }
        else
        { // If the variable is negative
            counts[clause[i].B_id - 1 + num_vars]++;
        }
        if (clause[i].C_val)
        { // If the variable is positive
            counts[clause[i].C_id - 1]++;
        }
        else
        { // If the variable is negative
            counts[clause[i].C_id - 1 + num_vars]++;
        }
    }

    // Make a copy of the clauses
    vector<Clause> clauses_copy = clause;
    int num_iterations = 0; // Keep track of the number of iterations
    // With every iteration, remove the variable that appears the most in the formula and remove all clauses that are satisfied by that variable.
    while (clauses_copy.size() > 0)
    {
        int prev_max_satisfiable = max_satisfiable;
        // Print out the current count values of the variables
        cout << "Counts (R = removed): " << endl;
        for (int i = 0; i < num_vars * 2; i++)
        {
            if (i < num_vars - 1)
            {
                if (counts[i] != -999999999 && counts[i] != -999999998)
                    cout << " x" << i + 1 << ":" << counts[i] << "\t";
                else
                    cout << " x" << i + 1 << ":R\t";
            }
            else if (i == num_vars - 1)
            {
                if (counts[i] != -999999999 && counts[i] != -999999998)
                    cout << " x" << i + 1 << ":" << counts[i] << "\n";
                else
                    cout << " x" << i + 1 << ":R\n";
            }
            else
            {
                if (counts[i] != -999999999 && counts[i] != -999999998)
                    cout << "-x" << i + 1 - num_vars << ":" << counts[i] << "\t";
                else
                    cout << "-x" << i + 1 - num_vars << ":R\t";
            }
        }
        cout << endl;

        num_iterations++;
        // Find the variable that appears the most in the formula
        int max_count = 0;
        int max_index = 0;
        for (int i = 0; i < num_vars * 2; i++)
        {
            if (counts[i] > max_count)
            {
                max_count = counts[i];
                // cout << "NEW MAX COUNT: " << max_count << endl;
                max_index = i;
                // cout << "NEW MAX INDEX: " << max_index << endl;
            }
        }
        // Remove the variable from the formula
        counts[max_index] = -999999999;
        if (max_index < num_vars)
        {
            cout << "Removing x" << max_index + 1 << " = T" << endl;
        }
        else
        {
            cout << "Removing x" << max_index - num_vars + 1 << " = F" << endl;
        }
        // If the variable was positive, also reduce the negative, and vise versa
        if (max_index < num_vars)
        {
            counts[max_index + num_vars] = -999999998;
        }
        else
        {
            counts[max_index - num_vars] = -999999998;
        }
        // Create a new vector, and only add the clauses that are not satisfied by the removed variable.
        // For each clause that is satisfied, increment the number of clauses satisfied.
        // For each clause satisfied, decrement the count of the variables in the clause, unless the count is already -1.
        // After, set the clauses_copy to the new vector.
        vector<Clause> new_clauses;
        for (int i = 0; i < clauses_copy.size(); i++)
        {
            // If the clause is satisfied by the removed variable,
            // increment the number of clauses satisfied,
            // and decrement the count of the variables in the clause, unless the count is already -1.
            // cout << "Checking clause " << i << " with max_index: " << max_index << endl;
            // cout << "A: " << clauses_copy[i].A_id << " " << clauses_copy[i].A_val << endl;
            // cout << "B: " << clauses_copy[i].B_id << " " << clauses_copy[i].B_val << endl;
            // cout << "C: " << clauses_copy[i].C_id << " " << clauses_copy[i].C_val << endl;
            if ((clauses_copy[i].A_val && clauses_copy[i].A_id == max_index + 1) ||
                (!clauses_copy[i].A_val && clauses_copy[i].A_id == max_index + 1 - num_vars) ||
                (clauses_copy[i].B_val && clauses_copy[i].B_id == max_index + 1) ||
                (!clauses_copy[i].B_val && clauses_copy[i].B_id == max_index + 1 - num_vars) ||
                (clauses_copy[i].C_val && clauses_copy[i].C_id == max_index + 1) ||
                (!clauses_copy[i].C_val && clauses_copy[i].C_id == max_index + 1 - num_vars))
            {
                max_satisfiable++;
                // cout << "MAX INCREMENTED" << endl;
                if (clauses_copy[i].A_val && counts[clauses_copy[i].A_id - 1] != -999999999 && counts[clauses_copy[i].A_id - 1] != -999999998)
                {
                    counts[clauses_copy[i].A_id - 1]--;
                }
                else if (!clauses_copy[i].A_val && counts[clauses_copy[i].A_id - 1 + num_vars] != -999999999 && counts[clauses_copy[i].A_id - 1 + num_vars] != -999999998)
                {
                    counts[clauses_copy[i].A_id - 1 + num_vars]--;
                }
                if (clauses_copy[i].B_val && counts[clauses_copy[i].B_id - 1] != -999999999 && counts[clauses_copy[i].B_id - 1] != -999999998)
                {
                    counts[clauses_copy[i].B_id - 1]--;
                }
                else if (!clauses_copy[i].B_val && counts[clauses_copy[i].B_id - 1 + num_vars] != -999999999 && counts[clauses_copy[i].B_id - 1 + num_vars] != -999999998)
                {
                    counts[clauses_copy[i].B_id - 1 + num_vars]--;
                }
                if (clauses_copy[i].C_val && counts[clauses_copy[i].C_id - 1] != -999999999 && counts[clauses_copy[i].C_id - 1] != -999999998)
                {
                    counts[clauses_copy[i].C_id - 1]--;
                }
                else if (!clauses_copy[i].C_val && counts[clauses_copy[i].C_id - 1 + num_vars] != -999999999 && counts[clauses_copy[i].C_id - 1 + num_vars] != -999999998)
                {
                    counts[clauses_copy[i].C_id - 1 + num_vars]--;
                }
                // Print out the variables of the clause that was satisfied
                cout << "Clause satisfied: (";
                if (clauses_copy[i].A_val)
                {
                    cout << "x" << clauses_copy[i].A_id << ", ";
                }
                else
                {
                    cout << "-x" << clauses_copy[i].A_id << ", ";
                }
                if (clauses_copy[i].B_val)
                {
                    cout << "x" << clauses_copy[i].B_id << ", ";
                }
                else
                {
                    cout << "-x" << clauses_copy[i].B_id << ", ";
                }
                if (clauses_copy[i].C_val)
                {
                    cout << "x" << clauses_copy[i].C_id << ")";
                }
                else
                {
                    cout << "-x" << clauses_copy[i].C_id << ")";
                }
                cout << endl;
            }
            else
            { // If the clause is not satisfied by the removed variable, add it to the new vector
                new_clauses.push_back(clauses_copy[i]);
            }
        }
        // Set the clauses_copy to the new vector
        clauses_copy = new_clauses;
        // Update the curMaxString
        curMaxString = "";
        for (int i = 0; i < num_vars; i++)
        {
            if (counts[i] == -999999999)
            {
                curMaxString += "T";
            }
            else if (counts[i] == -999999998)
            {
                curMaxString += "F";
            }
            else
            {
                curMaxString += "X";
            }
        }

        // Print the results of the iteration
        cout << "\n\tCurrent max satisfiable: " << max_satisfiable << endl;
        cout << "\tCurrent max string: " << curMaxString << endl;
        for (int j = 0; j < num_vars; j++)
        {
            cout << "\tx" << j + 1 << "=" << curMaxString[j] << " ";
        }
        cout << endl;
        cout << "\tNumber of iterations tested: " << num_iterations << endl
             << endl;

        // If the max_satisfiable has not increased, then we have found the max satisfiable
        if (max_satisfiable == prev_max_satisfiable)
        {
            break;
        }
    }
    return curMaxString + " " + to_string(max_satisfiable) + " " + to_string(num_iterations);
}

void createOutputFile(string inputFileName, string additionalOutput)
{
    // Output file name is the same as the input file name, but with _answer.dat
    string outputFileName = inputFileName.substr(0, inputFileName.find_last_of(".")) + "_answer.dat";
    ofstream outputFile(outputFileName);
    // Copy the contents of the input file to the output file
    ifstream inputFile(inputFileName);
    outputFile << inputFile.rdbuf();
    // Add the additional output to the output file
    outputFile << additionalOutput;
    // Close the files
    inputFile.close();
    outputFile.close();
}

int main(int argc, char *argv[])
{

    // PART 1: Read and parse the input file

    if (argc != 2)
    {
        cout << "Usage: " << argv[0] << " input_file" << endl;
        return 1;
    }

    ifstream input_file(argv[1]);
    if (!input_file)
    {
        cout << "Error: could not open input file " << argv[1] << endl;
        return 1;
    }

    int num_vars = -1; // If n=4, then num_vars=4
    vector<Clause> clauses;

    string line;
    while (getline(input_file, line))
    {
        if (line.empty())
        {
            // skip empty lines
            continue;
        }
        // If we are at the top of the input file
        if (line[0] == 'n')
        {
            // read the number of variables
            size_t pos = line.find('=');
            if (pos == string::npos)
            {
                cout << "Error: invalid n line" << endl;
                return 1;
            }
            num_vars = stoi(line.substr(pos + 1));
        }
        else if (line[0] == '$')
        {
            // end of input
            break;
        }
        else
        {
            // parse a clause line
            int A_id, B_id, C_id;
            bool A_val, B_val, C_val;
            int num_read = sscanf(line.c_str(), "%d %d %d", &A_id, &B_id, &C_id);
            if (num_read != 3)
            {
                cout << "Error: invalid clause line" << endl;
                return 1;
            }
            A_val = (A_id > 0);
            B_val = (B_id > 0);
            C_val = (C_id > 0);
            A_id = abs(A_id);
            B_id = abs(B_id);
            C_id = abs(C_id);
            clauses.push_back(Clause(A_val, B_val, C_val, A_id, B_id, C_id));
        }
    }

    input_file.close();

    // PART 2: Solve the 3SAT problem

    cout << "\nCurrent Input (number of variables = " << num_vars << "):"
         << endl;
    printFormula(clauses);
    cout << "\nWhich method would you like to use to solve the 3SAT problem?"
         << endl;
    cout << "1. Brute Force" << endl;
    cout << "2. Greedy Hueristic" << endl;
    cout << "3. Exit" << endl;
    cout << "\nPlease enter your choice: ";
    int choice = -1;
    cin >> choice;

    if (choice != 1 && choice != 2 && choice != 3)
    {
        cout << "Invalid choice. Exiting..." << endl;
        return 1;
    }
    else if (choice == 1)
    {
        cout << "Running brute force method..." << endl;
        string answer = bruteForce(clauses, num_vars);

        stringstream ss(answer);
        string values = "";
        string num_satisfied = "";
        string num_iterations = "";
        ss >> values >> num_satisfied >> num_iterations;
        string parsed_answer = "Answer: \n   Number of clauses satisfied: " + num_satisfied +
                               "\n   Number of iterations: " + num_iterations + "\n   Values: " + values + "\n";
        cout << parsed_answer << endl;
        // Create the output file
        createOutputFile(argv[1], "\n*Brute Force used*\n" + parsed_answer);
    }
    else if (choice == 2)
    {
        cout << "Running hueristic method..." << endl;
        string answer = EJs_HueristicMethod(clauses, num_vars);
        stringstream ss(answer);
        string values = "";
        string num_satisfied = "";
        string num_iterations = "";
        ss >> values >> num_satisfied >> num_iterations;
        string parsed_answer = "Answer: \n   Number of clauses satisfied: " + num_satisfied +
                               "\n   Number of iterations: " + num_iterations + "\n   Values: " + values + "\n";
        cout << parsed_answer << endl;
        // Create the output file
        createOutputFile(argv[1], "\n*Greedy Hueristic used*\n" + parsed_answer);
    }
    else
    {
        cout << "Exiting..." << endl;
        return 0;
    }
    return 0;
}