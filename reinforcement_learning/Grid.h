////////////////////////////////
//
// University of California, Riverside
// CS170 Spring 2015 Assignment 3
// grid.h
//
// You are encouraged to modify and add to this file
//////////////////////////////
// The Grid class provides a print utility and necessary accessors.
// Initialization of the Grid class is incomplete.
//
#ifndef GRID_H
#define GRID_H
#include <vector>
#include <string>
#include <utility>

///////////////
/// Utility ///
///////////////
const double POSITIVE_REWARD = 10.0;
const double NEGATIVE_REWARD = -10.0;
const double PROBABILITY_GOOD = 0.8;
const double PROBABILITY_BAD = 0.1; // 0.1 instead of 0.2 because there are two possible outcomes aside from what we want
//tuning values
const double GAMMA = .90; // discount
const double OPTIMISTIC_REWARD = 2.0; // reward for relatively unexplored spaces
const int LEARNING_RATE = 10; // defines how many times a state can be visited before it loses value (promotes exploration)
const double DELTA_THRESHOLD = 1.0e-9; //minimum improvement for an iteration, below value assume convergence and stop algorithm
const int MAX_ITERATIONS = 10000;

const int NUM_DIRECTIONS = 4;
enum Direction { NORTH, EAST, SOUTH, WEST, NONE };

///////////
// Function centerStr
///////////
// In -> string s      - value of string we are centering.
//       int reserve   - # of characters (width).
//       char autofill - char to print for the left and right remaining space.
// Out -> string
std::string centerStr(const std::string & s, int reserve, char autofill = ' ');
///////////
// Function toString
///////////
// In -> double
// Out -> string
std::string toString(double value, int prec = 2);

///////////
// Function toString
///////////
// In -> Direction
// Out -> string
std::string toString(Direction dir);

///////////////////////
/// GridCell Object ///
///////////////////////
//
// Contains information for a Cell in "GridWorld"
// and nice ascii printing functionality.
struct GridCell
{
    // GridCellType definitions:
    // BLANK - traversable cell
    // TERMINAL - episode ends if agent ever steps on this cell
    // OBSTACLE - non-traversable cell
    enum GridCellType { BLANK, TERMINAL, OBSTACLE };
    GridCellType type;
    
    // Reward for landing on this cell.
    double reward;
    
    // Flag which specifies whether this is a start location for the agent.
    bool start;
    
    // Current policy where the first is the direction and the second is the value.
    std::pair<Direction, double> policy;
    
    // long term utility value at specific grid cell (state)
    double utility_value;
    
    // Constructors
    GridCell();
    GridCell(GridCellType type, double reward = 0.0, bool start = false);
    
    ///////////
    // Function print
    ///////////
    // In -> string pstr - agent symbol, can be the empty string if not need print.
    //       int reserve - the amount of characters (width) we can print for each line.
    // Out -> vector<string> - index by row, the output contents of this cell
    std::vector<std::string> print(const std::string & pstr, int reserve) const;
    
    ///////////
    // Function getPolicy
    ///////////
    // In ->
    // Out -> pair<Direction, double>
    std::pair<Direction, double> getPolicy() const;
};

///////////////////
/// Grid Object ///
///////////////////
//
// Contains information about "GridWorld"
// and nice ascii printing functionality.
class Grid
{
private:
    // Number of rows and columns respectively
    std::pair<int,int> bounds;
    
    // Cell contents organized in a 2D vector
    std::vector<std::vector<GridCell> > grid;
    
    // starting (row,column) for the agent
    std::pair<int,int> startLocation;
public:
    // Constructors
    Grid(int n, int m);
    
    // Accessors to the 2D grid
    GridCell& operator[](const std::pair<int,int> & pos);
    const GridCell& operator[](const std::pair<int,int> & pos) const;
    std::pair<int,int> getBounds() const;
    int getRows() const;
    int getCols() const;
    std::pair<int,int> getStartLocation() const;
    
    ///////////
    // Function print
    ///////////
    // In -> pair<int,int> agentPos -
    //          position of agent provided. If indicies are negative or out
    //          of bounds. This parameter is ignored.
    // Out ->
    void print(const std::pair<int,int> & agentPos = std::make_pair(-1, -1)) const;
    
    void copy_matrix(const std::vector < std::vector<double> > & source);
    double action_north (const int & row, const int & col);
    double action_south (const int & row, const int & col);
    double action_west (const int & row, const int & col);
    double action_east (const int & row, const int & col);
    int find_max_index (double* & array);
    Direction index_to_direction (const int & max_index);
    void update_utility_prime (const int & row, const int & col, std::vector < std::vector<double> > & utility_prime);
    int perform_exploration_exploitation (std::vector < std::vector<double> > & utility_prime, std::vector < std::vector<int> > & exploration);
    bool is_trapped (const int & row, const int & col, std::vector < std::vector<int> > & exploration);
    void locate_no_action_states (std::vector < std::vector<int> > & exploration);
};
#include "Grid.cc"
#endif
