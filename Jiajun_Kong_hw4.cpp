/*
Author:         Jiajun Kong

Date:           4/7/2020

Description:    This program uses an algorithm to find an optimal pattern to seat
                guests in the Coronavirus time, in a n * m theatre where n is the
                number of rows and m is the number of seats, for social distancing.
                The seat moves circularly, like Knight's Tour, and it checks the
                Euclidean, vertical, horizontal distance between two seats. 1 on
                the pattern indicate somebody should seat there, 0 otherwise.
*/

#include <iostream>
#include<time.h>
#include <math.h> 
using namespace std;

//prototypes
bool findSeat(int**, int x, int y, double, int, int, const int[], const int[], int, double, double);
void printSol(int**, int, int);
int randomInRange(int, int);
bool isValid(int, int, int, int, int** const, int, int, double, double, double);

int main()
{
    //random number seed
    srand(time(NULL));

    //variable declaration
    int numOfRows; // num of rows
    int numOfSeats;  // num of columns
    double seatDist;// distance between seats
    double rowDist;// distance between rows
    int ranNum; // random number
    int unit;
    int** arr = NULL; // 2D array
    double minDist; //min distance
    int moveCounter = 1;
    //all possible moves
    const int xMove[8] = { 2, 1, -1, -2, -2, -1,  1,  2 };
    const int yMove[8] = { 1, 2,  2,  1, -1, -2, -2, -1 };

    //Promt the user to input num of rows and num of seats
    cout << "This is a seat arrangement program for COVID-19." << endl;
    cout << "Enter number of rows: ";
    cin >> numOfRows;
    cout << "Enter number of setas: ";
    cin >> numOfSeats;

    //Get unit
    cout << "Let's get the distance between seats and rows.\nFirst, please enter an unit of measurement, 0 for meters, 1 for feet: ";
    cin >> unit;
    while (unit < 0 || unit > 1)
    {
        cout << "Invalid input. Please enter an unit of measurement, 0 for meters, 1 for feet: ";
        cin >> unit;
    }

    //Get distance of seats and rows
    cout << "Enter the distance between seats: ";
    cin >> seatDist;
    cout << "Enter the distance between rows: ";
    cin >> rowDist;

    //Set minimum distance regard to the unit
    if (unit == 0)
    {
        minDist = 1.5;
    }
    //Convert it to meters if the user entered English units
    // 1 feet = 3.281 meters
    else if (unit == 1)
    {
        minDist = 6 / 3.281; // 6 feet = 1.83 meters
        seatDist /= 3.281;
        rowDist /= 3.281;
    }

    //Dynamically allocate a 2D array
    arr = new int* [numOfRows];
    for (int i = 0; i < numOfRows; i++)
    {
        arr[i] = new int[numOfSeats];
    }

    //initialize the array to 0
    for (int i = 0; i < numOfRows; i++)
    {
        for (int j = 0; j < numOfSeats; j++)
        {
            arr[i][j] = 0;
        }
    }

    //Random start number for the starting point in the first row, within the range [0, numOfSeats)
    ranNum = randomInRange(0, numOfSeats);

    //Randomly start from (0, y), mark -1 as the seat taken
    arr[0][ranNum] = 1;

    //Calling recursive function to find seats
    findSeat(arr, 0, ranNum, minDist, numOfRows, numOfSeats, xMove, yMove, moveCounter, rowDist, seatDist);

    //print solution
    printSol(arr, numOfRows, numOfSeats);

    //Free memory
    for (int i = 0; i < numOfRows; i++)
    {
        delete[] arr[i];
    }
    delete[] arr;

    return 0;
}

//Generates a random number from min to max, [min, max-min)
int randomInRange(int min, int max)
{
    return rand() % (max - min) + min;
}

//Recursive function to find all the qualified seats
bool findSeat(int** arr, int x, int y, double minDist, int rows, int columns, const int xMove[8], const int yMove[8], int moveCount, double rowDist, double colDist)
{
    //Base case
    if (moveCount == rows * columns)
        return true;

    //Loop to move to the new point
    for (int i = 0; i < 8; i++)
    {
        //New points for the next move
        int xNew = x + xMove[i];
        int yNew = y + yMove[i];

        //Check if the new seat is good, then update the array
        if (isValid(xNew, yNew, x, y, arr, rows, columns, minDist, rowDist, colDist) == true)
        {
            // set to 1 as the seat has been checked qualified
            arr[xNew][yNew] = 1;

            //Recursion
            if (findSeat(arr, xNew, yNew, minDist, rows, columns, xMove, yMove, moveCount + 1, rowDist, colDist) == true)
            {
                return true;
            }
        }
    }

    //keep recurring until moveCount equals rows*columns, 
    return false;

}


//This function checks if the new seat is valid
bool isValid(int x, int y, int oldX, int oldY, int** const arr, int rows, int columns, double minDist, double rowDist, double colDist)
{

    //Check if the new point is within the range of the matrix
    if (x < rows && y < columns && x >= 0 && y >= 0)
    {
        //If the seat is taken, return false
        if (arr[x][y] == 1)
        {
            return false;
        }

        //Check euclidean distance between new point and old point
        //return false if it is less than 1.5 meters or 6 feet(1.83 meters)
        const double distance = sqrt(((x - oldX) * rowDist) * ((x - oldX) * rowDist) + ((y - oldY) * colDist) * ((y - oldY) * colDist));
        if (distance < minDist)
        {
            return false;
        }

        //If the row distance is less than the min distance,
        //check the row distance between seats nearby
        if (rowDist < minDist)
        {
            //In first row, check higher row index
            if (x == 0)
            {
                if (arr[x + 1][y] == 1)
                {
                    return false;
                }

            }
            //In last row, check lower row index
            else if (x == rows - 1)
            {
                if (arr[x - 1][y] == 1)
                {
                    return false;
                }

            }
            //neither
            else
            {
                //check higher and lower row index
                if (arr[x - 1][y] == 1 || arr[x + 1][y] == 1)
                {
                    return false;
                }

            }

        }

        //If the column distance is less than the min distance,
        //check the column distance between seats nearby
        if (colDist < minDist)
        {
            //In first column, check right
            if (y == 0)
            {
                if (arr[x][y + 1] == 1)
                {
                    return false;
                }

            }
            //In last column, check left
            else if (y == columns - 1)
            {
                if (arr[x][y - 1] == 1)
                {

                    return false;
                }

            }
            //neither
            else
            {
                //Check right and left
                if (arr[x][y + 1] == 1 || arr[x][y - 1] == 1)
                {
                    return false;
                }
            }
        }

        //Diagonal euclidean distance 
        const double diaDist = sqrt((rowDist) * (rowDist)+(colDist) * (colDist));

        //Check diagonal
        if (diaDist < minDist)
        {
            //First column
            if (y == 0)
            {
                //left upper corner
                if (x == 0)
                {
                    if (arr[x + 1][y + 1] == 1)
                    {
                        return false;
                    }
                }
                //left lower corner
                else if (x == rows - 1)
                {
                    if (arr[x - 1][y + 1] == 1)
                    {
                        return false;
                    }
                }
                //In between
                else
                {
                    if (arr[x - 1][y + 1] == 1 || arr[x + 1][y + 1] == 1)
                    {
                        return false;
                    }
                }


            }
            // first row
            else if (x == 0)
            {
                //in the first row between corners
                if (y > 0)
                {
                    if (arr[x + 1][y - 1] == 1 || arr[x + 1][y + 1] == 1)
                        return false;
                }
            }
            //Last column
            else if (y == columns - 1)
            {
                //right upper corner
                if (x == 0)
                {
                    if (arr[x + 1][y - 1] == 1)
                    {
                        return false;
                    }
                }
                //right lower corner
                if (x == rows - 1)
                {
                    if (arr[x - 1][y - 1] == 1)
                    {
                        return false;
                    }
                }
                //In between
                else
                {
                    if (arr[x - 1][y - 1] == 1 || arr[x + 1][y - 1] == 1)
                    {
                        return false;
                    }
                }
            }

            // last row
            else if (x == rows - 1)
            {
                if (y > 0)
                {
                    if (arr[x - 1][y - 1] == 1 || arr[x - 1][y + 1] == 1)
                        return false;
                }
            }
            //In between
            else
            {
                if (arr[x - 1][y - 1] == 1 || arr[x - 1][y + 1] == 1 || arr[x + 1][y - 1] || arr[x + 1][y + 1] == 1)
                {
                    return false;
                }
            }
        }

        //The new seat is valid when it gets here.
        return true;
    }

    //The new seat is out of range, return false
    else
    {
        return false;
    }
}


// displays the solution
void printSol(int** tour, int x, int y)
{

    cout << "The value 1 indicate the seats where people should seat, 0 otherwise.";
    cout << "\nGood seats to avoid Coronavirus: " << endl;

    for (int i = 0; i < x; i++)
    {
        for (int j = 0; j < y; j++)
        {
            cout << tour[i][j] << " ";
        }
        cout << endl;
    }
}
