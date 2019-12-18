#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <algorithm>

using namespace std;

struct Point{
    int x;
    int y;
    
    Point& operator=(const Point& p)
    {
        x = p.x;
        y = p.y;
        return *this;
    }
};

ostream& operator<<(ostream& os, const Point& p)
{
    os << '(' << p.x << ',' << p.y << ')';
    return os;
}

bool operator<(const Point& a, const Point& b)
{
    if(a.x == b.x)
        return a.y < b.y;
    
    return a.x < b.x;
}

struct Point_order{
    bool operator()(const Point& a, const Point& b) const
    {
        if(a.x == b.x) return a.y < b.y;
        return a.x < b.x;
    }
};


struct Movement{
    char d; //Direction
    int s; //Number of steps
};


bool isValid(char dir)
{
    if(dir == 'R' || dir == 'L' || dir == 'U' || dir == 'D') return true;
    return false;
}

ostream& operator<<(ostream& os, const Movement& m)
{
    os << m.d << m.s;
    return os;
}

istream& operator>>(istream& is, Movement& m)
{
    char dir;
    is >> dir;
    if(!is || !isValid(dir)){
        is.unget();
        is.clear(ios_base::failbit);
        return is;
    }
    
    m.d = dir;
    
    int steps;
    char comma;
    is >> steps >> comma;
    if(!is && !is.eof()) throw runtime_error("Error");
    if(comma == '|') is.unget();
    m.s = steps;
    return is;
}

void end_of_loop(istream& is, char term, const string& message)
{
    if(is.fail()){
        is.clear();
        char ch;
        is >> ch;
        if(is && ch == term) return;
        throw runtime_error(message);
    }
}

void calc_movement(vector<Point>& wire, const Movement& m) //adds points to the given wire based off the given movement
{
    Point start = wire.back();
    if(m.d == 'U'){
        for(int i = 0; i < m.s; ++i)
        {
            ++start.y;
            wire.push_back(start);
        }
    }
    
    if(m.d == 'D'){
        for(int i = 0; i < m.s; ++i)
        {
            --start.y;
            wire.push_back(start);
        }
    }
    
    if(m.d == 'L'){
        for(int i = 0; i < m.s; ++i)
        {
            --start.x;
            wire.push_back(start);
        }
    }
    
    if(m.d == 'R'){
        for(int i = 0; i < m.s; ++i)
        {
            ++start.x;
            wire.push_back(start);
        }
    }
}

struct Manhattan_order{
    bool operator()(const Point& a, const Point& b)
    {
        int distA = abs(a.x) + abs(a.y);
        int distB = abs(b.x) + abs(b.y);
        return distA < distB;
    }
};

int manhattan_distance(const Point& p)
{
    return abs(p.x) + abs(p.y);
}


int main(int argc, const char * argv[]) {
    ifstream is {"advent3.txt"};
    if(!is) throw runtime_error("Unable to open input.");
    
    is.exceptions(is.exceptions()|ios_base::badbit);
    
    //Wire 1 Movements
    vector<Movement> vm;
    for(Movement m; is >> m;)
        vm.push_back(m);
    end_of_loop(is, '|', "Bad termination of file");
    vector<Point> wire_1;
    wire_1.push_back(Point{0,0}); //Initialize at the origin
    for(Movement m: vm)
        calc_movement(wire_1, m);

    //Wire 2 Movements
    vector<Movement> vm2;
    for(Movement m; is >> m;)
        vm2.push_back(m);
    vector<Point> wire_2;
    wire_2.push_back(Point{0,0}); //Initialize at the origin
    for(Movement m: vm2)
        calc_movement(wire_2, m);
    
    //Sort the two wires so that I can use STL set_intersection
    sort(wire_1.begin(), wire_1.end(),Point_order());
    sort(wire_2.begin(),wire_2.end(),Point_order());
    
    
    vector<Point> intersection; //The intersect of the two sets of points gives the points where the wires cross
    set_intersection(wire_1.begin(),wire_1.end(),wire_2.begin(),wire_2.end(),back_inserter(intersection));
    sort(intersection.begin(), intersection.end(), Manhattan_order()); //Sort the set of points according to Manhattan distance from the origin
    
    Point solution = intersection[1]; //Skip the first element, the origin
    
    cout << "The intersection point closest to the origin is " << solution << '.' << endl;
    cout << "The manhattan distance is " << manhattan_distance(solution) << '.' << endl;
    
    return 0;
}

