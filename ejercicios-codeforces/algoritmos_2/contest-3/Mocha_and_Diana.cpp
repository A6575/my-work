/*
Estudiante: Astrid Alvarado
User CodeForce: A_6575
Ejercicio: Mocha and Diana (Easy Version) (https://codeforces.com/problemset/problem/1559/D1)
Veredicto: Accepted

Solucion:
la idea consite en representar los forest de Mocha y Diana usando DSU,
e ir iterando en ambos para verificar si dos extremo de una arista 
está en ambos forest, y si no se encuentran entonces se puede agregar en ambos.
*/

#include<iostream>
#include<vector>
#include<algorithm>


using namespace std;

//Implementación de DSU:
int Mocha_forest[1001], //representante del forest de Mocha
    Diana_forest[1001]; //representante del forest de Diana

int find(int x, int repr[]){
	if (repr[x] != x){
		return repr[x] = find(repr[x], repr); 
	}
	return x;
}

void join(int n1, int n2, int repr[]){
	n1 = find(n1, repr);
	n2 = find(n2, repr);
	repr[n1] = n2;

}

void make_set(int idx, int repr[]){
	repr[idx] = idx;
}

int main(){
	//Variables del programa
	int n, 
	    m1, 
		m2,
		u, 
		v,
		M_u, 
		M_v, 
		D_u, 
		D_v, 
		h = 0;
	cin>>n>>m1>>m2;
	vector<pair<int, int> > answer(n); //vector de pares donde se almacenarán las aristas que se pueden agregar
	
    for(int i=1; i<=n; i++){
    	//ingresamos la cantidad de nodos a ambos representantes
    	make_set(i, Mocha_forest);
    	make_set(i, Diana_forest);
    }	
	for(int i=0; i<m1; i++){
		//guardamos las aristas del forest de Mocha.
		//Las aristas se representan de la forma repr[u] = v.
		cin>>u>>v;
		join(u, v, Mocha_forest);
	}
	for(int i=0; i<m2; i++){
		//guardamos las aristas del forest de Diana.
		//Las aristas se representan de la forma repr[u] = v.
		cin>>u>>v;
		join(u, v, Diana_forest);
	}
	//iteramos sobre ambos forest
	for(int i=1; i<=n; i++){
    	for(int j=1; j<=n; j++){
    		//y buscamos el valor correspondiente a los nodos i y j
    		M_u = find(i, Mocha_forest); 
		    M_v = find(j, Mocha_forest);
		    D_u = find(i, Diana_forest);
		    D_v = find(j, Diana_forest);
		    //si los valores de los nodos i y j en el forest de Mocha son
		    //diferentes y además los valores de los nodos i y j en el forest 
		    //de Diana también son diferentes, quiere decir que dichos valores
		    //no están en los forest de ambos y se puede agregar esa arista
    		if(M_u != M_v && D_u != D_v){
    			h++; //contamos las veces en las que se cumple la condición
    			answer[h].first = i;
    			answer[h].second = j; //almacenamos la arista
    			Mocha_forest[M_u] = M_v; 
    			Diana_forest[D_u] = D_v; //vinculamos los conjuntos con la arista en ambos forest
    		}
        }
    }
    //Finalmente retornamos los valores
    cout<<h;
    for(int i = 1; i<=h; i++){
    	cout<<"\n"<<answer[i].first<<" "<<answer[i].second;
    }
	return 0;
}
