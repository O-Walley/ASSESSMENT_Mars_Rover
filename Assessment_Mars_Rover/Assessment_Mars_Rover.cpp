#include <iostream>
#include <string>

using namespace std;

int main()
{
    // Get grid dimensions
    int grid_width, grid_height;

    cout << "Enter grid width: ";
    cin >> grid_width;

    cout << "Enter grid height: ";
    cin >> grid_height;

    cout << "Grid dimensions are " << grid_width << "x" << grid_height << "\n" << endl;
    
    // Get robot start position and operation string
    int x_pos, y_pos;
    char heading;
    string operations;

    cout << "Enter robot start x: ";
    cin >> x_pos;

    cout << "Enter robot start y: ";
    cin >> y_pos;

    cout << "Enter robot start heading: ";
    cin >> heading;

    cout << "Enter robot operation string: ";
    cin >> operations;

    cout << "Robot starts at coordinates " << x_pos << "," << y_pos << " heading " << heading << endl;
    cout << "Robot will perform the following operations: " << operations << endl;

}
