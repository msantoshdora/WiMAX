#ifndef NETWORK_H
#define NETWORK_H


#define V 6

class Network{
	private:
		int src;
		int dest;
		double bw;
		double htime;
		float sn;
		double dl;
		double ts;
		clock_t br;
		int dist[V];  // The output array. dist[i] will hold
                // the shortest distance from src to i
 
	          // sptSet[i] will true if vertex i is included / in shortest
	         // path tree or shortest distance from src to i is finalized
	         bool sptSet[V];
	 
	         // Parent array to store shortest path tree
	         int parent[V];	
	public:
		Network();
		Request generateRequest(int source);
		double calculateTotalScore();
		double calculateWaitTime(const clock_t start);
		double updateWaitTime();
		double updateTotalScore(float sinr,int type, double price,const clock_t start);
		void findShortestPath(int graph[V][V], int source);
		double allocateBandwidth();
		double assignDeadline();
		float allocateSinr();
		void beginServing();
		void releaseResources();
		int minDistance(int dist[], bool sptSet[]);
		void printPath(int j);
		
};

#endif
