#include <iostream>
#include <string>
#include <vector>

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


int main()
{
    char again;

    do
    {
        // Get grid dimensions
        pair <int, int> grid_dim; // (X, Y)

        cout << "Enter grid width: ";
        cin >> grid_dim.first;

        cout << "Enter grid height: ";
        cin >> grid_dim.second;

        cout << "Grid dimensions are " << grid_dim.first << "x" << grid_dim.second << "\n" << endl;



        // Get robot start position and operation string
        position robot_1;
        string operations;

        cout << "Enter robot start x: ";
        cin >> robot_1.x_pos;

        cout << "Enter robot start y: ";
        cin >> robot_1.y_pos;

        cout << "Enter robot start heading: ";
        cin >> robot_1.heading;

        cout << "Enter robot operation string: ";
        cin >> operations;

        cout << "\nRobot starts at coordinates " << robot_1.x_pos << "," << robot_1.y_pos << " heading " << robot_1.heading << endl;
        cout << "Robot will perform the following operations: " << operations << endl;



        // Retrieve the movement log
        vector<position> movement_log_1 = move_robot(robot_1, operations);

        // Get final position
        robot_1 = movement_log_1.back();

        cout << "\nRobot ends at coordinates " << robot_1.x_pos << "," << robot_1.y_pos << " heading " << robot_1.heading << endl;

        cout << "\nTry again? (y/n):" << endl;
        cin >> again;

        cout << "\n\n\n";

    } while (again != 'n');

}
