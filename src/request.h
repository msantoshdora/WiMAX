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
