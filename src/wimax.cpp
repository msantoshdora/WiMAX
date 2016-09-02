#include<bits/stdc++.h>


#define m 64
#define FRAMESLOTS 300
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
		
	}
};

/**
 * To Add a Priority to deadline for the priority
 */
bool operator<(const Request& a, const Request& b) {
    return a.totalScore < b.totalScore;
}

/**
 * It is the Relay Station,which relays the request to base Station.
 */
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

/**
 * To get the request Type,if it is UGS,RTPS,NRTPS,BE,etc.
 */
int SubscriberStation::getRequestType(){
	return requestType;
}

/**
 * It gives an integer value to the request type.
 */
void SubscriberStation::setRequestType(int r){
	switch(r){
	case 0: requestType = 10;
		break;
	case 1: requestType = 9;
		break;
	case 2: requestType = 8;
		break;
	case 3: requestType = 7;
		break;
	case 4: requestType = 6;
		break;
	}
}

/**
 * To get a new request
 */
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
		/*
 		*  Check if the input is in correct Format or not
 		* 	
 		*/
		while(cin.fail()){
			cout<<"Wrong Input!!!\n";
			cin.clear();
			cin.ignore(256,'\n');
			cout<<"Enter Again:\n";
			cin>>numberOfRequests;
		}
		getTrafficData(i);
        }
}

/**
 * To get a traffic data
 */
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
		}else{
			cout<<"Request is discarded because of poor channel quality\n";
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
	totalSlots = FRAMESLOTS;
}

int BaseStation::getTotalSlots(){
	return totalSlots;
}

void BaseStation::displayPendingRequest(){
	priority_queue<Request> temp;
	temp = pendingRequest;
       cout<<"Source\t"<<"Receiver\t"<<"Score\t"<<"Type\t"<<"Deadline\n";
	while(!temp.empty()){
		Request t;
		t = temp.top();
cout<<t.sender<<"\t"<<t.destination<<"\t\t"<<t.totalScore<<"\t"<<t.type<<"\t"<<t.deadline<<endl;
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
		switch(i){
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
		sum +=temp*1024/log2(m);	
			pendingRequest.pop();		
			
	}	
	
cout<<"Slots wasted: "<<ts-sum<<endl;
}


void BaseStation::updateTotalScore(){
	priority_queue<Request> temp;
	float w1,w2,w3,w4,sum,a1;  //w1 = sinr w2 = price w3= deadline w4=type
//	temp = pendingRequest;
	
	while(!pendingRequest.empty()){
		Request t;
		t= pendingRequest.top();
		pendingRequest.pop();
		
		sum = t.price + t.sinr + calculateWaitTime(t)+ t.type;
		w1 = t.sinr/sum;
		w2 = t.price/sum;
		w3 = calculateWaitTime(t)/sum;
		w4  = 1 - (w1+w2+w3);
		t.totalScore = w1*t.sinr + w2*t.price + w3*calculateWaitTime(t) + w4*t.type;

	
		temp.push(t);
		 	
	}	
pendingRequest = temp;
}

int main(){
 
BaseStation bs;
bs.getTrafficRequest();
bs.getPendingRequest();
//bs.display(0);
//bs.displayPendingRequest();
bs.updateTotalScore();
bs.displayPendingRequest();
bs.allocateBandwidth();
//cout<<"Requests Available for next value:\n";
//bs.displayPendingRequest();
  return 0;
}
