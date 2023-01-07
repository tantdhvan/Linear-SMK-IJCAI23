#ifndef OBJ_CPP
#define OBJ_CPP
#include <omp.h>
#include "mygraph.cpp"

using namespace std;
using namespace mygraph;

vector<bool> emptySetVector;

#ifdef MAXCOV
bool monotone = true;
size_t marge(size_t &nEvals, tinyGraph &g, node_id u, vector<bool> &set,
             vector<bool> &cov)
{

   if (set[u])
      return 0;

   ++nEvals;

   return g.getDegreeMinusSet(u, cov) + 1;
}

size_t marge(size_t &nEvals, tinyGraph &g, node_id u, vector<bool> &set)
{
   if (set[u])
      return 0;

   vector<bool> cov(g.n, false);
   size_t val = 0;
   for (node_id u = 0; u < g.n; ++u)
   {
      if (set[u])
      {
         if (!cov[u])
         {
            cov[u] = true;
            val += 1;
         }
         vector<tinyEdge> &neis = g.adjList[u].neis;
         for (size_t j = 0; j < neis.size(); ++j)
         {
            node_id v = neis[j].target;
            if (!cov[v])
            {
               cov[v] = true;
               val += 1;
            }
         }
      }
   }

   return marge(nEvals, g, u, set, cov);
}

size_t compute_valSet(size_t &nEvals, tinyGraph &g, vector<bool> &set,
                      vector<bool> &cov = emptySetVector)
{
   ++nEvals;
   cov.assign(g.n, false);
   size_t val = 0;
   for (node_id u = 0; u < g.n; ++u)
   {
      if (set[u])
      {
         if (!cov[u])
         {
            cov[u] = true;
            val += 1;
         }
         vector<tinyEdge> &neis = g.adjList[u].neis;
         for (size_t j = 0; j < neis.size(); ++j)
         {
            node_id v = neis[j].target;
            if (!cov[v])
            {
               cov[v] = true;
               val += 1;
            }
         }
      }
   }

   return val;
}

size_t compute_valSet(size_t &nEvals, tinyGraph &g, vector<node_id> &set)
{
   ++nEvals;
   vector<bool> cov(g.n, false);

   size_t val = 0;
   for (size_t i = 0; i < set.size(); ++i)
   {
      node_id u = set[i];
      if (!cov[u])
      {
         cov[u] = true;
         val += 1;
      }
      vector<tinyEdge> &neis = g.adjList[u].neis;
      for (size_t j = 0; j < neis.size(); ++j)
      {
         node_id v = neis[j].target;
         if (!cov[v])
         {
            cov[v] = true;
            val += 1;
         }
      }
   }

   return val;
}
#endif
#ifdef REVMAX_MON
bool monotone = true;
vector<double> alpha;

void init_alpha(tinyGraph &g)
{
   uniform_real_distribution<double> unidist(0, 1);
   alpha.assign(g.n, 0.0);
   mt19937 gen(0); // same sequence each time

   for (node_id u = 0; u < g.n; ++u)
   {
      alpha[u] = unidist(gen);
   }
}

double compute_valSet(size_t &nEvals, tinyGraph &g, vector<bool> &set,
                      vector<bool> &cov = emptySetVector)
{
   if (alpha.size() == 0)
   {
      init_alpha(g);
   }

   ++nEvals;
   cov.assign(g.n, false);
   double val = 0;

   for (node_id u = 0; u < g.n; ++u)
   {
      vector<tinyEdge> &neis = g.adjList[u].neis;
      double valU = 0.0;
      for (size_t j = 0; j < neis.size(); ++j)
      {
         node_id v = neis[j].target;
         if (set[v])
         {
            valU += neis[j].weight;
         }
      }
      valU = pow(valU, alpha[u]);
      val += valU;
   }

   return val;
}

double compute_valSet(size_t &nEvals, tinyGraph &g, vector<node_id> &sset)
{
   if (alpha.size() == 0)
   {
      init_alpha(g);
   }

   vector<bool> set(g.n, false);
   for (size_t i = 0; i < sset.size(); ++i)
   {
      set[sset[i]] = true;
   }

   ++nEvals;

   double val = 0;

   for (node_id u = 0; u < g.n; ++u)
   {
      vector<tinyEdge> &neis = g.adjList[u].neis;
      double valU = 0.0;
      for (size_t j = 0; j < neis.size(); ++j)
      {
         node_id v = neis[j].target;
         if (set[v])
         {
            valU += neis[j].weight;
         }
      }
      valU = pow(valU, alpha[u]);
      val += valU;
   }

   return val;
}

double marge(size_t &nEvals, tinyGraph &g, node_id x, vector<bool> &set,
             vector<bool> &cov = emptySetVector)
{
   if (alpha.size() == 0)
   {
      init_alpha(g);
   }

   if (set[x])
      return 0;

   vector<tinyEdge> &neis = g.adjList[x].neis;
   double gain = 0.0;
   for (size_t j = 0; j < neis.size(); ++j)
   {
      node_id v = neis[j].target;
      vector<tinyEdge> &neisV = g.adjList[v].neis;
      double valV = 0.0;
      double valVwithX = 0.0;
      for (size_t k = 0; k < neisV.size(); ++k)
      {
         node_id w = neisV[k].target;
         if (w != x)
         {
            if (set[w])
            {
               valV += neisV[k].weight;
               valVwithX += neisV[k].weight;
            }
         }
         else
         {
            valVwithX += neisV[k].weight;
         }
      }

      if (valV == 0)
         gain += pow(valVwithX, alpha[v]);
      else
         gain += pow(valVwithX, alpha[v]) - pow(valV, alpha[v]);
   }
   ++nEvals;
   return gain;
}

#endif

#ifdef MAXCUT
bool monotone = false;
signed long marge(size_t &nEvals, tinyGraph &g, node_id u, vector<bool> &set, vector<bool> &ancillary = emptySetVector)
{

   if (set[u])
      return 0;

   ++nEvals;

   signed long m;
   double mx = 2 * g.getWeightedDegreeMinusSet(u, set);
   double my = g.getWeightedDegree(u);

   m = (mx - my);

   return m;
}


size_t compute_valSet(size_t &nEvals, tinyGraph &g, vector<bool> &set)
{
   ++nEvals;
   size_t val = 0;
   #pragma omp parallel for reduction(+:val)
   for (node_id u = 0; u < g.n; ++u)
   {
      vector<tinyEdge> &neis = g.adjList[u].neis;
      for (size_t j = 0; j < neis.size(); ++j)
      {
         node_id v = neis[j].target;
         if ((set[u] && !set[v]) || (!set[u] && set[v]))
            val += neis[j].weight;
      }
   }

   return val / 2;
}

double compute_valSet(size_t &nEvals, tinyGraph &g, vector<node_id> &set_id)
{
   vector<bool> set(g.n, false);
   for (size_t i = 0; i < set_id.size(); ++i)
   {
      set[set_id[i]] = true;
   }

   ++nEvals;
   double val = 0;
   #pragma omp parallel for reduction(+:val)
   for (node_id u = 0; u < g.n; ++u)
   {
      vector<tinyEdge> &neis = g.adjList[u].neis;
      for (size_t j = 0; j < neis.size(); ++j)
      {
         node_id v = neis[j].target;
         if ((set[u] && !set[v]) || (!set[u] && set[v]))
            val += neis[j].weight;
      }
   }

   return val / 2;
}

#endif

#ifdef REVMAX_NM
bool monotone = false;
vector<double> alpha;

void init_alpha(tinyGraph &g)
{
   uniform_real_distribution<double> unidist(0, 1);
   alpha.assign(g.n, 0.0);
   mt19937 gen(0); // same sequence each time

   for (node_id u = 0; u < g.n; ++u)
   {
      alpha[u] = unidist(gen);
   }
}

double compute_valSet(size_t &nEvals, tinyGraph &g, vector<bool> &set, vector<bool> &cov = emptySetVector)
{
   if (alpha.size() == 0)
   {
      init_alpha(g);
   }

   ++nEvals;
   cov.assign(g.n, false);
   double val = 0;

   for (node_id u = 0; u < g.n; ++u)
   {
      if (!set[u])
      {
         vector<tinyEdge> &neis = g.adjList[u].neis;
         double valU = 0.0;
         for (size_t j = 0; j < neis.size(); ++j)
         {
            node_id v = neis[j].target;
            if (set[v])
            {
               valU += neis[j].weight;
            }
         }
         valU = pow(valU, alpha[u]);
         val += valU;
      }
   }

   return val;
}

double compute_valSet(size_t &nEvals, tinyGraph &g, vector<node_id> &sset)
{
   if (alpha.size() == 0)
   {
      init_alpha(g);
   }
   vector<bool> set(g.n, false);
   for (size_t i = 0; i < sset.size(); ++i)
   {
      set[sset[i]] = true;
   }

   ++nEvals;

   double val = 0;
   #pragma omp parallel for reduction(+:val)
   for (node_id u = 0; u < g.n; ++u)
   {
      if (!set[u])
      {
         vector<tinyEdge> &neis = g.adjList[u].neis;
         double valU = 0.0;
         for (size_t j = 0; j < neis.size(); ++j)
         {
            node_id v = neis[j].target;
            if (set[v])
            {
               valU += neis[j].weight;
            }
         }
         valU = sqrt(valU);
         val += valU;
      }
   }

   return val;
}

double marge(size_t &nEvals, tinyGraph &g, node_id x, vector<bool> &set,
             vector<bool> &cov = emptySetVector)
{
   if (alpha.size() == 0)
   {
      init_alpha(g);
   }
   if (set[x])
      return 0;

   double loss = 0.0;

   vector<tinyEdge> &neis = g.adjList[x].neis;
   double valX = 0.0;
   for (size_t j = 0; j < neis.size(); ++j)
   {
      node_id v = neis[j].target;
      if (set[v])
      {
         valX += neis[j].weight;
      }
   }

   valX = pow(valX, alpha[x]);

   loss = valX;

   double gain = 0.0;
   for (size_t j = 0; j < neis.size(); ++j)
   {
      node_id v = neis[j].target;
      vector<tinyEdge> &neisV = g.adjList[v].neis;
      double valV = 0.0;
      double valVwithX = 0.0;
      for (size_t k = 0; k < neisV.size(); ++k)
      {
         node_id w = neisV[k].target;
         if (w != x)
         {
            if (set[w])
            {
               valV += neisV[k].weight;
               valVwithX += neisV[k].weight;
            }
         }
         else
         {
            valVwithX += neisV[k].weight;
         }
      }

      gain += pow(valVwithX, alpha[v]) - pow(valV, alpha[v]);
   }

   ++nEvals;
   return gain - loss;
}

#endif

#ifdef IMG
bool monotone = false;
signed long marge(size_t &nEvals, tinyGraph &g, node_id u, vector<bool> &set, vector<bool> &ancillary = emptySetVector)
{

   if (set[u])
      return 0;

   ++nEvals;

   signed long m;
   double mx = 2 * g.getWeightedDegreeMinusSet(u, set);
   double my = g.getWeightedDegree(u);

   m = (mx - my);

   return m;
}


size_t compute_valSet(size_t &nEvals, tinyGraph &g, vector<bool> &set)
{
   ++nEvals;
   size_t val = 0;
   #pragma omp parallel for reduction(+:val)
   for (node_id u = 0; u < g.n; ++u)
   {
      vector<tinyEdge> &neis = g.adjList[u].neis;
      for (size_t j = 0; j < neis.size(); ++j)
      {
         node_id v = neis[j].target;
         if ((set[u] && !set[v]) || (!set[u] && set[v]))
            val += neis[j].weight;
      }
   }

   return val / 2;
}

double compute_valSet(size_t &nEvals, tinyGraph &g, vector<node_id> &set_id)
{
   vector<bool> set(g.n, false);
   for (size_t i = 0; i < set_id.size(); ++i)
   {
      set[set_id[i]] = true;
   }

   ++nEvals;
   double val = 0;
   
   #pragma omp parallel for reduction(+:val)
   for (node_id u = 0; u < g.n; ++u)
   {     
      double max=0;
      for(node_id v=0;v <g.n; v++)
      {
         if(set[u] && set[v])
         {
            val-=(double)g.getEdgeWeight(u,v)/(double)g.n;
         }
         if(set[u]==false && set[v]==true)
         {
            double tmp=g.getEdgeWeight(u,v);
            if(tmp>max)
               max=tmp;
         }
      }
      val+=max;
   }

   return val;
}

#endif

#endif