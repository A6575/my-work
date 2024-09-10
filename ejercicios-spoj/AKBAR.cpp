/*
Estudiante: Astrid Alvarado
User SPOJ: a_7565
Ejercicio: Akbar, The great (https://www.spoj.com/problems/AKBAR/)
Veredicto: Accepted

Soluci�n
La idea consiste en realizar un BFS por el grafo tomando como ra�ces aquellas ciudades d�nde fue asignado un guardia, marcando como
visitado las ciudades que pueden custodiar en funci�n a su fuerza. De esta forma, si se encuentra que m�s de un guardia est� cuidando
una misma ciudad o no todas las ciudades fueron marcadas como visitadas, se dice que los guardias no est�n distribuidos de forma �ptima.
En otro caso, se dice que s� lo est�n.
*/

#include<iostream>
#include<vector>
#include<queue>

using namespace std;

//Variables Globals del problema
const int MAXN = 1e6 + 5;			//Valor m�ximo de N
pair<int, int> K_roots[MAXN];		//Arreglo de pares de enteros donde la primera posici�n representa las ciudades asignadas a los guardias y la segunda posici�n su fuerza 

// Clase de nodo para tener mejor manejo de los datos relacionados con los nodos del grafo
class Nodo{
	public:
		int node;		//Identificador del nodo
		int dist;		//Distancia desde la raiz hasta el nodo actual
		int num;		//Variable auxiliar
		
		// Constructor de la clase Nodo. Si no se indica dist y num toman como valor default 0
		Nodo(int n, int d = 0, int u = 0){
			this->node = n;
			this->dist = d;
			this->num = u;
		}
};

// Clase de grafo implementado como una lista de adyacencia junto con funciones b�sicas para manipular el mismo
class Grafo_Ady_List{
	int nodes;							//Cantidad de nodos
	vector<vector<Nodo> > edges;		//Lista de adyacencia
	vector<int> visited;				//Arreglo para marcar como visitados a los nodos
	public:
		//Constructor de la clase Grafo_Ady_List
		Grafo_Ady_List(int s){
			this->nodes = s;
			this->edges = vector<vector<Nodo> >(s+1);  //Para trabajar con 1-indexaci�n, se crea la lista de adyacencia con n�mero de nodos+1
			this->visited = vector<int>(s+1, -1);      //se inicializa con -1 el vector para indicar que no ha sido visitado
		}
		//Funci�n para agregar un nuevo arco
		void new_edge(Nodo x, Nodo y){
			edges[x.node].push_back(y);
		}
		//Funci�n para agregar un nuevo nodo
		Nodo new_node(int x, int d, int u){
			nodes++;
			edges.push_back(vector<Nodo> ());
			return Nodo(x, d, u);
		}
		//Funci�n para obtener el n�mero de adyacentes que posee un nodo
		int get_num_ady(Nodo x){
			return edges[x.node].size();
		}
		//Funci�n para obtener el nodo adyacente i del nodo x
		int get_ith_ady_of(Nodo x, int i){
			return edges[x.node][i].node;
		}
		//Versi�n modificada de BFS para la resoluci�n del ejercicio
		bool bfs(int sz, int n){
			//Cola de nodos para el funcionamiento de BFS
			queue<Nodo> Q;
			//Almacnamos todas los guardias para que as� sean los puntos de partida del recorrido
			for(int i = 1; i<sz+1; i++){
				Q.push(Nodo(K_roots[i].first, K_roots[i].second, i));
				
				//Si el nodo ya fue visitado, quiere decir que los hay m�s de un guardia cuidando la misma ciudad, por lo tanto no est�n colocados de manera �ptima
				if(visited[K_roots[i].first] != -1){
					return false;
				}
				//Marcamos como visitado a los nodos raices, establleciendo esto como el guardia
				visited[K_roots[i].first] = i;
			}
			//Mientras que la cola no est� vac�a 
			while(!Q.empty()){
				//Sacamos un nodo de la misma y eliminamos
				Nodo cur = Q.front();
				Q.pop();
				
				//S�lo nos interesa aplicar BFS sobre aquellos guardias con fuerza mayor a 0, pues por enunciado si
				//la fuerza del guardia es exactamente 0 s�lo es capaz de cuidar la ciudad donde fue asignado
				if(cur.dist > 0){
					//Recorremos por los adyacentes de nodo actual
					int num_ady = get_num_ady(cur);
					for(int i=0; i<num_ady; i++){
						int ady = get_ith_ady_of(cur, i);
						//Y si este no ha sido visitado a�n
						if(visited[ady] == -1){
							//Marcamos al guardia ue la visit�
							visited[ady] = cur.num;
							//Y guardamos en la cola este adyacente con distancia disminuida en 1
							Q.push(Nodo(ady, cur.dist-1, cur.num));
						}else{
							//En caso de que ya haya sido visitado y por otro guardia distinto al actual, decimos que no est�n bien distribuidos
							if(visited[ady] != -1 && visited[ady] != cur.num){
								return false;
							}
						}
					}
				}
			}
			//En caso de que no todas las ciudades est�n cuidadas por un guardia, se debe indicar que no hay suficientes guardias
			for(int i=1; i<n+1; i++){
				if(visited[i] == -1){
					return false;
				}
			}
			
			return true;
		}
};

//PROGRAMA PRINCIPAL
int main(){
	// Modificaci�n para la optimizaci�n de la entrada y salida de datos
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);
	
	//Variables del programa (por orden): n�mero de casos, n�meros de ciudades, n�mero de guardias, n�mero de carreteras y variables auxiliares a y b
	int T, N, M, R, a, b;
	
	//Se ingresa el n�mero de casos
	cin>>T;
	for(int i=1; i<T+1; i++){
		//Se ingresa la cantidad d ciudades, carreteras y guardias
		cin>>N>>R>>M;
		//Se crea un grafo de tama�o N
		Grafo_Ady_List G(N);
		for(int i=1; i<R+1; i++){
			//Se ingresa las conexiones entre ciudades
			cin>>a>>b;
			G.new_edge(Nodo(a), Nodo(b));
			G.new_edge(Nodo(b), Nodo(a));
		}
		//Se ingresa la ciudad donde fue asignado un guardia y su fuerza
		for(int i=1; i<M+1; i++){
			cin>>K_roots[i].first>>K_roots[i].second;
		}
		//Se realiza BFS, si retorna TRUE quiere decir que todos los guardias est�n ubicados de forma �ptima y se imprime "Yes"
		//De lo contrario se imprime "No"
		if(G.bfs(M,N)){
			cout<<"Yes"<<endl;
		}else{
			cout<<"No"<<endl;
		}
	}
	
	return 0;
}
