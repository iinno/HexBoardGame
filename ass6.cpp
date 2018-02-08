//
//  main.cpp
//  Assignment6
//
//  Created by Vincent Kim on 12/6/17.
//  Copyright © 2017 Vincent Kim. All rights reserved.
//

#include <iostream>
#include <vector>
#include <deque>
#include <algorithm>
#include <cstdlib>
#include "time.h"
#include <random>
#include <list>
#include<sys/types.h>




using namespace std;

//DEFINES THE BOARD SIZE
const int SIZE = 7;


//Class used for keeping track of the coordinates, id, and its adjacent nodes.
class Node{
public:
  //First constructor that is used for creating a temp node in find function.
  Node(int id_2){
    id = id_2;
    state = "*";
  }
  //Second constructor to create a node with all parameters.
  Node(int i_2, int j_2, deque<int> connectedEdges_2){
    i = i_2;
    j = j_2;
    connectedEdges = connectedEdges_2;
    id = i*SIZE + j; //Here 11 to 5
    state = "*";
  };
    
  //Getters
  int getDegree(){return this->connectedEdges.size();};
  int getId(){return this->id;};
  deque<int> getAdjNodes(){return this->connectedEdges;};
  string getState(){return this->state;};
  int getJ(){return j;};
  //Setters

  int getI(){return i;};
    
  void changeState(string newState){state = newState;};
    
  //Operator overloading used for find function.
  bool operator==(const Node& n)
  {
    return (this->id == n.id);
  }
    
  //Deque of adjacent nodes.
  deque<int> connectedEdges;
    
private:
    
  //Variables.
  int i = 0, j = 0, id = 0;
  string state = "*";
    
};

//Class created to keep track of the hexboard.
class HexGraph{
private:
  int victor = -1; //If victor == 0, X(AI1) has won, if victor == 1 O(AI2) has won, if victor == 2 O(Human) has won.
  vector< deque<int> > edgelist;
public:
  //Semi - Trees for player X and player O.
  vector<Node> treeX; //PLYAER X REPRESENTS HUMAN PLAYER
  vector<Node> treeO; //PLAYER O REPRESENTS AI PLAYER
  vector<Node> nodelist;
  int points[SIZE*SIZE] = {0};
  int emptySpc = SIZE*SIZE;
  int iteration = 100;
    
  //Temp copies to store the data of the original hexgraph before random simulation.
  vector<Node> tempTreeX;
  vector<Node> tempTreeO;
  vector<Node> tempNodeList;
  int tempEmptySpc = 0;
  int tempVictor = -1;
  mt19937 r{random_device{}()};
    
  //Constructor that creates 121 nodes, each initialized with correct adjacency deque and coordinates.
  HexGraph(){
    int count = 0;
    edgelist.resize(SIZE*SIZE); //121 to 25
    for (int i = 0; i < SIZE; i++) { //10 to 5
      for (int j = 0; j < SIZE; j++) { //10 to 5
	//Corner nodes with degree 2.
	if ((i == 0 && i == j) || (i == (SIZE-1) && i == j)) { // 10 to 4
	  if (i == 0) {
	    edgelist[count].push_front(1);
	    edgelist[count].push_front(SIZE); //11 to 5
	  } else {
	    edgelist[count].push_front((i-1)*SIZE + j); //119 to 21
	    edgelist[count].push_front(SIZE-1); //119 to 19
	  }
	  //Corner ndoes with degree 3.
	} else if ((i == 0 && j == (SIZE-1)) || (i == (SIZE-1) && j == 0)){ // 10 to 4
	  if (i == 0) {
	    edgelist[count].push_front(j-1); //9
	    edgelist[count].push_front((i+1)*SIZE + (j-1)); //20
	    edgelist[count].push_front((i+1)*SIZE + j); //21
	  } else {
	    edgelist[count].push_front(i*SIZE + 1); //111
	    edgelist[count].push_front((i-1)*SIZE ); //100
	    edgelist[count].push_front((i-1)*SIZE + 1); //99
	  }
	  //Edge nodes with degree 4.
	} else if (i == 0 || j == 0 || i == (SIZE-1) || j == (SIZE-1)){ //4 to 10
	  if (i == 0){
	    edgelist[count].push_front(j - 1);
	    edgelist[count].push_front((SIZE-1) + j);//10
	    edgelist[count].push_front(SIZE + j);//11
	    edgelist[count].push_front(j + 1);
	  } else if (j == 0){
	    edgelist[count].push_front(i*SIZE + 1);
	    edgelist[count].push_front((i-1)*SIZE);
	    edgelist[count].push_front((i-1)*SIZE + 1);
	    edgelist[count].push_front((i+1)*SIZE);
	  } else if (i == 4){
	    edgelist[count].push_front(i*SIZE + (j-1));//109
	    edgelist[count].push_front(i*SIZE + (j+1));//111
	    edgelist[count].push_front((i-1)*SIZE + j);//99
	    edgelist[count].push_front((i-1)*SIZE + (j+1));//100
	  } else {
	    edgelist[count].push_front((i-1)*SIZE + j);//10
	    edgelist[count].push_front((i+1)*SIZE + (j-1));//9
	    edgelist[count].push_front((i+1)*SIZE + j);//10
	    edgelist[count].push_front(i*SIZE + (j-1));//9
	  }
	  //Internal nodes with degree 6.
	} else {
	  edgelist[count].push_front((i-1)*SIZE + j); //All 5 to 11
	  edgelist[count].push_front((i-1)*SIZE + j + 1);
	  edgelist[count].push_front(i*SIZE + j - 1);
	  edgelist[count].push_front(i*SIZE + j + 1);
	  edgelist[count].push_front((i+1)*SIZE + j - 1);
	  edgelist[count].push_front((i+1)*SIZE + j);
	}
	//Pushing the created node to list of nodes.
	nodelist.push_back(Node(i, j, edgelist[count++]));
      }
    }
  }
    
  //Print function to print the current state of the hexboard.
  void printGraph(){
    for (int i = 0; i < SIZE; i++) {
      for (int j = 0; j < SIZE; j++) {
	int id_3 = i*SIZE + j;
	cout << nodelist.at(id_3).getState() << " ";
      }
      cout << "\n";
      int spaceCount = i + 1;
      while (spaceCount != 0) {
	cout << " ";
	spaceCount--;
      }
    }
    cout << "\n";
        
    //Checking if the game has a winner.
    if (victor == 0) {
      cout << "X won!" << endl;
    } else if (victor == 2){
      cout << "O won!" << endl;
    }
  }
    
  //Function that prints the Node information for debugging.
  void printNode(int i, int j){
    int id = i*SIZE + j;
    cout << nodelist[id].getDegree() << endl;
    cout << nodelist[id].getId() << endl;
    deque<int> neighbors = nodelist[id].getAdjNodes();
    for (int k = 0; k < neighbors.size(); k++) {
      cout << neighbors.at(k) << endl;
    }
  }
    
  //A function that randomly selects a node and marks is as X or O depending on the turn. This function also calls DFS whenever a new node is filled.
  void playRandom(vector<Node> availableSpots, int initialMove){
        
    //AFTER FINISHING MONTE CARLO, MAKE SURE THE AVAILABLE SPOTS IS
    //THE ONE RANDOMLY SHUFFLED AND BUT EVEN BEFORE THAT MAKE THE
    //INITIAL MOVE IS MADE. ALSO CREATE A ARRAY OF INTS FROM THE AVAILABLE SPOTS.
    int nodes[SIZE*SIZE];
    for(int n = 0; n < SIZE*SIZE; n++){
      nodes[n] = -1;
    }
    for (int i = 0; i < availableSpots.size();i++) {
      nodes[i] = availableSpots.at(i).getId();
    }
        
    //SHUFFLING THE AVAILABLE SPOTS
    shuffle(begin(nodes), end(nodes), r);

        
    int alternate = 1;
        
    Node initTemp(initialMove);
    vector<Node> :: iterator l = find(nodelist.begin(), nodelist.end(), initTemp);
    int initNodeIndex = distance(nodelist.begin(), l);
    nodelist.at(initNodeIndex).changeState("X");
    treeX.push_back(nodelist.at(initNodeIndex));
        
        
    //Going through a shuffled list of ints which correspond to ID of nodes and filling the random node, then calling DFS on the updated semi-tree.
    for (int p = 0; p < (SIZE*SIZE); p++) {
      Node temp(nodes[p]);
      vector<Node> :: iterator k = find(nodelist.begin(), nodelist.end(), temp);
      int nodeIndex = distance(nodelist.begin(), k);
            
      if(nodes[p] == -1 || nodes[p] == initialMove){
                
      } else if (alternate == 0) {
	nodelist.at(nodeIndex).changeState("X");
	treeX.push_back(nodelist.at(nodeIndex));
	alternate = 1;
      } else {
	nodelist.at(nodeIndex).changeState("O");
	treeO.push_back(nodelist.at(nodeIndex));
	alternate = 0;
      }
            
      //Checking who won once when there are no more empty spaces.
      emptySpc--;
      if(emptySpc == 0){
	if (dfs(treeX) == 1) {
	  victor = 0;
	} else {
	  victor = 1;
	}
	break;
      }
    }
  }
    
    
    
    
  //Depth First Search function called on the semi-tree.
  int dfs(vector<Node> semiTree){
    vector<Node> edgeNodes;
        
    //Grabbing the list of nodes on the edge corresponding to the player.
    edgeNodes = edgeExist(semiTree);
    bool visited[semiTree.size()];
    vector<int> path;
    int gameState = 0;
        
    //Initialzing the visted boolean array to keep track of which nodes have been visted.
    for(int i = 0; i < semiTree.size(); i++){
      visited[i] = false;
    }
        
    while(!edgeNodes.empty()){
      dfsRe(edgeNodes[0], visited, path, semiTree);
      edgeNodes.erase(edgeNodes.begin());
            
      //122 is added to the path when a path has been established thus, marking the game over.
      int endNode = path.at(path.size()-1);
      if(endNode == 1000){
	gameState = 1;
	break;
      } else {
	path.clear();
      }
    }
    return gameState;
  }
    
  //Recurring function. Checks if the new node to be handled is a node on the goal edge.
  void dfsRe(Node edgeNode, bool visited[], vector<int> &path, vector<Node> semiTree){
    vector<Node> :: iterator p = find(semiTree.begin(), semiTree.end(), edgeNode);
    int nodeIndex = distance(semiTree.begin(), p);
    visited[nodeIndex] = true;
        
    //Checking what player the semi tree belongs to.
    if (semiTree.at(0).getState() == "X") {
      if (edgeNode.getId() % SIZE == (SIZE-1)) {
	path.push_back(edgeNode.getId());
	path.push_back(1000);
	return;
      }
    } else {
      if (edgeNode.getId()/SIZE == (SIZE-1)) {
	path.push_back(edgeNode.getId());
	path.push_back(1000);
	return;
      }
    }
        
    //Checking all of the adjacent nodes of the node passed in as a parameter to select the next node to traverse to.
    path.push_back(edgeNode.getId());
    deque<int> :: iterator k;
    for (k = edgeNode.connectedEdges.begin(); k != edgeNode.connectedEdges.end(); ++k){
      if (path.at(path.size()-1) == 1000) {
	return;
      }
      Node temp(*k);
      vector<Node> :: iterator h = find(semiTree.begin(), semiTree.end(), temp);
      if(h != semiTree.end()){
	int nodeIndex_2 = distance(semiTree.begin(), h);
	if (!visited[nodeIndex_2]) {
	  dfsRe(semiTree.at(nodeIndex_2), visited, path, semiTree);
	}
      }
    }
  }
    
  //This function is used to return a vector of nodes on the initial edge. For X all of the nodes marked X on the left edge of the graph and for O all of the nodes marked O on the top row.
  vector<Node> edgeExist(vector<Node> semiGraph){
    vector<Node> edgeNodes;
    if (semiGraph.at(0).getState() == "X") {
      for (int i = 0; i < semiGraph.size(); i ++) {
	if (semiGraph.at(i).getJ() == 0) {
	  edgeNodes.push_back(semiGraph.at(i));
	}
      }
    } else {
      for (int i = 0; i < semiGraph.size(); i ++) {
	if (semiGraph.at(i).getI() == 0) {
	  edgeNodes.push_back(semiGraph.at(i));
	}
      }
    }
    return edgeNodes;
  }
    
  //SAVE GRAPH: save the data of the current hex graph in order to restore after the random simulation ends.
  void saveGraph(){
    tempTreeO = treeO;
    tempTreeX = treeX;
    tempNodeList = nodelist;
    tempEmptySpc = emptySpc;
    tempVictor = victor;
  }
    
  //RESTORE GRAPH: restoring the original graph.
  void restoreGraph(){
    treeO = tempTreeO;
    treeX = tempTreeX;
    nodelist = tempNodeList;
    emptySpc = tempEmptySpc;
    victor = tempVictor;
  }
    
    
  //MONTE-CARLO: Running the actual monte-carlo algorithm.
  int monteCarlo(){
        
    //Creating a vector of avaliable spots and filling them.
    vector<Node> avSpots;
    for(int i = 0; i < nodelist.size(); i++){
      if(nodelist.at(i).getState() == "*"){
	avSpots.push_back(nodelist.at(i));
      }
    }
        
    //Initially setting the best move, current move, current probabilty and the best probabilty to -1.
    int bestMove = -1;
    int currentMove = -1;
    double bestProb = -1;
    double currentProb = -1;
        
    saveGraph();
        
    for (int k = 0; k < avSpots.size(); k++) {
      //The number of wins and losses from randomly playing. Initially set to 0.
      int win = 0, lose = 0;
      currentMove = avSpots.at(k).getId();
            
      //Iterating through the chosen number.
      for (int p = 0; p < iteration; p++) {
                
	playRandom(avSpots, currentMove);
	if(victor == 0){
	  win++;
	} else {
	  lose++;
	}
	restoreGraph();
                
      }
            
      //Getting the probability of winning by maming the current move.
      currentProb = ((win*1.0) / ((double)iteration));
            
      //Checking if the current move has a better probability of winning then the current best move.
      if(currentProb > bestProb){
	bestProb = currentProb;
	bestMove = currentMove;
      }
            
            
    }
        
    return bestMove;
  }
    
    
  //PLAY MONTE-CARLO: Handles the user interaction and actual game play.
  void playMonteCarlo(){
    cout << "Welcome to Hex-board\n" << endl;
    nodelist.at(((SIZE*SIZE)-1)/2).changeState("X"); // 60
    treeX.push_back(nodelist.at(((SIZE*SIZE)-1)/2));//60
    printGraph();
        
    // alternate == 1 -> Human player's turn, alternate == 0 -> Monte Carlos turn.
    int alternate = 1;
        
    //While victor hasn't been decided yet.
    while (victor == -1) {
      if(alternate == 0){
	//AIs TURN
	int move = monteCarlo();
	nodelist.at(move).changeState("X");
	treeX.push_back(nodelist.at(move));
	alternate = 1;
	if (dfs(treeX) == 1) {
	  victor = 0;
	}
                
      } else {
	//HUMANS TURN
	int rowP = -1, colP = -1, nodeIndex = -1;
	bool Nvalid = true;
	while(Nvalid){
	  cout << "Please specify row" << endl;
	  cin >> rowP;
	  cout << "Please specify col" << endl;
	  cin >> colP;
	  nodeIndex = (rowP*SIZE) + colP;
	  if (nodeIndex >= (SIZE*SIZE) || nodeIndex < 0 || nodelist[nodeIndex].getState() != "*") {
	    cout << "INVALID CELL PLEASE REINPUT" << endl;
	  } else {
	    Nvalid = false;
	  }
	}
                
	nodelist.at(nodeIndex).changeState("O");
	treeO.push_back(nodelist.at(nodeIndex));
	alternate = 0;
	if (dfs(treeO) == 1) {
	  victor = 2;
	  alternate = 1;
	}
      }
            
      if(alternate == 1){
	printGraph();
      }
    }
  }
};


int main(int argc, const char * argv[]) {
    
  //Initial seed for randomness.
  srand(time(0));
  HexGraph graph;
  graph.playMonteCarlo();
  return 0;
}
