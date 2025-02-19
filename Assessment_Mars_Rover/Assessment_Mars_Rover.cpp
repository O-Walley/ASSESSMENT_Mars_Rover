#include <iostream>
#include <string>
#include <vector>
#include <regex>
#include <sstream>
#include <ctype.h>

using namespace std;

struct position {
    int x_pos;
    int y_pos;
    char heading;
};

vector<position> move_robot(position start_pos, string operations) {

    position robot = start_pos;
    vector<position> movement_log;

    vector<char> directions = { 'N','E','S','W' };
    int step_size = 1;


    movement_log.push_back(robot);

    // Loop through string of operations
    for (char& op : operations) {
        
        if (op == 'M') // Move robot one step forwards
        {
            switch (robot.heading)
            {
                case 'N': robot.y_pos += step_size; break; // North/Up
                case 'E': robot.x_pos += step_size; break; // East/Right
                case 'S': robot.y_pos -= step_size; break; // South/Down
                case 'W': robot.x_pos -= step_size; break; // West/Left   
            }

            cout << "Updated position: " << robot.x_pos << "," << robot.y_pos << " heading " << robot.heading << endl;
        }
        else // Rotate robot
        {
            // Find index of current heading char in vector
            auto it = find(directions.begin(), directions.end(), robot.heading);

            if (it != directions.end()) // Found char index
            {
                int index = distance(directions.begin(), it);

                //cout << "Index: " << index << endl;

                //cout << "Currently heading: " << robot.heading << endl;

                if (op == 'R') index = (index + 1) % directions.size(); // Right turn
                else index = (index - 1 + directions.size()) % directions.size(); // Left turn

                //cout << "Index: " << index << endl;

                robot.heading = directions.at(index);

                cout << "Updated heading: " << robot.heading << endl;
            }
            else // Char not found in array (should not be possible)
            {
                cerr << "Direction char not found in array" << endl;
            }
        }

        // Add updated position to log
        movement_log.push_back(robot);
    }

    return movement_log;
}

string get_valid_input(regex pattern) {

    string val;

    // Ask user for input until valid entry
    while (true) {

        getline(cin, val);

        if (!regex_match(val, pattern))
        {
            cerr << "Invalid Input Format.\n" << endl;
        }
        else return val;
    }
}

vector<string> split_string(string val, char delimiter) {

    stringstream sstr(val);
    string seg;
    vector<string> seglist;

    // Split given string into vector using delimiter
    while (getline(sstr, seg, delimiter))
    {
        seglist.push_back(seg);
    }

    return seglist;
}


int main()
{
    char again;

    do
    {
        // Get grid dimensions
        pair <int, int> grid_dim; // (X, Y)
        string grid_str;
        regex grid_pattern(R"(^([1-9]\d*|0),([1-9]\d*|0)$)");

        cout << "Enter grid dimensions in the following format 'width,height' (e.g. 10,16):" << endl;
        
        grid_str = get_valid_input(grid_pattern);

        // Split dimensions string into values using ',' delimiter
        vector<string> dim_values = split_string(grid_str, ',');

        grid_dim.first = stoi(dim_values[0]);
        grid_dim.second = stoi(dim_values[1]);

        cout << "Grid dimensions are " << grid_dim.first << "x" << grid_dim.second << "\n" << endl;



        // Get robot start position with validation
        position robot_1;
        string robot_str;
        regex pos_pattern(R"(^([1-9]\d*|0)-([1-9]\d*|0)-[NESW]$)");

        cout << "Enter a starting grid position in the following format 'X-Y-HEADING'" << endl;
        cout << "Heading must be either N, E, S, or W. (e.g. 2-3-N):" << endl;
        
        robot_str = get_valid_input(pos_pattern);

        // Split start position string into values using '-' delimiter
        vector<string> pos_values = split_string(robot_str, '-');

        robot_1.x_pos = stoi(pos_values[0]);
        robot_1.y_pos = stoi(pos_values[1]);
        robot_1.heading = pos_values[2].c_str()[0];

        cout << "Robot starts at coordinates " << robot_1.x_pos << "," << robot_1.y_pos << " heading " << robot_1.heading << "\n" << endl;



        // Get operations string with validation
        string operations;
        regex op_pattern(R"(^[LRM]+$)");

        cout << "Enter robot operation string (R = turn right, L = Turn left, M = Move one step towards current heading): " << endl;

        operations = get_valid_input(op_pattern);

        cout << "Robot will perform the following operations: " << operations << endl;



        // Perform operations and retrieve the movement log
        vector<position> movement_log_1 = move_robot(robot_1, operations);

        // Get final position
        robot_1 = movement_log_1.back();

        cout << "\nRobot ends at coordinates " << robot_1.x_pos << "," << robot_1.y_pos << " heading " << robot_1.heading << endl;

        cout << "\nTry again? (y/n):" << endl;
        cin >> again;

        cout << "\n\n\n";

    } while (again != 'n');

}
