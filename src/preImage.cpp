#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include <random>

using namespace std;

double cosine_similarity(vector<int> &A, vector<int> &B)
{
    double dot = 0.0, denom_a = 0.0, denom_b = 0.0;
    for (int i = 0; i < A.size(); i++)
    {
        dot += A[i] * B[i];
        denom_a += A[i] * A[i];
        denom_b += B[i] * B[i];
    }
    return dot / (sqrt(denom_a) * sqrt(denom_b));
}
double rmsValue(vector<int> &arr, int n)
{
    int square = 0;
    double mean = 0.0, root = 0.0;
 
    // Calculate square.
    for (int i = 0; i < n; i++) {
        square += pow(arr[i], 2);
    }
 
    // Calculate Mean.
    mean = (square / (double)(n));
 
    // Calculate Root.
    root = sqrt(mean);
 
    return root;
}
int main(int argc, char **argv)
{
    random_device rd;
	mt19937 gen(rd());
    uniform_real_distribution<double> dist(0.001, 1.0);
    string ifile = argv[1], ofile = argv[2];
    ifstream infile(ifile, ios::in | ios::binary);
    //cout << ifile << endl;
    //cout << ofile << endl;
    ofstream oufile(ofile);
    vector<pair<int, vector<int>>> A;
    
    for (int i = 0; i < 10000; i++)
    {
        unsigned char x;
        infile.read((char *)&x, 1);
        int type=static_cast<int>(x);
        //cout<<"type: "<<type<<endl;
        vector<int> tmp;
        for (int j = 0; j < 3072; j++)
        {
            unsigned char bit;
            infile.read((char *)&bit, 1);
            tmp.push_back(static_cast<int>(bit));
        }
        //cout << tmp.size() << endl;
        A.push_back(make_pair(type, tmp));
    }
    
    vector<vector<int>> B;
    for (int i = 0; i < 10000; i++)//chọn 500 hình
    {
        //cout<<A[i].first<<endl;
        //if((char)A[i].first!=1) continue;
        double rand1 = dist(gen);
        if(rand1<0.5) continue;
        B.push_back(A[i].second);
        if(B.size()==500) break;
    }
    oufile<<500<<" "<<250000<<endl;
    vector<double> cost;
    double sum=0;
    for(int i=0;i<500;i++)//khởi tạo cost
    {
        double tmp=rmsValue(B[i],B[i].size());
        cost.push_back(tmp);
        sum+=tmp;
        //oufile<<rmsValue(B[i],B[i].size())<<endl;
    }
    double tl=50.0/sum;
    for(int i=0;i<500;i++)
    {
        oufile<<cost[i]*tl<<endl;
    }
    for(int i=0;i<500;i++)
    {
        for(int j=0;j<500;j++)
        {
            double tmp=cosine_similarity(B[i],B[j]);
            oufile<<i<<" "<<j<<" "<<tmp<<endl;
        }
    }
    
    infile.close();
    oufile.close();
    return 1;
}