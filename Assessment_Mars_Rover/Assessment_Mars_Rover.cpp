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

vector<position> move_robot(position start_pos, string operations, pair <int, int> grid_dim, vector<pair <int, int>> obstacles) {

    position robot = start_pos;
    vector<position> movement_log;

    vector<char> directions = { 'N','E','S','W' };
    int step_size = 1;


    movement_log.push_back(robot);

    // Loop through each char in string of operations
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

            // Maintain robot within grid boundaries
            if (robot.x_pos < 0 || robot.x_pos > grid_dim.first || robot.y_pos < 0 || robot.y_pos > grid_dim.second)
            {
                cout << "\n##### WARNING #####" << endl;
                cout << "The operations given lead the robot to position: " << robot.x_pos << "," << robot.y_pos << " heading " << robot.heading << endl;
                cout << "This is out of grid boundaries." << "\n" << endl;

                robot.heading = 'O';
                movement_log.push_back(robot);

                return movement_log;
            }
            else if(!obstacles.empty()) // Check for collision with listed obstacles (e.g. other robots)
            {
                pair <int, int> coords (robot.x_pos, robot.y_pos);
                auto it = find(obstacles.begin(), obstacles.end(), coords);

                if (it != obstacles.end())
                {
                    cout << "\n##### WARNING #####" << endl;
                    cout << "The operations given lead the robot to position: " << robot.x_pos << "," << robot.y_pos << " heading " << robot.heading << endl;
                    cout << "This would result in a collision, as there is an obstacle listed at " << robot.x_pos << "," << robot.y_pos << "\n" << endl;

                    robot.heading = 'X';
                    movement_log.push_back(robot);

                    return movement_log;
                }
            }

            cout << "Updated position: " << robot.x_pos << "," << robot.y_pos << " heading " << robot.heading << endl;
        }
        else // Rotate robot
        {
            // Find index of current heading char in vector
            auto it = find(directions.begin(), directions.end(), robot.heading);

            if (it != directions.end()) // Found char in vector
            {
                int index = distance(directions.begin(), it);

                if (op == 'R') index = (index + 1) % directions.size(); // Right turn
                else index = (index - 1 + directions.size()) % directions.size(); // Left turn      

                robot.heading = directions.at(index);

                cout << "Updated heading: " << robot.heading << endl;
            }
            else // Char not found in vector (should not be possible)
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
        vector<pair <int, int>> obstacles;

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

        
        
        int robot_count = 0;

        while (robot_count < 2) // 2 Robots - Can be changed for more or less
        {

            // Get robot start position with validation
            position robot;
            string robot_str;
            regex pos_pattern(R"(^([1-9]\d*|0)-([1-9]\d*|0)-[NESW]$)");
            vector<string> pos_values;
            bool valid_pos = false;

            cout << "##### ROBOT " << robot_count << " #####" << endl;

            cout << "Enter a starting grid position in the following format 'X-Y-HEADING'" << endl;
            cout << "Heading must be either N, E, S, or W. (e.g. 2-3-N):" << endl;
            
            do
            {
                robot_str = get_valid_input(pos_pattern);

                // Split start position string into values using '-' delimiter
                pos_values = split_string(robot_str, '-');

                // Check to see if attempting to start on obstacle
                pair <int, int> coords(stoi(pos_values[0]), stoi(pos_values[1]));
                auto it = find(obstacles.begin(), obstacles.end(), coords);

                if (it != obstacles.end())
                {
                    cout << "\n##### WARNING #####" << endl;
                    cout << "Obstacle listed at " << coords.first << "," << coords.second << endl;
                    cout << "Choose an alternative start point. \n" << endl;
                }
                else valid_pos = true;
            } 
            while (!obstacles.empty() && !valid_pos);



            string operations;
            regex op_pattern(R"(^[LRM]+$)");

            do
            {
                // Initialise robot with start input
                robot.x_pos = stoi(pos_values[0]);
                robot.y_pos = stoi(pos_values[1]);
                robot.heading = pos_values[2].c_str()[0];

                cout << "Robot " << robot_count << " starts at coordinates " << robot.x_pos << ", " << robot.y_pos << " heading " << robot.heading << "\n" << endl;



                // Get operations string with validation
                cout << "Enter robot " << robot_count << " operation string (R = turn right, L = Turn left, M = Move one step towards current heading): " << endl;

                operations = get_valid_input(op_pattern);

                cout << "Robot " << robot_count << " will perform the following operations: " << operations << endl;



                // Perform operations and retrieve the movement log
                vector<position> movement_log_1 = move_robot(robot, operations, grid_dim, obstacles);
                
                // Nothing is really done with the movement log here, but it could be useful for visualisations (e.g. showing the robot's path) or repeating operations 

                // Get final position
                robot = movement_log_1.back();

            } while (robot.heading == 'O' || robot.heading == 'X'); // Ask for new operations in event of collision or out of bounds

            // Add robot to list of obstacles once operations are completed
            obstacles.push_back(pair <int, int>(robot.x_pos, robot.y_pos));

            cout << "\nRobot " << robot_count << " ends at coordinates " << robot.x_pos << ", " << robot.y_pos << " heading " << robot.heading << "\n" << endl;
            
            robot_count++;
        }

        cout << "\nRun again? (y/n):" << endl;
        cin >> again;

        cout << "\n\n\n";

    } while (again != 'n' && again != 'N');

}
