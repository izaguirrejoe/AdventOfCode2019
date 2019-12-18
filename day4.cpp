#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

vector<int> break_down(int n)
{
    vector<int> v;
    while(n > 0)
    {
        v.push_back(n%10);
        n /= 10;
    }
    reverse(v.begin(),v.end());
    return v;
}

bool six_digit(const vector<int>& v)
{
    if(v.size() == 6)
        return true;
    return false;
}

bool two_adjacent(const vector<int>& v)
{
    if(v[0] == v[1] || v[1] == v[2] || v[2] == v[3] || v[3] == v[4] || v[4] == v[5] || v[5] == v[6]) return true;
    return false;
}

bool never_decrease(const vector<int>& v)
{
    if(v[1] < v[0] || v[2] < v[1] || v[3] < v[2] || v[4] < v[3] || v[5] < v[4]) return false;
    return true;
}

bool only_two_adjacent(const vector<int>& v)
{
    if(v.size() < 2) return false;
    int i = 0, j = 1;
    while(j < v.size())
    {
        if(v[i] != v[j])
        {
            i = j;
            ++j;
        }
        else
        {
            while(j < v.size() && v[i] == v[j])
                ++j;
            if(j - i == 2) return true;
        }
    }
    return false;
}

int main()
{
    //Part A
    int solution = 0;
    for(int i = 153517; i <= 630395; ++i)
    {
        vector<int> vi = break_down(i);
        if(six_digit(vi) && two_adjacent(vi) && never_decrease(vi))
            ++solution;
        
    }
    cout << solution << endl;
    
    //Part B
    solution = 0;
    for(int i = 153517; i <= 630395; ++i)
    {
        vector<int> vi = break_down(i);
        if(six_digit(vi) && two_adjacent(vi) && only_two_adjacent(vi) && never_decrease(vi))
            ++solution;
    }
    cout << solution << endl;
    
    return 0;
}
