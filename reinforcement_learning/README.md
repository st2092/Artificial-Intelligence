__Reinforcement Learning approach__:

<img src ="http://puu.sh/iD6T6/379d729e2d.png" />

__Assumptions__:
- If the difference between an update after an iteration is less than a threshold amount, I assume convergence. The threshold amount is 1.0 x 10^-9.
- gamma = 0.9, R+ = 2.0
- if the state was visited for less than N times, I assume it is still relatively unexplored. N in this case acts as the learning rate and is 10 for purpose of this project
- Maximum number of iterations allow is 10,000
