#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "test1.h"

using namespace std;

vector <bike> bike_V;
vector <station> station_V;
vector <user> user_V; 



int main (){
    string command;
    while (cin >> command){
        if (command == "add_station"){
            add_station();
            continue;
        }       
        else if (command == "add_bike"){
            add_bike();
            continue;
        } 
        else if (command == "rent_bike"){
            rent_bike();
            continue;
        }
        else if (command == "return_bike"){
            return_bike();
            continue;
        }
        else if (command == "station_status"){
            station_status();
            continue;
        }
        else if (command == "report_users"){
            users_report();
            continue;
        }
   }
}





void add_station (){
    string s_name;
    cin >> s_name;
    int capacity;
    cin >> capacity;
    for (station &temp : station_V){
        if(temp.name == s_name){
            cout << "Station already exists" << "\n";
            return;
        }
    }
    cout << "OK" << endl;
    station new_S{s_name,capacity,0,0 };
    station_V.push_back(new_S);
    return;
    
}

int find_bike_from_id(string bike_id){ // returns the index -1 for not been found
    for (int i = 0; i < bike_V.size(); i++){
        if (bike_V[i].id == bike_id){
            return i;
        }
    }
    return -1;
}

int find_user_from_id(string user_id){ // returns the index -1 for not been found
    for (int i = 0; i < user_V.size(); i++){
        if (user_V[i].name == user_id){
            return i;
        }
    }
    return -1;
}

int find_station_from_id(string station_id){
    for (int i = 0; i < station_V.size(); i++){
        if (station_id == station_V[i].name){
            return i;
       }
    } 
    return -1;   
}

void add_bike(){
    string bike_id, station_id, type;
    cin >> bike_id >> station_id >> type;
    // check if stattion is full
    int s_id = find_station_from_id(station_id);
    if (station_V[s_id].num_r_bikes + station_V[s_id].num_e_bikes < station_V[s_id].capacity){
        bike new_bike{type[0], bike_id, station_id};
        bike_V.push_back(new_bike);
        if (type[0] == 'e'){
            station_V[s_id].num_e_bikes++;
            station_V[s_id].ebikes_in_station.push_back(bike_id);
            cout << "Ok" << endl;
        }
        else if (type[0] == 'r'){
            station_V[s_id].num_r_bikes++;
            station_V[s_id].rbikes_in_station.push_back(bike_id);
            cout << "Ok" << endl;
        }
        return;
    }
    else {
        cout << "Station is full" << endl;
        return;
    }
}


bool rentbike_has_error(int user_index, char type, int station_index){
    bool Error_flag = false;
    if (user_index != -1){
        if (user_V[user_index].has_bike == true){ // check for errors
        cout << "User already has an active ride " << endl;
        }
    }
    
    if (type == 'r')
        if(station_V[station_index].num_r_bikes <= 0){
            cout << "No available bike of this type" << endl;
            Error_flag = true;
        }
    if (type == 'e'){
        if(station_V[station_index].num_e_bikes <= 0){
            cout << "No available bike of this type" << endl;
            Error_flag = true;
        }    
    }   
    
    return Error_flag;
}

void rent_bike(){
    string user_id, station_id, type;
    int time;
    cin >> user_id >> station_id >> type >> time;
    int user_index = find_user_from_id(user_id);
    int station_index = find_station_from_id(station_id);
        bool Error = rentbike_has_error(user_index,type[0],station_index);
        if (Error)
            return; 
    
        // giving bike to user
    cout << "Ok " << endl;
    int bike_index;
    if(type[0] == 'r'){
        bike_index = determine_first_bike(station_V[station_index].rbikes_in_station);
    }
    else if (type[0] == 'e'){
        bike_index = determine_first_bike(station_V[station_index].ebikes_in_station);
    }
    if (user_index != -1){
        user_V[user_index].has_bike = true;
        user_V[user_index].bike_index = bike_index;
        user_V[user_index].time_start = time;
    }
    else{ // new user 
        user new_user{user_id,true, 0, bike_index, time,0,0};
        user_V.push_back(new_user);
    }
    remove_bike(bike_index, station_index);   
    
       
}

void remove_bike(int bike_index, int station_index){
    string bike_name = bike_V[bike_index].id;
    if (bike_V[bike_index].type == 'r'){
        int i = 0;
        while (bike_name != station_V[station_index].rbikes_in_station[i])
            i++;
        station_V[station_index].num_r_bikes -=1;
        station_V[station_index].rbikes_in_station.erase(station_V[station_index].rbikes_in_station.begin() + i); // cuz earase only accept itteraitors
    }
    else{
        int i = 0;
        while (bike_name != station_V[station_index].ebikes_in_station[i])
            i++;
        station_V[station_index].num_e_bikes -=1;
        station_V[station_index].ebikes_in_station.erase(station_V[station_index].ebikes_in_station.begin() + i); // cuz earase only accept itteraitors
    
    }
}

int determine_first_bike(vector<string> bikes_in_station){ // return bike index in bike_V
    sort(bikes_in_station.begin(), bikes_in_station.end());
    return find_bike_from_id(bikes_in_station[0]); 
}

void return_bike(){
    string user_id, station_id;
    int time;
    cin >> user_id >> station_id >> time;
    int user_index = find_user_from_id(user_id);
    if (user_index == -1){
        cout << "User does not have an active ride" << endl;
        return;
    }
    int bike_index = user_V[user_index].bike_index;
    int station_index = find_station_from_id(station_id);
    if (returnbike_has_error(user_index, station_index, bike_V[bike_index].type))
        return;
    return_bike_to_station_for_station(bike_index, station_index);
    int cost = cost_cal(user_index, bike_V[bike_index].type, time);
    int duration_on_bike = return_bike_for_user(user_index,time, cost);
    

    // printing
    cout << "Ride finished: duration = " << duration_on_bike << " mins, cost = " << cost << endl;
}

bool returnbike_has_error(int user_index, int station_index, char type){
    bool error = false;
    if (station_V[station_index].num_r_bikes + station_V[station_index].num_e_bikes == station_V[station_index].capacity){
        error = true;
        cout << "Station is full" << endl;
    }

    if(!user_V[user_index].has_bike){
        error = true;
        cout << "User does not have an active ride" << endl;
    }
    return error;
}

void return_bike_to_station_for_station(int bike_index, int station_index){
    bike_V[bike_index].station_id = station_V[station_index].name;
    switch(bike_V[bike_index].type)
    {
        case 'r':
        station_V[station_index].num_r_bikes += 1;
        station_V[station_index].rbikes_in_station.push_back(bike_V[bike_index].id);
        break;
        case 'e':
        station_V[station_index].num_e_bikes += 1;
        station_V[station_index].ebikes_in_station.push_back(bike_V[bike_index].id);
        break;
    }
}

int return_bike_for_user(int user_index, int time_return, int cost){
    user_V[user_index].has_bike = false;
    user_V[user_index].time_end = time_return;
    int duration = time_return - user_V[user_index].time_start;
    user_V[user_index].riding_record_t += duration;
    user_V[user_index].cost_history += cost;
    return duration;
}

int cost_cal(int user_index, char type, int current_time){
    int duration = current_time - user_V[user_index].time_start;
    int coefitient;
    if (type == 'r')
        coefitient = 2;
    else
        coefitient = 5;
    return coefitient * duration;
}

void station_status(){
    string station_id;
    cin >> station_id;
    int station_index = find_station_from_id(station_id);
    vector<string> combined = station_V[station_index].ebikes_in_station;

    combined.insert(
        combined.end(),
        station_V[station_index].rbikes_in_station.begin(),
        station_V[station_index].rbikes_in_station.end()
    );
    sort(combined.begin(), combined.end());
    int combined_size  = combined.size();
    int capacity = station_V[station_index].capacity;
    // printing
    cout << "Station " << station_id << ":\n" << "Capacity: " << capacity << endl
    << "Available bikes: " << combined_size << " (Regular: " << station_V[station_index].num_r_bikes <<", Electrical: " 
    << station_V[station_index].num_e_bikes 
    << ")" << endl << "Empty slots: " 
    << capacity - combined_size << endl 
    
    << "Bikes: " ;
    int flag = 0;
    station_status_error(station_index, flag);
    if (flag)
        return;
    cout <<combined[0];
    for (int i = 1; i < combined_size; i++ ){
        cout << ", "<< combined[i] ;
    }
    cout << endl;
   
}

void station_status_error(int station_index, int &flag){
    if (station_V[station_index].num_e_bikes + station_V[station_index].num_r_bikes == 0){
        cout << " None" << endl;
        flag = 1;
    }
    return;
}

void users_report() {
    vector<user> users_available;

    for (int i = 0; i < user_V.size(); i++) {
        if (!user_V[i].has_bike && user_V[i].riding_record_t > 0) {
            users_available.push_back(user_V[i]);
        }
    }
    int error =  user_report_problem(users_available);

    sort(users_available.begin(), users_available.end(),
         [](const user& a, const user& b) {            // sort ascaiding in tems of riding recoed
             if (a.riding_record_t != b.riding_record_t)
                return a.riding_record_t > b.riding_record_t;
            return a.name < b.name;
         });
         printing_for_user_report(users_available);
}

void printing_for_user_report(vector<user> & list){
    for (int i = 0; i < list.size(); i++){
        cout << "User "<< list[i].name 
        << ": Total time = " <<  list[i].riding_record_t << " mins, Total cost = "
        << list[i].cost_history << endl;
    }

}

int user_report_problem(vector<user> vec_user){
    if (vec_user.size() == 0){
        cout << "No rides completed yet" << endl;
        return 1;
    }
    else 
        return 0;
}
