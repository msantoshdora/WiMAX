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
        int type;
	float  wt;
	float near;
       	float totalScore;
	float price;
	Request(){}
	Request(string s,string d,float datar,float snr,int at,int dl,int ty,float pric){
		sender = s;
		destination = d;
		datarate = datar;
		sinr = snr;
		arrivalTime = at;
		deadline = dl;		
		type = ty;
		price = pric;
		near = sinr*type;
		totalScore = near + price;
		//wt = 0.5*()/();
	}
};

/**
 * To Add a Priority to deadline for the priority
 */
bool operator<(const Request& a, const Request& b) {
    return a.totalScore < b.totalScore;
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
		int requestType;
		int p;
	public:
		priority_queue<Request> ugs;
		priority_queue<Request> rtps;
		priority_queue<Request> ertps;
		priority_queue<Request> nrtps;
		priority_queue<Request> be;
	        
		void getCurrentRequest();
		void getTrafficData(int traffic);
	        void display(int r);
		void setRequestType(int r);
		int getRequestType();
};

int SubscriberStation::getRequestType(){
	return requestType;
}

void SubscriberStation::setRequestType(int r){
	switch(r){
	case 0: requestType = 10;
		break;
	case 1: requestType = 9;
		break;
	case 2: requestType = 8;
		break;
	case 3: requestType = 6;
		break;
	case 4: requestType = 3;
		break;
	}
}

void SubscriberStation::getCurrentRequest(){
	for(int i=0;i<5;i++){
		switch(i){
		case 0: 
			cout<<"Enter the number of ugs Requests:\n";
			setRequestType(i);
			break;
		case 1:
			cout<<"Enter the number of rtps Requests:\n";
			setRequestType(i);
			break;
		case 2:
			cout<<"Enter the number of ertps Requests:\n";
			setRequestType(i);
			break;
		case 3:
			cout<<"Enter the number of nrtps Requests:\n";
			setRequestType(i);
			break;
		case 4: cout<<"Enter the number of Best Effort Requests:\n";
			setRequestType(i);
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
		//cout<<"Enter the arrival time:\n";
		at = time(0);
		cout<<"Enter the deadline in seconds(just for trial):\n";
		cin>>dl;
		dl = dl+time(0);
		cout<<"Enter price:\n";
		cin>>p;
		if(snr>=5.0){
		 	temp.push(Request(s,d,dr,snr,at,dl,getRequestType(),p));
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

void SubscriberStation::display(int r){
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

/*
 * BaseStation which manages all the calls and bandwidth
 *
 */
class BaseStation{
	private:
		int totalSlots;
		int numberOfSubscriberStation;
		priority_queue<Request> ugs;
		priority_queue<Request> rtps;
		priority_queue<Request> ertps;
		priority_queue<Request> nrtps;
		priority_queue<Request> be;	
		priority_queue<Request> pendingRequest;
	public:
		void getTrafficRequest();
		void totalSubscriberStation();
		void display(int r);
		priority_queue<Request> getRequest(priority_queue<Request> r,priority_queue<Request> temp);
		void getPendingRequest();
		void displayPendingRequest();
		void setTotalSlots();
		int  getTotalSlots();
		void allocateBandwidth();
		int calculateWaitTime(Request r);
		void updateTotalScore();		
//		void displayPendingRequest();
	//	void checkTraffic();
	//	float watRequest();
	//	allocateBandwidth();
};



int BaseStation::calculateWaitTime(Request r){

	int age;
	float score;
	age = time(0)-r.arrivalTime;
	r.deadline = r.deadline-time(0);
	score = 0.5*age + 0.5*r.deadline;
	return score;
}

void BaseStation::setTotalSlots(){
	totalSlots = 300;
}

int BaseStation::getTotalSlots(){
	return totalSlots;
}

void BaseStation::displayPendingRequest(){
	priority_queue<Request> temp;
	temp = pendingRequest;
	while(!temp.empty()){
		Request t;
		t = temp.top();
		cout<<"Sender: "<<t.sender<<" Receiver: "<<t.destination<<"Near: "<<t.totalScore<<"Deadline: "<<t.deadline<<endl;
		temp.pop();
	}	
}

void BaseStation::getPendingRequest(){
	for(int i=0;i<5;i++){
		if(i==0){
			pendingRequest = getRequest(pendingRequest,ugs);
		}
		if(i==1){
			pendingRequest = getRequest(pendingRequest,rtps);
		}	
		if(i==2){
			pendingRequest = getRequest(pendingRequest,ertps);
		}
		if(i ==3){
			pendingRequest = getRequest(pendingRequest,nrtps);
		}
		if(i==4){
			pendingRequest = getRequest(pendingRequest,be);
		}
	}
}

priority_queue<Request> BaseStation::getRequest(priority_queue<Request> r,priority_queue<Request> temp){

		while(!temp.empty()){
			Request t = temp.top();
			temp.pop();
			r.push(t);	
		}
return r;
}

void BaseStation::display(int r){
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
		cout<<t.sender<<" "<<t.destination<<" "<<t.near<<endl;
		
	}     
	
}

void BaseStation::totalSubscriberStation(){
	cout<<"Enter number of Subscriber Station:\n";
	cin>>numberOfSubscriberStation;
}

void BaseStation:: getTrafficRequest(){
	totalSubscriberStation();
//	SubscriberStation ss1,ss2;
	priority_queue<Request> s1,s2,temp;
        int serial = 1;
	while(numberOfSubscriberStation--){
		cout<<"Get Traffic Request for "<<serial<<" station.\n";
		serial++;
		SubscriberStation ss;
		ss.getCurrentRequest();
	for(int i=0;i<5;i++){
	switch(i){
		case 0: 
//  			temp = ugs;
			s1  = ss.ugs;
			break;
		case 1:
//			temp = rtps;
			s1 = ss.rtps;
			break;
		case 2: 
//			temp = ertps;
			s1 = ss.ertps;
			break;
		case 3:
//			temp = nrtps;
			s1 = ss.nrtps;
			break;
		case 4: 
//			temp = be;
			s1 = ss.be;
			break; 
		}
/*
		while(!s1.empty()){
			Request t;
			t = s1.top();
			s1.pop();
			temp.push(t);
	
		}
		
*/		switch(i){
		 case 0: ugs = getRequest(ugs,s1);
			break;
		 case 1: rtps = getRequest(rtps,s1);
			break;
	       	 case 2: ertps = getRequest(ertps,s1);
			break;
		 case 3: nrtps = getRequest(nrtps,s1);
			break;
		case 4: be = getRequest(be,s1);
			break;
		}
	
     }	
//	ss.display(0);	
}


}



void BaseStation::allocateBandwidth(){
	int ts,sum=0;
	float temp;
	setTotalSlots();	
	ts = getTotalSlots();
	while(sum<=ts && !pendingRequest.empty()){
        	Request t = pendingRequest.top();
		temp = t.datarate;

		if(temp>=14.2875 && temp<=21.4285){
			sum+=2;
		}
		if(temp>=21.4285 && temp<=28.5714){
			sum+=2;
		}
		if(temp>=28.5714 && temp<=42.8570){
			sum+=4;
		}
		
		if(temp>=42.8570 && temp<=57.1428){
			sum+=6;
		}
		if(temp>=57.1428 && temp<=64.2857){
			sum+=6;
		}
		
	
			pendingRequest.pop();		
		
		
	}		
cout<<"Slots wasted: "<<ts-sum<<endl;
}


void BaseStation::updateTotalScore(){
	priority_queue<Request> temp;
	temp = pendingRequest;
	while(!pendingRequest.empty()){
		Request t;
		t= pendingRequest.top();
		pendingRequest.pop();
		t.totalScore = t.price;//+t.near+calculateWaitTime(t);
		temp.push(t);
		 	
	}	
pendingRequest = temp;
}

int main(){
 
BaseStation bs;
bs.getTrafficRequest();
bs.getPendingRequest();
//bs.display(0);
bs.displayPendingRequest();
bs.updateTotalScore();
bs.allocateBandwidth();
cout<<"Requests Available for next Frame: "<<endl;
bs.displayPendingRequest();
  return 0;
}
