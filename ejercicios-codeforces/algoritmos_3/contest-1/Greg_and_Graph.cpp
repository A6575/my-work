/*
Estudiante: Astrid Alvarado
User CodeForces: A_6575
Ejercicio: Greg and Graph (https://codeforces.com/problemset/problem/295/B)
Veredicto: Accepted

Solución:
La idea consiste en ir consiguiendo la suma de los costos de cada paso al realizar el camino de costo mínimo. Puesto que se debe conseguir
el camino de costo mínimo para cada nodo del grafo, se usa Floyd-Warshall para coseguir dicho costo. Finalmente se retorna la suma de 
los costos para cada vértice que se desea eliminar
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
int MaxN = 550;

//Nueva clase de grafo implementado como una matriz de adyacencia
class Grafo_Ady_Matrix{
	int nodes;									//cantidad de nodos del grafo
	vector<vector<int> >matrix;					//matriz de adyacencia
	vector<vector<long long> > m_weight;		//matriz de pesos de los nodos, aquí el peso ubicado en la posición i,j corresponde con el arco desde el nodo i al nodo j
	public:
		vector<long long> weight;				//vector de pesos para la función Floyd-Warshall
		vector<bool> visited;					//vector e visitaos de los nodos
		Grafo_Ady_Matrix(int s){
			this->nodes = s;
			this->matrix = vector<vector<int> >(s+1, vector<int>(s+1, 0));
			this->weight = vector<long long>(s+1, INF);
			this->m_weight = vector<vector<long long> >(s+1, vector<long long>(s+1, INF));
			this->visited = vector<bool>(s+1, false);
		}
		//función que marca como 1 en la matriz de adyacencia cuando existe un arco desde el nodo i hasta el nodo j
		void new_conection(int i, int j){
			matrix[i][j] = 1; 
		}
		//función para agregar nuevos nodos a la representación
		void new_node_m(int num_nodes){
			matrix.resize(nodes+num_nodes+1, vector<int>(nodes+num_nodes+1, 0));
		}
		//función para guardar los pesos de los arcos correspondientes
		void new_weight(int i, int j, long long w){
			m_weight[i][j] = w;
		}
		
		//Algoritmo de Floyd-Warshall modificado para que además de calcular el camino de costo mínimo, obtenga la suma de los
		//costos en cada paso
		void Floyd_Warshall(int len, vector<long long> &v){
			
			//El algoritmo de Floyd-Warshall consiste en ir expandiendo los caminos del subconjunto V_p e ir actualizando el costo
			//haciendo la comparación entre el nodo actual con algún nodo intermediario k de este conjunto. Es por esto que es necesario
			//empezar a iterar por los nodos a eliminar desde el último hasta el primero, para así conseguir el costo de caminos de tamaño p
			for(int p=len; p>0; p--){
				//el costo antes del paso p es igual a 0
				weight[p] = 0;
				
				long long k = v[p];		//se calcula el costo usando como nodo intermediario el último nodo a eliminar
				
				//se marca como visitado
				visited[k] = true;
				
				//y se actualiza los cosotos de la matriz de pesos del grafo
				for(int i=1; i<len+1; i++){
					for(int j=1; j<len+1; j++){
						//se busca el costo consiguiendo el mínimo valor entre el costo de los vértices i,j con algún camino
						//formado con el nodo k como intermediario k
						m_weight[i][j] = min(m_weight[i][j], m_weight[i][k] + m_weight[k][j]);
					}

				}
				//Una ve actualizado el costo, se busca la suma de todos los costos del camino p si los nodos i,j ya fueron visitados
				for(int i=1; i<len+1; i++){
					for(int j=1; j<len+1; j++){
						if(visited[i]&&visited[j]){
							weight[p] += m_weight[i][j];
						}
					}
				}
			}
		}
		
};

Grafo_Ady_Matrix G(MaxN);
vector<long long> deleted_nodes(MaxN);

//PROGRAMA PRINCIPAL
int main(){
	// Modificación para la optimización de la entrada y salida de datos
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);
	
	//variables del programa
	long long w;
	int n;
	
	//se ingresa el número de nodos
	cin>>n;
	
	//se almacenan los costos de cada arco i,j
	for(int i=1; i<n+1; i++){
		for(int j=1; j<n+1; j++){
			cin>>w;
			G.new_weight(i,j,w);
		}
	}
	//se ingresan los nodos a eliminar
	for(int i=1; i<n+1; i++){
		cin>>deleted_nodes[i];
	}
	//se aplica el algoritmo de Floyd-Warshall
	G.Floyd_Warshall(n, deleted_nodes);
	
	//finalmente, se imprime los costos acumulados antes de elminar los nodos dados
	for(int i=1; i<n+1; i++){
		cout<<G.weight[i]<<" ";
	}
	cout<<endl;
	
	return 0;
}
