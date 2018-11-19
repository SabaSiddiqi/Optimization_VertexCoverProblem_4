#define MAX 500
// defined std::unique_ptr
#include <memory>
// defines Var and Lit
#include "minisat/core/SolverTypes.h"
// defines Solver
#include "minisat/core/Solver.h"
// defined std:://////cout
#include <iostream>
#include <vector>

using namespace std;
using namespace Minisat;

int compare(const void * a, const void * b)
{
    return ( *(int*)a - *(int*)b );
}

int main(void) {
    
	// -- allocate on the heap so that we can reset later if needed
    std::unique_ptr<Minisat::Solver> solver(new Minisat::Solver());
    
	//Defining variables
	char input[MAX];
    char command;
    char *split=" {}<>,E";
    char *token;
    int e,i;
    int edges[MAX];
	int n;//number of vertices
	int k; //vector cover number 
	int j,m,q,p,l;
	int items; //n x k
	
	//continuously read input unless eof
	while (fgets(input,MAX,stdin)!=NULL){
	
	command = input[0];
	
	switch(command){
	case 'V':
		sscanf(input,"V %d",&n);  // extract #of vertex from input
		break;
		
	case 'E':
		e=0;
		i=0;
        token=strtok(input,split);
        while (token){
            sscanf(token,"%d",&edges[e]);
            token = strtok(NULL,split);
             e++;
            }
		for(i=0;i<e-1;i+=2){
            }
		break;
	
	default:
		break;
	}
	
	if((e-1)>0){
	
	vec <Lit> element;
	vector <int> g;
	
	for(k=1;k<=n;k++){
		
		element.clear();
		
		//initiate literals 
		for(j=1;j<=k;j++){
			for( i = 1; i <= n; i++){
				Lit l = mkLit(solver->newVar());
				element.push(l);
			}
		}
		
		//CLAUSE 1
		Minisat::vec<Minisat::Lit> clause1; 			//create clause 
		for (j=1;j<=k;j++){ 							//j -> [1,k]
			for (i=1;i<=n;i++){ 						// i -> [1-n]
				clause1.push(element[((j-1)*n)+i-1]);
			}
			solver->addClause(clause1);
			clause1.clear();
		}

		//CLAUSE 2
		Minisat::vec<Minisat::Lit> clause2; 			//create clause 
		for (m=1;m<=n;m++){ 							//m -> [1,n]
			for (q=1;q<=k;q++){ 						// q -> [1-k]
				for(p=1;p<q;p++){ 						// p<q
					clause2.push(~element[((p-1)*n)+m-1]);	// add to clause
					clause2.push(~element[((q-1)*n)+m-1]);	// add to clause
					solver->addClause(clause2);			// push clause to solver 
					clause2.clear();					
				}
			}
		}

		//CLAUSE 3
		Minisat::vec<Minisat::Lit> clause3; 			//create clause 
		for (m=1;m<=k;m++){ 							//m -> [1,k]
			for (q=1;q<=n;q++){ 						// q -> [1-n]
				for(p=1;p<q;p++){ 						// p<q
					clause3.push(~element[((m-1)*n)+p-1]);	// add to clause 
					clause3.push(~element[((m-1)*n)+q-1]);	// add to clause 
					solver->addClause(clause3);			// push clause to solver 
					clause3.clear();					// clear clause for reuse
				}
			}
		}		

		//CLAUSE 4
		Minisat::vec<Minisat::Lit> clause4; //create clause 
		for (l=0;l<e-1;l+=2){ //extracting edges
			i=edges[l]+1;
			j=edges[l+1]+1;
			if(i>n || j>n){
				return 0;
			}
			for (m=1;m<=k;m++){ // q -> [1-n]
				clause4.push(element[((m-1)*n)+i -1]);
			}
			
			for (p=1;p<=k;p++){ // q -> [1-n]
				clause4.push(element[((p-1)*n)+j-1]);
			}
			solver->addClause(clause4);
			clause4.clear();
		}
    
	bool res = solver->solve();
    
	items = element.size();
	
	int f[items];
	int value[k];
	int idx=0;
	if (res==1){
		for(i=0;i<items;i++){
			f[i]=Minisat::toInt(solver->modelValue(element[i]));
			if(f[i]==0){
				value[idx]=(i%n);	
				idx++;
			}
		}
		
		qsort(value, k, sizeof(int), compare);
		
		for(idx=0;idx<k;idx++){	
			printf("%d ",value[idx]);
		}
	
	
	printf("\n");
	}

	solver.reset (new Minisat::Solver());
	
	if(res==1){
		//resetting edge array
		e=0;
		edges[MAX]={0};
		res=0;
		break;}
	} // for k loop close 
		
		e=0;
		edges[MAX]={0};
	
	}
		
	} //while close

    return 0;
}
