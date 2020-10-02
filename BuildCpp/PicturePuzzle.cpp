#include <iostream>
#define MAX 500
using namespace std;

int n = 9; //total elements
int d = 3; //elements on a row
int picture[] = {1,2,3,4,0,5,6,7,8}; //Picture 3x3
int picKey;
/*puzzle forging steps
123405678->103425678->013425678->413025678->413205678->413275608
->413275068->413075268->413705268->403715268->430715268->435710268
->435701268->435761208->435761280->435760281->435706281*/
int puzzle[] = {4,3,5,7,6,1,2,8,0};
int moves[] = {1,-1,d,-d};
int costs[] = {0,0,0,0};
int trace[MAX];
int currentTrace = 0;
/* Results:
Depth 1 step: couldn't solve 435710268,435760281,435706281 (solved 13)
Depth 6 steps: solved until 435760281 (solved 14)
*/
void printPicture(int *pic){
	for(int i=0; i<n; i++){
		cout<<pic[i];
		if((i+1)%d==0) cout<<endl;
	}
}

int getKey(int *pic){
	int key = pic[0];
	for(int i=1; i<n; i++) key = key*10+pic[i];
	return key;
}

int* tryStep(int *puzzle, int dir){
	int i=0;
	while(puzzle[i]!=0) i++;
	if(dir==-1 && (i+d)%d==0) return NULL; //piece 0 is first in row can't move backward
	if(dir==1 && (i+1)%d==0) return NULL; //piece 0 is last in row can't move forward
	if((i+dir)>=0 && (i+dir)<n){
		swap(puzzle[i], puzzle[i+dir]);
		return puzzle;
	}
	return NULL; //piece 0 is on top, bottom row can't move up/down
}

int getCost(int *puzzle, int step){
	int cost = step;
	for(int i=0; i<n; i++) if(puzzle[i]!=picture[i]) cost += 1;
	return cost;
}

int getMin(int a, int b){
	if(a<0) return b;
	if(b<0) return a;
	return(a<b)?a:b;
}

bool hadTrace(int key){
	int i=0;
	while(trace[i]!=0 && trace[i]!=key) i++;
	return(trace[i]!=key)?false:true;
}

int estimateCost(int *puzzle, int currentStep, int estimated, int maxSteps){
	int c[] = {0,0,0,0};
	for(int i=0; i<4; i++){
		if(tryStep(puzzle, moves[i])!=NULL){
		//if step is valid and is not in traces
			if(!hadTrace(getKey(puzzle))){
				//add trace
				currentTrace += 1;
				trace[currentTrace] = getKey(puzzle);
				//estimate deeper steps
				if(estimated<maxSteps) c[i] = getCost(puzzle, currentStep) 
					+ estimateCost(puzzle, currentStep+1, estimated+1, maxSteps);
				//remove trace
				trace[currentTrace] = 0;
				currentTrace -= 1;
			}
			else c[i] = -1;
			tryStep(puzzle, -moves[i]); //undo step
		}
		else c[i] = -1;
	}
	return getMin(c[0],getMin(c[1],getMin(c[2],c[3])));
}

void solvePuzzle(int* puzzle){
	int step = 0;
	while(getKey(puzzle)!=picKey && currentTrace<MAX &&
			trace[currentTrace] != trace[currentTrace-1]){ //stop if traces duplicated
		for(int i=0; i<4; i++){
			if(tryStep(puzzle, moves[i])!= NULL){
				if(!hadTrace(getKey(puzzle))){
					//add trace
					currentTrace += 1;
					trace[currentTrace] = getKey(puzzle);
					//use 0 for max steps of estimateCost for depth = 1;
					costs[i] = getCost(puzzle,step)+estimateCost(puzzle,step+1,1,6);
					//remove trace
					trace[currentTrace] = 0;
					currentTrace -= 1;
				}
				else costs[i] = -1;
				//undo step
				tryStep(puzzle, -moves[i]);
			}
			else costs[i] = -1;
		}
		int min = getMin(costs[0],getMin(costs[1],getMin(costs[2],costs[3])));
		int i = 0;
		while(i<4 && min != costs[i]) i++;
		tryStep(puzzle, moves[i]); //perform min cost step
		step += 1;
		currentTrace += 1;
		trace[currentTrace] = getKey(puzzle); //add trace
		printPicture(puzzle);
		cout<<"----"<<endl;
	}
	if(getKey(puzzle)==getKey(picture)) cout<<"Solved after "<<step<<" moves";
	else cout<<"Could not solve after "<<step<<" moves";
}

int main(){
	cout<<"Picture:"<<endl;
	printPicture(picture);
	picKey = getKey(picture);
	//---------------------------------
	cout<<endl<<"Puzzle:"<<endl;
	printPicture(puzzle);
	trace[currentTrace] = getKey(puzzle); //first trace is puzzle key
	//---------------------------------
	cout<<endl<<"Solving puzzle:"<<endl;
	solvePuzzle(puzzle);
	getchar();
	return 0;
}
