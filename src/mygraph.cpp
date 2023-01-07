#ifndef MYGRAPH_CPP
#define MYGRAPH_CPP
#include <list>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <map>
#include <unordered_set>
#include <thread>
#include <iomanip>
#include <cmath>
#include <utility>
#include <queue>
#include <set>
#include <thread>
#include <mutex>
#include <random>
#include "logger.cpp"
#include "binheap.cpp"

using namespace std;

vector<size_t> emptySizetVector;


template <typename T>
bool vector_ctns(std::vector<T> &vec, T item)//tìm kiếm item có tồn tại trong vec
{
	for (std::size_t i = 0; i < vec.size(); ++i)
	{
		if (vec[i] == item)
		{
			return true;
		}
	}
	return false;
}

namespace mygraph
{

	random_device rd;
	mt19937 gen(rd());

	typedef uint32_t node_id;
	typedef struct myQue{
		node_id u;
		double mg;
		double mg2;
		int l;
		double current_f;
		double f;
		bool check;
		myQue(node_id _u,double _mg, double _mg2, int _l,double _current_f,double _f,bool _check)
		{
			u=_u;
			mg=_mg;
			l=_l;
			current_f=_current_f;
			f=_f;
			check=_check;
		}
	};
	struct greater_myQ
	{
    	bool operator()(myQue const  &a, myQue const  &b) const
		{ 
			return a.mg<b.mg;		
		}
	};
	struct greater_myQ2
	{
    	bool operator()(myQue const  &a, myQue const  &b) const
		{ 
			return a.mg2<b.mg2;		
		}
	};
	typedef struct myWeight{
		node_id u=-1;
		vector<double> w;
		int frequency;
		int s_len;
		bool check;
		myWeight()
		{
			u = 4294967295;
      		w.push_back(0);
      		w.push_back(0);
      		w.push_back(0);
		}
	};
	struct greater1
	{
    	bool operator()(myWeight const  &a, myWeight const  &b) const  { 
			
			if(a.w.size()>=3 && b.w.size()>=3)
				return a.w[1] > b.w[1];
			else
				return true;		
			}
	};
	struct greater2
	{
    	bool operator()(myWeight const &a, myWeight const &b) const { 
			if(a.w.size()>=3 && b.w.size()>=3)
				return a.w[2] > b.w[2];
			else
				return true; }
	};
	
	double iRand()
	{
		uniform_real_distribution<double> dist(0.001, 1.0);
		double rand = dist(gen);
		return rand;
	}
	double fun_g(double x)
	{
		return 1.0-exp(-0.2*x);
	}
	// Graph structure solely for removing isolated nodes
	// Removes all isolated nodes, renumbers vertices from 0, n-1
	// unweighted, undirected graphs only
	class simplifyNode
	{
	public:
		node_id id;
		vector<simplifyNode *> neis;//danh sách đỉnh kề
		vector<double> nei_weights;//trọng số của cạnh
		double wht;
	};

	class simplifyGraph
	{
	public:
		node_id n;
		Logger logg;
		vector<simplifyNode *> adjList;//danh sách đỉnh

		// undirected
		void add_edge(node_id from, node_id to, double wht)//thêm cạnh giữa hai đỉnh
		{
			if (!vector_ctns(adjList[from]->neis, adjList[to]))//kiểm tra xem đỉnh [to] đã có trong danh sách đỉnh kề của [from] chưa
			{
				adjList[from]->neis.push_back(adjList[to]);
				adjList[from]->nei_weights.push_back(wht);
			}
			if (!vector_ctns(adjList[to]->neis, adjList[from]))//kiểm tra xem đỉnh [from] đã có trong danh sách đỉnh kề của [to] chưa
			{
				adjList[to]->neis.push_back(adjList[from]);
				adjList[to]->nei_weights.push_back(wht);
			}
		}

		void remove_isolates()//Xoá đỉnh độc lập
		{
			vector<simplifyNode *> newAdjList;
			for (node_id i = 0; i < n; ++i)
			{
				if (!adjList[i]->neis.empty())
					newAdjList.push_back(adjList[i]);
			}

			adjList.swap(newAdjList);
			n = static_cast<node_id>(adjList.size());
		}

		void renumber_vertices()//đánh lại số thứ tự đỉnh
		{
			for (node_id i = 0; i < n; ++i)
			{
				adjList[i]->id = i;
			}
		}

		/*
		 * Writes full adjacency list to
		 * binary format of tinyGraph
		 */
		void write_bin(string fname)
		{
			ofstream ifile(fname.c_str(), ios::out | ios::binary);

			ifile.write((char *)&n, sizeof(node_id));
			double preprocessTime = 0.0;
			ifile.write((char *)&preprocessTime, sizeof(double));

			size_t ss;
			node_id nei_id;
			double w;
			double wtmp=1.0;
			for (unsigned i = 0; i < n; ++i)
			{
				ss = adjList[i]->neis.size();
				wtmp=adjList[i]->wht;
				ifile.write((char *)&wtmp, sizeof(double));//số đỉnh kề
				ifile.write((char *)&ss, sizeof(size_t));//số đỉnh kề
				for (unsigned j = 0; j < ss; ++j)
				{
					nei_id = adjList[i]->neis[j]->id;
					w = adjList[i]->nei_weights[j];
					ifile.write((char *)&nei_id, sizeof(node_id));
					ifile.write((char *)&w, sizeof(double));
				}
			}
		}
		

		void read_edge_list(string fname)
		{
			logg << "Reading edge list from file " << fname << endL;
			ifstream ifile(fname.c_str());
			uint32_t n,m;
			ifile>>n>>m;
			logg<<n<<" "<< m <<endL;
			this->n=n;
			unsigned from, to;
			double weight = 1.0;
			simplifyNode *emptyNode;
			for(unsigned i=0;i<n;i++)
			{
				ifile>>weight;
				emptyNode=new simplifyNode;
				emptyNode->id = i;
				emptyNode->wht=weight;
				adjList.push_back(emptyNode);
			}
			for(unsigned i=0;i<m;i++)
			{
				ifile>>from>>to>>weight;
				cout<<"weight:"<<weight<<endl;
				add_edge(from, to, weight);				
			}			
			ifile.close();
		}

		void read_csv(string fname)
		{
			logg << "Reading edge list from file " << fname << endL;
			ifstream ifile(fname.c_str());

			string sline;
			stringstream ss;
			unsigned line_number = 0;
			bool weighted;
			while (getline(ifile, sline))
			{
				if (sline[0] != '#')
				{
					ss.clear();
					ss.str(sline);

					if (line_number == 0)
					{
						ss >> this->n; // Value is currently ignored
						ss >> weighted;
						if (weighted)
						{
							logg << "Graph is weighted." << endL;

							this->n = 0;
						}
						else
						{
							logg << "Graph is unweighted." << endL;
							this->n = 0;
							// init_empty_graph();
						}
					}
					else
					{
						// have an edge on this line
						unsigned from, to;
						unsigned wht = 1;
						char comma;
						ss >> from;
						ss >> comma;
						ss >> to;
						if (weighted)
							ss >> wht;
						simplifyNode *emptyNode;
						while (to >= this->n)
						{
							emptyNode = new simplifyNode;
							emptyNode->id = this->n;
							adjList.push_back(emptyNode);
							++this->n;
						}

						while (from >= this->n)
						{
							emptyNode = new simplifyNode;
							emptyNode->id = this->n;
							adjList.push_back(emptyNode);
							++this->n;
						}

						add_edge(from, to, wht);
					}

					++line_number;
				}
			}

			ifile.close();
		}
	};

	// Compact, undirected graph structure

	uint32_t bitMask = ~(3 << 30);
	uint32_t bitS = (1 << 31);
	uint32_t bitW = (1 << 30);

	/*
	 * Edge classes
	 */
	class tinyEdge
	{
	public:
		// last 30 bits are target node_id
		// first bit is inS, second bit is inW
		uint32_t target;
		double weight;

		node_id getId() const
		{
			return target & bitMask;
		}

		bool inS()
		{
			return (target >> 31);
		}

		tinyEdge()
		{
			target = 0;
		}

		tinyEdge(node_id nid, double w)
		{
			target = nid; // inS = inW = false;
			weight = w;
		}

		tinyEdge(const tinyEdge &rhs)
		{
			target = rhs.target;
			weight = rhs.weight;
		}
	};

	/*
	 * Only works if inS and inW are 0
	 * Faster than operator<
	 */
	bool tinyEdgeCompare(const tinyEdge &a, const tinyEdge &b)
	{
		return a.target < b.target;
	}

	/*
	 * Works regardless of status of front bits
	 */
	bool operator<(const tinyEdge &a, const tinyEdge &b)
	{
		return a.getId() < b.getId();
	}

	/*
	 * Full edge class 'fEdge'
	 */
	class fEdge
	{
	public:
		node_id x;
		node_id y;

		mutable double w;
		mutable bool inS = false;

		fEdge() {}
		fEdge(node_id xx, node_id yy) : x(xx), y(yy), w(1.0) {}
		fEdge(node_id xx, node_id yy, double ww) : x(xx), y(yy), w(ww) {}
		fEdge(const fEdge &rhs)
		{
			x = rhs.x;
			y = rhs.y;
			w = rhs.w;
		}
	};

	struct fEdgeLT
	{
		bool operator()(const fEdge &f1, const fEdge &f2)
		{
			node_id firstL;
			node_id firstR;
			node_id secL;
			node_id secR;

			if (f1.x > f1.y)
			{
				firstL = f1.y;
				firstR = f1.x;
			}
			else
			{
				firstL = f1.x;
				firstR = f1.y;
			}

			if (f2.x > f2.y)
			{
				secL = f2.y;
				secR = f2.x;
			}
			else
			{
				secL = f2.x;
				secR = f2.y;
			}

			if (firstL != secL)
				return (firstL < secL);
			else
				return (firstR < secR);
		}
	};

	// Ignores edge weight, undirected
	bool operator==(const fEdge &f1, const fEdge &f2)
	{
		return ((f1.x == f2.x) && (f1.y == f2.y)) || ((f1.x == f2.y) && (f2.x == f1.y));
	}

	// For printing, ignores weights
	ostream &operator<<(ostream &os, const fEdge &f2)
	{
		os << '(' << f2.x << ',' << f2.y << ')' << ' ';
		return os;
	}

	// Node class
	class tinyNode
	{
	public:
		vector<tinyEdge> neis;
		double wht;
		tinyNode() {}
		tinyNode(const tinyNode &rhs)
		{
			neis.assign(rhs.neis.begin(), rhs.neis.end());
		}
	};

	class tinyGraph
	{
	public:
		vector<tinyNode> adjList;//danh sách đỉnh
		unsigned n;//số đỉnh
		unsigned m;//số cạnh
		double total_cost;//tổng cost của graph
		Logger logg;

		double preprocessTime;
		tinyGraph()
		{
			n = 0;
			m = 0;
		}

		tinyGraph(const tinyGraph &h)
		{
			adjList.assign(h.adjList.begin(), h.adjList.end());
			n = h.n;
			m = h.m;
			preprocessTime = h.preprocessTime;
		}

		void assign(const tinyGraph &h)
		{
			adjList.assign(h.adjList.begin(), h.adjList.end());
			n = h.n;
			m = h.m;
			preprocessTime = h.preprocessTime;
		}

		void init_empty_graph()
		{
			tinyNode emptyNode;
			adjList.assign(n, emptyNode);
		}

		size_t getDegree(node_id v)//lấy bậc của 1 đỉnh
		{
			return adjList[v].neis.size();
		}

		size_t coverAdjacent(node_id v, vector<bool> &covered, vector<size_t> &nodesAdded = emptySizetVector)
		{
			size_t deg = 0;
			if (!covered[v])
			{
				++deg;
				nodesAdded.push_back(v);
				covered[v] = true;
			}
			for (size_t i = 0; i < adjList[v].neis.size(); ++i)
			{
				node_id w = adjList[v].neis[i].getId();
				if (!(covered[w]))
				{
					covered[w] = true;
					++deg;
					nodesAdded.push_back(w);
				}
			}
			return deg;
		}

		size_t getWeightedDegree(node_id v)
		{
			size_t wd = 0;
			for (size_t i = 0; i < adjList[v].neis.size(); ++i)
			{
				wd += adjList[v].neis[i].weight;
			}
			return wd;
		}		

		size_t getDegreeMinusSet(node_id v, vector<bool> &set)
		{
			size_t deg = 0;
			for (size_t i = 0; i < adjList[v].neis.size(); ++i)
			{
				node_id w = adjList[v].neis[i].getId();
				if (!(set[w]))
				{
					++deg;
				}
			}
			return deg;
		}

		size_t getWeightedDegreeMinusSet(node_id v, vector<bool> &set)
		{
			size_t deg = 0;
			for (size_t i = 0; i < adjList[v].neis.size(); ++i)
			{
				node_id w = adjList[v].neis[i].getId();
				if (!(set[w]))
				{
					deg += adjList[v].neis[i].weight;
				}
			}
			return deg;
		}

		
		/*
		 * Replaces graph with undirected, unweighted ER graph with n nodes
		 * edge prob p
		 */
		void erdos_renyi_undirected(node_id n, double p)
		{
			adjList.clear();
			this->n = n;
			this->m = 0;
			init_empty_graph();
			uniform_real_distribution<double> dist(0.0, 1.0);

			for (node_id i = 0; i < n; ++i)
			{
				for (node_id j = i + 1; j < n; ++j)
				{
					double rand = dist(gen);
					if (rand < p)
					{
						++this->m;
						add_edge_immediate(i, j,iRand());
					}
				}
			}
			for (node_id i = 0; i < n; ++i)
			{
				double tmp=0;
				vector<tinyEdge> &neis = adjList[i].neis;
				for (size_t j = 0; j < neis.size(); ++j)
				{
					tmp+=neis[j].weight;
				}
				adjList[i].wht=fun_g(tmp);
			}
		}

		//Replaces graph with undirected, BA graph
		void barabasi_albert(node_id n, size_t m0, size_t m)
		{
			adjList.clear();
			this->n = n;
			this->m = 0;
			init_empty_graph();

			for (node_id u = 0; u < m0; ++u)
			{
				for (node_id v = u + 1; v < m0; ++v)
				{
					++this->m;
					add_edge_immediate(u, v, iRand());
				}
			}

			uniform_real_distribution<double> dist(0.0, 1.0);
			node_id u = m0;
			while (u < n)
			{
				for (size_t i = 0; i < m; ++i)
				{
					for (size_t j = 0; j < u; ++j)
					{
						double rand = dist(gen);
						size_t dj = getDegree(j);
						if (rand < static_cast<double>(dj) / (2 * this->m))
						{
							++this->m;
							fEdge ee(u, j);
							add_edge(ee);
							break;
						}
					}
				}
				++u;
			}
		}

		/*
		 * Adds undirected edge immediately into graph
		 */
		void add_edge_immediate(unsigned from, unsigned to, double wht)
		{
			tinyEdge FT(to, wht);
			tinyEdge TF(from, wht);
			adjList[from].neis.push_back(FT);
			adjList[to].neis.push_back(TF);
		}

		/*
		 * Adds half of undirected edge, keeping adj. list sorted by target id
		 * Checks to make sure graph remains simple, does not add edge
		 * otherwise
		 */
		bool add_edge_half(node_id from, node_id to, vector<tinyEdge>::iterator &edgeAdded, double wht = 1)
		{
			if (from == to)
				return false;

			vector<tinyEdge> &v1 = adjList[from].neis;

			auto it = v1.begin();
			while (it != v1.end())
			{
				if (it->getId() >= to)
					break;

				++it;
			}

			tinyEdge newEdge(to, wht);

			if (it != v1.end())
			{
				if (it->getId() == to)
				{
					// This edge already exists, so do not add it
					return false;
				}
				// The element should be inserted
				edgeAdded = v1.insert(it, newEdge); // O( max_deg )
				++m;
				return true;
			}

			edgeAdded = v1.insert(it, newEdge);
			++m;
			return true;
		}

		bool add_edge(fEdge &e)
		{
			vector<tinyEdge>::iterator tmp;
			if (add_edge_half(e.x, e.y, tmp, e.w))
			{
				add_edge_half(e.y, e.x, tmp, e.w);
				return true;
			}
			return false;
		}

		bool add_edge(node_id &from, node_id &to,double w = 1)
		{
			vector<tinyEdge>::iterator tmp;
			if (add_edge_half(from, to, tmp, w))
			{
				add_edge_half(to, from, tmp, w);

				return true;
			}

			return false;
		}

		double getEdgeWeight(node_id from, node_id to)
		{
			double w;
			vector<tinyEdge> &v1 = adjList[from].neis;

			auto it = v1.begin();
			while (it != v1.end())
			{
				if (it->getId() >= to)
					break;
				++it;
			}

			if (it != v1.end())
			{
				if (it->getId() == to)
				{
					w = it->weight;
					return w;
				}
			}
			return 0;
		}

		double remove_edge_half(node_id from, node_id to)
		{
			if (from == to)
				return 0;
			double w;

			vector<tinyEdge> &v1 = adjList[from].neis;

			auto it = v1.begin();
			while (it != v1.end())
			{
				if (it->getId() >= to)
					break;

				++it;
			}

			if (it != v1.end())
			{
				if (it->getId() == to)
				{
					w = it->weight;
					v1.erase(it);
					--m;
					return w;
				}
			}

			return 0;
		}		

		void read_bin(string fname)
		{
			this->adjList.clear();
			this->m = 0;
			this->n = 0;

			ifstream ifile(fname.c_str(), ios::in | ios::binary);
			unsigned n;
			ifile.read((char *)&n, sizeof(node_id));
			ifile.read((char *)&preprocessTime, sizeof(double));

			this->n = n;

			init_empty_graph();
			size_t ss;
			tinyEdge temp;
			node_id nei_id;
			double w;
			double weight=1.0;
			total_cost=0;
			for (unsigned i = 0; i < n; ++i)
			{
				ifile.read((char *)&weight, sizeof(double));
				ifile.read((char *)&ss, sizeof(size_t));
				adjList[i].neis.assign(ss, temp);
				adjList[i].wht=weight;
				total_cost+=weight;
				this->m += ss;
				for (unsigned j = 0; j < ss; ++j)
				{
					ifile.read((char *)&nei_id, sizeof(node_id));
					ifile.read((char *)&w, sizeof(double));
					adjList[i].neis[j].target = nei_id;
					adjList[i].neis[j].weight = w;
				}
			}

			if (preprocessTime == 0.0)
			{
				clock_t t_start = clock();
				for (unsigned i = 0; i < n; ++i)
				{
					sort(adjList[i].neis.begin(), adjList[i].neis.end(), tinyEdgeCompare);
				}
				preprocessTime = double(clock() - t_start) / CLOCKS_PER_SEC;
			}
		}

		void write_bin(string fname)
		{
			ofstream ifile(fname.c_str(), ios::out | ios::binary);
			ifile.write((char *)&n, sizeof(node_id));
			ifile.write((char *)&preprocessTime, sizeof(double));

			size_t ss;
			tinyEdge temp;
			node_id nei_id;
			double w;
			for (unsigned i = 0; i < n; ++i)
			{

				ss = adjList[i].neis.size();
				ifile.write((char *)&ss, sizeof(size_t));

				for (unsigned j = 0; j < ss; ++j)
				{
					nei_id = adjList[i].neis[j].target;
					w = adjList[i].neis[j].weight;
					ifile.write((char *)&nei_id, sizeof(node_id));
					ifile.write((char *)&w, sizeof(double));
				}
			}

			ifile.close();
		}

		void write_edge_list(string fname)
		{
			ofstream ifile(fname.c_str(), ios::out);
			ifile <<n<<" "<<m<<endl;
			size_t ss;
			tinyEdge temp;
			node_id nei_id;
			for (unsigned i = 0; i < n; ++i)
			{
				ifile<<adjList[i].wht<<'\n';
			}

			for (unsigned i = 0; i < n; ++i)
			{
				ss = adjList[i].neis.size();
				for (unsigned j = 0; j < ss; ++j)
				{
					nei_id = adjList[i].neis[j].target;
					if(nei_id>i)
						ifile << i << ' ' << nei_id << ' '<<adjList[i].neis[j].weight<<'\n';
				}
			}
			ifile.flush();
			ifile.close();
		}

		/*
		 * Reads undirected graph from edge list
		 * Returns pre-processing time for graph
		 */
		double read_edge_list(string fname, bool checkSimple = true)
		{
			cout << "Reading edge list from file " << fname << endl;
			ifstream ifile(fname.c_str());
			init_empty_graph();
			uint32_t n,m;
			ifile>>n>>m;	
			this->n=n;
			unsigned from, to;
			double weight = 1.0;
			for(unsigned i=0;i>n;i++)
			{
				ifile>>weight;
				adjList[i].wht=weight;
			}

			for(unsigned i=0;i<m;i++)
			{
				ifile>>from>>to>>weight;
				add_edge_immediate(from, to, weight);			
			}
			
			
			ifile.close();

			clock_t t_start = clock();
			for (unsigned i = 0; i < n; ++i)
			{
				sort(adjList[i].neis.begin(), adjList[i].neis.end(), tinyEdgeCompare);
			}
			preprocessTime = double(clock() - t_start) / CLOCKS_PER_SEC;
			return double(clock() - t_start) / CLOCKS_PER_SEC;
		}

		/*
		 * Reads undirected graph from edge list
		 * Returns pre-processing time for graph
		 */
		double read_directed_edge_list(string fname, bool checkSimple = true)
		{
			logg << "Reading edge list from file " << fname << endL;
			ifstream ifile(fname.c_str());
			bool weighted;
			uint32_t n;

			string sline;
			stringstream ss;
			unsigned line_number = 0;
			m = 0;
			while (getline(ifile, sline))
			{
				if (sline[0] != '#')
				{
					ss.clear();
					ss.str(sline);
					if (line_number == 0)
					{
						ss >> n;
						ss >> weighted;
						this->n = n;

						if (weighted)
							logg << "Graph is weighted." << endL;
						else
							logg << "Graph is unweighted." << endL;

						init_empty_graph();
					}
					else
					{
						// have an edge on this line
						unsigned from, to;
						double weight = 1.0;
						ss >> from;
						ss >> to;

						if (weighted)
						{

							ss >> weight;
						}

						if (!checkSimple)
							add_edge_immediate(from, to, static_cast<unsigned char>(weight));
						else
						{
							while (to >= this->n)
							{
								++this->n;
								tinyNode emptyNode;
								adjList.push_back(emptyNode);
							}

							while (from >= this->n)
							{
								++this->n;
								tinyNode emptyNode;
								adjList.push_back(emptyNode);
							}
							vector<tinyEdge>::iterator throwAway;
							if (add_edge_half(from, to, throwAway, static_cast<unsigned char>(weight)))
								++m;
						}
					}

					++line_number;
				}
			}
			ifile.close();

			//      logg(INFO, "Sorting neighbor lists..." );
			clock_t t_start = clock();
			for (unsigned i = 0; i < this->n; ++i)
			{
				//	    adjList[i].sort( tinyEdgeCompare );
				sort(adjList[i].neis.begin(), adjList[i].neis.end(), tinyEdgeCompare);
				// update location of mate pairs
				//	for (unsigned j = 0; j < adjList[i].neis.size(); ++j) {
				//	  uint32_t& target = adjList[i].neis[ j ].target;
				//	  uint32_t& mp = adjList[i].neis[ j ].matePairLoc;
				//	  (adjList[ target  ].neis[ mp ]).matePairLoc = j;
				// }
			}
			preprocessTime = double(clock() - t_start) / CLOCKS_PER_SEC;
			logg << "Preprocessing took " << preprocessTime << "s" << endL;
			logg << "Graph has n=" << this->n << ", m=" << m << endL;
			return double(clock() - t_start) / CLOCKS_PER_SEC;
		}

		vector<tinyEdge>::iterator findEdgeInList(node_id source, node_id target)
		{
			vector<tinyEdge> &v1 = adjList[source].neis;
			for (auto it = v1.begin();it != v1.end();++it)
			{
				if (it->getId() == target)
					return it;
			}
			return v1.end(); // Edge not found
		}

		void print(ostream &os)
		{
			for (size_t i = 0; i < adjList.size(); ++i)
			{
				os << i << endl;
				for (size_t j = 0; j < adjList[i].neis.size(); ++j)
				{
					os << adjList[i].neis[j].getId() << ' ';
				}
				os << endl;
			}
		}

		/*
		 * Removed weight of edge removed
		 * 0 if no edge removed
		 */
		unsigned char remove_edge(node_id s, node_id t)
		{
			if (remove_edge_half(s, t) > 0)
			{
				return remove_edge_half(t, s);
			}

			return 0;
		}

		/*
		 * Removed weight of edge removed
		 * 0 if no edge removed
		 */
		unsigned char remove_edge(fEdge &e)
		{
			if (remove_edge_half(e.x, e.y) > 0)
			{
				return remove_edge_half(e.y, e.x);
			}

			return 0;
		}		
	};

	bool operator==(const tinyGraph &f1, const tinyGraph &f2)
	{
		if (f1.n != f2.n)
			return false;
		for (size_t i = 0; i < f1.adjList.size(); ++i)
		{
			if (f1.adjList[i].neis.size() != f2.adjList[i].neis.size())
				return false;
			for (size_t j = 0; j < f1.adjList[i].neis.size(); ++j)
			{
				if (f1.adjList[i].neis[j].target != f2.adjList[i].neis[j].target)
					return false;
			}
		}

		return true;
	}

	class resultsHandler
	{
	public:
		map<string, vector<double>> data;

		void init(string name)
		{
			vector<double> tmp;
			data[name] = tmp;
		}

		void add(string name, double val)
		{
			(data[name]).push_back(val);
		}

		void print(string name)
		{
			vector<double> &vals = data[name];
			double sum = 0.0;
			for (size_t i = 0; i < vals.size(); ++i)
			{
				sum += vals[i];
			}

			cout << sum / vals.size();
		}

		void print(ostream &os, bool printStdDev = true)
		{
			// Print names
			os << '#';
			unsigned index = 1;
			for (auto it = data.begin();
				 it != data.end();
				 ++it)
			{
				os << setw(25);

				os << to_string(index) + it->first;
				++index;
				if (printStdDev)
				{
					os << setw(25);

					os << to_string(index) + it->first;
					++index;
				}
			}
			os << endl;
			for (auto it = data.begin();
				 it != data.end();
				 ++it)
			{
				vector<double> &tmp = it->second;
				double mean = 0.0;
				for (size_t i = 0; i < tmp.size(); ++i)
				{
					mean += tmp[i];
				}
				mean = mean / tmp.size();
				os << setw(25) << mean;

				if (printStdDev)
				{
					double stdDev = 0.0;
					for (size_t i = 0; i < tmp.size(); ++i)
					{
						stdDev += (tmp[i] - mean) * (tmp[i] - mean);
					}
					if (tmp.size() > 1)
					{
						stdDev = stdDev / (tmp.size() - 1);
						stdDev = sqrt(stdDev);
						os << setw(25) << stdDev;
					}
					else
					{
						os << setw(25) << 0.0;
					}
				}
			}
			os << endl;
		}
	};

	class algResult
	{
	public:
		string algName;
		string graphName;
		unsigned n;
		unsigned m;
		double p;
		double preprocess;
	};

	static uint32_t uint32Max = ~(static_cast<uint32_t>(0));

	class NodePath
	{
	public:
		vector<node_id> nodes;
		vector<fEdge> fedges;
		vector<set<fEdge, fEdgeLT>::iterator> edges;

		uint32_t length;
		NodePath() : length(0) {}

		NodePath(const NodePath &rhs) : nodes(rhs.nodes.begin(), rhs.nodes.end()),
										edges(rhs.edges.begin(), rhs.edges.end()), length(rhs.length) {}
		void addNode(const node_id &x, uint32_t w)
		{
			nodes.push_back(x);
			length += w;
		}
		bool ctns(const node_id &x)
		{
			for (size_t i = 0; i < nodes.size(); ++i)
			{
				if (nodes[i] == x)
					return true;
			}

			return false;
		}

		void print(ostream &os)
		{
			for (size_t i = 0; i < nodes.size(); ++i)
			{
				os << nodes[i] << ' ';
			}
			os << "len:" << length << endl;
		}
	};


}

#endif
