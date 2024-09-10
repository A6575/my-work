/*
Estudiante: Astrid Alvarado
User CodeForces: A_6575
Ejercicio: Jumping on Walls (https://codeforces.com/problemset/problem/198/B)
Veredicto: Accepted

Solución:
La idea consiste en recorrer el grafo representado con matriz de adyacencia usando el algoritmo de BFS. Este recorido se hace siguiendo los posibles
movimientos del ninja, y cuando conseguimos que la coordenada y alcanza o sobrepasa la altura de la pared se dice que puede escapar del cañón, en
caso contrario se dice que no es posible 

*/
#include<iostream>
#include<vector>
#include<queue>
#include<stack>
#include<string>
#include<algorithm>
#include<set>

using namespace std;

#define INF 2e9+11

//Variables globales del programa
int MaxN = 1e5+5;

//Nueva clase nodo para un grafo implmentado como una matriz de adyacencia
class Nodo_matrix{
	public:
		int x;				//nodo x de la matriz
		int y;				//nodo y de la matriz
		int lvl;			//nivel del nodo en el grafo
		
		//Constructor de la clase Nodo_matrix
		Nodo_matrix(){
			this->x=0;
			this->y=0;
			this->lvl=0;
		}
};

//Nueva clase de grafo implementado como una matriz de adyacencia
class Grafo_Ady_Matrix{
	int nodes;									//cantidad de nodos del grafo
	int m;
	vector<vector<int> >matrix;					//matriz de adyacencia
	//vector<vector<long long> > m_weight;		//matriz de pesos de los nodos, aquí el peso ubicado en la posición i,j corresponde con el arco desde el nodo i al nodo j
	public:
		//vector<long long> weight;				//vector de pesos para la función Floyd-Warshall
		//vector<bool> visited;					//vector e visitaos de los nodos
		
		//Constructor de la clase Grafo_Ady_Matrix
		Grafo_Ady_Matrix(int s){
			this->nodes = s;
			this->matrix = vector<vector<int> >(s+1, vector<int>(s+1, 0));
			/*
			this->weight = vector<long long>(s+1, INF);
			this->m_weight = vector<vector<long long> >(s+1, vector<long long>(s+1, INF));
			this->visited = vector<bool>(s+1, false);
			*/
		}
		//cosntructor de la clase Grafo_Ady_Matrix para la resolución del ejercicio
		Grafo_Ady_Matrix(int s, int s1){
			this->nodes = s;
			this->m = s1;
			this->matrix = vector<vector<int> >(s, vector<int>(s1, 0)); 
		}
		//función que marca como 1 en la matriz de adyacencia cuando existe un arco desde el nodo i hasta el nodo j
		void new_conection(int i, int j){
			matrix[i][j] = 1; 
		}
		//función para agregar nuevos nodos a la representación
		void new_node_m(int num_nodes){
			matrix.resize(nodes+num_nodes+1, vector<int>(nodes+num_nodes+1, 0));
		}
		
		//Algoritmo de recorrido BFS aplicado a la representación matricial de un grafo
		bool bfs(Nodo_matrix root, int n, int k){
			//Nodo auxiliar
			Nodo_matrix next;
			//Cola de nodos para el recorrido
			queue<Nodo_matrix> Q;
			//se agrega el nodo raiz
			Q.push(root);
			//se marca como 0 la raiz en el grafo para iniciar la búsqueda
			matrix[root.x][root.y] = 0;
			
			//mientras que no esté vacía
			while(!Q.empty()){
				//scamos y elminamos de la cola 
				Nodo_matrix cur = Q.front();
				Q.pop();
				
				//Si la coordenada y es mayor al número de nodos quiere decir que el ninja logra escapar correctamente,
				//por lo que se retorna true
				if(cur.y > n){
					return true;
				}
				
				//recorremos el grafo siguiendo 3 opciones
				for(int i=0; i<3; i++){
					//aumentamos el nivel del nodo
					next.lvl = cur.lvl + 1;
					
					if(i==0){				//en primer lugar intentamos desplazarnos a la derecha (equivalente a bajar una unidad la pared)
						next.x = cur.x;
						next.y = cur.y + 1;
					}else if(i == 1){		//en segundo lugar intentamos desplazarnos a la izquierda (equivalente a subir una unidad la pared)
						next.x = cur.x;
						next.y = cur.y - 1;
					}else if(i == 2){		//en tercer lugar intentamos desplazarnos a la pared opuesta con un salto de altura k
						next.x = 1 - cur.x;
						next.y = cur.y + k;
					}
					//si la coordenada y es mayor a n-1, quiere decir que el ninja puede escapar correctamente y además se mantiene en la dimensión de la matriz
					if(next.y > n-1){
						return true;
					}else{
						//en otro caso, si es posible desplazarse dentro de la matriz, es igual a 1 (una zona segura) y el nivel del nodo no sobrepasa la coordenada y 
						if(next.y > -1 && next.x > -1 && next.x < 2 && matrix[next.x][next.y] != 0 && next.lvl < next.y+1){
							//marcamos como 0 al nodo actual en la matriz y guardamos en la cola
							matrix[next.x][next.y] = 0;
							Q.push(next);
						}
					}
				}
			}
			return false;
		}		
};

//Varaible global:estructura de grafo
Grafo_Ady_Matrix G(2, MaxN);

//PROGRAMA PRINCIPAL
int main(){
	// Modificación para la optimización de la entrada y salida de datos
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);
	
	//Variables del programa
	int n, k;
	string walls[2];
	Nodo_matrix root;
	
	//ingresamos el número de nodos (altura de la pared) y los pasos a desplazarse (la altura del salto del ninja)
	cin>>n>>k;
	
	//ingresamos la decripción de las dos paredes
	for(int i=0; i<2; i++){
		cin>>walls[i];
		//iteramos por el string y al encontrar un "-" se marca como 1 en la matriz de adyacencia
		for(int j=0; j<n; j++){
			if(walls[i][j] == '-'){
				G.new_conection(i, j);
			}
		}
	}
	//si al recorer el grafo con bfs retorna true, se imprime que es posible que el ninja escape, en caso contrario se retorna que no es posible
	if(G.bfs(root, n, k)){
		cout<<"YES"<<endl;
	}else{
		cout<<"NO"<<endl;
	}
	
	return 0;
}
