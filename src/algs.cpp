#ifndef ALGS_CPP
#define ALGS_CPP

#include "mygraph.cpp"
#include "obj.cpp"
#include <set>
#include <map>
#include <string>
#include <vector>
#include <fstream>
#include <cfloat>

using namespace std;
using namespace mygraph;

enum Algs
{
   ALG1,
   ALG3,
   ALG3ACC,
   ALG4,
   ALG4ACC,
   SMKDET,
   SMKDETACC,
   SMKRAN,
   SMKRANACC,
   SMKSTREAM,
   SMKSTREAMACC,
   SAMPLEGREEDY,
   FANTOM
};

resultsHandler allResults;
mutex mtx;

class Args
{
public:
   Algs alg;
   string graphFileName;
   string outputFileName = "";
   size_t k = 2;
   tinyGraph g;
   double tElapsed;
   double wallTime;
   Logger logg;
   double epsi = 0.1;
   double delta = 0.1;
   double c = 1;
   size_t N = 1;
   size_t P = 10;
   bool plusplus = false;
   double tradeoff = 0.5;
   bool quiet = false;
   bool lazy = false;
   size_t nThreads = 1;
   double buget;

   Args() {}
   Args(const Args &args) : alg(args.alg),
                            graphFileName(args.graphFileName),
                            outputFileName(args.outputFileName),
                            k(args.k),
                            buget(args.buget),
                            g(args.g),
                            epsi(args.epsi),
                            delta(args.delta),
                            c(args.c),
                            N(args.N),
                            P(args.P),
                            plusplus(args.plusplus),
                            lazy(args.lazy),
                            nThreads(args.nThreads)
   {
   }
};

class MyPair
{
public:
   node_id u;
   double gain; // may be negative

   MyPair() {}
   MyPair(node_id a,
          double g)
   {
      u = a;
      gain = g;
   }

   MyPair(const MyPair &rhs)
   {
      u = rhs.u;
      gain = rhs.gain;
   }

   void operator=(const MyPair &rhs)
   {
      u = rhs.u;
      gain = rhs.gain;
   }
};

struct gainLT
{
   bool operator()(const MyPair &p1, const MyPair &p2)
   {
      return p1.gain < p2.gain;
   }
} gainLTobj;

struct revgainLT
{
   bool operator()(const MyPair &p1, const MyPair &p2)
   {
      return (p1.gain > p2.gain);
   }
} revgainLTobj;

void reportResults(size_t nEvals, size_t obj, size_t maxMem = 0)
{
   mtx.lock();
   allResults.add("obj", obj);
   allResults.add("nEvals", nEvals);
   allResults.add("mem", maxMem);
   mtx.unlock();
}

void random_set(tinyGraph &g, vector<bool> &C, vector<size_t> &A)
{
   C.assign(g.n, false);
   double prob = 0.5;
   uniform_real_distribution<double> unidist(0, 1);
   for (size_t i = 0; i < A.size(); ++i)
   {
      if (unidist(gen) < prob)
      {
         C[A[i]] = true;
      }
   }
}

vector<bool> emptyBoolVector;
vector<size_t> emptySize_tVector;
vector<double> emptyDoubleVector;
// thuật toán 3 là thuật toán 2 trong bài báo do thuật toán 2 đã bị bỏ trong bài báo
class Alg3
{
   random_device rd;
   mt19937 gen;
   Args &myArgs;
   size_t k;
   tinyGraph &g;
   size_t nEvals = 0;
   double epsi;
   double B;
   double tau, theta;
   vector<pair<double, node_id>> Q;
   vector<vector<double>> F_X; // lưu hàm f đã được tính
   vector<vector<double>> F_Y; // lưu hàm f đã được tính
public:
   Alg3(Args &args) : gen(rd()), myArgs(args), g(args.g)
   {
      k = args.k;
      epsi = args.epsi;
      B = args.buget * g.total_cost;
   }
   size_t run1()
   {
      double solVal = 0;
      vector<node_id> X;
      vector<node_id> Y;
      double B2 = B / 2;
      double Emax = 0;
      double current_fx = 0, current_fy = 0;
      for (node_id e = 0; e < g.n; e++)
      {
         // tìm 1 e max
         vector<node_id> tmp;
         tmp.push_back(e);
         double eVal = compute_valSet(nEvals, g, tmp);
         F_X[0][e] = eVal;
         F_Y[0][e] = eVal;

         if (eVal > Emax)
            Emax = eVal;

         if (g.adjList[e].wht <= B2)
         {
            bool flag_x = false;
            bool flag_y = false;
            double tmp_fx;
            if (F_X[X.size()][e] != -1.0)
            {
               tmp_fx = eVal;
            }
            else
            {
               X.push_back(e);
               tmp_fx = compute_valSet(nEvals, g, X);
               X.pop_back();
               F_X[X.size()][e] = tmp_fx;
            }

            if (((tmp_fx - current_fx) / g.adjList[e].wht) >= current_fx / B)
            {
               flag_x = true;
            }
            // Y

            double tmp_fy;
            if (F_Y[Y.size()][e] != -1.0)
               tmp_fy = eVal;
            else
            {
               Y.push_back(e);
               tmp_fy = compute_valSet(nEvals, g, Y);
               Y.pop_back();
               F_Y[Y.size()][e] = tmp_fy;
            }

            if (((tmp_fy - current_fy) / g.adjList[e].wht) >= current_fy / B)
            {
               flag_y = true;
            }
            //
            if (flag_x && flag_y)
            {
               if ((tmp_fx - current_fx) > (tmp_fy - current_fy))
               {
                  X.push_back(e);
                  current_fx = tmp_fx;
               }
               else
               {
                  Y.push_back(e);
                  current_fy = tmp_fy;
               }
            }
            else
            {
               if (flag_x)
               {
                  X.push_back(e);
                  current_fx = tmp_fx;
               }
               if (flag_y)
               {
                  Y.push_back(e);
                  current_fy = tmp_fy;
               }
            }
         }
      }

      vector<node_id> X2;
      vector<node_id> Y2;
      double tmpB = 0;
      for (int i = X.size() - 1; i > 0; i--)
      {
         if (tmpB + g.adjList[X[i]].wht <= B)
         {
            X2.push_back(X[i]);
            tmpB += g.adjList[X[i]].wht;
         }
         else
            break;
      }
      tmpB = 0;
      for (int i = Y.size() - 1; i > 0; i--)
      {
         if (tmpB + g.adjList[Y[i]].wht <= B)
         {
            Y2.push_back(Y[i]);
            tmpB += g.adjList[Y[i]].wht;
         }
         else
            break;
      }
      double fx = compute_valSet(nEvals, g, X2);
      double fy = compute_valSet(nEvals, g, Y2);
      solVal = max(max(fx, fy), Emax);
      return solVal;
   }
   size_t run()
   {
      nEvals = 0;
      double e2 = epsi / 12;
      double solVal = 0;
      // B = myArgs.buget;
      for (node_id u = 0; u < g.n; u++)
      {
         vector<double> tmpf(g.n, -1.0);
         F_X.push_back(tmpf);
         F_Y.push_back(tmpf);
      }
      tau = run1();
      // cout << "ALG2," << B << "," << tau << "," << nEvals<<endl;
      solVal = tau;
      vector<node_id> X;
      vector<node_id> Y;
      vector<node_id> Z;
      vector<bool> checkX(g.n, false);
      vector<bool> checkY(g.n, false);
      double cX = 0, cY = 0, cZ = 0;                         // cost
      double current_fx = 0, current_fy = 0, current_fz = 0; // current f
      double tmp_fx = 0.0, tmp_fy = 0, thetaX, thetaY;
      bool flag_x = false;
      bool flag_y = false;
      theta = 19 * tau / (6 * e2 * B);
      double nguong = (tau * (1 - e2)) / (6 * B);

      double l = e2 * B;
      while (theta > nguong)
      {
         for (node_id e = 0; e < g.n; e++)
         {
            flag_x = false;
            flag_y = false;
            if (checkX[e] == false && checkY[e] == false)
            {
               // chọn X
               if (cX + g.adjList[e].wht <= B)
               {
                  if (F_X[X.size()][e] != -1)
                  {
                     tmp_fx = F_X[X.size()][e];
                  }
                  else
                  {
                     X.push_back(e);
                     tmp_fx = compute_valSet(nEvals, g, X);
                     X.pop_back();
                     F_X[X.size()][e] = tmp_fx;
                  }
                  thetaX = (tmp_fx - current_fx) / g.adjList[e].wht;
                  if (thetaX >= theta)
                     flag_x = true; // e có thể thêm vào X
               }
               // chọn Y
               if (cY + g.adjList[e].wht <= B)
               {
                  if (F_Y[Y.size()][e] != -1)
                  {
                     tmp_fy = F_Y[Y.size()][e];
                  }
                  else
                  {
                     Y.push_back(e);
                     tmp_fy = compute_valSet(nEvals, g, Y);
                     Y.pop_back();
                     F_Y[Y.size()][e] = tmp_fy;
                  }
                  thetaY = (tmp_fy - current_fy) / g.adjList[e].wht;
                  if (thetaY >= theta)
                     flag_y = true; // e có thể thêm vào Y
               }
               // T rỗng

               if (flag_x == false && flag_y == false)
                  continue;  // T rỗng
               bool flag_xy; // đánh dấu để xem xet e cho vao X or Y
               if (flag_x == true && flag_y == true)
               {
                  // chọn X
                  if (thetaX > thetaY)
                     flag_xy = true;
                  else // chọn Y
                     flag_xy = false;
               }
               else
               {
                  if (flag_x)
                     flag_xy = true;
                  if (flag_y)
                     flag_xy = false;
               }
               if (flag_xy) // e được thêm vào X
               {
                  X.push_back(e);
                  cX += g.adjList[e].wht;
                  current_fx = tmp_fx;
                  checkX[e] = true;
               }
               else // e được thêm vào Y
               {
                  Y.push_back(e);
                  cY += g.adjList[e].wht;
                  current_fy = tmp_fy;
                  checkY[e] = true;
               }
            }
            /*else
            {
               // e ko nam trong X
               if (checkX[e] == false)
               {
                  if (cX + g.adjList[e].wht <= B)
                  {
                     if(X.size()==myF_X[e].size_s)
                     {
                        tmp_fx=myF_X[e].f;
                     }
                     else{
                        X.push_back(e);
                        tmp_fx = compute_valSet(nEvals, g, X);
                        X.pop_back();
                        myF_X[e].f=tmp_fx;
                        myF_X[e].size_s=X.size();
                     }

                     if (tmp_fx > current_fz)
                     {
                        Z = X;
                        Z.push_back(e);
                        current_fz = tmp_fx;
                     }
                  }
               }
               else
               {
                  if (cY + g.adjList[e].wht <= B)
                  {
                     if(Y.size()==myF_Y[e].size_s)
                     {
                        tmp_fy=myF_Y[e].f;
                     }else
                     {
                        Y.push_back(e);
                        tmp_fy = compute_valSet(nEvals, g, Y);
                        Y.pop_back();
                        myF_Y[e].f=tmp_fy;
                        myF_Y[e].size_s=Y.size();
                     }
                     if (tmp_fy > current_fy)
                     {
                        Z = Y;
                        Z.push_back(e);
                        current_fz = tmp_fy;
                     }
                  }
               }
            }*/
         }
         theta = (1 - epsi) * theta;
      }
      // cout << "ALG2," << B << "," << max(current_fx, current_fy) << "," << nEvals <<", "<<X.size()<<","<<Y.size()<<endl;

      int i = 0, j = 0;
      double c_Xi = 0.0, c_Yj = 0.0;
      vector<node_id> Xi;
      vector<node_id> Yj;
      c_Xi = 0;
      c_Yj = 0;
      vector<bool> checkXi(g.n, false);
      vector<bool> checkYj(g.n, false);
      double f_max_XiYj = 0, tmp_f_XiYj = 0;

      while (l <= B)
      {
         bool flag_x = false; // đánh dấu xem Xi có thay đổi kích thước hay không
         while (i < X.size())
         {
            if (c_Xi + g.adjList[X[i]].wht > l)
               break;

            checkXi[X[i]] = true;
            Xi.push_back(X[i]);
            c_Xi += g.adjList[X[i]].wht;
            i++;
            flag_x = true;
         }

         bool flag_y = false; // đánh dấu xem Xi có thay đổi kích thước hay không
         while (j < Y.size())
         {
            if (c_Yj + g.adjList[Y[j]].wht > l)
               break;

            checkYj[Y[j]] = true;
            Yj.push_back(Y[j]);
            c_Yj += g.adjList[Y[j]].wht;
            j++;
            flag_y = true;
         }

         for (node_id e = 0; e < g.n; e++)
         {
            if (checkXi[e] == false && flag_x && Xi.size() > 0)
            {
               if (c_Xi + g.adjList[e].wht <= B)
               {
                  if (F_X[Xi.size()][e] != -1)
                  {
                     tmp_f_XiYj = F_X[Xi.size()][e];
                  }
                  else
                  {
                     Xi.push_back(e);
                     tmp_f_XiYj = compute_valSet(nEvals, g, Xi);
                     Xi.pop_back();
                     F_X[Xi.size()][e] = tmp_f_XiYj;
                  }
                  if (tmp_f_XiYj > f_max_XiYj)
                     f_max_XiYj = tmp_f_XiYj;
               }
            }
            if (checkYj[e] == false && flag_y && Yj.size() > 0)
            {
               if (c_Yj + g.adjList[e].wht <= B)
               {
                  if (F_Y[Yj.size()][e] != -1)
                  {
                     tmp_f_XiYj = F_Y[Yj.size()][e];
                  }
                  else
                  {
                     Yj.push_back(e);
                     tmp_f_XiYj = compute_valSet(nEvals, g, Yj);
                     Yj.pop_back();
                     F_Y[Yj.size()][e] = tmp_f_XiYj;
                  }
                  if (tmp_f_XiYj > f_max_XiYj)
                     f_max_XiYj = tmp_f_XiYj;
               }
            }
         }
         // cout << "ALG2: "<< f_max_XiYj << "," << nEvals <<", "<<Xi.size()<<","<<Yj.size()<<endl;
         l = l * (1 + e2);
      }
      solVal = max(current_fx, max(current_fy, solVal));
      solVal = max(solVal, f_max_XiYj);
      cout << "ALG2," << B << "," << solVal << "," << nEvals;
      return solVal;
   }
};
class Alg3ACC
{
   random_device rd;
   mt19937 gen;
   Args &myArgs;
   size_t k;
   tinyGraph &g;
   size_t nEvals = 0;
   double epsi;
   double B;
   vector<myWeight> A;
   double tau, theta;
   vector<vector<double>> F_X; // lưu hàm f đã được tính
   vector<vector<double>> F_Y; // lưu hàm f đã được tính
public:
   Alg3ACC(Args &args) : gen(rd()), myArgs(args), g(args.g)
   {
      k = args.k;
      epsi = args.epsi;
      B = args.buget * g.total_cost;
      // cout<<g.total_cost<<endl;
   }
   double run1()
   {
      uniform_real_distribution<double> dist(0.0, 1.0);
      double p = sqrt(2) - 1;
      double alpha = sqrt(2 + sqrt(2));
      double cost = 0;
      double solVal = 0;
      vector<node_id> S;
      double B2 = B / 2;
      double Emax = 0;
      double current_f = 0;
      vector<double> V1;
      // khởi tạo
      for (node_id u = 0; u < g.n; u++)
      {
         vector<node_id> tmp;
         tmp.push_back(u);
         double tmp_f = compute_valSet(nEvals, g, tmp);
         myWeight a;
         a.u = u;
         a.w[1]=tmp_f / g.adjList[u].wht;
         a.w[2]=tmp_f;
         a.frequency = 0;
         a.s_len = 0;
         a.check = true;
         A.push_back(a);
         F_X[0][u] = tmp_f;
         F_Y[0][u] = tmp_f;
         if (tmp_f > Emax) // tìm Emax
            Emax = tmp_f;
      }
      vector<node_id> VP;
      for (node_id i = 0; i < g.n; i++)
      {
         if (g.adjList[i].wht > B2)
            continue;
         double rand = dist(gen);
         if (rand >= p)
         {
            VP.push_back(i);
         }
      }
      for (int i = 0; i < VP.size(); i++)
      {
         node_id e = VP[i];
         if (cost + g.adjList[e].wht <= B && A[e].w[1] >= current_f / B)
         {
            S.push_back(e);
            double tmp_f = compute_valSet(nEvals, g, S);
            S.pop_back();
            if ((tmp_f - current_f) / g.adjList[e].wht >= alpha * current_f / B)
            {
               S.push_back(e);
               current_f = tmp_f;
               cost += g.adjList[e].wht;
            }
         }
      }
      return max(current_f, Emax);
   }
   void run()
   {
      nEvals = 0;
      double e2 = epsi / 20;
      double solVal = 0;
      for (node_id u = 0; u < g.n; u++)
      {
         vector<double> tmpf(g.n, -1.0);
         F_X.push_back(tmpf);
         F_Y.push_back(tmpf);
      }
      tau = run1();
      solVal = tau;
      vector<node_id> X;
      vector<node_id> Y;
      double cost_x = 0, cost_y = 0;                         // cost
      double current_fx = 0, current_fy = 0; // current f
      double tmp_fx = 0.0, tmp_fy = 0;
      theta = 19 * tau / (6 * e2 * B);
      double nguong = (tau * (1 - e2)) / (6 * B);

      while (theta > nguong)
      {
         while(true)
         {
            std::sort(A.begin(), A.end(), greater1());
            int index1=-1;
            for(int i=0;i<A.size();i++)
            {
               if(A[i].check==false) continue;
               index1=i;
               break;
            }
            if(index1==-1) break;//Không còn phần tử nào break giảm ngưỡng
            if (A[index1].w[1] < theta) break; // nếu phần tử lớn nhất mà vẫn bé hơn ngưỡng thì break giảm ngưỡng

            node_id e = A[index1].u;
            if(cost_x + g.adjList[e].wht > B && cost_y + g.adjList[e].wht > B)
            {
               A[index1].check=false;
               continue;
            }
            //xet tap X
            tmp_fx=-1; tmp_fy=-1;
            bool danhDau1=false,danhDau2=false;
            if (cost_x + g.adjList[e].wht <= B)
            {
               danhDau1=true;
               if (F_X[X.size()][e] != -1) // kiểm tra f(S|e) được tính chưa
                  tmp_fx = F_X[X.size()][e];
               else
               {
                  X.push_back(e);
                  tmp_fx = compute_valSet(nEvals, g, X);
                  X.pop_back();
                  F_X[X.size()][e] = tmp_fx;
               }
            } // bỏ phần tử này ra khỏi hàng đợi luôn

            if (cost_y + g.adjList[e].wht <= B)
            {
               danhDau2=true;
               if (F_Y[Y.size()][e] != -1) // kiểm tra f(S|e) được tính chưa
                  tmp_fy = F_Y[Y.size()][e];
               else
               {
                  Y.push_back(e);
                  tmp_fy = compute_valSet(nEvals, g, Y);
                  Y.pop_back();
                  F_Y[Y.size()][e] = tmp_fy;
               }
            } // bỏ phần tử này ra khỏi hàng đợi luôn
            bool danhDau_x;
            if(danhDau1==true && danhDau2==true)
            {
               if(tmp_fx>tmp_fy)
                  danhDau_x=true;
               else
                  danhDau_x=false;
            }else
            {
               if(danhDau1==true)
                  danhDau_x=true;
               else
                  danhDau_x=false;
            }

            if(danhDau_x==true)//neu la X duoc chon
            {
               double delta=(tmp_fx-current_fx)/g.adjList[e].wht;
               if(delta>=theta)
               {
                  X.push_back(e);
                  cost_x += g.adjList[e].wht;
                  current_fx=tmp_fx;
                  A[index1].check=false;

                  std::sort(A.begin(), A.end(), greater2());
                  int index2=-1;
                  for(int i2=0;i2<A.size();i2++)
                  {
                     if(A[i2].check==false || cost_x+g.adjList[A[i2].u].wht > B) continue;
                     index2=i2;
                     break;
                  }
                  if (index2 != -1)
                  {
                     double tmpf;
                     node_id e2=A[index2].u;
                     if (F_X[X.size()][e2] != -1) // kiểm tra f(S|e) được tính chưa
                        tmpf = F_X[X.size()][e2];
                     else
                     {
                        X.push_back(e2);
                        tmpf = compute_valSet(nEvals, g, X);
                        X.pop_back();
                     }
                     solVal=max(tmpf,solVal);
                  }
               }else
               {
                  A[index1].w[1]=delta;
                  A[index1].w[2]=tmp_fx - current_fx;
               }
            }else//neu la Y duoc chon
            {
               double delta=(tmp_fy-current_fy)/g.adjList[e].wht;
               if(delta>=theta)
               {
                  Y.push_back(e);
                  cost_y += g.adjList[e].wht;
                  current_fy=tmp_fy;
                  A[index1].check=false;

                  std::sort(A.begin(), A.end(), greater2());
                  int index2=-1;
                  for(int i2=0;i2<A.size();i2++)
                  {
                     if(A[i2].check==false || cost_y+g.adjList[A[i2].u].wht > B) continue;
                     index2=i2;
                     break;
                  }
                  if (index2 != -1)
                  {
                     double tmpf;
                     node_id e2=A[index2].u;
                     if (F_Y[Y.size()][e2] != -1) // kiểm tra f(S|e) được tính chưa
                        tmpf = F_Y[Y.size()][e2];
                     else
                     {
                        Y.push_back(e2);
                        tmpf = compute_valSet(nEvals, g, Y);
                        Y.pop_back();
                     }
                     solVal=max(tmpf,solVal);
                  }
               }else
               {
                  A[index1].w[1]=delta;
                  A[index1].w[2]=tmp_fy - current_fy;
               }
            }           
         }
         theta = (1 - e2) * theta;
      }
      solVal = max(current_fx, max(current_fy, solVal));
      cout << "ALG2ACC," << B << "," << solVal << "," << nEvals<<","<<max(cost_x,cost_y);
   }
};

class Alg4
{
   random_device rd;
   mt19937 gen;
   Args &myArgs;
   size_t k;
   tinyGraph &g;
   size_t nEvals = 0;
   double epsi;
   double B;
   double tau, theta;

public:
   Alg4(Args &args) : gen(rd()), myArgs(args), g(args.g)
   {
      k = args.k;
      epsi = args.epsi;
      B = args.buget * g.total_cost;
   }

   // thuật toán 1
   size_t run1()
   {
      double solVal = 0;
      vector<node_id> X;
      vector<node_id> Y;
      double B2 = B / 2;
      double Emax = 0;
      double current_fx = 0, current_fy = 0;
      for (node_id e = 0; e < g.n; e++)
      {
         // tìm 1 e max
         vector<node_id> tmp;
         tmp.push_back(e);
         double eVal = compute_valSet(nEvals, g, tmp);
         if (compute_valSet(nEvals, g, tmp) > Emax)
            Emax = eVal;

         if (g.adjList[e].wht <= B2)
         {
            bool flag_x = false;
            bool flag_y = false;

            // X
            X.push_back(e);
            double tmp_fx = compute_valSet(nEvals, g, X);
            X.pop_back();
            if (((tmp_fx - current_fx) / g.adjList[e].wht) >= current_fx / B)
            {
               flag_x = true;
            }
            // Y

            Y.push_back(e);
            double tmp_fy = compute_valSet(nEvals, g, Y);
            Y.pop_back();

            if (((tmp_fy - current_fy) / g.adjList[e].wht) >= current_fy / B)
            {
               flag_y = true;
            }
            //
            if (flag_x && flag_y)
            {
               if ((tmp_fx - current_fx) > (tmp_fy - current_fy))
               {
                  X.push_back(e);
                  current_fx = tmp_fx;
               }
               else
               {
                  Y.push_back(e);
                  current_fy = tmp_fy;
               }
            }
            else
            {
               if (flag_x)
               {
                  X.push_back(e);
                  current_fx = tmp_fx;
               }
               if (flag_y)
               {
                  Y.push_back(e);
                  current_fy = tmp_fy;
               }
            }
         }
      }

      vector<node_id> X2;
      vector<node_id> Y2;
      double tmpB = 0;
      for (int i = X.size() - 1; i > 0; i--)
      {
         if (tmpB + g.adjList[X[i]].wht <= B)
         {
            X2.push_back(X[i]);
            tmpB += g.adjList[X[i]].wht;
         }
         else
            break;
      }
      tmpB = 0;
      for (int i = Y.size() - 1; i > 0; i--)
      {
         if (tmpB + g.adjList[Y[i]].wht <= B)
         {
            Y2.push_back(Y[i]);
            tmpB += g.adjList[Y[i]].wht;
         }
         else
            break;
      }
      double fx = compute_valSet(nEvals, g, X2);
      double fy = compute_valSet(nEvals, g, Y2);
      solVal = max(max(fx, fy), Emax);
      return solVal;
   }
   // thuật toán chính
   size_t run()
   {
      uniform_real_distribution<double> dist(0.0, 1.0);
      double solVal = 0;
      // B = myArgs.buget;
      tau = run1();
      double epsi2 = epsi / 5;
      solVal = tau;
      // cout<<"ALg1: "<<solVal<<endl;
      //
      int j = 0;
      vector<node_id> S;
      vector<bool> U(g.n, false);
      theta = 19 * tau / (4 * epsi2 * B);
      double nguong = (tau * (1 - epsi2)) / (4 * B);
      double cost_s = 0;
      double current_f = 0, tmp_f = 0;
      vector<pair<int, double>> save_f(g.n, make_pair(-1, 0));
      while (theta > nguong)
      {
         for (node_id e = 0; e < g.n; e++)
         {
            if (U[e] == true)
               continue;
            if (cost_s + g.adjList[e].wht <= B && g.adjList[e].wht <= (1 - epsi2) * B)
            {
               if (save_f[e].first == S.size())
               {
                  tmp_f = save_f[e].second;
               }
               else // neu f(s+u) chưa được tính
               {
                  S.push_back(e);
                  tmp_f = compute_valSet(nEvals, g, S);
                  S.pop_back();
                  save_f[e].first = S.size(); // lưu giá trị lại để lần sau dùng
                  save_f[e].second = tmp_f;
               }
               double delta = (tmp_f - current_f) / g.adjList[e].wht;
               if (delta >= theta)
               {
                  U[e] = true;
                  double rand = dist(gen);
                  if (rand > 0.5)
                  {
                     current_f = tmp_f;
                     S.push_back(e);
                     cost_s += g.adjList[e].wht;
                  }
               }
            }
         }
         theta = (1 - epsi2) * theta;
      }
      // kiểm tra xem f hiện tại có lớn hơn sol1 không
      // cout<<"Ket thuc vong lap 1: "<<current_f<<endl;
      if (current_f > solVal)
         solVal = current_f;

      double l = epsi2 * B;
      int i = 0;
      double c_Xi = 0.0;
      vector<node_id> Xi;
      vector<bool> checkXi(g.n, false);
      double f_max_Xi = 0, tmp_f_Xi = 0;

      while (l <= B)
      {
         int i_tmp = i;
         while (i < S.size())
         {
            if (c_Xi + g.adjList[S[i]].wht > l)
               break;
            checkXi[S[i]] = true;
            Xi.push_back(S[i]);
            c_Xi += g.adjList[S[i]].wht;
            i++;
         }
         if (i_tmp == i) // nếu i không thay đổi thì không cần tính lại
         {
            l = l * (1 + epsi);
            continue;
         }
         for (node_id e = 0; e < g.n; e++)
         {
            if (checkXi[e] == false)
            {
               if (c_Xi + g.adjList[e].wht <= B)
               {
                  Xi.push_back(e);
                  double tmp = compute_valSet(nEvals, g, Xi);
                  Xi.pop_back();
                  if (tmp > solVal)
                     solVal = tmp;
               }
            }
         }
         // cout<<"l: "<<l<<" i: "<<i<<" sol: "<<solVal<<endl;
         l = l * (1 + epsi);
      }
      cout << "ALG3," << B << "," << solVal << "," << nEvals;
      return solVal;
   }
};

class Alg4Acc
{
   random_device rd;
   mt19937 gen;
   Args &myArgs;
   size_t k;
   tinyGraph &g;
   size_t nEvals = 0;
   double epsi;
   double B;
   double tau, theta;
   vector<myWeight> A;
   vector<bool> check_u;
   vector<bool> check_s;
   vector<node_id> S;
   double cost,current_f=0;
   vector<vector<double>> FS; // lưu hàm f đã được tính

public:
   Alg4Acc(Args &args) : gen(rd()), myArgs(args), g(args.g)
   {
      k = args.k;
      epsi = args.epsi;
      B = args.buget * g.total_cost;
   }
   // thuật toán 1
   double run1()
   {
      uniform_real_distribution<double> dist(0.0, 1.0);
      double p = sqrt(2) - 1;
      double alpha = sqrt(2 + sqrt(2));
      double cost = 0;
      double solVal = 0;
      vector<node_id> S;
      double B2 = B / 2;
      double Emax = 0;
      double current_f = 0;
      vector<double> V1;
      // khởi tạo
      for (node_id u = 0; u < g.n; u++)
      {
         vector<node_id> tmp;
         tmp.push_back(u);
         double tmp_f = compute_valSet(nEvals, g, tmp);
         myWeight a;
         a.u = u;
         a.w[1]=tmp_f / g.adjList[u].wht;
         a.w[2]=tmp_f;
         a.frequency = 0;
         a.s_len = 0;
         a.check = true;
         A.push_back(a);
         FS[0][u] = tmp_f;
         if (tmp_f > Emax) // tìm Emax
            Emax = tmp_f;
      }
      vector<node_id> VP;
      for (node_id i = 0; i < g.n; i++)
      {
         if (g.adjList[i].wht > B2)
            continue;
         double rand = dist(gen);
         if (rand >= p)
         {
            VP.push_back(i);
         }
      }
      for (int i = 0; i < VP.size(); i++)
      {
         node_id e = VP[i];
         if (cost + g.adjList[e].wht <= B && A[e].w[1] >= current_f / B)
         {
            S.push_back(e);
            double tmp_f = compute_valSet(nEvals, g, S);
            S.pop_back();
            if ((tmp_f - current_f) / g.adjList[e].wht >= alpha * current_f / B)
            {
               S.push_back(e);
               current_f = tmp_f;
               cost += g.adjList[e].wht;
            }
         }
      }
      return max(current_f, Emax);
   }
   // thuật toán chính
   size_t run()
   { 
      uniform_real_distribution<double> dist(0.0, 1.0);
      double solVal = 0, epsi2 = epsi / 5, delta;
      nEvals = 0;
      for (node_id u = 0; u < g.n; u++)
      {
         vector<double> tmpf(g.n, -1.0);
         FS.push_back(tmpf);
      }
      tau = run1();
      solVal = tau;
      int j = 0;
      theta = 16.037 * tau / (4 * epsi2 * B);
      double nguong = (tau * (1 - epsi2)) / (10 * B);
      double tmp_f = 0;
      
      while (theta > nguong)
      {
         while(true)
         {
            std::sort(A.begin(), A.end(), greater1());
            int index1=-1;
            for(int i=0;i<A.size();i++)
            {
               if(A[i].check==false) continue;
               index1=i;
               break;
            }
            if(index1==-1) break;//Không còn phần tử nào break giảm ngưỡng
            if (A[index1].w[1] < theta) break; // nếu phần tử lớn nhất mà vẫn bé hơn ngưỡng thì break giảm ngưỡng

            node_id e = A[index1].u;
            if (cost + g.adjList[e].wht > B || g.adjList[e].wht > (1 - epsi2) * B)
            {
               A[index1].check=false;
               continue;
            }//bỏ phần tử này ra khỏi hàng đợi luôn

            if (FS[S.size()][e] != -1) // kiểm tra f(S|e) được tính chưa
               tmp_f = FS[S.size()][e];
            else
            {
               S.push_back(e);
               tmp_f = compute_valSet(nEvals, g, S);
               S.pop_back();
               FS[S.size()][e] = tmp_f;
            }
            delta = (tmp_f - current_f) / g.adjList[e].wht;
            if (delta >= theta) // nếu lớn hơn ngưỡng thì
            {
               A[index1].check=false;
               double rand = dist(gen);
               if (rand >= 0.5) // ngẫu nhiên cho vào S
               {
                  current_f = tmp_f;
                  S.push_back(e);
                  cost += g.adjList[e].wht;

                  std::sort(A.begin(), A.end(), greater2());
                  int index2=-1;
                  for(int i2=0;i2<A.size();i2++)
                  {
                     if(A[i2].check==false || cost+g.adjList[A[i2].u].wht > B) continue;
                     index2=i2;
                     break;
                  }
                  if (index2 != -1)
                  {
                     node_id e2=A[index2].u;
                     if (FS[S.size()][e2] != -1) // kiểm tra f(S|e) được tính chưa
                        tmp_f = FS[S.size()][e2];
                     else
                     {
                        S.push_back(e2);
                        tmp_f = compute_valSet(nEvals, g, S);
                        S.pop_back();
                     }
                     solVal=max(tmp_f,solVal);
                  }
               }
            }
            else // nếu delta bé hơn ngưỡng thì ném lại vào Queue và cập nhật lại theta
            {
               A[index1].w[1]=delta;
               A[index1].w[2]=tmp_f - current_f;
            }
         }
         theta = (1 - epsi2) * theta;
      }
      solVal=max(current_f,solVal);

      double l = epsi2 * B;
      int i = 0;
      double c_Xi = 0.0;
      vector<node_id> Xi;
      vector<bool> checkXi(g.n, false);
      double f_max_Xi = 0, tmp_f_Xi = 0;
      cout << "ALG3," << B << "," << solVal << "," << nEvals<<","<<cost;
      return solVal;
   }
};

class SmkDet
{
   random_device rd;
   mt19937 gen;
   Args &myArgs;
   size_t k;
   tinyGraph &g;
   size_t nEvals = 0;
   double epsi;
   double B;
   double alpha;
   double tau;

public:
   SmkDet(Args &args) : gen(rd()), myArgs(args), g(args.g)
   {
      k = args.k;
      epsi = args.epsi;
      B = args.buget * g.total_cost;
   }
   size_t run()
   {
      nEvals = 0;
      double solVal = 0;
      vector<node_id> tmp;
      vector<vector<node_id>> S(4, tmp);
      vector<double> cost(4, 0);
      vector<double> current_f(4, 0);
      vector<bool> check1(g.n, false);
      vector<vector<bool>> check(4, check1);

      while (true)
      {
         vector<double> a1(3, -1);
         vector<double> delta_max(3, 0);
         vector<double> tmp_f(3, 0);
         for (int i = 1; i <= 2; i++)
         {
            double max1 = 0, max2 = 0;
            double max_f1 = 0, max_f2 = 0;
            node_id a2 = -1;
            for (node_id e = 0; e < g.n; e++)
            {

               if (check[1][e] == false && check[2][e] == false && cost[i] + g.adjList[e].wht <= B)
               {
                  S[i].push_back(e);
                  double tmp = compute_valSet(nEvals, g, S[i]);
                  S[i].pop_back();
                  double delta = (tmp - current_f[i]) / g.adjList[e].wht;
                  if (delta > max1)
                  {
                     max1 = delta;
                     tmp_f[i] = tmp;
                     a1[i] = e;
                     delta_max[i] = max1;
                     max_f1 = tmp;
                  }
               }

               if (check[i][e] == false && cost[i] + g.adjList[e].wht <= B)
               {
                  S[i].push_back(e);
                  double tmp = compute_valSet(nEvals, g, S[i]);
                  S[i].pop_back();
                  double delta = tmp - current_f[i];
                  if (delta > max2)
                  {
                     max2 = delta;
                     a2 = e;
                     max_f2 = tmp;
                  }
               }
            }
            if (a2 != -1 && max_f2 > current_f[3])
            {
               current_f[3] = max_f2;
               S[3] = S[i];
               cost[3] = cost[i] + g.adjList[a2].wht;
               S[3].push_back(a2);
            }
         }
         // cout<<"a1[1]: "<<a1[1]<<" a1[2]: "<<a1[2]<<" delta_max[1]:"<<delta_max[1]<<" delta_max[2]:"<<delta_max[2]<<" tmp_f[1]:"<<tmp_f[1]<<" tmp_f[2]:"<<tmp_f[2]<<endl;
         if (a1[1] != 1 && a1[2] == -1)
         {
            if (delta_max[1] > 0)
            {
               S[1].push_back(a1[1]);
               cost[1] += g.adjList[a1[1]].wht;
               check[1][a1[1]] = true;
               current_f[1] = tmp_f[1];
            }
            else
               break;
         }
         else if (a1[1] == -1 && a1[2] != -1)
         {
            if (delta_max[2] > 0)
            {
               S[2].push_back(a1[2]);
               cost[2] += g.adjList[a1[2]].wht;
               check[2][a1[2]] = true;
               current_f[2] = tmp_f[2];
            }
            else
               break;
         }
         else if (a1[1] != -1 && a1[2] != -1)
         {
            if (delta_max[1] < 0 && delta_max[2] < 0)
               break;
            else
            {
               if (delta_max[1] > delta_max[2])
               {
                  S[1].push_back(a1[1]);
                  cost[1] += g.adjList[a1[1]].wht;
                  check[1][a1[1]] = true;
                  current_f[1] = tmp_f[1];
               }
               else
               {
                  S[2].push_back(a1[2]);
                  cost[2] += g.adjList[a1[2]].wht;
                  check[2][a1[2]] = true;
                  current_f[2] = tmp_f[2];
               }
            }
         }
         else
         {
            break;
         }
      }
      double s1 = compute_valSet(nEvals, g, S[1]);
      double s2 = compute_valSet(nEvals, g, S[2]);
      double s3 = compute_valSet(nEvals, g, S[3]);
      double m = max(s1, max(s2, s3));
      cout << "SMKDET," << B << "," << m << "," << nEvals;
      return m;
   }
};

class SmkDetAcc
{
   random_device rd;
   mt19937 gen;
   Args &myArgs;
   size_t k;
   tinyGraph &g;
   size_t nEvals = 0;
   double epsi;
   double B;
   double alpha;
   double tau;
   vector<vector<node_id>> D;
   vector<vector<myWeight>> A;
   vector<vector<node_id>> S;
   vector<double> current_f;
   vector<double> cost;
   vector<vector<bool>> check;

public:
   SmkDetAcc(Args &args) : gen(rd()), myArgs(args), g(args.g)
   {
      k = args.k;
      epsi = args.epsi;
      B = args.buget * g.total_cost;
   }
   void GetMax(int m, myWeight &a1, myWeight &a2)
   {
      // cout<<"--------------START GETMAX--------------------"<<endl;
      myWeight tmp;
      tmp.u = 4294967295;
      tmp.w.push_back(0);
      tmp.w.push_back(0);
      tmp.w.push_back(0);
      vector<myWeight> a;
      a.push_back(tmp);
      a.push_back(tmp);
      a.push_back(tmp);

      double k = g.n;
      double e2 = epsi / 20.0;

      for (int j = 1; j <= 2; j++)
      {
         if (j == 1)
            std::sort(A[m].begin(), A[m].end(), greater1());
         else
            std::sort(A[m].begin(), A[m].end(), greater2());
         double maxW = 0;

         for (int i = 0; i < A[m].size(); i++)
         {
            if (A[m][i].check == false)
               continue;
            if (j == 1)
            {
               if (check[1][A[m][i].u] == true || check[2][A[m][i].u] == true || cost[m] + g.adjList[A[m][i].u].wht > B)
                  continue;
            }
            else
            {
               if (check[m][A[m][i].u] == true || cost[m] + g.adjList[A[m][i].u].wht > B)
                  continue;
            }

            double oldWeight = A[m][i].w[j];
            if (maxW >= oldWeight)
               break;
            if (A[m][i].s_len != S[j].size())
            {
               S[m].push_back(A[m][i].u);
               double tmp_f = compute_valSet(nEvals, g, S[m]);
               S[m].pop_back();

               A[m][i].w[1] = (tmp_f - current_f[m]) / g.adjList[A[m][i].u].wht;
               A[m][i].w[2] = (tmp_f - current_f[m]);
               A[m][i].s_len = S[j].size();
               // cout<<"S["<<j<<"].size():"<<S[j].size()<<" A[m][i].s_len:"<<A[m][i].s_len<<endl;
               if (A[m][i].w[j] > oldWeight / (1 + e2))
               {
                  a[j] = A[m][i];
                  // cout<<"Found u:"<<a[j].u<<endl;
                  break;
               }
               else
               {
                  A[m][i].frequency = A[m][i].frequency + 1;
               }
            }
            if (A[m][i].w[j] > maxW)
            {
               maxW = A[m][i].w[j];
               a[j] = A[m][i];
               // cout<<"Found u_max_"<<j<<":"<<a[j].u<<endl;
            }
         }
      }
      double thres = log2(k / e2) / log2(1 + e2);
      for (int i = 0; i < A[m].size(); i++)
      {
         if (A[m][i].frequency > thres)
            A[m][i].check = false;
      }
      a1 = a[1];
      a2 = a[2];
      // cout<<"a1,a2: "<<a1.u<<" "<<a2.u<<" | "<<a[1].u<<" "<<a[2].u<<endl;
      // cout<<"--------------END GETMAX--------------------"<<endl<<endl;
   }
   size_t run()
   {
      nEvals = 0;
      double solVal = 0;
      vector<node_id> tmp;
      S = vector<vector<node_id>>(4, tmp);
      current_f = vector<double>(4, 0);
      cost = vector<double>(4, 0);
      vector<bool> check1(g.n, false);
      check = vector<vector<bool>>(4, check1);

      for (int i = 0; i <= 2; i++)
      {
         vector<myWeight> tmp;
         A.push_back(tmp);
      }
      for (int i = 1; i <= 2; i++)
      {
         for (node_id u = 0; u < g.n; u++)
         {
            vector<node_id> tmp;
            tmp.push_back(u);
            double tmp_f = compute_valSet(nEvals, g, tmp);
            myWeight a;
            a.u = u;
            a.w[1]=tmp_f / g.adjList[u].wht;
            a.w[2]=tmp_f;
            a.frequency = 0;
            a.s_len = 0;
            a.check = true;
            A[i].push_back(a);
         }
      }

      while (true)
      {
         myWeight tmp;
         tmp.w.push_back(0);
         tmp.w.push_back(0);
         tmp.w.push_back(0);
         vector<myWeight> a1;
         a1.push_back(tmp);
         a1.push_back(tmp);
         a1.push_back(tmp);

         for (int i = 1; i <= 2; i++)
         {
            myWeight tmpa1;
            tmpa1.u = 4294967295;
            myWeight tmpa2;
            tmpa2.u = 4294967295;
            GetMax(i, tmpa1, tmpa2);
            a1[i].u = tmpa1.u;
            a1[i].w = tmpa1.w;
            if (tmpa2.u != 4294967295 && tmpa2.w[2] > current_f[3] && cost[i] + g.adjList[tmpa2.u].wht < B)
            {
               current_f[3] = tmpa2.w[2];
               S[3] = S[i];
               cost[3] = cost[i] + g.adjList[tmpa2.u].wht;
               S[3].push_back(tmpa2.u);
            }
         }
         if (a1[1].u != 4294967295 && a1[2].u == 4294967295)
         {
            if (a1[1].w[1] > 0)
            {
               S[1].push_back(a1[1].u);
               cost[1] += g.adjList[a1[1].u].wht;
               check[1][a1[1].u] = true;
               current_f[1] = a1[1].w[2];
            }
            else
               break;
         }
         if (a1[1].u == 4294967295 && a1[2].u != 4294967295)
         {
            if (a1[2].w[1] > 0)
            {
               S[2].push_back(a1[2].u);
               cost[2] += g.adjList[a1[2].u].wht;
               check[2][a1[2].u] = true;
               current_f[2] = a1[2].w[2];
            }
            else
               break;
         }
         if (a1[1].u != 4294967295 && a1[2].u != 4294967295)
         {
            if (a1[1].w[1] < 0 && a1[2].w[1] < 0)
               break;
            else
            {
               if (a1[1].w[1] > a1[2].w[1])
               {
                  S[1].push_back(a1[1].u);
                  cost[1] += g.adjList[a1[1].u].wht;
                  check[1][a1[1].u] = true;
                  current_f[1] = a1[1].w[2];
               }
               else
               {
                  S[2].push_back(a1[2].u);
                  cost[2] += g.adjList[a1[2].u].wht;
                  check[2][a1[2].u] = true;
                  current_f[2] = a1[2].w[2];
               }
            }
         }
         if (a1[1].u == 4294967295 && a1[2].u == 4294967295)
            break;
      }

      double s1 = compute_valSet(nEvals, g, S[1]);
      double s2 = compute_valSet(nEvals, g, S[2]);
      double s3 = compute_valSet(nEvals, g, S[3]);
      double m = max(s1, max(s2, s3));
      cout << "SMKDETACC," << B << "," << m << "," << nEvals<<","<<max(cost[1],max(cost[2],cost[3]));
      return m;
   }
};

class SmkRan
{
   random_device rd;
   mt19937 gen;
   Args &myArgs;
   size_t k;
   tinyGraph &g;
   size_t nEvals = 0;
   double epsi;
   double B;
   double alpha;
   double tau;

public:
   SmkRan(Args &args) : gen(rd()), myArgs(args), g(args.g)
   {
      k = args.k;
      epsi = args.epsi;
      B = args.buget * g.total_cost;
   }

   size_t run()
   {
      uniform_real_distribution<double> dist(0.0, 1.0);
      nEvals = 0;
      vector<node_id> S;
      vector<node_id> S_1;
      double cost = 0;
      int j = 0;
      vector<bool> check_u(g.n, false);
      vector<bool> check_s(g.n, false);
      double current_f = 0;
      double max_f_sao = 0;
      while (true)
      {
         node_id u = 4294967295, u2 = 4294967295;
         double max_delta = 0, max_f = 0;
         double max_f2 = 0;
         for (node_id e = 0; e < g.n; e++)
         {
            if (check_u[e] == false && cost + g.adjList[e].wht <= B)
            {
               S.push_back(e);
               double tmp = compute_valSet(nEvals, g, S);
               S.pop_back();

               double delta = (tmp - current_f) / g.adjList[e].wht;
               if (delta > max_delta)
               {
                  max_delta = delta;
                  max_f = tmp;
                  u = e;
               }
            }

            if (check_s[e] == false && cost + g.adjList[e].wht <= B)
            {
               S.push_back(e);
               double tmp = compute_valSet(nEvals, g, S);
               S.pop_back();
               if (tmp > max_f2)
               {
                  u2 = e;
                  max_f2 = tmp;
               }
            }
         }

         if (u2 != 4294967295 && max_f2 > current_f)
         {
            max_f_sao = max_f2;
         }

         if (u != 4294967295)
            check_u[u] = true;
         if (u != 4294967295 && max_delta > 0)
         {
            double rand = dist(gen);
            if (rand > 0.5)
            {
               S.push_back(u);
               check_s[u] = true;
               cost += g.adjList[u].wht;
               current_f = max_f;
            }
         }
         else
         {
            break;
         }
      }
      double solVal = max(max_f_sao, current_f);
      cout << "SMKRAN," << B << "," << solVal << "," << nEvals;
      return solVal;
   }
};
class SmkRanAcc
{
   random_device rd;
   mt19937 gen;
   Args &myArgs;
   size_t k;
   tinyGraph &g;
   size_t nEvals = 0;
   double epsi;
   double B;
   double alpha;
   double tau;
   double current_f = 0;
   vector<myWeight> A;
   vector<node_id> S;
   vector<bool> check_u;
   vector<bool> check_s;
   double cost;
   vector<vector<bool>> DD;

public:
   SmkRanAcc(Args &args) : gen(rd()), myArgs(args), g(args.g)
   {
      k = args.k;
      epsi = args.epsi;
      B = args.buget * g.total_cost;
   }

   void GetMax(myWeight &a1, myWeight &a2)
   {
      // cout<<"-------------- GETMAX--------------------"<<endl<<endl;
      myWeight tmp;
      tmp.u = 4294967295;
      tmp.w.push_back(0);
      tmp.w.push_back(0);
      tmp.w.push_back(0);
      vector<myWeight> a;
      a.push_back(tmp);
      a.push_back(tmp);
      a.push_back(tmp);

      double k = g.n;
      double e2 = epsi / 20;

      for (int j = 1; j <= 2; j++)
      {
         if (j == 1)
            std::sort(A.begin(), A.end(), greater1());
         else
            std::sort(A.begin(), A.end(), greater2());
         double maxW = 0;

         for (int i = 0; i < A.size(); i++)
         {
            if (A[i].check == false)
               continue;
            if (j == 1)
            {
               if (check_u[A[i].u] == true || cost + g.adjList[A[i].u].wht > B)
                  continue;
            }
            else
            {
               if (check_s[A[i].u] == true || cost + g.adjList[A[i].u].wht > B)
                  continue;
            }

            double oldWeight = A[i].w[j];
            if (maxW >= oldWeight)
               break;
            if (DD[A[i].u][S.size()] == false)
            {
               S.push_back(A[i].u);
               double tmp_f = compute_valSet(nEvals, g, S);
               S.pop_back();

               A[i].w[1] = (tmp_f - current_f) / g.adjList[A[i].u].wht;
               A[i].w[2] = (tmp_f - current_f);
               DD[A[i].u][S.size()] = true;
               if (A[i].w[j] >= oldWeight/(1+e2))
               {
                  a[j] = A[i];
                  break;
               }
               else
               {
                  A[i].frequency = A[i].frequency + 1;
               }
            }

            if (A[i].w[j] > maxW)
            {
               maxW = A[i].w[j];
               a[j] = A[i];
            }
         }
      }
      double thres = log2(k / e2) / log2(1 + e2);
      for (int i = 0; i < A.size(); i++)
      {
         if (A[i].frequency > thres)
            A[i].check = false;
      }
      a1 = a[1];
      a2 = a[2];
   }

   size_t run()
   {
      uniform_real_distribution<double> dist(0.0, 1.0);
      nEvals = 0;

      check_u = vector<bool>(g.n, false);
      cost = 0;
      int j = 0;

      check_s = vector<bool>(g.n, false);

      double max_f_sao = 0;

      for (node_id u = 0; u < g.n; u++)
      {
         vector<bool> t(g.n + 1, false);
         DD.push_back(t);
      }
      //cout << "g.n:" << g.n << endl;
      for (node_id u = 0; u < g.n; u++)
      {
         vector<node_id> tmp;
         tmp.push_back(u);
         double tmp_f = compute_valSet(nEvals, g, tmp);
         //cout<<tmp_f<<endl;
         DD[u][0] = true;
         myWeight a;
         a.u = u;
         a.w[1]=tmp_f / g.adjList[u].wht;
         a.w[2]=tmp_f;
         a.frequency = 0;
         a.s_len = 0;
         a.check = true;
         A.push_back(a);
      }
      //cout << "Q1:" << nEvals << endl;
      while (true)
      {
         node_id u = 4294967295, u2 = 4294967295;
         myWeight tmp_a1, tmp_a2;
         GetMax(tmp_a1, tmp_a2);
         u = tmp_a1.u;
         u2 = tmp_a2.u;
         if (u2 != 4294967295 && cost + g.adjList[u2].wht<= B)
         {
            S.push_back(u2);
            double tmp_f = compute_valSet(nEvals, g, S);
            S.pop_back();
            if (tmp_f - current_f > 0 && tmp_f > max_f_sao)
               max_f_sao = tmp_f;
         }

         if (u != 4294967295 && cost + g.adjList[u].wht<= B)
         {
            check_u[u] = true;

            S.push_back(u);
            double tmp_f = compute_valSet(nEvals, g, S);
            S.pop_back();

            if (tmp_f - current_f > 0)
            {
               double rand = dist(gen);
               if (rand > 0.5)
               {
                  S.push_back(u);
                  check_s[u] = true;
                  cost += g.adjList[u].wht;
                  current_f = tmp_f;
               }
            }
            else
            {
               break;
            }
         }
         else
         {
            break;
         }
      }
      double solVal = max(max_f_sao, current_f);
      cout << "SMKRANACC," << B << "," << solVal << "," << nEvals<<","<<cost;
      return solVal;
   }
};

class SmkStream
{
   random_device rd;
   mt19937 gen;
   Args &myArgs;
   size_t k;
   tinyGraph &g;
   size_t nEvals = 0;
   double epsi;
   double B;
   double alpha, beta;
   double tau;
   double e2;
   int h = 2, r = 2;

public:
   SmkStream(Args &args) : gen(rd()), myArgs(args), g(args.g)
   {
      k = args.k;
      epsi = args.epsi;
      B = args.buget * g.total_cost;
      alpha = 1.0 / 6.0;
      beta = 1.0 / 6.0;
      e2 = epsi * beta;
   }

   vector<int> getC(double LB, double UP)
   {
      // cout<<"LB:"<<LB<<" UP:"<<UP<<endl;
      vector<int> C;
      int i = 0;
      while (true)
      {
         double tmp = pow((1 + e2), i);
         // cout<<"tmp:"<<tmp<<" i:"<<i<<endl;
         if (tmp < LB)
         {
            i++;
            continue;
         }
         if (tmp > UP)
            break;
         C.push_back(i);
         i++;
      }
      return C;
   }
   size_t run()
   {
      nEvals = 0;
      vector<node_id> S;
      vector<vector<vector<node_id>>> SIY;
      vector<vector<double>> SIY_f;
      vector<vector<double>> SIY_cost;
      vector<vector<bool>> SIY_check;
      for (int i = 0; i <= h; i++) // h
      {
         vector<vector<node_id>> tmp;
         vector<double> tmp_f;
         vector<double> tmp_c;
         vector<bool> tmp_b;
         SIY.push_back(tmp);
         SIY_f.push_back(tmp_f);
         SIY_cost.push_back(tmp_c);
         SIY_check.push_back(tmp_b);
      }
      // cout<<"Cai dat xong"<<endl;
      double LB = 0;
      for (node_id u = 0; u < g.n; u++)
      {
         if (g.adjList[u].wht > B)
            continue;

         vector<node_id> tmp;
         tmp.push_back(u);
         double tmp_f = compute_valSet(nEvals, g, tmp);
         if (tmp_f > LB)
         {
            LB = tmp_f;
            S = tmp;
         }

         // cout<<"tmp_f:"<<tmp_f<<" g.adjList[u].wht:"<<g.adjList[u].wht<<endl;
         vector<int> C = getC((tmp_f * alpha) / (B * (1 + e2)), tmp_f / g.adjList[u].wht);
         /*
         vector<int> C2=getC((alpha*LB)/(B*(1+e2)),LB);
         for(int i=1;i<=h;i++)
         {
            for(int j=0;j<SIY_check[i].size();j++)
            {
               SIY_check[i][j]=false;
            }
            for(int j:C2)
            {
               if(SIY_check[i].size()>j)
                  SIY_check[i][j]=true;
            }
         }
         //cout<<"Remove all"<<endl;
         */
         for (int y : C)
         {
            // cout<<"y:"<<y<<endl;
            for (int i = 1; i <= h; i++)
            {
               while (true)
               {
                  if (SIY[i].size() > y)
                     break;
                  vector<node_id> tmp;
                  SIY[i].push_back(tmp);
                  SIY_f[i].push_back(0);
                  SIY_cost[i].push_back(0);
                  SIY_check[i].push_back(true);
               }
            }
            // cout<<"Cai day Y"<<endl;

            double max = 0, max_f = 0;
            int x;
            for (int j = 1; j <= h; j++)
            {
               SIY[j][y].push_back(u);
               double tmp_f = compute_valSet(nEvals, g, SIY[j][y]);
               SIY[j][y].pop_back();

               double delta = tmp_f - SIY_f[j][y];
               if (delta > max)
               {
                  max = delta;
                  x = j;
                  max_f = tmp_f;
               }
            }

            if (SIY_cost[x][y] + g.adjList[u].wht <= B && max / g.adjList[u].wht >= pow(1 + e2, y))
            {
               SIY[x][y].push_back(u);
               SIY_cost[x][y] += g.adjList[u].wht;
               SIY_f[x][y] = max_f;
               if (max_f > LB)
               {
                  S = SIY[x][y];
                  LB = max_f;
               }
            }
         }
         // u++;
      }
      // cout<<"LB:"<<LB<<endl;
      // cout<<"Xong vong 1"<<endl;

      for (node_id u = 0; u < g.n; u++)
      {
         for (int i = 1; i <= h; i++)
         {
            // cout<<"SIY[i].size():"<<SIY[i].size()<<endl;
            for (int y = 0; y < SIY[i].size(); y++)
            {
               /*if(SIY_check[i][y]==false) continue;
               bool check_u=false;
               for(int j=0;j<SIY[i][y].size();j++)
               {
                  if(SIY[i][y][j]==u)
                  {
                     check_u=true;
                     break;
                  }
               }*/
               if (SIY_cost[i][y] + g.adjList[u].wht <= B)
               {
                  SIY[i][y].push_back(u);
                  double tmp_f = compute_valSet(nEvals, g, SIY[i][y]);
                  SIY[i][y].pop_back();
                  if (tmp_f > LB)
                  {
                     LB = tmp_f;
                     S = SIY[i][y];
                     S.push_back(u);
                  }
               }
            }
         }
         u++;
      }
      cout << "SMKSTREAM," << B << "," << LB << "," << nEvals;
      return LB;
   }
};

class SampleGreedy
{
   random_device rd;
   mt19937 gen;
   Args &myArgs;
   size_t k;
   tinyGraph &g;
   size_t nEvals = 0;
   double epsi;
   double B;
   double alpha;
   double tau;
   struct mypair
   {
      node_id u;
      double weight;
      int count;
      bool check;
      int size_S;
      bool operator<(const mypair &rhs) const
      {
         return (weight < rhs.weight);
      }
      static bool compareByPosition(const mypair &lhs, const mypair &rhs)
      {
         return lhs.weight > rhs.weight;
      }
   };

public:
   SampleGreedy(Args &args) : gen(rd()), myArgs(args), g(args.g)
   {
      k = args.k;
      epsi = args.epsi;
      B = args.buget * g.total_cost;
   }

   size_t run()
   {
      double e2 = epsi / 6.0;
      uniform_real_distribution<double> dist(0.0, 1.0);
      nEvals = 0;
      double solVal = 0;
      vector<mypair> Q;
      for (node_id i = 0; i < g.n; i++)
      {
         vector<node_id> tmpS;
         tmpS.push_back(i);
         double tmp = compute_valSet(nEvals, g, tmpS);
         mypair my;
         my.u = i;
         my.weight = tmp / g.adjList[i].wht;
         my.count = 0;
         my.check = false;
         my.size_S = 0;
         Q.push_back(my);
         if (tmp > solVal)
            solVal = tmp;
      }

      vector<node_id> S;
      vector<bool> check(g.n, false);
      double current_f_S = 0;
      double cost_s = 0;
      double gain = log(g.n / e2) / e2;
      while (true)
      {
         sort(Q.begin(), Q.end(), mypair::compareByPosition);
         bool flag = true;
         for (int j = 0; j < g.n; j++)
         {
            mypair m = Q[j];
            if (m.check == true || cost_s + g.adjList[m.u].wht > B)
               continue;

            flag = false;
            S.push_back(m.u);
            double tmp = compute_valSet(nEvals, g, S);
            S.pop_back();

            double delta = (tmp - current_f_S) / g.adjList[m.u].wht;
            if (delta >= m.weight / (1 + e2))
            {
               S.push_back(m.u);
               current_f_S = tmp;
               cost_s += g.adjList[m.u].wht;
               Q[j].check = true;
            }
            else
            {
               Q[j].count++;
               Q[j].weight = delta;
               if (Q[j].count > gain)
               {
                  Q[j].check = true;
               }
            }
         }
         if (flag == true)
            break;
      }
      solVal = max(solVal, current_f_S);
      cout << "SampleGreedy," << B << "," << solVal << "," << nEvals << "," <<cost_s;
      return solVal;
   }
};

class Fantom
{
   random_device rd;
   mt19937 gen;
   Args &myArgs;
   size_t k;
   tinyGraph &g;
   size_t nEvals = 0;
   double epsi;
   double B;
   double alpha;
   double tau;

public:
   Fantom(Args &args) : gen(rd()), myArgs(args), g(args.g)
   {
      k = args.k;
      epsi = args.epsi;
      B = args.buget * g.total_cost;
   }

   vector<node_id> GDT(vector<bool> &check, double threshold)
   {
      vector<node_id> S;
      double current_f_s = 0;
      double cost_s = 0;
      while (true)
      {
         double flag = false;
         for (node_id u = 0; u < g.n; u++)
         {
            if (check[u] == true || cost_s + g.adjList[u].wht > B) continue;
            S.push_back(u);
            double tmp_s = compute_valSet(nEvals, g, S);
            S.pop_back();
            double delta = (tmp_s - current_f_s) / g.adjList[u].wht;
            if (delta >= threshold)
            {
               S.push_back(u);
               current_f_s = tmp_s;
               cost_s += g.adjList[u].wht;
               check[u] = true;
               flag = true;
            }
         }
         if (flag == false)
            break;
      }

      node_id u1 = -1;
      double max_f = 0;
      for (node_id u = 0; u < g.n; u++)
      {
         if (g.adjList[u].wht > B) continue;
         vector<node_id> tmp_S;
         tmp_S.push_back(u);
         double tmpf_s = compute_valSet(nEvals, g, tmp_S);
         tmp_S.pop_back();
         if (tmpf_s >= max_f)
         {
            u1 = u;
            max_f = tmpf_s;
         }
      }

      if (current_f_s >= max_f)
      {
         return S;
      }
      else
      {
         vector<node_id> tmp_S;
         tmp_S.push_back(u1);
         return tmp_S;
      }
   }

   vector<node_id> iGDT(int p, double threshold)
   {
      // cout<<"iGDT--------------------------------------"<<endl;
      vector<bool> check(g.n, false);
      double max_f = 0;
      vector<node_id> S;
      for (int i = 1; i <= p + 1; i++)
      {
         vector<node_id> S_tmp = GDT(check, threshold);
         double tmp_f = compute_valSet(nEvals, g, S_tmp);
         // cout<<"GDT:"<<tmp_f<<" size:"<<S_tmp.size()<<endl;
         if (tmp_f > max_f)
         {
            max_f = tmp_f;
            S = S_tmp;
         }
      }
      return S;
   }
   size_t run()
   {
      int p = 2;
      double M = 0;
      nEvals = 0;
      double solVal = 0;
      for (node_id u = 0; u < g.n; u++)
      {
         vector<node_id> tmpS;
         tmpS.push_back(u);
         double tmpf = compute_valSet(nEvals, g, tmpS);
         if (tmpf > M)
            M = tmpf;
      }
      double gama = (2 * p * M) / ((p + 1) * (2 * p + 1));
      for (int i = 0; i <= g.n; i++)
      {
         double threshold = pow(1 + epsi, i) * gama;
         vector<node_id> Stmp = iGDT(p, threshold);
         double tmpf = compute_valSet(nEvals, g, Stmp);
         if (tmpf > solVal)
            solVal = tmpf;
      }
      cout << "Fantom," << B << "," << solVal << "," << nEvals;
      return solVal;
   }
};
#endif
