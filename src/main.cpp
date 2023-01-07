#include "mygraph.cpp"
#include "algs.cpp"
#include <iostream>
#include <string>
#include <unistd.h>
#include <chrono>
#include <getopt.h>
#include <time.h>

using namespace mygraph;
using namespace std;

void print_help()
{
   cout << "Options: " << endl;
   cout << "-g <graph filename in binary format>" << endl
        << "-k <cardinality constraint>" << endl
        << "-b <buget" << endl
        << "-o <outputFileName>" << endl
        << "-N <repetitions>" << endl
        << "-e <accuracy parameter epsilon (default 0.1)>" << endl
        << "-p [Only with -Q, run QuickStream++]" << endl
        << "-t <Tradeoff in error, only applies to P-Pass>" << endl
        << "-c <blocksize, only applies to QuickStream(++)>" << endl
        << "-q [quiet mode]" << endl;
}

Algs string_to_alg(string sin)
{
   transform(sin.begin(), sin.end(), sin.begin(), ::toupper);
   if (sin == "ALG3")
      return ALG3;
   if (sin == "ALG4")
      return ALG4;
   if (sin == "SMKDETACC")
      return SMKDETACC;
   if (sin == "SMKRANACC")
      return SMKRANACC;
   if (sin == "SMKSTREAM")
      return SMKSTREAM;
   if (sin == "SAMPLEGREEDY")
      return SAMPLEGREEDY;
   if (sin == "FANTOM")
      return FANTOM;
   return ALG1;
}

void parseArgs(int argc, char **argv, Args &arg)
{
   int c;
   extern char *optarg;

   if (argc == 1)
   {
      print_help();
      exit(2);
   }

   string sarg;

   while (1)
   {
      static struct option long_options[] =
          {
              {"nthreads", required_argument, 0, 'x'},
              {"alg", required_argument, 0, 'a'},
              {"lazy", no_argument, 0, 'l'},
              {0, 0, 0, 0}};
      c = getopt_long(argc, argv, ":g:k:b:N:Co:e:c:pt:qd:a:lx:", long_options, NULL);

      if (c == -1)
         break;

      switch (c)
      {
      case 'x':
         sarg.assign(optarg);
         arg.nThreads = stoi(sarg);
         break;
      case 'a':
         sarg.assign(optarg);
         arg.alg = string_to_alg(sarg);
         break;
      case 'c':
         sarg.assign(optarg);
         arg.c = stoi(sarg);
         break;
      case 'l':
         arg.lazy = true;
         break;
      case 't':
         sarg.assign(optarg);
         arg.tradeoff = stod(sarg);
         break;
      case 'e':
         sarg.assign(optarg);
         arg.epsi = stod(sarg);
         break;
      case 'd':
         sarg.assign(optarg);
         arg.delta = stod(sarg);
         break;
      case 'o':
         sarg.assign(optarg);
         arg.outputFileName = sarg;
         break;
      case 'g':
         arg.graphFileName.assign(optarg);
         break;
      case 'k':
         sarg.assign(optarg);
         arg.k = stoi(sarg);
         break;
      case 'b':
         sarg.assign(optarg);
         arg.buget = stod(sarg);
         break;
      case 'N':
         sarg.assign(optarg);
         arg.N = stoi(sarg);
         break;
      case 'p':
         arg.plusplus = true;
         break;
      case 'q':
         arg.quiet = true;
         break;
      case '?':
         print_help();
         exit(2);
         break;
      }
   }

   if (arg.quiet)
   {
      arg.logg.enabled = false;
      arg.g.logg.enabled = false;
   }
}

void readGraph(Args &args)
{
   args.g.read_bin(args.graphFileName);
}

void worker_run(Args &args, size_t N)
{
   for (size_t i = 0; i < N; ++i)
   {
      std::chrono::steady_clock::time_point begin = std::chrono::steady_clock::now();
      switch (args.alg)
      {
         case ALG3:
         {
            Alg3 alg3(args);
            alg3.run();
         }
         break;
         case ALG4:
         {
            Alg4 alg4(args);
            alg4.run();
         }
         break;
         case SMKDETACC:
         {
            SmkDetAcc smkdetacc(args);
            smkdetacc.run();
         }
         break;
         case SMKRANACC:
         {
            SmkRanAcc Smkranacc(args);
            Smkranacc.run();
         }
         break;
         case SMKSTREAM:
         {
            SmkStream Smkstream(args);
            Smkstream.run();
         }
         break;
         case SAMPLEGREEDY:
         {
            SampleGreedy Samplegreedy(args);
            Samplegreedy.run();
         }
         break;
         case FANTOM:
         {
            Fantom fantom(args);
            fantom.run();
         }
         break;
      }

      std::chrono::steady_clock::time_point end = std::chrono::steady_clock::now();
      size_t WallTimeMillis = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();
      double time = WallTimeMillis / 1000.0;
      cout << "," << time << endl;
   }
}

void runAlg(Args &args)
{
   size_t N = args.N;
   allResults.init("obj");
   allResults.init("nEvals");
   allResults.init("k");
   allResults.init("b");
   allResults.init("n");
   allResults.init("mem");
   allResults.add("k", args.k);
   allResults.add("b", args.buget);
   allResults.add("n", args.g.n);
   allResults.init("epsi");
   allResults.add("epsi", args.epsi);

   vector<Args> thread_args(args.nThreads, args);

   if (args.nThreads == 1 || N == 1)
   {
      worker_run(args, N);
   }
   else
   {
      thread *wThreads = new thread[args.nThreads];
      size_t tot_reps = 0;
      for (size_t i = 0; i < args.nThreads; ++i)
      {
         size_t nreps = N / args.nThreads;
         if (i == args.nThreads - 1)
         {
            nreps = N - tot_reps;
         }

         if (args.g == thread_args[i].g)
         {
            cout << "graphs are equal" << endl;
         }
         else
         {
            cout << "graphs are not equal" << endl;
            return;
         }
         wThreads[i] = thread(worker_run,
                              ref(thread_args[i]),
                              nreps);
         tot_reps += nreps;
      }

      for (size_t i = 0; i < args.nThreads; ++i)
         wThreads[i].join();

      delete[] wThreads;
   }
}

void outputResults(Args &args)
{
   if (args.outputFileName != "")
   {
      args.g.logg << "Writing output to file: " << args.outputFileName << endL;
      ofstream of(args.outputFileName.c_str(), ofstream::out | ofstream::app);
      allResults.print(of);
   }
   else
   {
      allResults.print("obj");
      cout << ' ';
      allResults.print("nEvals");
      cout << endl;
   }
}

int main(int argc, char **argv)
{
   Args args;
   parseArgs(argc, argv, args);
   readGraph(args);
   runAlg(args);
}
