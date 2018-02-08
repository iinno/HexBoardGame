# HexBoardGame
README-CMPS109-Assignment6-Dec/6/2017
Vincent Kim, viikim@ucsc.edu

FUNCTIONS CREATED:
-SaveGraph: Stores data of current Hexgraph to restore it after random simulation.
-RestoreGraph: Uses the temp data saved from SaveGraph to restore the Hexgraph to its state before simulation.
-MonteCarlo: Calls the randomPlay function to randomly simulate a game specified amount of times (int iteration) with a certain move for all available moves.
-PlayMonteCarlo: Calls monteCarlo and also deals with user interaction.

FUNCTIONS MODIFIED:
-playRandom: Has been modified to support monteCarlo.

*Important Side note*
-The board size in the application can be changed in the top of the file. (const int SIZE = 7 -> in the submitted file).
-The iteration which is defined as 100 in the HexGraph class can be altered for better AI skill.
-At the moment playing a 100-iteration, 11x11 HexGame is slightly slow (30-40sec between every move). More time is needed to make it efficient. However, on a 7x7 board, the game runs pretty quickly.


ASSESMENT:
Fully completed the assignment, there should be no problem.

What I learned:
I learned that monte-carlo is a very interesting discovery. Writing the monte-carlo algorithm was much simpler then alpha-beta pruning.
