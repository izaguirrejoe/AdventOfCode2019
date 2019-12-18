#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

int main()
{
    int result = 0;
    for(int i = 153517; i <= 630395; ++i)
    {
        //Check to see if only increasing
        string n = to_string(i);
        string cpy = n;
        sort(n.begin(),n.end());
        if(n == cpy)
        { //if n is only increasing check if there is a set of two characters
            map<char,int> words;
            for(char c: n)
                ++words[c];
            for(const auto& p : words)
                if(p.second == 2)
                {
                    ++result;
                    break;
                }
        } // if n is not increasing move on to next n
    }
    cout << result << endl;
    return 0;
}
