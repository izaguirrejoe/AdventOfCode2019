#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

const int add = 1;
const int multiply = 2;
const int input = 3;
const int output = 4;
const int jump_if_true = 5;
const int jump_if_false = 6;
const int less_than = 7;
const int equals = 8;
const int quit = 99;
const int one_param_size = 2;
const int two_param_size = 3;
const int three_param_size = 4;
const int position_mode = 0;
const int immediate_mode = 1;

bool is_valid_mode(int i)
{
    if(i == position_mode || i == immediate_mode)
        return true;
    return false;
}

bool write_to_third_param(int i)
{
    if(i == add || i == multiply || i == less_than || i == equals)
        return true;
    return false;
}

bool isValidOp(int i)
{
    if(i == add || i == multiply || i == input || i == output || i == jump_if_true || i == jump_if_false || i == less_than || i == equals || i == quit)
        return true;
    return false;
}

int instruction_size(int OpCode)
{
    if(!isValidOp(OpCode))
        throw runtime_error("Cannot calculate the size of an invalid OpCode.");
    if(OpCode == add || OpCode == multiply || OpCode == less_than || OpCode == equals)
        return three_param_size;
    if(OpCode == input || OpCode == output)
        return one_param_size;
    if(OpCode == jump_if_true || OpCode == jump_if_false)
        return two_param_size;
    
    return 0;
}

int num_of_parameters(int OpCode)
{
    return instruction_size(OpCode) - 1;
}

class Parameter{ //A valid parameter consists of a number and an associated parameter mode
    int number; //The number which is given a value based on its parameter mode
    int parameter_mode;
    
public:
    Parameter(int n, int p) : number{n}, parameter_mode{p} //Invariant: the parameter mode is valid
    {
        if(!is_valid_mode(parameter_mode))
            throw runtime_error("Error in creating a parameter: the parameter mode is invalid.");
    }
    int get_number() const {return number;}
    int get_param_mode() const{return parameter_mode;}
    void set_mode(int i) {parameter_mode = i;}
    
};


class instruction{ //an instruction consists of an opcode and a list of parameters
    int OpCode;
    vector<Parameter> parameters;
    
public:
    instruction(int op, vector<Parameter> vp) : OpCode{op} , parameters {vp} //Invariant: The OpCode must be valid. The number of parameters required by the opcode should be the number of parameters in the vector. An input instruction is always in immediate mode. An output instruction is in position mode. A binary operation saves to position in immediate mode
    {
        if(!isValidOp(OpCode))
            throw runtime_error("Error in creating an instruction: Invalid OpCode.");
        if(num_of_parameters(OpCode) != parameters.size())
            throw runtime_error("Error in creating an instruction: The number of parameters needed by the opcode is not the same as the number of parameters in the parameter vector.");
        if(OpCode == input)
            parameters[0].set_mode(immediate_mode);
        if(OpCode == output)
            parameters[0].set_mode(position_mode);
        if(write_to_third_param(OpCode)) //the position written to is in immediate mode
            parameters[2].set_mode(immediate_mode);
    }
    
    int get_opcode() const {return OpCode;}
    const vector<Parameter>&  get_parameters() const {return parameters;}
};

ostream& operator<<(ostream& os, const instruction& i)
{
    os << "OpCode: " << i.get_opcode() << endl;
    vector<Parameter> vp = i.get_parameters();
    for(Parameter p : vp)
        os << "Parameters: " << endl << p.get_number() << ", " << p.get_param_mode() << endl;
    return os;
}

string pad_five_digits(const string& s) //adds zeroes to the front of a string with less than five digits until it has five digits. Anything with more than five digits is an error
{
    if(s.size() > 5) throw runtime_error("String has more than 5 digits.");
    string t = s;
    while(t.size() < 5)
        t = "0" + t;
    return t;
}

string pad_three_digits(const string& s) //adds zeroes to the front of a string with less than three digits until it has three digits. Anything with more than three digits is an error
{
    if(s.size() > 3) throw runtime_error("String has more than 3 digits.");
    string t = s;
    while(t.size() < 3)
        t = "0" + t;
    return t;
}

vector<int> initial_values(int i) //Given an integer, this function computes the "initial values" i.e. the opcode and the parameter modes. Whether or not the these are valid is determined when constructing the parameter and the instruction
{
    string s = to_string(i);
    int opcode = s.back() - '0'; //if the string has only one character, that is the opcode
    if(s.size() > 1) //if the string has more than one character, the opcode is the last two positions
        opcode = stoi(s.substr(s.size()-2,2));
    if(!isValidOp(opcode))
        throw runtime_error("Invalid opcode");
    
    vector<int> vi;
    vi.push_back(opcode);

    if(instruction_size(opcode) == three_param_size) //Standard length for binary operation "ABCDE"
        s = pad_five_digits(s);
    if(instruction_size(opcode) == one_param_size) //Standard length for unary operation "ABC"
        s = pad_three_digits(s);
    
    s = s.substr(0, s.size() - 2); //Remove the last two elements, the opcode and the extra 0;
    reverse(s.begin(), s.end());
    
    for(char c: s)
        vi.push_back(c - '0');
    
    return vi;
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

void calculate(vector<int>& vi, const instruction& i, int& pointer) //Modifies the diagnostic and the pointer according to the instruction
{
    vector<int> values;    //Get the values
    vector<Parameter> parameters = i.get_parameters();
    for(Parameter p: parameters) //The values depend on the parameter modes
    {
        if(p.get_param_mode() == position_mode)
            values.push_back(vi[p.get_number()]);
        if(p.get_param_mode() == immediate_mode)
            values.push_back(p.get_number());
    }
    
    switch(i.get_opcode())
    {
        case add:
        {
            int v1 = values[0];
            int v2 = values[1];
            int v3 = values[2];
            vi[v3] = v1 + v2;
            cout << "Adding " << v1  << " and " << v2 << " and saving it to position " << v3 << endl;
            break;
        }
            
        case multiply:
        {
            int v1 = values[0];
            int v2 = values[1];
            int v3 = values[2];
            vi[v3] = v1 * v2;
            cout << "Multiplying " << v1  << " and " << v2 << " and saving it to position " << v3 << endl;
            break;
        }
            
        case less_than:
        {
            int v1 = values[0];
            int v2 = values[1];
            int v3 = values[2];
            if(v1 < v2)
                vi[v3] = 1;
            else
                vi[v3] = 0;
            break;
        }
            
        case equals:
        {
            int v1 = values[0];
            int v2 = values[1];
            int v3 = values[2];
            if(v1 == v2)
                vi[v3] = 1;
            else
                vi[v3] = 0;
            break;
        }

        case input:
        {
            int pos = values[0];
            cout << "Enter a value to save to position " << pos << endl;;
            int v1;
            cin >> v1;
            cout << "Saving " << v1 << " in position " << pos << endl;
            vi[pos] = v1;
            break;
        }
            
        case output:
        {
            int v1 = values[0];
            cout << "Output: " << v1 << endl;
            break;
        }
            
        case jump_if_true:
        {
            int v1 = values[0];
            int v2 = values[1];
            if(v1 != 0)
                pointer = v2;
            break;
        }
            
        case jump_if_false:
        {
            int v1 = values[0];
            int v2 = values[1];
            if(v1 == 0)
                pointer = v2;
            break;
        }
            
        default:
            throw runtime_error("Invalid OpCode");
    }
}


int main()
{
    ifstream is {"input.txt"};
    if(!is) throw runtime_error("Couldn't open input file.");
    
    ofstream os {"output.txt"};
    if(!os) throw runtime_error("Couldn't open output file.");
    
    vector<int> vi = fill_vec(is,'|');
    
    int i = 0;
    while(i < vi.size())
    {
        vector<int> init = initial_values(vi[i++]);
        int OpCode = init[0];
        if(OpCode == quit)
            break;
        vector<int> parameter_numbers;
        for(int k = 0; k < num_of_parameters(OpCode); ++k)
            parameter_numbers.push_back(vi[i++]);
        vector<Parameter> parameters;
        for(int k = 0; k < parameter_numbers.size(); ++k)
            parameters.push_back(Parameter{parameter_numbers[k], init[k+1]}); //Skip the OpCode
        instruction instruct {OpCode,parameters};
        calculate(vi,instruct,i);
    }

    return 0;
}
