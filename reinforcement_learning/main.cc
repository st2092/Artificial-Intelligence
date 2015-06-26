#include <iostream>
#include <stdlib.h>
#include "Grid.h"
#include <vector>
#include <utility>
#include <iomanip>
using namespace std;

/* helper function to print utility & exploration matrix*/
template < typename type >
void
print_matrix ( const vector < vector<type> > & v, const int & n)
{
  for (int row = 0; row < n; row++)
  {
    for (int col = 0; col < n; col++)
    {
      cout << setfill(' ') << setw(10) << v[row][col] ;
    }
    cout << endl;
  }
}

int main (int argc, char* argv[])
{
  if (argc != 3)
  {
    cerr << "Incorrect format, to use program use the following command line\n<program name> < n: grid size > < m: # of terminal reward states>" << endl;
    exit(-1);
  }
  int n = atoi(argv[1]); // n x n grid, n obstacles
  int m = atoi(argv[2]); // m +10/-10 reward states
  cout << "n = " << n << " m = " << m << endl;
  
  int seed = time(NULL);
  cout << "seed: " << seed << endl;
  srand(seed);
  if (n*n <= (2*m + n))
  {
    cout << "Total number of obstacles, positive rewards, and negative rewards states are larger than grid!";
    exit(-1);
  }
  
  Grid g(n,m);
  g.print();
  
  //create utility prime matrix for value iteration, this one is use for updating
  vector < vector <double> > utility_prime (n, vector <double> (n)); //create n x n matrix for utility 
  vector < vector <int> > exploration (n, vector <int> (n)); //create n x n matrix for exploration
  
  int iterations = g.perform_exploration_exploitation(utility_prime, exploration);
  g.locate_no_action_states (exploration);
  cout << "\nAfter " << iterations << " iterations" << endl;
  g.print();
  
  return 0;
}
