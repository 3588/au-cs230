#include"Runway.h"
#include"Random.h"
//#include"AllHead.h"

void run_idle(int time)
{
	cout<<time<<":Runway is idle."<<endl;
}

void initialize(int &end_time,int &queue_limit,double &arrival_rate,double &departure_rate)
{
	cout<<"This program simulates an airport with only one runway."<<endl
		<<"One plane can land or depart in each unit of time."<<endl;
	cout<<"Up to what number of planes can be waiting to land "
		<<"or take off at any time? "<<flush;
	cin>>queue_limit;

	cout<<"How many units of time will the simulation run?" << flush;
	cin>>end_time;
	bool acceptable;
	do{
		cout<<"Expected number of arrivals per unit time?"<<flush;
		cin>>arrival_rate;
		cout<<"Expected number of departures per unit time?"<<flush;
		cin>>departure_rate;
		if(arrival_rate<0.0||departure_rate<0.0)cerr<<"These rates must be nonnegative."<<endl;
		else acceptable=true;
		if(acceptable&&arrival_rate+departure_rate>1.0)cerr<<"Safety Warning:This airport will become saturated."<<endl;
	}while(!acceptable);
}

int main()
{
	int end_time;
	int queue_limit;
	int flight_number=0;
	double arrival_rate,departure_rate;
	initialize(end_time,queue_limit,arrival_rate,departure_rate);
	Random variable(false);
	Runway small_airport(queue_limit);
	for(int current_time=0;current_time<end_time;current_time++){
		int number_arrivals=variable.poisson(arrival_rate);
		for(int i=0;i<number_arrivals;i++){
			Plane current_plane(flight_number++,current_time,arriving);
			if(small_airport.can_land(current_plane)!=true)current_plane.refuse();
		}
		int number_departures=variable.poisson(departure_rate);
		for(int j=0;j<number_departures;j++){
			Plane current_plane(flight_number++,current_time,departing);
			if(small_airport.can_depart(current_plane)!=true)current_plane.refuse();
		}
		Plane moving_plane;
		switch(small_airport.activity(current_time,moving_plane)){
			case lands:
				moving_plane.land(current_time);
				break;
			case takeoffs:
				moving_plane.fly(current_time);
				break;
			case idle:
				run_idle(current_time);
		}
	}
	small_airport.shut_down(end_time);
	system("pause");
	return 0;
}