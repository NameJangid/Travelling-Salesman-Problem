#include <iostream>
#include <ctime>
#include <vector>
#include <algorithm>
#include <cmath>
#include <cfloat>
#include <limits>

using namespace std;

double probablity(double currentEnergy, double newEnergy, double temperature)
{
	if(newEnergy<currentEnergy)
		return 1;
	else 
		return (double) exp( (double) (currentEnergy - newEnergy)/temperature );
}


int ranFunc (int i) { return (rand()*time(0)+1) % i; }

int main(int argc, char const *argv[])
{
	/* code */
	/*********Read input  start********/
	bool eucFlag=true;
	string typ;
	cin>>typ;
	if(typ=="euclidean")
		eucFlag=true;
	else
		eucFlag=false;

	int N;
	cin>>N;

	double distance[N][N];

	double coordinates[N][2];
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < 2; ++j)
		{
			cin>>coordinates[i][j];//i is row and j is column
		}
	}
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			cin>>distance[i][j];//i is row and j is column
		}
	}
	/*********Read input END********/

	/**************Start initialising stuff**********/
	double temperature=10000;
	double coolingRate=0.05;

	int n=N;

	vector<int> currState(n);

	for (int l = 0; l < n; ++l)//generatin random state
		currState[l]=l;	

	random_shuffle(currState.begin(), currState.end(), ranFunc); 

	double currentEnergy=0;
	for (int l = 0; l < n-1; ++l)//generatin random state
		currentEnergy+=distance[currState[l]][currState[l+1]];
	currentEnergy+=distance[currState[n-1]][currState[0]];
	double newEnergy;


	double bestEnergy = currentEnergy;
	vector<int> bestState(currState);


	for (int i = 0; i < 100; ++i)
	{
		temperature=	100000000;//found maximum at 10^18
		//shuffling again for new random state
		random_shuffle(currState.begin(), currState.end(), ranFunc); 

		currentEnergy=0;
		for (int l = 0; l < n-1; ++l)//generatin random state
			currentEnergy+=distance[currState[l]][currState[l+1]];
		currentEnergy+=distance[currState[n-1]][currState[0]];
		/**************END initialising stuff**********/
		//[TO DO] add iterations for the whole temperature variance(from high to low)
		while(temperature>1)
		{
			
			//[TO DO] generate proper random cities
			//shifting cities in current state on indices city1 and city2
			int iterations=1000;
			while(iterations--)
				{
				int city1=ranFunc(n);
				int city2=ranFunc(n);


				// cout<<"city1: "<<city1<<"city2: "<<city2<<"\t";
				vector<int> newState(currState);


				while(city1==city2)
					city2=ranFunc(n);

				int tmp=newState[city1];
				newState[city1]=newState[city2];
				newState[city2]=tmp;



				newEnergy=0;
				for (int k = 0; k < n-1; ++k)
						newEnergy+=distance[newState[k]][newState[k+1]];
					newEnergy+=distance[newState[n-1]][newState[0]];
				
				double p=probablity(currentEnergy,newEnergy,temperature);
				
				if(p > (double) rand()/  (double) RAND_MAX)
				{
					// cout<<"acceptin p"<<endl;
					currState=newState;
					currentEnergy=newEnergy;
				}


				if(bestEnergy>currentEnergy)
				{
					bestState=currState;
					bestEnergy=currentEnergy;

					for (int k = 0; k < n-1; ++k)
						cout<<bestState[k]<<" ";
					cout<<bestState[n-1]<<endl;
				}
				

			}
			temperature=temperature*(1- coolingRate );//1-coolingRate
			// cout<<"new bestEnergy: "<<bestEnergy<<"Temperature: "<<temperature<<endl;
		}
}

	return 0;
}