#include <iostream>
#include <vector>
#include <fstream>
#include <set>
#include <algorithm>
#include <map>

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

bool operator==(const Point& a, const Point& b)
{
    return (a.x == b.x && a.y == b.y);
}

bool operator!=(Point& a, Point& b)
{
    return !(a==b);
}


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
    end_of_loop(is, '|', "Bad termination of file"); //Added a '|' character to indicate end of first wire in input.
    vector<Point> wire_1;
    wire_1.push_back(Point{0,0}); //Initialize at the origin
    for(const Movement& m: vm)
        calc_movement(wire_1, m);

    //Wire 2 Movements
    vector<Movement> vm2;
    for(Movement m; is >> m;)
        vm2.push_back(m);
    vector<Point> wire_2;
    wire_2.push_back(Point{0,0}); //Initialize at the origin
    for(const Movement& m: vm2)
        calc_movement(wire_2, m);
    
    vector<Point> wire_1_srt = wire_1; //sorted version of wire_1
    vector<Point> wire_2_srt = wire_2; //sored version of wire_2
    
    //Sort the two wires so that I can use STL set_intersection
    sort(wire_1_srt.begin(), wire_1_srt.end(),Point_order());
    sort(wire_2_srt.begin(),wire_2_srt.end(),Point_order());
    
    
    vector<Point> intersection; //The intersect of the two sets of points gives the points where the wires cross
    set_intersection(wire_1_srt.begin(),wire_1_srt.end(),wire_2_srt.begin(),wire_2_srt.end(),back_inserter(intersection));
    sort(intersection.begin(), intersection.end(), Manhattan_order()); //Sort the set of points according to Manhattan distance from the origin
    intersection.erase(intersection.begin()); //(0,0) doesn't count
    
    Point solution = intersection[0];
    
    cout << "The intersection point closest to the origin is " << solution << '.' << endl;
    cout << "The manhattan distance is " << manhattan_distance(solution) << '.' << endl;
    
    //Part B
    map<int, Point> intersect_distance;
    for(const Point& p: intersection)
    {
        //Calculate for Wire 1
        auto it = find(wire_1.begin(),wire_1.end(),p);
        if(it == wire_1.end()) throw runtime_error("Couldn't find the intersection point.");
        long dist_1 = it - wire_1.begin();
        
        //Calculate for Wire2;
        it = find(wire_2.begin(),wire_2.end(),p);
        if(it == wire_2.end()) throw runtime_error("Couldn't find the intersection point.");
        long dist_2 = it - wire_2.begin();
        
        long total = dist_1 + dist_2;
        intersect_distance.insert({total, p});
    }
    
    auto solution2 = *(intersect_distance.begin());
    cout << "The closest point in terms of minimizing signal delay is " << solution2.second << " with a distance of " << solution2.first << '.' << endl;
    
    
    return 0;
}

