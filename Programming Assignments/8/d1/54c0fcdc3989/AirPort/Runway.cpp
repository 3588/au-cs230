#include"Runway.h"

Runway::Runway(int limit)
{
	queue_limit=limit;
	num_land_requests=num_takeoff_requests=0;
	num_landings=num_takeoffs=0;
	num_land_accepted=num_takeoff_accepted=0;
	num_land_refused=num_takeoff_refused=0;
	land_wait=takeoff_wait=idle_time=0;
}

bool Runway::can_land(const Plane &current)
{
	bool result;
	if(landing.size()<queue_limit){
		result=true;
		landing.push(current);
	}
	else result=false;
	num_land_requests++;
	if(result!=true)num_land_refused++;
	else num_land_accepted++;
	return result;
}

bool Runway::can_depart(const Plane &current)
{
	bool result;
	if(takeoff.size()<queue_limit){
		result=true;
		takeoff.push(current);
	}
	else result=false;
	num_takeoff_requests++;
	if(result!=true)num_takeoff_refused++;
	else num_takeoff_accepted++;
	return result;
}

Runway_activity Runway::activity(int time, Plane &moving)
{
	Runway_activity in_progress;
	if(!landing.empty()){
		moving=landing.front();
		land_wait+=time-moving.started();
		num_landings++;
		in_progress=lands;
		landing.pop();
	}else if(!takeoff.empty()){
		moving=takeoff.front();
		takeoff_wait+=time-moving.started();
		num_takeoffs++;
		in_progress=takeoffs;
		takeoff.pop();
	}else{
		idle_time++;
		in_progress=idle;
	}
	return in_progress;
}

void Runway::shut_down(int time)const
{
  cout << "Simulation has concluded after " << time << " time units." << endl
        << "Total number of planes processed "
        << (num_land_requests + num_takeoff_requests) << endl
        << "Total number of planes asking to land "
        << num_land_requests << endl
        << "Total number of planes asking to take off "
        << num_takeoff_requests << endl
        << "Total number of planes accepted for landing "
        << num_land_accepted << endl
        << "Total number of planes accepted for takeoff "
        << num_takeoff_accepted << endl
        << "Total number of planes refused for landing "
        << num_land_refused << endl
        << "Total number of planes refused for takeoff "
        << num_takeoff_refused << endl
        << "Total number of planes that landed "
        << num_landings << endl
        << "Total number of planes that took off "
        << num_takeoffs << endl
        << "Total number of planes left in landing queue "
        << landing.size() << endl
        << "Total number of planes left in takeoff queue "
        << takeoff.size() << endl;
   cout << "Percentage of time runway idle "
        << 100.0 * (( float ) idle_time) / (( float ) time) << "%" << endl;
   cout << "Average wait in landing queue "
        << (( float ) land_wait) / (( float ) num_landings) << " time units";
   cout << endl << "Average wait in takeoff queue "
        << (( float ) takeoff_wait) / (( float ) num_takeoffs)
        << " time units" << endl;
   cout << "Average observed rate of planes wanting to land "
        << (( float ) num_land_requests) / (( float ) time)
        << " per time unit" << endl;
   cout << "Average observed rate of planes wanting to take off "
        << (( float ) num_takeoff_requests) / (( float ) time)
        << " per time unit" << endl;
}