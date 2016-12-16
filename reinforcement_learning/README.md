## Reinforcement Learning approach
<img src ="http://www.brightdevelopers.com/wp-content/uploads/2016/12/reinforcement_learning_approach.png" />

###Assumptions:
- If the difference between an update after an iteration is less than a threshold amount, I assume convergence. The threshold amount is 1.0 x 10^-9.
- gamma = 0.9, R+ = 2.0
- if the state was visited for less than N times, I assume it is still relatively unexplored. N in this case acts as the learning rate and is 10 for purpose of this project
- Maximum number of iterations allow is 10,000

## To compile program
- use the makefile to compile
- run the program in terminal with: \<program name> \<grid size> \<\# of rewards and penalties>

## Sample run
![Sample run of 5x5 world with 2 rewards and penalties](http://www.brightdevelopers.com/wp-content/uploads/2016/12/sample_rl_output.png)
