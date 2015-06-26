////////////////////////////////
//
// University of California, Riverside
// CS170 Spring 2015 Assignment 3
// grid.cpp
//
// You are encouraged to modify and add to this file
//////////////////////////////
#include "Grid.h"

#include <iostream>
#include <iomanip>
#include <cfloat>
#include <cmath>
#include <sstream>

using namespace std;

///////////////
/// Utility ///
///////////////

///////////
// Function centerStr
///////////
// In -> string s      - value of string we are centering.
//       int reserve   - # of characters (width).
//       char autofill - char to print for the left and right remaining space.
// Out -> string
//
// Returns a string with length of the reserved space with the string, s, centered.
string centerStr(const string & s, int reserve, char autofill)
{
    int blanks = reserve - s.length();
    int lbuf = blanks / 2; // Add one to favor remainder to right
    int rbuf = (blanks+1) / 2;
    stringstream ss;
    ss << setfill(autofill) << setw(lbuf) << "" << s << setw(rbuf) << "";
    return ss.str();
}

///////////
// Function toString
///////////
// In -> double
// Out -> string
//
// Returns a double converted to a string with fixed precision.
string toString(double value, int prec)
{
    stringstream ss;
    if (value > 0.0)
        ss << '+';
    ss << setprecision(prec) << fixed << value;
    return ss.str();
}

///////////
// Function toString
///////////
// In -> Direction
// Out -> string
//
// Returns an ascii version of Direction as a string.
string toString(Direction dir)
{
    switch (dir)
    {
        case NORTH:
            return "^";
        case EAST:
            return ">";
        case SOUTH:
            return "v";
        case WEST:
            return "<";
        default:
            return "?";
    }
}

///////////////////////
/// GridCell Object ///
///////////////////////

// Constructors
GridCell::GridCell()
    : type(BLANK), reward(0.0), start(false), policy(NONE, 0.0), utility_value (0.0)
{
}

GridCell::GridCell(GridCellType type, double reward, bool start)
    : type(type), reward(reward), start(start), policy(NONE, 0.0), utility_value (0.0)
{
}
    
///////////
// Function print
///////////
// In -> string pstr - agent symbol, can be the empty string if not need print.
//       int reserve - the amount of characters (width) we can print for each line.
// Out -> vector<string> - index by row, the output contents of this cell
//
// Returns the output contents of this cell in four different rows.
// Refer to Grid class for more information of the ascii-grid layout
vector<string> GridCell::print(const string & pstr, int reserve) const
{
    vector<string> ret(4);
        
    string prefix1st = "+";
    string prefixedge = "|";
    if (type == OBSTACLE)
    {
        ret[0] = prefix1st + centerStr("", reserve - prefix1st.length(), '-');
        ret[1] = prefixedge + centerStr("xxxx", reserve - prefixedge.length(), ' ');
        ret[2] = prefixedge + centerStr("xxxx", reserve - prefixedge.length(), ' ');
        ret[3] = prefixedge + centerStr("xxxx", reserve - prefixedge.length(), ' ');
    }
    else
    {
        ret[0] = prefix1st + centerStr("", reserve - prefix1st.length(), '-');
        ret[1] = prefixedge + centerStr(toString(reward + policy.second), reserve - prefixedge.length());
        ret[2] = prefixedge + centerStr(start ? pstr + "S" : pstr, reserve - prefixedge.length());
        if (type == TERMINAL)
            ret[3] = prefixedge + centerStr("*", reserve - prefixedge.length());
        else
            ret[3] = prefixedge + centerStr(toString(policy.first), reserve - prefixedge.length());
    }
        
    return ret;
}
    
///////////
// Function getPolicy
///////////
// In ->
// Out -> pair<Direction, double>
//
// Returns the current policy in the form of the pair,
// The first being the action and the the second being the value.
pair<Direction, double> GridCell::getPolicy() const
{
    return policy;
}

///////////////////
/// Grid Object ///
///////////////////
    
///////////////
// Constructors
///////////////
// In -> int n, parameter described by assignment prompt
//       int m, parameter described by assignment prompt
// Out ->
Grid::Grid(int n, int m)
{
    bounds = pair<int,int>(n, n);
    grid = vector<vector<GridCell> >(bounds.first, vector<GridCell>(bounds.second));
    //hard-coded starting location
    startLocation = pair<int,int>(0, 0);
    
    //set grid cell as the starting point for Agent
    grid[startLocation.first][startLocation.second].start = true; 
    
    //populate grid with n obstacles
    int obstacle_count = 0;  
    int row, col;
    while (obstacle_count < n)
    {
      row = rand() % bounds.first;
      col = rand() % bounds.second;
      //grid[row][col].print();
      
      if ((grid[row][col].type != GridCell::OBSTACLE && grid[row][col].type != GridCell::TERMINAL)&& !grid[row][col].start)
      {  
        grid[row][col].type = GridCell::OBSTACLE;
        obstacle_count++;
        //cout << "Valid obstacle coordinates # " << obstacle_count << ": [ " << row << " " << col << " ]" << endl;
      }
      /* For now hard code the coordinates as on provided example output */
      /*
      //grid[1][0].type = GridCell::OBSTACLE;
      obstacle_count++;
      //grid[0][1].type = GridCell::OBSTACLE;
      obstacle_count++;
      obstacle_count = n;
      //grid[2][2].type = GridCell::OBSTACLE;
      //obstacle_count++;
      //grid[3][0].type = GridCell::OBSTACLE;
      //obstacle_count++;
      */
    }
    // populate grid with m +10/-10 terminal states
    int positive_reward_count = 0;
    int negative_reward_count = 0;
    while (positive_reward_count < m)
    {
      row = rand() % bounds.first;
      col = rand() % bounds.second;
      
      if ((grid[row][col].type != GridCell::OBSTACLE && grid[row][col].type != GridCell::TERMINAL)&& !grid[row][col].start)
      {
        grid[row][col].type = GridCell::TERMINAL;
        grid[row][col].reward = POSITIVE_REWARD;
        positive_reward_count++;
      }
      
      //hardcode for now
      /*
      grid[1][0].type = GridCell::TERMINAL;
      grid[1][0].reward = POSITIVE_REWARD;
      grid[0][1].type = GridCell::TERMINAL;
      grid[0][1].reward = POSITIVE_REWARD;
      positive_reward_count++;
      positive_reward_count = m;
      */
    }
    
    while (negative_reward_count < m)
    {
      row = rand() % bounds.first;
      col = rand() % bounds.second;
      if ((grid[row][col].type != GridCell::OBSTACLE && grid[row][col].type != GridCell::TERMINAL)&& !grid[row][col].start)
      {
        grid[row][col].type = GridCell::TERMINAL;
        grid[row][col].reward = NEGATIVE_REWARD;
        negative_reward_count++;
      }
      
      // hardcode for now
      /*
      grid[n-n][n-1].type = GridCell::TERMINAL;
      grid[n-n][n-1].reward = NEGATIVE_REWARD;
      negative_reward_count++;
      */
    }
}
    
// Accessors to the 2D grid
GridCell& Grid::operator[](const pair<int,int> & pos)
{
    return grid[pos.first][pos.second];
}
const GridCell& Grid::operator[](const pair<int,int> & pos) const
{
    return grid[pos.first][pos.second];
}
pair<int,int> Grid::getBounds() const
{
    return bounds;
}
int Grid::getRows() const
{
    return bounds.first;
}
int Grid::getCols() const
{
    return bounds.second;
}
pair<int,int> Grid::getStartLocation() const
{
    return startLocation;
}
    
///////////
// Function print
///////////
// In -> pair<int,int> agentPos -
//          position of agent provided. If indicies are negative or out
//          of bounds. This parameter is ignored.
// Out ->
//
// Prints each cell content with special information centered on its row in the following format:
//
// +------
// |value
// |start
// |policy
//
// value - displays the value of the current policy in the format [+/-]##.##
// start - displays 'S' for whether this cell is a start location. A 'P' is also appended
//          if the agentPos is located at that cell.
// policy - displays '^' '>' 'v' '<' if the best policy is NORTH, EAST, SOUTH, or WEST respectively.
//              If policy is set to NONE, it will display '?'
//
// Special cases:
// - If cell is an obstacle, the contents of the cell for each line contain "xxxx" instead
// - if cell is a terminal, the policy displayed for that cell will be '*' instead
//
// Once each cell is printed, it will append a final right and bottom edge
// to complete the ascii table.
void Grid::print(const pair<int,int> & agentPos) const
{
    const int RESERVE = 7;
    for (int i = 0; i < bounds.first; ++i)
    {
        vector<string> outputs(4);
            
        for (int j = 0; j < bounds.second; ++j)
        {
            vector<string> ret;
            if (i == agentPos.first && j == agentPos.second)
                ret = grid[i][j].print("P", RESERVE);
            else
                ret = grid[i][j].print(" ", RESERVE);
            outputs[0] += ret[0];
            outputs[1] += ret[1];
            outputs[2] += ret[2];
            outputs[3] += ret[3];
        }
        cout << outputs[0] << "+" << endl
        << outputs[1] << "|" << endl
        << outputs[2] << "|" << endl
        << outputs[3] << "|" << endl;
    }
    cout << left << setfill('-');
    for (int j = 0; j < bounds.second; ++j)
        cout << setw(RESERVE) << "+";
    cout << "+";
    cout << endl;
}

/* function to copy one matrix's content onto grid */
void
Grid::copy_matrix(const std::vector < std::vector<double> > & source)
{
  for (int row = 0; row < getRows(); row ++)
  {
    for (int col = 0; col < getCols(); col++)
    {
      if (grid[row][col].type == GridCell::BLANK)
      {
        grid[row][col].utility_value = source[row][col];
      }
    }
  }
}

/* go north action */
double 
Grid::action_north (const int & row, const int & col)
{
  //if cannot go north
  if ( (row == 0) || (grid[row - 1][col].type == GridCell::OBSTACLE) )
  {
    return grid[row][col].utility_value;
  }
  else
  {
    if (grid[row - 1][col].type == GridCell::TERMINAL)
    {
      return grid[row - 1][col].reward;
    }
    else
    {
      return grid[row - 1][col].utility_value;
    }
  }
}

/* go south action */
double 
Grid::action_south (const int & row, const int & col)
{
  //if cannot go south
  if ( (row == (getRows() - 1)) || (grid[row + 1][col].type == GridCell::OBSTACLE) )
  {
    return grid[row][col].utility_value;
  }
  else
  {
    if (grid[row + 1][col].type == GridCell::TERMINAL)
    {
      return grid[row + 1][col].reward; 
    }
    else
    {
      return grid[row + 1][col].utility_value;
    }
  }
}
/* go west action */
double 
Grid::action_west (const int & row, const int & col)
{
  //if cannot go west
  if ( (col == 0) || (grid[row][col - 1].type == GridCell::OBSTACLE) )
  {
    return grid[row][col].utility_value;
  }
  else
  {
    if (grid[row][col - 1].type == GridCell::TERMINAL)
    {
      return grid[row][col - 1].reward;
    }
    else
    {
      return grid[row][col - 1].utility_value;
    }
  }
}

/* go east action */
double 
Grid::action_east (const int & row, const int & col)
{
  //if cannot go east
  if ( (col == (getCols() - 1)) || (grid[row][col + 1].type == GridCell::OBSTACLE) )
  {
    return grid[row][col].utility_value;
  }
  else
  {
    if (grid[row][col + 1].type == GridCell::TERMINAL)
    {
      return grid[row][col + 1].reward;
    }
    else
    {
      return grid[row][col + 1].utility_value;
    }
  }
}

/* function to find the index of the max value in an array of doubles */
int
Grid::find_max_index (double* & array)
{
  int max_index = 0; 
  for (int i = 1; i < NUM_DIRECTIONS; i++)
  {
    if (array[max_index] < array[i])
    {
      max_index = i; 
    }
  }
  return max_index;
}

/* function that returns corresponding direction for best action (based on index)*/
Direction
Grid::index_to_direction (const int & max_index)
{
  if (max_index == 0) //north
  {
    return NORTH;
  }
  else if (max_index == 1) //south
  {
    return SOUTH;
  }
  else if (max_index == 2) //east
  {
    return EAST;
  }
  else if (max_index == 3) //west
  {
    return WEST;
  }
  else
  {
    return NONE;
  }
}

/* function to update utility matrix at row, column */
void
Grid::update_utility_prime (const int & row, const int & col, std::vector < std::vector<double> > & utility_prime)
{
  // if it is a terminal state or obstacle
  if ((grid[row][col].type == GridCell::TERMINAL) || (grid[row][col].type == GridCell::OBSTACLE))
  {
    utility_prime[row][col] = grid[row][col].reward; 
  }
  else
  {
    //compute value for each action accounting for stochastic outcome
    double* action_value = new double [NUM_DIRECTIONS];
    action_value[0] = PROBABILITY_GOOD*action_north(row, col) + PROBABILITY_BAD*action_west(row, col) + PROBABILITY_BAD*action_east(row, col); //north
    action_value[1] = PROBABILITY_GOOD*action_south(row, col) + PROBABILITY_BAD*action_west(row, col) + PROBABILITY_BAD*action_east(row, col); //south
    action_value[2] = PROBABILITY_GOOD*action_east(row, col) + PROBABILITY_BAD*action_north(row, col) + PROBABILITY_BAD*action_south(row, col); //east
    action_value[3] = PROBABILITY_GOOD*action_west(row, col) + PROBABILITY_BAD*action_north(row, col) + PROBABILITY_BAD*action_south(row, col); //west
    
    //find the max of the 4 values
    int max_index = find_max_index (action_value);
    //update utility prime matrix
    utility_prime[row][col] = grid[row][col].reward + GAMMA*action_value[max_index];
    
    //update policy
    pair <Direction, double> updated_policy (index_to_direction(max_index), utility_prime[row][col]);
    grid[row][col].policy = updated_policy;
  }
}  

/* exploration-exploitation function */    
int 
Grid::perform_exploration_exploitation (std::vector < std::vector<double> > & utility_prime, std::vector < std::vector<int> > & exploration)
{
  int iteration = 0;
  double delta;
  //hardcoded starting position is always row = 0 and col = 0
  //thus, we need to check north west corner if agent is trapped
  pair <int, int> starting_position = getStartLocation();
  int row = starting_position.first;
  int col = starting_position.second;
  if ( ( grid[row+1][col].type == GridCell::TERMINAL && grid[row][col+1].type == GridCell::TERMINAL && (grid[row+1][col].reward + grid[row][col+1].reward) < 0 ) || //surrounded by 2 negative terminal state
       ( grid[row+1][col].type == GridCell::TERMINAL && grid[row+1][col].reward <= 0 && grid[row][col+1].type == GridCell::OBSTACLE ) || // 1 obstacle, 1 negative terminal
       ( grid[row][col+1].type == GridCell::TERMINAL && grid[row][col+1].reward <= 0 && grid[row+1][col].type == GridCell::OBSTACLE ) || // 1 obstacle, 1 negative terminal
       ( grid[row][col+1].type == GridCell::OBSTACLE && grid[row+1][col].type == GridCell::OBSTACLE ) // surrounded by 2 obstacles 
      )
  {
    return iteration;
  }
  while (iteration < MAX_ITERATIONS)
  {
    //update the long term utility by setting it equal to utility prime
    copy_matrix(utility_prime);
    
    //compute changes in utility prime using the previous values of utility prime
    for (int row = 0; row < getRows(); row++)
    {
      for (int col = 0; col < getCols(); col++)
      {
        //ignore obstacles
        if (grid[row][col].type != GridCell::OBSTACLE)
        {
          //promote exploration
          if (grid[row][col].type == GridCell::BLANK && exploration[row][col] < LEARNING_RATE)
          {
            exploration[row][col] = exploration[row][col] + 1; //increase visit to state by 1
            utility_prime[row][col] = grid[row][col].reward + GAMMA*OPTIMISTIC_REWARD;
            //cout << utility_prime[row][col] << endl;
          }
          else // exploitation after exploring
          {
            update_utility_prime (row, col, utility_prime);
            delta = abs(utility_prime[row][col] - grid[row][col].utility_value); //subtract updated value with previous value
          }
        }
      }
    }
    iteration++;
    //if difference between utilities one iteration ago and this iteration is less than threshold improvement we stop 
    if (delta <  DELTA_THRESHOLD)
    {
      cout << "Difference between utilities are less than threshold ( " << DELTA_THRESHOLD << " ). Assuming convergence has occurred." << endl;
      break;
    }
  }
  return iteration;
}

/* Helper function to detect for trapped state (action-less) */
bool 
Grid::is_trapped (const int & row, const int & col, std::vector < std::vector<int> > & exploration)
{
  //cout << "Row: " << row << "\tColumn: " << col << endl;
  if ( row == 0 ) //north
  {
    if (col == 0) //north west corner
    {
      if ( (exploration[row+1][col] <= 0 && grid[row+1][col].reward <= 0) && (exploration[row][col+1] <= 0 && grid[row][col+1].reward <= 0) )
      {
        return true;
      }
    }
    else if (col == (getCols() - 1 ) ) //north east corner
    {
      if ( (exploration[row+1][col] <= 0 && grid[row+1][col].reward <= 0 ) && ( exploration[row][col-1] <= 0 && grid[row][col-1].reward <= 0) )
      {
        return true;
      }
    }
    else //north edge
    {
      if ( ( exploration[row][col+1] <= 0 && grid[row][col+1].reward <= 0 ) &&
           ( exploration[row+1][col] <= 0 && grid[row+1][col].reward <= 0 ) &&
           ( exploration[row][col-1] <= 0 && grid[row][col-1].reward <= 0 ) )
      {
        return true;
      }
    }
  }
  
  else if ( row == (getRows() - 1) ) //south
  {
    if ( col == 0 ) //south  west corner
    {
      if ( (exploration[row-1][col] <= 0  && grid[row-1][col].reward <= 0) && (exploration[row][col+1] <= 0 && grid[row][col+1].reward <= 0) )
      {
        return true;
      }
    }
    else if ( col == (getCols() - 1) ) //south east corner
    {
      if ( ( exploration[row-1][col] <= 0 && grid[row-1][col].reward <= 0) && (exploration[row][col-1] <= 0 && grid[row][col-1].reward <= 0) )
      {
        return true;
      }
    }
    else //south edge
    {
      if ( ( exploration[row-1][col] <= 0 && grid[row-1][col].reward <= 0 ) &&
           ( exploration[row][col+1] <= 0 && grid[row][col+1].reward <= 0 ) &&
           ( exploration[row][col-1] <= 0 && grid[row][col-1].reward <= 0 ) )
      {
        return true;
      }
    }
  }
  
  else if ( col == 0 ) //west
  {
    if (row == 0) //north west corner
    {
      if ( ( exploration[row+1][col] <= 0 && grid[row+1][col].reward <= 0)&& ( exploration[row][col+1] <= 0 && grid[row][col+1].reward <= 0) )
      {
        return true;
      }
    }
    else if (row == (getRows() - 1 ) ) //south west corner
    {
      if ( ( exploration[row-1][col] <= 0 && grid[row-1][col].reward <= 0) && ( exploration[row][col+1] <= 0 && grid[row][col+1].reward <= 0) )
      {
        return true;
      }
    }
    else //west edge
    {
      if ( ( exploration[row-1][col] <= 0 && grid[row-1][col].reward <= 0 ) && 
           ( exploration[row][col+1] <= 0 && grid[row][col+1].reward <= 0 ) && 
           ( exploration[row+1][col] <= 0 && grid[row+1][col].reward <= 0 ) )
      {
        return true;
      }
    }
  }
  
  else if ( col == (getCols() - 1) ) //east
  {
    if ( row == 0 ) //north east corner
    {
      if ( (exploration[row+1][col] <= 0 && grid[row+1][col].reward <= 0) && ( exploration[row][col-1] <= 0 && grid[row][col-1].reward <= 0 ) )
      {
        return true;
      }
    }
    else if ( row == (getRows() - 1) ) //south east corner
    {
      if ( ( exploration[row+1][col] <= 0 && grid[row+1][col].reward <= 0) && ( exploration[row][col-1] <= 0 && grid[row][col-1].reward <= 0 ) )
      {
        return true;
      }
    }
    else //east edge
    {
      if ( ( exploration[row-1][col] <= 0 && grid[row-1][col].reward <= 0 ) &&
           ( exploration[row+1][col] <= 0 && grid[row+1][col].reward <= 0 ) &&
           ( exploration[row][col-1] <= 0 && grid[row][col-1].reward <= 0) )
      {
        return true;
      }
    }
  }
  
  else //state is somewhere in middle of grid (only trap if in all 4 directions is an obstacle or negative reward)
  {
    if ( ( exploration[row-1][col] <= 0 && grid[row-1][col].reward <= 0 ) &&
         ( exploration[row][col+1] <= 0 && grid[row][col+1].reward <= 0 ) &&
         ( exploration[row+1][col] <= 0 && grid[row+1][col].reward <= 0 ) &&
         ( exploration[row][col-1] <= 0 && grid[row][col-1].reward <= 0 ) )
    {
      return true;
    }
  }
  return false;
}

/* 
   Function that locate states that cannot make any actions. That is
   the state is surrounded by obstacles and or negative rewards. 
*/
void
Grid::locate_no_action_states (std::vector < std::vector<int> > & exploration)
{
  //grid and exploration are same size
  for (int row = 0; row < getRows(); row++)
  {
    for (int col = 0; col < getCols(); col++)
    {
      if (is_trapped(row, col, exploration))
      {
        pair <Direction, double> updated_policy(NONE, 0.0);
        grid[row][col].policy = updated_policy;
      }
    }
  }
}
