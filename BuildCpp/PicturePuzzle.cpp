#include <iostream>
#include <windows.h>
using namespace std;

int n = 9; //total elements
int d = 3; //elements on a row
int picture[] = {1,2,3,4,0,5,6,7,8};
int picKey;
int puzzle[] = {4,3,0,7,1,5,2,6,8}; //unable to solve: 435710268
int moves[] = {1,-1,d,-d};
int costs[] = {0,0,0,0};
int trace[200];
int currentTrace = 0;

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
	while(i<n && puzzle[i]!=0) i++;
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
	else return(a<b)?a:b;
}

bool hadTrace(int key){
	int i=0;
	while(trace[i]!=0 && trace[i]!=key) i++;
	return(trace[i]!=key)?false:true;
}

void solvePuzzle(int* puzzle){
	int step = 0;
	while(getKey(puzzle)!=getKey(picture) && 
			trace[currentTrace-1] != trace[currentTrace-2]){ //stop if traces duplicated
		for(int i=0; i<4; i++){
			if(tryStep(puzzle, moves[i])!=NULL){
			//if step is valid and is not in traces
				if(!hadTrace(getKey(puzzle))) costs[i] = getCost(puzzle, step);
				else costs[i] = -1;
				tryStep(puzzle, -moves[i]); //undo step
			}
			else costs[i] = -1;
		}
		int min = getMin(costs[0],getMin(costs[1],getMin(costs[2],costs[3])));
		int i = 0;
		while(i<4 && min != costs[i]) i++;
		tryStep(puzzle, moves[i]); //perform min cost step
		step += 1;
		trace[currentTrace] = getKey(puzzle); //add trace
		currentTrace += 1;
		printPicture(puzzle);
		cout<<"----"<<endl;
		Sleep(1000);
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
	currentTrace += 1;
	//---------------------------------
	cout<<endl<<"Solving puzzle:"<<endl;
	solvePuzzle(puzzle);
	return 0;
}
