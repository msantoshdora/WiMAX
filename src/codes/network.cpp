#include"request.h"
#include"network.h"
#include<iostream>
#include<bits/stdc++.h>
#include<ctime>
#include<random>
#include<stdlib.h>
#include<time.h>
#include<climits>
#include<sys/time.h> //For gettimeofday()


#define V 6

//Network constructor
Network::Network(){}

//Network request generator
Request Network::generateRequest(int source){
	Request obj;
        std::random_device rd, rd1;			//For randomly generating default_random_engine
        std::default_random_engine generator(rd());
	std::default_random_engine generator1(rd1());
	std::poisson_distribution<int> distribution(10);
	std::poisson_distribution<int> distribution1(10);
	/* initialize random seed */
	srand(time(NULL));

	src = source;
	//dest = rand() % 6 +1;
	dest = distribution1(generator1)%6 + 1;
        br = clock();
	bw = allocateBandwidth();
	//std::cout<<bw<<"This is the bandwidth allocated\n";
	htime = distribution(generator);
	sn = allocateSinr();
	dl = assignDeadline();
	
	ts = updateTotalScore(sn,10,bw,br);
	obj.setRequest(src,dest,htime,bw,sn,dl,br,ts);
//	std::cout<<"Holding Time: "<<htime<<"\n";	
	return obj;
}

double Network::allocateBandwidth(){

	/* initialize random seed */
	timeval t1;
	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec);             //Seeding is better in this case

	int randNum = (rand() % 25) + 1;
	//std::cout<<randNum<<"Checking\n";
	if(randNum>=1 && randNum<=5)
		return 64;
	if(randNum>= 6 && randNum<=10)
		return 128;
	if(randNum>=11 && randNum<=15)
		return 256;
	if(randNum>=16 && randNum<=20)
		return 512;
	if(randNum>=21 && randNum<=25)
		return 1024;
}

double Network::assignDeadline(){
	/* initialize random seed */
	timeval t1;
	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec);             //Seeding is better in this case

	int randNum = (rand() % 25) + 1;
	//std::cout<<randNum<<"Checking\n";
	if(randNum>=1 && randNum<=5)
		return 6400;
	if(randNum>= 6 && randNum<=10)
		return 12800;
	if(randNum>=11 && randNum<=15)
		return 256000;
	if(randNum>=16 && randNum<=20)
		return 5120000;
	if(randNum>=21 && randNum<=25)
		return 10240000000;
	
}

float Network::allocateSinr(){
	/* initialize random seed */
	timeval t1;
	gettimeofday(&t1, NULL);
	srand(t1.tv_usec * t1.tv_sec);             //Seeding is better in this case

	int randNum = (rand() % 25) + 1;
	//return randNum;
	//std::cout<<randNum<<"Checking\n";
	if(randNum>=1 && randNum<=5)
		return 6.4;
	if(randNum>= 6 && randNum<=10)
		return 12.8;
	if(randNum>=11 && randNum<=15)
		return 25.6;
	if(randNum>=16 && randNum<=20)
		return 5.12;
	if(randNum>=21 && randNum<=25)
		return 1.024;


}

//Initialize the serving
void Network::beginServing(){}

//Release the Resource after finishing the request
void Network::releaseResources(){}

//A utility function to find the vertex with minimum distance
//value, from the set of vertices not yet included in shortest
//path tree
int Network::minDistance(int dist[], bool sptSet[]){
    // Initialize min value
    int min = INT_MAX, min_index;
 
    for (int v = 0; v < V; v++)
        if (sptSet[v] == false && dist[v] <= min)
            min = dist[v], min_index = v;
 
    return min_index;

}

// To print the shortest path from source to destination
void Network::printPath(int destin){
    // Base Case : If j is source
    if (parent[destin]==-1)
        return;
// 	std::cout<<"Destination: "<<destin<<" ";
    printPath( parent[destin]);
   
    printf("%d ", destin);
}


//Find shortest path to allocate the resources
void Network::findShortestPath(int graph[V][V], int src){

	
 
    // Initialize all distances as INFINITE and stpSet[] as false
    for (int i = 0; i < V; i++)
    {
        parent[src] = -1;
        dist[i] = INT_MAX;
        sptSet[i] = false;
    }
 
    // Distance of source vertex from itself is always 0
    dist[src] = 0;
 
    // Find shortest path for all vertices
    for (int count = 0; count < V-1; count++)
    {
        // Pick the minimum distance vertex from the set of
        // vertices not yet processed. u is always equal to src
        // in first iteration.
        int u = minDistance(dist, sptSet);
 
        // Mark the picked vertex as processed
        sptSet[u] = true;
 
        // Update dist value of the adjacent vertices of the
        // picked vertex.
        for (int v = 0; v < V; v++)
 
            // Update dist[v] only if is not in sptSet, there is
            // an edge from u to v, and total weight of path from
            // src to v through u is smaller than current value of
            // dist[v]
            if (!sptSet[v] && graph[u][v] &&
                dist[u] + graph[u][v] < dist[v])
            {
                parent[v]  = u;
                dist[v] = dist[u] + graph[u][v];
            }  
    }
	
}




double Network::updateTotalScore(float sinr,int type, double price,const clock_t start){
		double waitTime = calculateWaitTime(start);
		double totalScore,w1,w2,w3,w4;
	       	double sum = sinr + waitTime + type;
		w1 = sinr/sum;
		w2 = price/sum;
		w3 = waitTime/sum;
		w4  = 1 - (w1+w2+w3);
		totalScore = w1*sinr + w2*price + w3*waitTime+ w4*type;
		return totalScore;

}

double Network::calculateWaitTime(const clock_t start){
	clock_t now = clock();
	clock_t delta = now - start;
	double seconds_elapsed = static_cast<double>(delta) / CLOCKS_PER_SEC * 100.0;
	return seconds_elapsed;
}
//double Network::updateWaitTime();
//void Network::updateTotalScore();



