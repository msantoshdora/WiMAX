#include<iostream>
#include<bits/stdc++.h>
#include<algorithm>
#include<ctime>
#include<fstream>
#include"request.h"
#include"network.h"
#include<random>
#include<map>
#include<list>
#include"spt.h"


#define  NETWORK_MEAN 6
#define  NETWORK_NODES 6
#define UNIT_TIME 1
#define  REQUESTS 6
#define  WL 5                   //Number of Wave Lengths
#define ALLOWED_SERVE_TIME 5
#define BWT 2000

int wavelengths[WL*2];         //To store available Wavelengths
int currentWavelength;	       //Current Available Wavelength to allocate
std::priority_queue<Request> reqs;	//Data Structure to store Requests generated in unit time to serve
std::priority_queue<Request> reqsS1,reqsS2,reqsS3,reqsS4,reqsS5,reqsS6,reqSatisfy;  //Individual request storage for stations
Request t;
int total_requests;
int stationRequest[6][10];               //To store station-request generated data for 10 unit time
int stationRequestSatisfy[6][10];        //To store request satisfy per station for 10 unit time
int sinrRequestSatisfy[6][10];		//To store request satisfy per sinr for 10 unit time
int currentStation;
int count;
double gHoldTime;
Network n1;

void paus(double holdTime);
void assignWavelengths();
void displayResult(int total_requests,int count_number_of_requests_served);
double displayPath(Request temp,double elapsed_time,clock_t start,bool firstIteration);
bool checkDeadline(Request temp,double delayedTime);
void initializeTable();
void displayStationRequest();
void displayStationRequestSatisfy();
void displaySinrRequestSatisfy();
void generateRequest();
void calculateTotalScore();


/**
 * To Add a Priority to signal to noise ratio to server the request in that order.
 */
bool operator<(const Request& a, const Request& b) {
    return a.total_score < b.total_score;
}

int main(){

	double temp_elapsed_time = 0;     
	int bandwidth_allocated = 0;             //Reqd for Serving or allocating into the frame
	int bandwidth_available =  BWT;		//Reqd for serving or allocating into the frame
	
        count = 0;
	initializeTable();
	bool firstTime = true;		      //This is used to check if it's the first time for pop() of request
	bool firstIteration  =true;           //This is used to check if it's first iteration to serve the request
	generateRequest();                    //Generate the request for the first time
		
	 clock_t start = clock();
	
	int serial = 1;
	temp_elapsed_time = 0;
        //To contiue for indefinite period
	while(count< UNIT_TIME || !reqs.empty()){
			++count;                       //Number of Unit Time
	       
			//currentStation = serial;                       //For stationRequestStatisfy table
			if(firstTime || t.holding_time <= 0){
				calculateTotalScore();
				 start = clock();
				temp_elapsed_time = 0;
				firstTime = false;
				firstIteration = true;
				t = reqs.top();
				reqs.pop();
			std::cout<<"Request Number: "<<serial<<"\n";
			++serial;
			}
			
			temp_elapsed_time = displayPath(t,temp_elapsed_time,start,firstIteration);
			t.holding_time = gHoldTime; 
			std::cout<<"U r here\n";
			if(count < UNIT_TIME){
				generateRequest();
			}

	}
	
		

	//displayStationRequest();
	//displayStationRequestSatisfy();
	//displaySinrRequestSatisfy();
		return 0;
}



void generateRequest(){

		double diff = 0;
		int req;
		 total_requests = 0;
		clock_t begin = clock();
		std::random_device rd;
	        std::default_random_engine generator(rd());
		//std::default_random_engine generator;
		std::default_random_engine request_generator(rd());
		std::poisson_distribution<int> distribution(NETWORK_MEAN);
		std::poisson_distribution<int> no_of_request(REQUESTS);
		
		
	  	
		//For first 6 seconds             
		for(int i = 1; i<= NETWORK_NODES; i++){                  // i is the request number
	
		       req = no_of_request(request_generator);
		       std::cout<<"For Station "<<i<<" , Number of New Requests Generated: "<<req<<"\n";
		      // stationRequest[i-1][count-1] = req;          //Fill the stationRequest matrix for station vs time unit data
	      	       total_requests+=req;
		       Request r1;
		       diff = 0;                         //Intializing to 0
		       begin = clock();		        // Recording the beginning Time
			while((diff < 1) ){
	
				while((diff<1) && (req>0) ){
					
					r1 = n1.generateRequest(i);	
					reqs.push(r1);	
					//network.insert(std::pair<int,Request> (i,r1));             //Storing of request in network
				 	if(i == 1){
						//std::cout<<"Number of Requests Generated..."<<i<<"\n";
						reqsS1.push(r1);					
					}
					if(i == 2){
						reqsS2.push(r1);					
					}
					if(i == 3){
						reqsS3.push(r1);					
					}
					if(i == 4){
						reqsS4.push(r1);					
					}
					if(i == 5){
						reqsS5.push(r1);					
					}
					if(i == 6){
						reqsS6.push(r1);					
					}
					clock_t end = clock();
					diff = double(end - begin) / CLOCKS_PER_SEC;
					req--;
				}
			clock_t end = clock();
			diff = double(end - begin)/CLOCKS_PER_SEC;
		}
	}

}


/**
 * Input: Object of the network which is a mapping from node number to Request Object
 * To Display the path and the allocated Resources 
 */
double displayPath(Request temp1,double se,clock_t start,bool firstIteration){

 	
	 int lsource,ldestination;      // For Iterator way
	 int ls,ld;                     //For Priority Queue way
	 int wl_used;
	 int time_hold;
	 int trequests;
	 int satisfiedRequest = 0;

	 int count_number_of_requests_served = 0;
	 double seconds_elapsed, delayedTime;
	


	 do {   
		 clock_t now1 = clock();
		// temp1 = temp.top();
        	 clock_t delta1 = now1 - temp1.birth ;          //birth is the request generation time.(Time request was born)
         	 delayedTime = (double)delta1 / CLOCKS_PER_SEC * 100.0;                //To calculate the deadline
		// std::cout<<"Waiting to serve...\n";
		 paus(5);
		 while(checkDeadline(temp1,delayedTime)){
		       //Checks if the Requests has not crossed the deadline
			    std::cout<<delayedTime<<"Request Deadline: "<<temp1.deadline<<"\n";
                            std::cout<<"Request Discarded because deadline is over!!\n";
			
				temp1.holding_time = 0;
				gHoldTime = temp1.holding_time;
			   
		 }

		if(temp1.holding_time == 0){
			//To handle exception case
		    t.holding_time = 0;
		    gHoldTime = temp1.holding_time;
		    
		    return std::max(seconds_elapsed,se);		
		}
		 count_number_of_requests_served++;
   		 creat();
		 //temp1 = temp.top();
	
		 ls = temp1.source;
   		 ld = temp1.destination;
		/* if(ls == ld || ((ls==1 && ld == 6) || (ls==6 && ld ==1)  ) ){
			temp.pop();
		  	trequests--;
			continue;
		 } */
		 
	
		 if(firstIteration){
			 std::cout <<"\n\t\tSource: "<< ls<< " Destination: " << ld<<"\n";
			 SPT(ls,ld);
			 std::cout<<"\t\tTotal Score: "<<temp1.total_score<<"\n";
			 std::cout<<"\t\tBandwidth Given: "<<temp1.bandwidth<<"\n";
			 std::cout<<"\t\tProcessing Time: "<<temp1.holding_time<<"\n";
			 std::cout<<"\t\tSignal to Noise Ratio : "<<temp1.sinr<<"\n";
		
		 //Wait for the holding time of the request then show path
	         // paus(5);           //Waiting For 5ms
		 // temp1.holding_time = temp1.holding_time - 5;
		 // gHoldTime = temp1.holding_time;
		 //For Displaying path
		  time_hold = temp1.holding_time;          //for priority queue

//END OF PRIORITY QUEUE DATA STRUCTURE


		 
		
		
	
	
   		 for(int k=top;k>=0;k--){
			if(k==top){
				std::cout<<"               Path: ";
			}
			
 			std::cout<<stack[k];
			
			if(time_hold == 0){
				std::cout<<"Cannot allocate this resource!\n";
				break;			
			}
			if(k>0){
				
				std::cout<<"-->";
				//paus(clock(),&time_hold);
			}
		  }
		std::cout<<"\n";
			firstIteration = false;
		}

 	 	//To check if under given serving time or not.

		clock_t now = clock();
        	clock_t delta = now - start;

         	seconds_elapsed = static_cast<double>(delta) / CLOCKS_PER_SEC + se;
		//std::cout<<"Seconds Elapsed: "<<seconds_elapsed<<"\n";
   		 
 	 } while (seconds_elapsed < ALLOWED_SERVE_TIME && temp1.holding_time > 0);    //&& mycomp((*it++).first, highest)
	
		

	  temp1.holding_time = temp1.holding_time - std::min(seconds_elapsed,temp1.holding_time);
	  std::cout<<"Holding Time: "<<temp1.holding_time<<"\n";
	  gHoldTime = temp1.holding_time;

	//stationRequestSatisfy[currentStation-1][count-1] = satisfiedRequest;


	
	//std::cout<<"Size after removing: "<<temp.size()<<" Seconds Elapsed: "<<seconds_elapsed<<"\n";
 	//displayResult(trequests,count_number_of_requests_served);

return seconds_elapsed;


}



void initializeTable(){
	//int stationRequest[6][10];               //To store station-request generated data for 10 unit time
//int stationRequestSatisfy[6][10];        //To store request satisfy per station for 10 unit time
//int sinrRequestSatisfy[6][10];		//To store request satisfy per sinr for 10 unit time


	for(int i=0;i< NETWORK_NODES;i++){
		for(int j=0;j< UNIT_TIME;j++){
			stationRequest[i][j] = 0;
			stationRequestSatisfy[i][j] = 0;
			sinrRequestSatisfy[i][j] = 0;
		}	
	}
	
}

/**
*  If deadline gets over,then it returns true;else false
*/

bool checkDeadline(Request temp,double delayedTime){

	//if(temp.empty()) return false;
	if(temp.deadline > delayedTime) return false;

	return true;
}

void displayResult(int total_requests,int count_number_of_requests_served){

	std::cout<<"Total Requests: "<<total_requests<<"\n";
	std::cout<<"Requests Handled within 1 unit time: "<<count_number_of_requests_served<<"\n\n";
}



void displayStationRequest(){
	
	std::cout<<"-------------------Station-Request Table--------------"<<"\n";
	for(int i=0;i< NETWORK_NODES;i++){
		for(int j=0;j< UNIT_TIME;j++){
			std::cout<<stationRequest[i][j]<<" ";
			//stationRequestSatisfy[i][j] = 0;
			//sinrRequestSatisfy[i][j] = 0;
		}
		std::cout<<"\n";	
	}
}
void displayStationRequestSatisfy(){
	std::cout<<"-------------------Station-Request-Satisfy Table--------------"<<"\n";
	for(int i=0;i< NETWORK_NODES;i++){
		for(int j=0;j< UNIT_TIME;j++){
			std::cout<<stationRequestSatisfy[i][j]<<" ";
			//stationRequestSatisfy[i][j] = 0;
			//sinrRequestSatisfy[i][j] = 0;
		}
		std::cout<<"\n";	
	}

}
void displaySinrRequestSatisfy(){
	std::cout<<"-------------------Sinr-Request Table--------------"<<"\n";
	for(int i=0;i< NETWORK_NODES;i++){
		for(int j=0;j< UNIT_TIME;j++){
			std::cout<<sinrRequestSatisfy[i][j]<<" ";
			//stationRequestSatisfy[i][j] = 0;
			//sinrRequestSatisfy[i][j] = 0;
		}
		std::cout<<"\n";	
	}
	
}






/**
 *  Assign available wavelengths
 */
void assignWavelengths(){
	int wl1 = 64;
	for(int i = 0; i<WL;i++){
		wavelengths[i] = wl1;
		wavelengths[i+WL] = wl1;  //To keep a temporary Storage of the wavelengths 
		wl1= wl1*2;			
	}
}
/**
*  To give a pause till the holding time of the serving request gets finished.
*/
void paus(double holdTime){
	std::cout<<"\n              Processing... \n";


	const clock_t start = clock();
	clock_t now = clock();

	clock_t delta = now - start;
	double seconds_elapsed = static_cast<double>(delta) / CLOCKS_PER_SEC;
	while(seconds_elapsed <= (holdTime/10)){
	
		clock_t now = clock();
		clock_t delta = now - start;
		seconds_elapsed = static_cast<double>(delta) / CLOCKS_PER_SEC;
	}
	
}

void calculateTotalScore(){

	Request res;
	std::priority_queue<Request> reqsS, tempo;
	reqsS = reqs;    //Temporary Holding of requests

	while(!reqs.empty()){
		res = reqs.top();
		reqs.pop();
		res.total_score = n1.updateTotalScore(res.sinr,10, res.bandwidth,res.birth);
		tempo.push(res);
	}
	reqs = tempo;
	
	
 	
}
