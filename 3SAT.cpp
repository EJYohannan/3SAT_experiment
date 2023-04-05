#include <iostream>
#include <fstream>
#include <string>
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

// Print out the entire formula
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
            cout << "  ∧  ";
        }
    }
    cout << ")" << endl;
}

string bruteForce(const std::vector<Clause> &clause, int num_vars)
{
    int max_satisfiable = 0;           // Keep track of the max number of clauses satisfied
    bool curVals[num_vars];            // Keep track of the current values of the variables
    string curMaxString = "";          // Keep track of the current values of the variables as a string
    for (int i = 0; i < num_vars; i++) // Set initial value of curMaxString to all F's
    {
        curMaxString += "F";
    }
    // For all 2^num_var possible combinations of values
    for (int i = 0; i < (int)pow(2.0, num_vars); i++)
    {
        // Convert the current number to a binary string
        string binary = bitset<32>(i).to_string();
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
        }
        if ((i + 1) % 1000 == 0)
        { // Print out the current iteration every 1000 iterations
            cout << "\t------------ " << i + 1 << " iterations tested ------------" << endl;
        }
    }
    cout << endl;
    return curMaxString + " " + to_string(max_satisfiable);
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
    cout << "2. Hueristic" << endl;
    cout << "3. Exit" << endl;
    cout << "\nPlease enter your choice: ";
    int choice = -1;
    while (choice == -1)
    {
        cin >> choice;
        if (choice != 1 && choice != 2 && choice != 3)
        {
            cout << "Invalid choice. Please try again: ";
            choice = -1;
        }
    }
    if (choice == 1)
    {
        cout << "Running brute force method..." << endl;
        bruteForce(clauses, num_vars);
    }
    else if (choice == 2)
    {
        cout << "Running hueristic method..." << endl;
    }
    else
    {
        cout << "Exiting..." << endl;
        return 0;
    }
    return 0;
}