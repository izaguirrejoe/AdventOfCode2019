#include <iostream>
#include <fstream>
#include <vector>
#include <map>
using namespace std;

const int add = 1;
const int multiply = 2;
const int quit = 99;
const int bin_op_size = 4;

bool isValidOp(int i)
{
    if(i == add || i == multiply || i == quit)
        return true;
    return false;
}

int instruction_size(int OpCode)
{
    if(!isValidOp(OpCode))
        throw runtime_error("Cannot calculate the size of an invalid OpCode.");
    if(OpCode == add || OpCode == multiply)
        return bin_op_size;
    return -1;
}

void end_of_loop(istream& is, char term, const string& message)
{
    if(is.fail())
    {
        is.clear();
        char ch;
        if(is >> ch && ch == term)
            return;
        throw runtime_error(message);
    }
}

vector<int> fill_vec(istream& is, char term)
{
    vector<int> vi;
    for(int i; is >> i;)
    {
        vi.push_back(i);
        char comma;
        is >> comma;
        if(comma != ',')
        {
            is.unget();
            is.clear(ios_base::failbit);
        }
    }
    end_of_loop(is, term , "Invalid end of file.");
    return vi;
}


int main()
{
    ifstream is {"input.txt"};
    if(!is) throw runtime_error("Couldn't open input file.");
    
    ofstream os {"output.txt"};
    if(!os) throw runtime_error("Couldn't open output file.");
    
    vector<int> vi = fill_vec(is,'|');
    
    for(int i = 0; i < vi.size(); i += instruction_size(vi[i]))
    {
        int OpCode = vi[i];
        if(!isValidOp(OpCode)) throw runtime_error("Invalid Opcode");
        
        if(OpCode == quit)
            break;
        
        int num1 = vi[vi[i+1]];
        int num2 = vi[vi[i+2]];
        int location = vi[i+3];
        
        if(OpCode == add)
            vi[location] = num1 + num2;
        
        if(OpCode == multiply)
            vi[location] = num1 * num2;
    }
    
    for(int i: vi)
        os << i << ", ";
    
    return 0;
}
