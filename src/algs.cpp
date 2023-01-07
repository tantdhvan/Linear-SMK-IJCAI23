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
   ALG3,
   ALG4,
   SMKDETACC,
   SMKRANACC,
   SMKSTREAM,
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
   double gain;

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
   vector<myWeight> A;
   double tau, theta;
   vector<vector<double>> F_X; // lưu hàm f đã được tính
   vector<vector<double>> F_Y; // lưu hàm f đã được tính
public:
   Alg3(Args &args) : gen(rd()), myArgs(args), g(args.g)
   {
      k = args.k;
      epsi = args.epsi;
      B = args.buget * g.total_cost;
   }
   double alg1()
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
         a.w[1] = tmp_f / g.adjList[u].wht;
         a.w[2] = tmp_f;
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
      double e2 = epsi / 20, solVal = 0;
      for (node_id u = 0; u < g.n; u++)
      {
         vector<double> tmpf(g.n, -1.0);
         F_X.push_back(tmpf);
         F_Y.push_back(tmpf);
      }
      tau = alg1();
      solVal = tau;
      vector<node_id> X, Y;
      double cost_x = 0, cost_y = 0, current_fx = 0, current_fy = 0, tmp_fx = 0.0, tmp_fy = 0, nguong = (tau * (1 - e2)) / (6 * B);
      theta = 19 * tau / (6 * e2 * B);
      while (theta > nguong)
      {
         while (true)
         {
            std::sort(A.begin(), A.end(), greater1());
            int index1 = -1;
            for (int i = 0; i < A.size(); i++)
            {
               if (A[i].check == false)
                  continue;
               index1 = i;
               break;
            }
            if (index1 == -1)
               break;
            if (A[index1].w[1] < theta)
               break;

            node_id e = A[index1].u;
            if (cost_x + g.adjList[e].wht > B && cost_y + g.adjList[e].wht > B)
            {
               A[index1].check = false;
               continue;
            }
            tmp_fx = -1;
            tmp_fy = -1;
            bool danhDau1 = false, danhDau2 = false;
            if (cost_x + g.adjList[e].wht <= B)
            {
               danhDau1 = true;
               if (F_X[X.size()][e] != -1)
                  tmp_fx = F_X[X.size()][e];
               else
               {
                  X.push_back(e);
                  tmp_fx = compute_valSet(nEvals, g, X);
                  X.pop_back();
                  F_X[X.size()][e] = tmp_fx;
               }
            }
            if (cost_y + g.adjList[e].wht <= B)
            {
               danhDau2 = true;
               if (F_Y[Y.size()][e] != -1)
                  tmp_fy = F_Y[Y.size()][e];
               else
               {
                  Y.push_back(e);
                  tmp_fy = compute_valSet(nEvals, g, Y);
                  Y.pop_back();
                  F_Y[Y.size()][e] = tmp_fy;
               }
            }
            bool danhDau_x;
            if (danhDau1 == true && danhDau2 == true)
            {
               if (tmp_fx > tmp_fy)
                  danhDau_x = true;
               else
                  danhDau_x = false;
            }
            else
            {
               if (danhDau1 == true)
                  danhDau_x = true;
               else
                  danhDau_x = false;
            }
            if (danhDau_x == true)
            {
               double delta = (tmp_fx - current_fx) / g.adjList[e].wht;
               if (delta >= theta)
               {
                  X.push_back(e);
                  cost_x += g.adjList[e].wht;
                  current_fx = tmp_fx;
                  A[index1].check = false;
                  std::sort(A.begin(), A.end(), greater2());
                  int index2 = -1;
                  for (int i2 = 0; i2 < A.size(); i2++)
                  {
                     if (A[i2].check == false || cost_x + g.adjList[A[i2].u].wht > B)
                        continue;
                     index2 = i2;
                     break;
                  }
                  if (index2 != -1)
                  {
                     double tmpf;
                     node_id e2 = A[index2].u;
                     if (F_X[X.size()][e2] != -1)
                        tmpf = F_X[X.size()][e2];
                     else
                     {
                        X.push_back(e2);
                        tmpf = compute_valSet(nEvals, g, X);
                        X.pop_back();
                     }
                     solVal = max(tmpf, solVal);
                  }
               }
               else
               {
                  A[index1].w[1] = delta;
                  A[index1].w[2] = tmp_fx - current_fx;
               }
            }
            else
            {
               double delta = (tmp_fy - current_fy) / g.adjList[e].wht;
               if (delta >= theta)
               {
                  Y.push_back(e);
                  cost_y += g.adjList[e].wht;
                  current_fy = tmp_fy;
                  A[index1].check = false;
                  std::sort(A.begin(), A.end(), greater2());
                  int index2 = -1;
                  for (int i2 = 0; i2 < A.size(); i2++)
                  {
                     if (A[i2].check == false || cost_y + g.adjList[A[i2].u].wht > B)
                        continue;
                     index2 = i2;
                     break;
                  }
                  if (index2 != -1)
                  {
                     double tmpf;
                     node_id e2 = A[index2].u;
                     if (F_Y[Y.size()][e2] != -1)
                        tmpf = F_Y[Y.size()][e2];
                     else
                     {
                        Y.push_back(e2);
                        tmpf = compute_valSet(nEvals, g, Y);
                        Y.pop_back();
                     }
                     solVal = max(tmpf, solVal);
                  }
               }
               else
               {
                  A[index1].w[1] = delta;
                  A[index1].w[2] = tmp_fy - current_fy;
               }
            }
         }
         theta = (1 - e2) * theta;
      }
      solVal = max(current_fx, max(current_fy, solVal));
      cout << "ALG3," << B << "," << solVal << "," << nEvals;
   }
};

class Alg4
{
   random_device rd;
   mt19937 gen;
   Args &myArgs;
   size_t k, nEvals = 0;
   tinyGraph &g;
   double epsi, B, tau, theta, cost, current_f = 0;
   vector<myWeight> A;
   vector<bool> check_u, check_s;
   vector<node_id> S;
   vector<vector<double>> FS;
public:
   Alg4(Args &args) : gen(rd()), myArgs(args), g(args.g)
   {
      k = args.k;
      epsi = args.epsi;
      B = args.buget * g.total_cost;
   }
   double alg2()
   {
      uniform_real_distribution<double> dist(0.0, 1.0);
      vector<node_id> S;
      vector<double> V1;
      double cost = 0, solVal = 0,B2 = B / 2, Emax = 0, current_f = 0, alpha = sqrt(2 + sqrt(2)), p = sqrt(2) - 1;
      for (node_id u = 0; u < g.n; u++)
      {
         vector<node_id> tmp;
         tmp.push_back(u);
         double tmp_f = compute_valSet(nEvals, g, tmp);
         myWeight a;
         a.u = u;
         a.w[1] = tmp_f / g.adjList[u].wht;
         a.w[2] = tmp_f;
         a.frequency = 0;
         a.s_len = 0;
         a.check = true;
         A.push_back(a);
         FS[0][u] = tmp_f;
         if (tmp_f > Emax)
            Emax = tmp_f;
      }
      vector<node_id> VP;
      for (node_id i = 0; i < g.n; i++)
      {
         if (g.adjList[i].wht > B2)
            continue;
         double rand = dist(gen);
         if (rand >= p)
            VP.push_back(i);
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
      tau = alg2();
      solVal = tau;
      int j = 0;
      theta = 16.037 * tau / (4 * epsi2 * B);
      double nguong = (tau * (1 - epsi2)) / (10 * B), tmp_f = 0;
      while (theta > nguong)
      {
         while (true)
         {
            std::sort(A.begin(), A.end(), greater1());
            int index1 = -1;
            for (int i = 0; i < A.size(); i++)
            {
               if (A[i].check == false)
                  continue;
               index1 = i;
               break;
            }
            if (index1 == -1)
               break;
            if (A[index1].w[1] < theta)
               break;

            node_id e = A[index1].u;
            if (cost + g.adjList[e].wht > B || g.adjList[e].wht > (1 - epsi2) * B)
            {
               A[index1].check = false;
               continue;
            }

            if (FS[S.size()][e] != -1)
               tmp_f = FS[S.size()][e];
            else
            {
               S.push_back(e);
               tmp_f = compute_valSet(nEvals, g, S);
               S.pop_back();
               FS[S.size()][e] = tmp_f;
            }
            delta = (tmp_f - current_f) / g.adjList[e].wht;
            if (delta >= theta)
            {
               A[index1].check = false;
               double rand = dist(gen);
               if (rand >= 0.5)
               {
                  current_f = tmp_f;
                  S.push_back(e);
                  cost += g.adjList[e].wht;
                  std::sort(A.begin(), A.end(), greater2());
                  int index2 = -1;
                  for (int i2 = 0; i2 < A.size(); i2++)
                  {
                     if (A[i2].check == false || cost + g.adjList[A[i2].u].wht > B)
                        continue;
                     index2 = i2;
                     break;
                  }
                  if (index2 != -1)
                  {
                     node_id e2 = A[index2].u;
                     if (FS[S.size()][e2] != -1)
                        tmp_f = FS[S.size()][e2];
                     else
                     {
                        S.push_back(e2);
                        tmp_f = compute_valSet(nEvals, g, S);
                        S.pop_back();
                     }
                     solVal = max(tmp_f, solVal);
                  }
               }
            }
            else
            {
               A[index1].w[1] = delta;
               A[index1].w[2] = tmp_f - current_f;
            }
         }
         theta = (1 - epsi2) * theta;
      }
      solVal = max(current_f, solVal);
      cout << "ALG4," << B << "," << solVal << "," << nEvals;
      return solVal;
   }
};

class SmkDetAcc
{
   random_device rd;
   mt19937 gen;
   Args &myArgs;
   size_t k, nEvals = 0;
   tinyGraph &g;
   double epsi,B,alpha,tau;
   vector<vector<node_id>> D,S;
   vector<vector<myWeight>> A;
   vector<double> current_f, cost;
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
      myWeight tmp;
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
               if (A[m][i].w[j] > oldWeight / (1 + e2))
               {
                  a[j] = A[m][i];
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
            a.w[1] = tmp_f / g.adjList[u].wht;
            a.w[2] = tmp_f;
            a.frequency = 0;
            a.s_len = 0;
            a.check = true;
            A[i].push_back(a);
         }
      }

      while (true)
      {
         myWeight tmp;
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
      cout << "SMKDETACC," << B << "," << m << "," << nEvals;
      return m;
   }
};

class SmkRanAcc
{
   random_device rd;
   mt19937 gen;
   Args &myArgs;
   size_t k, nEvals = 0;
   tinyGraph &g;
   double epsi, B, alpha, tau, current_f = 0, cost;
   vector<myWeight> A;
   vector<node_id> S;
   vector<bool> check_u, check_s;
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
      myWeight tmp;
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
               if (A[i].w[j] >= oldWeight / (1 + e2))
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
      for (node_id u = 0; u < g.n; u++)
      {
         vector<node_id> tmp;
         tmp.push_back(u);
         double tmp_f = compute_valSet(nEvals, g, tmp);
         DD[u][0] = true;
         myWeight a;
         a.u = u;
         a.w[1] = tmp_f / g.adjList[u].wht;
         a.w[2] = tmp_f;
         a.frequency = 0;
         a.s_len = 0;
         a.check = true;
         A.push_back(a);
      }
      while (true)
      {
         node_id u = 4294967295, u2 = 4294967295;
         myWeight tmp_a1, tmp_a2;
         GetMax(tmp_a1, tmp_a2);
         u = tmp_a1.u;
         u2 = tmp_a2.u;
         if (u2 != 4294967295 && cost + g.adjList[u2].wht <= B)
         {
            S.push_back(u2);
            double tmp_f = compute_valSet(nEvals, g, S);
            S.pop_back();
            if (tmp_f - current_f > 0 && tmp_f > max_f_sao)
               max_f_sao = tmp_f;
         }
         if (u != 4294967295 && cost + g.adjList[u].wht <= B)
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
      cout << "SMKRANACC," << B << "," << solVal << "," << nEvals;
      return solVal;
   }
};

class SmkStream
{
   random_device rd;
   mt19937 gen;
   Args &myArgs;
   size_t k, nEvals = 0;
   tinyGraph &g;
   double epsi, B, alpha, beta, tau, e2;
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
      vector<int> C;
      int i = 0;
      while (true)
      {
         double tmp = pow((1 + e2), i);
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
      vector<vector<double>> SIY_f, SIY_cost;
      vector<vector<bool>> SIY_check;
      for (int i = 0; i <= h; i++)
      {
         vector<vector<node_id>> tmp;
         vector<double> tmp_f, tmp_c;
         vector<bool> tmp_b;
         SIY.push_back(tmp);
         SIY_f.push_back(tmp_f);
         SIY_cost.push_back(tmp_c);
         SIY_check.push_back(tmp_b);
      }
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
         vector<int> C = getC((tmp_f * alpha) / (B * (1 + e2)), tmp_f / g.adjList[u].wht);  
         for (int y : C)
         {
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
      }
      for (node_id u = 0; u < g.n; u++)
      {
         for (int i = 1; i <= h; i++)
         {
            for (int y = 0; y < SIY[i].size(); y++)
            {
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
   size_t k,nEvals = 0;
   tinyGraph &g;
   double epsi,B,alpha,tau;
   struct mypair
   {
      node_id u;
      double weight;
      int count,size_S;
      bool check;
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
      double current_f_S = 0, cost_s = 0, gain = log(g.n / e2) / e2;
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
      cout << "SampleGreedy," << B << "," << solVal << "," << nEvals;
      return solVal;
   }
};

class Fantom
{
   random_device rd;
   mt19937 gen;
   Args &myArgs;
   size_t k,nEvals = 0;
   tinyGraph &g;
   double epsi,B,alpha,tau;
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
      double current_f_s = 0, cost_s = 0;
      while (true)
      {
         double flag = false;
         for (node_id u = 0; u < g.n; u++)
         {
            if (check[u] == true || cost_s + g.adjList[u].wht > B)
               continue;
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
         if (g.adjList[u].wht > B)
            continue;
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
      vector<bool> check(g.n, false);
      double max_f = 0;
      vector<node_id> S;
      for (int i = 1; i <= p + 1; i++)
      {
         vector<node_id> S_tmp = GDT(check, threshold);
         double tmp_f = compute_valSet(nEvals, g, S_tmp);
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
      double M = 0, solVal = 0;
      nEvals = 0;
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
