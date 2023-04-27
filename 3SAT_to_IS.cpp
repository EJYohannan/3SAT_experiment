// Name: Elijah Yohannan.
// Compile with: g++ -std=c++11 -Wall -o 3sat_to_IS 3SAT_to_IS.cpp
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <vector>
#include <math.h>
#include <map>

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

void createOutputFile(string inputFileName, string mainOutput, string mappingOutput)
{
    // Output file name is the same as the input file name, but with _answer.dat
    string outputFileName = inputFileName.substr(0, inputFileName.find_last_of(".")) + "_to_IS.dat";
    ofstream outputFile(outputFileName);
    // Add the main output to the output file
    outputFile << mainOutput;
    // Add the mapping output to the output file
    outputFile << "$\n"
               << mappingOutput << endl;
    // Close the files
    outputFile.close();
}

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

    // PART 2: Write the output file for Independent Set

    // For each clause, we will create 3 vertices.
    // These three vertices will be unique from all previous ones, even if their values are the same.
    // For instance, the set of clauses {1, 2, 3} and {-1, 2, 3} will create 6 vertices,
    //      labeled 1, 2, 3, 4, 5, 6.
    // However, because we still want to keep track of their actual identities, we will start building
    //     a map of the vertices to their actual identities. This will be printed after the dollar-sign.
    // For instance, in the example stated about, the final mapping would show:
    //      1 --> x1
    //      2 --> x2
    //      3 --> x3
    //      4 --> -x1
    //      5 --> x2
    //      6 --> x3
    // With each iteration, we will create the mapping by adding to a vector of integers.
    //    For instance, if the value at index 3 is 8, then vertex 3 is actually x8.
    // We will also create the main output by building a string of the edges.
    //    This is a two step process:
    //    1. For each clause, we will create 3 edges, forming a triangle.
    //          Note: 6 lines will be used, because each edge is undirected.
    //    2. For each vertex, if its identity is the opposite of another's,
    //          we will create an edge between them.
    //          For instance, an instance of x1 and -x1 will be connected together.
    //          Note: 2 lines will be used for each edge, because each edge is undirected.

    string mainOutput = "";
    // This is the array used to keep track of the mappings.
    //    For instance, if the value at index 3 is 8, then vertex 3 is actually x8.
    int mappings[clauses.size() * 3 + 1];
    // This map will be used to later find verticies that are the opposite of each other.
    //    For instance, if we are considering index 3 (of the array) being equal to x8,
    //    then we must find all instances of -x8.
    //    We will accomplish this by using the x value as the key of the map.
    //    The negative values can be calculated by multiplying the key by -1.
    map<int, vector<int>> map_of_vector;
    mappings[0] = 0;                             // The first index will be 0, because we will not use it.
    mainOutput += to_string(clauses.size() * 3); // The first line of the output file will be the number of vertices
    mainOutput += "\n";
    int count = 1;
    for (int i = 0; i < clauses.size(); i++)
    {
        Clause &clause = clauses[i];
        // Create the mappings
        // On first iteration, A = 1, B = 2, C = 3
        if (clause.A_val)
        {
            mappings[count] = clause.A_id;
            map_of_vector[clause.A_id].push_back(count);
        }
        else
        {
            mappings[count] = -1 * clause.A_id;
            map_of_vector[-1 * clause.A_id].push_back(count);
        }
        if (clause.B_val)
        {
            mappings[count + 1] = clause.B_id;
            map_of_vector[clause.B_id].push_back(count + 1);
        }
        else
        {
            mappings[count + 1] = -1 * clause.B_id;
            map_of_vector[-1 * clause.B_id].push_back(count + 1);
        }
        if (clause.C_val)
        {
            mappings[count + 2] = clause.C_id;
            map_of_vector[clause.C_id].push_back(count + 2);
        }
        else
        {
            mappings[count + 2] = -1 * clause.C_id;
            map_of_vector[-1 * clause.C_id].push_back(count + 2);
        }
        // Create the main output
        // Edge 1: First vertex to second vertex.
        mainOutput += to_string(count); // First vertex
        mainOutput += " ";
        mainOutput += to_string(count + 1); // Second vertex
        mainOutput += " 1\n";               // Edge weight

        //     Reverse of Edge 1 (because graph is undirected)
        mainOutput += to_string(count + 1); // Second vertex
        mainOutput += " ";
        mainOutput += to_string(count); // First vertex
        mainOutput += " 1\n";           // Edge weight

        // Edge 2: Second vertex to third vertex.
        mainOutput += to_string(count + 1); // Second vertex
        mainOutput += " ";
        mainOutput += to_string(count + 2); // Third vertex
        mainOutput += " 1\n";               // Edge weight

        //     Reverse of Edge 2 (because graph is undirected)
        mainOutput += to_string(count + 2); // Third vertex
        mainOutput += " ";
        mainOutput += to_string(count + 1); // Second vertex
        mainOutput += " 1\n";               // Edge weight

        // Edge 3: Third vertex to first vertex.
        mainOutput += to_string(count + 2); // Third vertex
        mainOutput += " ";
        mainOutput += to_string(count); // First vertex
        mainOutput += " 1\n";           // Edge weight

        //     Reverse of Edge 3 (because graph is undirected)
        mainOutput += to_string(count); // First vertex
        mainOutput += " ";
        mainOutput += to_string(count + 2); // Third vertex
        mainOutput += " 1\n";               // Edge weight

        count += 3;
    }
    string mappingOutput = "\nMappings:\n";
    for (int i = 1; i < clauses.size() * 3 + 1; i++)
    {
        mappingOutput += to_string(i);
        mappingOutput += " --> ";
        if (mappings[i] > 0)
        {
            mappingOutput += "x";
        }
        else
        {
            mappingOutput += "-x";
        }
        mappingOutput += to_string(abs(mappings[i]));
        mappingOutput += "\n";
    }
    // Now, for each vertex, we must add an edge between it and any vertices with the opposite value.
    // For instance if vertex 3 maps to x8, then we must find all instances of -x8.
    // For each edge, we will add two lines to the output file.
    for (int i = 1; i < clauses.size() * 3 + 1; i++)
    {
        int x_val = mappings[i];
        if (x_val > 0)
        {
            // Find all instances of -x_val
            vector<int> opposite_vertices = map_of_vector[-1 * x_val];
            for (int j = 0; j < opposite_vertices.size(); j++)
            {
                // Add the edge
                mainOutput += to_string(i);
                mainOutput += " ";
                mainOutput += to_string(opposite_vertices[j]);
                mainOutput += " 1\n";
                // Reverse edge is not necessary, as it will be added when the opposite vertex is considered.
            }
        }
        else
        {
            // Find all instances of x_val
            vector<int> opposite_vertices = map_of_vector[-1 * x_val];
            for (int j = 0; j < opposite_vertices.size(); j++)
            {
                // Add the edge
                mainOutput += to_string(i);
                mainOutput += " ";
                mainOutput += to_string(opposite_vertices[j]);
                mainOutput += " 1\n";
                // Reverse edge is not necessary, as it will be added when the opposite vertex is considered.
            }
        }
    }

    // Now that we have the main output and the mapping output, we can create the output file.
    createOutputFile(argv[1], mainOutput, mappingOutput);
    return 0;
}
