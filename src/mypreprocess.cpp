#include <fstream>
#include <iostream>
#include <vector>
#include <random>

using namespace std;
double r_min,r_max;
double fun_g(double x)
{
	return 1.0-exp(-0.2*x);
}
int main(int argc, char** argv)
{
    string ifile=argv[1], ofile=argv[2];
    r_min=stod(argv[3]);
    r_max=stod(argv[4]);
    cout<<ifile<<endl;
    cout<<ofile<<endl;
    random_device rd;
	mt19937 gen(rd());
    uniform_real_distribution<double> dist(0.01, 1.0);
    int no_nodes,no_edges;
    ifstream infile(ifile);
    ofstream oufile(ofile);
    infile>>no_nodes>>no_edges;
    
    int s_node,e_node;
    int new_no_edges=0;
    vector<pair<int,int>> ed;
    vector<double> cost_ed;
    vector<double> cost_nodes;
    for(int i=0;i<no_nodes;i++)
    {
        cost_nodes.push_back(0);
    }

    //Đọc vào danh sách cạnh
    for(int i=0;i<no_edges;i++)
    {
        infile>>s_node>>e_node;
        if(e_node>s_node)
        {
            new_no_edges++;
            double rand1 = dist(gen);

            cost_nodes[s_node]+=rand1;
            cost_nodes[e_node]+=rand1;
            cost_ed.push_back(rand1);
            ed.push_back(make_pair(s_node,e_node));
        }
    }
    oufile<<no_nodes<<" "<<ed.size()<<endl;//ghi đầu file số lượng nodes và số lượng cạnh
    
    vector<double> tmpCost;
    uniform_real_distribution<double> dist5(r_min, r_max);
    double max_cost=0;
    for(int i=0;i<no_nodes;i++)//sinh ngẫu nhiên trọng số đỉnh (chí phí - cost) từ 1.0 đến 5.0
    {
        double rand1 = dist5(gen);
        tmpCost.push_back(rand1);
        if(rand1>max_cost)
            max_cost=rand1;
    }
    int count=0;
    for(int i=0;i<no_nodes;i++)//sinh ngẫu nhiên trọng số đỉnh (chí phí - cost) từ 1.0 đến 5.0
    {
        double t=tmpCost[i];
        //if(i<=no_nodes * 0.75)
        //{
        //    count++;
        //    t=t/100;
        //}
        oufile<<t<<endl;
    }
    cout<<"count: "<<count<<endl;
    int p=0;
    for(pair<int,int> e:ed)//trọng số cạnh u, v, p_1, P_2,....., p_k
    {
        oufile<<e.first<<" "<<e.second<<" "<<cost_ed[p]<<endl;
        p++;
    }
    
    infile.close();
    oufile.close();
    return 1;
}