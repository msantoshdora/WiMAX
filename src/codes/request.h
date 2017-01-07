#ifndef REQUEST_H
#define REQUEST_H

#include<ctime>
#include<bits/stdc++.h>

class Request{
	public:
		int source;
		int destination;
		double holding_time;
		double bandwidth;
		float sinr;
		double deadline;
		double total_score;
		clock_t birth;
	public:
		Request();
		Request(int src, int dest, double h_time, double bw,float sn,double dl,clock_t br,double ts);
		void setRequest(int src, int dest, double h_time, double bw,float sn, double dl,clock_t br,double ts);
		
};

#endif
