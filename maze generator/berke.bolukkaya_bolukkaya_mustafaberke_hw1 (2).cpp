#include <iostream>
#include <vector>
#include <stdlib.h>
#include <fstream>
#include <random>
#include <string>
#include <queue>
using namespace std;
// Mustafa Berke BOLUKKAYA 32683
//________________________________________________________________

template <class T>
struct Node //node for the stack object
{
    T value;
    Node<T> *next;

};

template <class T>
class theStack
{
private:
    Node<T> *top; //top of the stack

public:
    theStack(){
        top = nullptr;
    };
    void push(T val){
        Node<T> *newNode;
        newNode = new Node<T>; //create a new node to store the value
        newNode->value = val;

        if (isEmpty())
        {
            top = newNode;
            newNode->next = nullptr;
        }
        else
        {
            newNode->next = top;
            top = newNode;
        }
    };
    void pop(T &val){
        Node<T> *temp;

        if (isEmpty())
        {
            cout << "The stack is empty.\n";
        }
        else
        {
            val = top->value;
            temp = top->next;
            delete top;
            top = temp;
        }
    };
    bool isEmpty(){
        bool check;
        if (top == nullptr)
            check = true;
        else
            check = false;

        return check;
    };
    T getTop() {
        if (isEmpty()) {
            std::cerr << "The stack is empty.\n";
            exit(1); // Alternatively, you can handle this error condition as per your application's requirement
        } else {
            return top->value;
        }
    }
};
// _______________________________________________________________
struct maze_cell{

    int x,y;
    int up=1,right=1,left=1,down=1;
    bool visited=false;
    bool visited_for_solving=false;
    bool go_down= false,go_up=false, go_left=false, go_right=false;

    maze_cell() {}
    maze_cell(int x,int y) {
        x=x;
        y=y;
        up=1;
        left=1;
        right=1;
        down=1;
    }

};
// _______________________________________________________________
bool rand_popping(vector<vector<maze_cell>> & maze, maze_cell & dummy_cell, theStack<maze_cell> & maze_stack, bool & failed)
{   //pops cell walls in a random manner. Keeps going until all options are tried.
    int r;
    bool r1,r2,r3,r4;
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<int> distribution(0, 3);
    r  = distribution(gen);

    if (r == 0) //try popping the bottom
    {
        if (dummy_cell.x + 1 <= maze.size() - 1 && !maze[dummy_cell.x + 1][dummy_cell.y].visited)
        {
            maze[dummy_cell.x][dummy_cell.y].down = 0;
            maze[dummy_cell.x + 1][dummy_cell.y].visited = true;
            maze[dummy_cell.x + 1][dummy_cell.y].up  = 0;
            r1=true;
        }
        else{
            r1=false;
        }

        if (r1) //if popping bottom works
        {
            dummy_cell = maze[(dummy_cell.x + 1)][dummy_cell.y]; //modify the dummy_cell as the bottom cell
            maze_stack.push(maze[dummy_cell.x][dummy_cell.y]); //and push it to the stack
            return true;
        }
        dummy_cell.go_down = true;
    }
    else if (r == 1) //pop left
    {
        if (dummy_cell.y - 1 >= 0 && !maze[dummy_cell.x][dummy_cell.y - 1].visited)
        {
            maze[dummy_cell.x][dummy_cell.y].left = 0; //breaks the walls down
            maze[dummy_cell.x][dummy_cell.y - 1].visited = true; //sets it as visited
            maze[dummy_cell.x][dummy_cell.y - 1].right = 0;
            r2= true;
        }
        else{
            r2=false;
        }
        if (r2) //if popping left works
        {
            dummy_cell = maze[dummy_cell.x][dummy_cell.y - 1]; //modify the dummy_cell as the left cell
            maze_stack.push(maze[dummy_cell.x][dummy_cell.y]); //and push it to the stack
            return true;
        }
        dummy_cell.go_left = true;
    }
    else if (r == 2)
    {
        if (dummy_cell.x - 1 >= 0 && !maze[dummy_cell.x - 1][dummy_cell.y].visited)
        {
            maze[dummy_cell.x][dummy_cell.y].up = 0; //breaks the walls down
            maze[dummy_cell.x - 1][dummy_cell.y].visited = true; //and sets it as visited
            maze[dummy_cell.x - 1][dummy_cell.y].down = 0;
            r3= true;
        }
        else{
            r3=false;
        }
        if (r3) //if popping up works
        {
            dummy_cell = maze[dummy_cell.x - 1][dummy_cell.y]; //modify the dummy_cell as the top cell
            maze_stack.push(maze[dummy_cell.x][dummy_cell.y]); //and push it to the stack
            return true;
        }
        dummy_cell.go_up = true;
    }
    else
    {
        if (dummy_cell.y + 1 <= maze[0].size() - 1 && !maze[dummy_cell.x][dummy_cell.y + 1].visited)
        {
            maze[dummy_cell.x][dummy_cell.y].right = 0; //breaks the walls down
            maze[dummy_cell.x][dummy_cell.y + 1].visited = true; //sets is as visited
            maze[dummy_cell.x][dummy_cell.y + 1].left = 0;
            r4= true;
        }
        else{
            r4=false;
        }
        if (r4) //if popping right works
        {
            dummy_cell = maze[dummy_cell.x][dummy_cell.y + 1]; //modify the dummy_cell as the right cell
            maze_stack.push(maze[dummy_cell.x][dummy_cell.y]); //and push it to the stack
            return true;
        }
        dummy_cell.go_right= true;
    }
    if (dummy_cell.go_right && dummy_cell.go_left && dummy_cell.go_up && dummy_cell.go_down)
    {
        failed = true;
        return true;
    }
    return false;
}


// _______________________________________________________________
void stackToQueue(theStack<maze_cell> & stack, std::queue<maze_cell> & queue ) {
    maze_cell t;
    if (stack.isEmpty()) {
        return;
    }
    stack.pop(t);
    stackToQueue(stack, queue);
    queue.push(t); // Push the element into the queue
}

void PrintQueue(std::queue<maze_cell> & queue, std::ofstream & pout, const int & M) {
    while (!queue.empty()) {
        maze_cell t = queue.front();
        queue.pop();
        pout << t.y << " " << (M - t.x - 1) << std::endl;
    }
}



int main() {
    int K;
    int M;
    int N;
    cout << "Enter the number of mazes: ";
    cin >> K;
    cout << "Enter the number of rows and columns (M and N): ";
    cin >> M >> N ;

    vector<vector<vector<maze_cell>>> vec;
    for (int i = 1; i <= K; i++)
    {
        vector<vector<maze_cell>> maze(M,vector<maze_cell>(N));
        for (int j = 0; j < M; j++)
        {
            for (int k = 0; k < N; k++)
            {
                maze[j][k].x = j;
                maze[j][k].y = k;
            }
        }
        theStack<maze_cell> mazeMaker;
        mazeMaker.push(maze[0][0]);
        maze[0][0].visited = true;
        maze_cell dummy_cell = maze[0][0]; //dummy cell to test

        while (!mazeMaker.isEmpty())
        {
            bool finish = false;
            bool can_go = false;
            while (!finish)
            {
                finish = rand_popping(maze, dummy_cell, mazeMaker, can_go); //keep trying to pop a wall randomly
            }
            if (can_go)
                mazeMaker.pop(dummy_cell); //pop to go a step back
        }
        vec.push_back(maze); //push the maze to the maze vector
        int wanted_version=0;
        ofstream output;
        string file_maze="maze_";
        string maze_no=to_string(i);
        string txt=".txt";
        auto file_name = file_maze + maze_no + txt ;
        output.open(file_name.c_str());
        output << maze.size() << " " << maze[0].size() << endl;


        for (int x = 0; x < maze.size(); x++)
        {
            for (int y = 0; y < maze[0].size(); y++)
            {
                output << "x=" << y << " y=" << x << " l=" << maze[M-1-wanted_version][y].left << " r=" << maze[M-1-wanted_version][y].right << " u=" << maze[M-1-wanted_version][y].up << " d=" << maze[M-1-wanted_version][y].down << endl;
            }
            wanted_version++;
        }
        output.close();
    }

    cout << "All mazes are generated." << endl;
    //MAZE SOLVING
    int mazeID,x1,x2,y1,y2;

    cout << "Enter a maze ID between 1 and " << K << " inclusive to find a path: ";
    cin >> mazeID;
    cout << endl;

    cout << "Enter x and y coordinates of the entry points (x,y) or (column,row): ";
    cin >> x1 >> y1;
    cout << endl;
    cout << "Enter x and y coordinates of the exit points (x,y) or (column,row): ";
    cin >> x2 >> y2;
    cout << endl;

    theStack<maze_cell> solve; //solving stack
    vector<vector<maze_cell>> maze; //maze that we will solve
    maze_cell temp; //temporary current cell


    //converting input to wanted version;
    int a,b,c,d;
    a = M-1-y1;
    b = x1;

    maze = vec[mazeID - 1];
    maze[a][b].visited_for_solving=true;
    temp = maze[a][b]; //set it as the starting point
    temp.visited_for_solving=true;
    solve.push(temp); //push it to the stack
    // point conversion
    c= M-1-y2;
    d= x2;


    while (!(temp.x == c && temp.y == d)) //looping till our target
    {
        bool failed = false;

        if (maze[temp.x][temp.y].up== 0 && !maze[temp.x-1][temp.y].visited_for_solving)
        {

            maze[temp.x-1][temp.y].visited_for_solving=true;
            temp = maze[temp.x-1][temp.y]; //update the temp as the new upper cell
            maze[temp.x][temp.y].visited_for_solving = true; //set it as visited
            solve.push(temp); //push the new cell to the stack

        }
            //going down (same idea as above)
        else if (maze[temp.x][temp.y].down== 0 && !maze[temp.x+1][temp.y].visited_for_solving)
        {

            maze[temp.x+1][temp.y].visited_for_solving=true;
            temp = maze[temp.x+1][temp.y];
            maze[temp.x][temp.y].visited_for_solving = true;
            solve.push(temp);

        }
            //going right
        else if (maze[temp.x][temp.y].right== 0 && !maze[temp.x][temp.y+1].visited_for_solving)
        {

            maze[temp.x][temp.y+1].visited_for_solving=true;
            temp = maze[temp.x][temp.y+1];
            maze[temp.x][temp.y].visited_for_solving = true;
            solve.push(temp);

        }
            //going left
        else if (maze[temp.x][temp.y].left== 0 && !maze[temp.x][temp.y-1].visited_for_solving)
        {

            maze[temp.x][temp.y-1].visited_for_solving=true;
            temp = maze[temp.x][temp.y-1];
            maze[temp.x][temp.y].visited_for_solving = true;
            solve.push(temp);

        }
        else
            failed = true;

        if (failed == true)
        {

            solve.pop(temp); //popping out wrong cell
            temp=solve.getTop();

        }
    }
    std::queue<maze_cell>  queue;
    ofstream pout; //for path_out
    string filename = "maze_" + to_string(mazeID) + "_path_" + to_string(x1) + "_" + to_string(y1) + "_" + to_string(x2) + "_" + to_string(y2) + ".txt"; //set the filename
    pout.open(filename.c_str());
    //first stack to queue then printing
    stackToQueue(solve, queue);
    PrintQueue( queue,  pout,  M);
    pout.close(); //close the output file


    return 0;





}
