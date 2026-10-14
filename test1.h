#ifndef TEST1_H
#define TEST1_H

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>

using namespace std;

struct bike {
    char type; // r for regular, e for electrical
    string id;
    string station_id;
};

struct station {
    string name;
    int capacity;
    int num_e_bikes;
    int num_r_bikes;
    vector<string> ebikes_in_station;
    vector<string> rbikes_in_station;
};

struct user {
    string name;
    bool has_bike;
    int riding_record_t;
    int bike_index;
    int time_start;
    int time_end;
    int cost_history;
};

// Global variables
extern vector<bike> bike_V;
extern vector<station> station_V;
extern vector<user> user_V;

// Function declarations
void add_station();
int find_bike_from_id(string bike_id);
int find_user_from_id(string user_id);
int find_station_from_id(string station_id);
void add_bike();
bool rentbike_has_error(int user_index, char type, int station_index);
void rent_bike();
void remove_bike(int bike_index, int station_index);
int determine_first_bike(vector<string> bikes_in_station);
void return_bike();
bool returnbike_has_error(int user_index, int station_index, char type);
void return_bike_to_station_for_station(int bike_index, int station_index);
int cost_cal(int user_index, char type, int current_time);
void station_status();
void station_status_error(int station_index, int &flag);
void users_report();
void printing_for_user_report(vector<user> &list);
int user_report_problem(vector<user> vec_user);
int return_bike_for_user(int user_index, int time_return, int cost);

#endif // TEST1_H