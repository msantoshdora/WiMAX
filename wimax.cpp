#include<bits/stdc++.h>

using namespace std;

class Request{

public:
	string sender;
	string destination;
	float datarate;
	float sinr;  //Signal to noise Ratio
	int arrivalTime;
	int deadline;
 
	Request(string s,string d,float datar,float snr,int at,int dl){
		sender = s;
		destination = d;
		datarate = datar;
		sinr = snr;
		arrivalTime = at;
		deadline = dl;		
	}
};

/**
 * To Add a Priority to deadline for the priority
 */
bool operator<(const Request& a, const Request& b) {
    return a.deadline < b.deadline;
}

class SubscriberStation{
	private:
		int numberOfRequests;
		string s;
		string d;
		float dr;
		float snr;
		int at;
		int dl;
	protected:
		priority_queue<Request> ugs;
		priority_queue<Request> rtps;
		priority_queue<Request> ertps;
		priority_queue<Request> nrtps;
		priority_queue<Request> be;
	public:
		void getCurrentRequest();
		void getTrafficData(int traffic);
		friend void display(int r);
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
	int serial=1;
	priority_queue<Request> temp;
        while(numberOfRequests--){
		cout<<"Enter the Request for "<<serial<<" SubscriberStation:\n";
		cout<<"Enter the sender:\n";
		cin>>s;
		cout<<"Enter the receiver:\n";
		cin>>d;
		cout<<"Enter the datarate value:\n";
		cin>>dr;
                 cout<<"Enter the sinr ratio:\n";
		cin>>snr;
		cout<<"Enter the arrival time:\n";
		cin>>at;
		cout<<"Enter the deadline:\n";
		cin>>dl;
		
		if(snr>=5.0){
		 	temp.push(Request(s,d,dr,snr,at,dl));
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

void display(int r){
 	priority_queue<Request> obj;
	switch(r){
	case 0:obj = ugs;
		break;
	case 1:
		obj = rtps;
		break;
	case 2: obj = ertps;
		break;
	case 3: obj = nrtps;
		break;
	case 4: obj = be;
		break;
	}
	while(!obj.empty()){

		Request t = obj.top();
		obj.pop();
		cout<<t.sender<<" "<<t.destination<<endl;
		
	}     
}

class BaseStation{
	private:
		int numberOfSubscriberStation;
		priority_queue<Request> ugs;
		priority_queue<Request> rtps;
		priority_queue<Request> ertps;
		priority_queue<Request> nrtps;
		priority_queue<Request> be;	
	public:
		void getTrafficRequest();
		void totalSubscriberStation();
	//	void checkTraffic();
	//	watRequest();
	//	allocateBandwidth();
};

void BaseStation::totalSubscriberStation(){
	cout<<"Enter number of Subscriber Station:\n";
	cin>>numberOfSubscriberStation;
}

void BaseStation:: getTrafficRequest(){
	SubscriberStation ss1,ss2;
	priority_queue<Request> s1,s2,temp;
        int serial = 1;
	while(numberOfSubscriberStation--){
		cout<<"Get Traffic Request for: "<<serial<<" station.\n";
		serial++;
		SubscriberStation ss;
		ss.getCurrentRequest();
	for(int i=0;i<5;i++){
		switch(i){
		case 0: temp = ugs;
			s1  = ss.ugs;
			break;
		case 1:
			temp = rtps;
			s1 = ss.rtps;
			break;
		case 2: 
			temp = ertps;
			s1 = ss.ertps;
			break;
		case 3:
			temp = nrtps;
			s1 = ss.nrtps;
			break;
		case 4: 
			temp = be;
			s1 = ss.be;
			break; 
		}

		while(!s1.empty()){
			Request t;
			t = s1.top();
			s1.pop();
			temp.push(t);
	
		}
		
		switch(i){
		 case 0: ugs = temp;
			break;
		 case 1: rtps = temp;
			break;
	       	 case 2: ertps = temp;
			break;
		 case 3: nrtps = temp;
			break;
		case 4: be = temp;
			break;
		}
     }		
}

int main(){
 // SubscriberStation ss1,ss2;
 // ss1.getCurrentRequest();
 // ss2.getCurrentRequest();
BaseStation bs;
bs.getTrafficRequest();
cout<<"To display the Traffic(choose 0-ugs,1 for rtPS,2-ertPS,3,nrtPS,4-BE):\n";
  int r;
cin>>r;
bs.display(r);
  return 0;
}
