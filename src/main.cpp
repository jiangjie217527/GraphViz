// remove-item -path "E:\workspace\Assignment1\build-GraphViz-Desktop_Qt_6_3_2_MinGW_64_bit-Debug" -recurse
#include <iostream>
#include <fstream>
#include <cstring>
#include <cstring>
#include <cmath>
#include <vector>
#include <stdlib.h>
#include "SimpleGraph.h"
const double Pai        = 3.14159265358979323;
const double k_repel    = 0.05;
const double k_attract  = 0.01;
const double k_random   = 0.001;
const double k_penalties= 0.02;
const double friction   = 4;
double sec;int num_of_nodes,num_of_edges,flg;
using namespace std;
vector<Node>v; //velocity extension
struct ARG{
    size_t A,B,C;
};
typedef struct ARG Arg;
vector<Arg>arg;
//std::vector
void Welcome();
void init(SimpleGraph&);
void movegraph(SimpleGraph&);
double solve_arg(double,double,double,double);
//ofstream test("testoutput");

// Main method

int main() {
    Welcome();
    srand(time(NULL));

    while(1){
        SimpleGraph graph;
        InitGraphVisualizer(graph);
        init(graph);
        time_t startTime = time(NULL);
        while(difftime(time(NULL), startTime)<=sec){
            if(flg == num_of_nodes)
                movegraph(graph);
            DrawGraph(graph);
        }
        cout<<"Do you want to try it again?(y/n)"<<endl;
        string op;cin>>op;
        if(op=="n")break;
    }
    exit(0);
    return 0;
}

/* Prints a message to the console welcoming the user and
 * describing the program. */

void Welcome() {
    cout << "Welcome back.I have waited for long time" << endl;
    cout << "This program uses a force-directed graph layout algorithm" << endl;
    cout << "to render sleek, snazzy pictures of various graphs." << endl;
    cout << endl;
}

void init(SimpleGraph &graph){
    string name;sec = 0;

    cout<< "please enter the file name:" ;
    cin >>  name;
    ifstream my_file(name);
    if(my_file.fail()){
        cout<< "WARRING,THE FILE DOESN'T EXIST!" << endl;
        exit(0);
    }
    cout<< "please enter working time:";
    cin >> sec;int cnt = 0;
    while(sec <= 0){
        if(cnt == 0)
            cout << "please enter a positive number" << endl;
        if(cnt == 1)
            cout << "well I really can't do so" << endl;
        if(cnt == 2)
            cout << "are you serious?,final warring,positive please." << endl;
        if(cnt >= 3)
            cout << "I really want to destroy the world!" << endl;
        cin>>sec;
        cnt++;
    }

    num_of_nodes = 0;
    my_file>>num_of_nodes;
    size_t x,y;
    while( my_file>>x>>y ){
        if( x > y )swap( x , y );
        graph.edges.push_back( Edge{.start = x, .end = y} );
    }
    num_of_edges = graph.edges.size();

    //te(4) pan(4)
    int d[301];
    memset( d , 0 , sizeof(d) ) ;
    for(int i = 0 ; i < num_of_edges ; i++ ){
        Edge u = graph.edges[i];
        d[ u.end ]++;
        d[u.start]++;
    }
    flg = num_of_nodes;
    for(int i = 0 ; i < num_of_nodes ; i++){
        if(d[i] == num_of_nodes - 1 ){
            flg = i;
            break;
        }
    }
    if(flg == num_of_nodes)
        for(int i = 0 ; i < num_of_nodes ; i++)
          graph.nodes.push_back( Node{ cos(2*Pai*i/num_of_nodes), sin(2*Pai*i/num_of_nodes)} );
    else{
        for(int i = 0 ; i < num_of_nodes ; i++ ){
            if(i == flg)graph.nodes.push_back( Node{0,0});
            else        graph.nodes.push_back( Node{cos(2*Pai*i/(num_of_nodes-1)), sin(2*Pai*i/(num_of_nodes-1))} );
        }
    }
    for(int i = 0 ; i < num_of_nodes;i++){
        for(int j=i+1;j<num_of_nodes;j++){
            if( graph.edges[i].end==graph.edges[j].end ){
                arg.push_back(Arg{graph.edges[i].start,
                                  graph.edges[i].end,
                                  graph.edges[j].start}  );
            }
            else if( graph.edges[i].start==graph.edges[j].start ){
                arg.push_back(Arg{graph.edges[i].end,
                                  graph.edges[j].start,
                                  graph.edges[j].end }  );
            }
            else if( graph.edges[i].end==graph.edges[j].start){
                arg.push_back(Arg{graph.edges[i].start,
                                  graph.edges[i].end,
                                  graph.edges[j].end}   );
            }
        }
    }
    v.resize( num_of_nodes );
    return ;
}

void movegraph(SimpleGraph &g){
    //v.resize(num_of_nodes);
    //Have each node exert a repulsive force on each other node.
    for(int i = 0 ; i < num_of_nodes ; i++){

        Node   A  = g.nodes[i];
        double x0 = A.x;
        double y0 = A.y;
        for(int j = i + 1 ; j < num_of_nodes ; j++){

            Node   B  = g.nodes[j];
            double x1 = B.x;
            double y1 = B.y;

            double dis_x   = x1 - x0;
            double dis_y   = y1 - y0;
            double theta   = atan2(dis_y,dis_x);
            double f_repel = k_repel/sqrt( pow(dis_x,2) + pow(dis_y,2) );

            v[i].x -= f_repel * cos(theta);
            v[i].y -= f_repel * sin(theta);
            v[j].x += f_repel * cos(theta);
            v[j].y += f_repel * sin(theta);
        }

    }

    //Have each edge exert an attractive force on its endpoints.
    for(int i = 0,edge_tot = g.edges.size();i < edge_tot; i++){
        Edge u = g.edges[i];
        int a0 = u.start;
        int a1 = u.end;
        Node A = g.nodes[ a0 ];
        Node B = g.nodes[ a1 ];

        double x0 = A.x,y0 = A.y;
        double x1 = B.x,y1 = B.y;
        double dis_x = x1 - x0;
        double dis_y = y1 - y0;

        double theta     = atan2( dis_y , dis_x );
        double f_attract = k_attract * ( pow(dis_x,2) + pow(dis_y,2) ) ;

        v[a0].x += f_attract * cos(theta);
        v[a0].y += f_attract * sin(theta);
        v[a1].x -= f_attract * cos(theta);
        v[a1].y -= f_attract * sin(theta);
    }
//extension penalties for low resolution
    for(int i = 0, tot_arg = arg.size(); i < tot_arg ; i++){
        int a = arg[i].A,
            b = arg[i].B,
            c = arg[i].C;
        double x1 = g.nodes[a].x - g.nodes[b].x,
               x2 = g.nodes[c].x - g.nodes[b].x,
               y1 = g.nodes[a].y - g.nodes[b].y,
               y2 = g.nodes[c].y - g.nodes[b].y;
        double phi= solve_arg( x1 , x2 , y1 , y2 );
        if(phi < Pai / 3){
            double dis_x   = g.nodes[a].x - g.nodes[c].x;
            double dis_y   = g.nodes[a].y - g.nodes[c].y;
            double theta   = atan2(dis_y,dis_x);
            double f_penalties = k_penalties/sqrt( pow(dis_x,2) + pow(dis_y,2) );

            v[c].x -= f_penalties * cos(theta);
            v[c].y -= f_penalties * sin(theta);
            v[a].x += f_penalties * cos(theta);
            v[a].y += f_penalties * sin(theta);
        }
        else if(phi > 2 * Pai / 3 ){
            double dis_x   = g.nodes[a].x - g.nodes[c].x;
            double dis_y   = g.nodes[a].y - g.nodes[c].y;

            double theta     = atan2( dis_y , dis_x );
            double f_penalties = k_penalties * ( pow(dis_x,2) + pow(dis_y,2) ) ;

            v[c].x += f_penalties * cos(theta);
            v[c].y += f_penalties * sin(theta);
            v[a].x -= f_penalties * cos(theta);
            v[a].y -= f_penalties * sin(theta);
        }
    }

//extinsion of myself
/*
    double sum=0;
    for(int i = 0 ; i < num_of_edges ; i++){
        Edge u =  g.edges[i];
        int A  =  u.start;
        int B  =  u.end;
        double dis_x   = g.nodes[A].x -  g.nodes[B].x;
        double dis_y   = g.nodes[A].y -  g.nodes[B].y;
        sum+=sqrt( pow(dis_x,2) + pow(dis_y,2) );
    }
    sum/=num_of_edges;
    for(int i = 0 ; i < num_of_edges ; i++){
        Edge u =  g.edges[i];
        int  A =  u.start;
        int  B =  u.end;
        double dis_x   = g.nodes[A].x -  g.nodes[B].x;
        double dis_y   = g.nodes[A].y -  g.nodes[B].y;
        if(sqrt( pow(dis_x,2) + pow(dis_y,2) ) <sum/10){
            double theta     = atan2( dis_y , dis_x );
            v[B].x -= 10 * k_repel * cos(theta);
            v[B].y -= 10 * k_repel * sin(theta);
            v[A].x += 10 * k_repel * cos(theta);
            v[A].y += 10 * k_repel * sin(theta);
        }
    }
*/
//extension giving each node a slight“push” in a random direction
    //Move the nodes according to the net force(which result in the velocity) acting on them.
    for(int i = 0; i < num_of_nodes ; i++){
        g.nodes[i].x += v[i].x + k_random * ( rand() - RAND_MAX ) / RAND_MAX;
        g.nodes[i].y += v[i].y + k_random * ( rand() - RAND_MAX ) / RAND_MAX;
        v[i].x /= friction;
        v[i].y /= friction;
        //without friction,you will get nothing
    }
    return ;
}

double solve_arg(double x1,double x2,double y1,double y2){
    return acos(
                (x1 * x2 + y1 * y2) /
        (sqrt(x1 * x1 + y1 * y1) * sqrt(x2 * x2 + y2 * y2))
                );
}
