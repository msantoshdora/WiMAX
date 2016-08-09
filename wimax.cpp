#include<bits/stdc++.h>

using namespace std;


struct request{
	string sender;
	string destination;
	float datarate;
	float sinr;  //Signal to noise Ratio
	int arrivalTime;
	int deadline;
};

class SubscriberStation{
	private:
		int numberOfRequests;
	protected:
		queue<request> ugs;
		queue<request> rtps;
		queue<request> ertps;
		queue<request> nrtps;
		queue<request> be;
	public:
		void getCurrentRequest();
		void getTrafficData(int traffic);
};

void SubscriberStation::getCurrentRequest(){
	for(int i=0;i<5;i++){
		switch(i){
		case 0: 
			cout<<"Enter the number of ugs Requests:\n";
			break;
		case 1:
			cout<<"Enter the number of rtps Requests:\n";
			break;
		case 2:
			cout<<"Enter the number of ertps Requests:\n";
			break;
		case 3:
			cout<<"Enter the number of nrtps Requests:\n";
			break;
		case 4: cout<<"Enter the number of Best Effort Requests:\n";
			break;	
		}
		cin>>numberOfRequests;
		getTrafficData(i);
        }
}


void SubscriberStation:: getTrafficData(int traffic){
	request obj;
	queue<request> temp;
        while(numberOfRequests--){
		cout<<"Enter the sender:\n";
		cin>>obj.sender;
		cout<<"Enter the receiver:\n";
		cin>>obj.destination;
		cout<<"Enter the sinr value:\n";
		cin>>obj.sinr;
		cout<"Enter the datarate value:\n";
		cin>>obj.datarate;
		cout<<"Enter the arrival time:\n";
		cin>>obj.arrivalTime;
		cout<<"Enter the deadline:\n";
		cin>>obj.deadline;
		
		if(obj.sinr>=5.0){
		 	temp.push(obj);
		}
	}	

	switch(traffic){
		case 0: ugs = temp;
			break;
		case 1:
			rtps = temp;
			break;
		case 2:
			ertps = temp;
			break;
		case 3:
			nrtps = temp;
			break;
		case 4:
			be = temp;
			break;	
	}
	
}
//class BaseStation: public SubscriberStation{
//	public:
//		 getTrafficRequest();
//		checkTraffic();
//		watiRequest();
//		allocateBandwidth();
//};


int main(){
  SubscriberStation obj;
  return 0;
}
