/*
Estudiante: Astrid Alvarado
User CodeForces: A_6575
Ejercicio: Hongcow Builds a Nation (https://codeforces.com/problemset/problem/744/A)
Veredicto: Accepted

Solución:
La idea consiste en realizar un recorrido por el grafo partiendo desde los nodos que representan a los gobiernos y generar
diversas componentes conexas para cada gobierno. De esta forma al aplicar dfs, se consigue calcular los nodos disponibles y 
no disponibles con los cuales se puede agregar más arcos y además el grafo siga siendo estable.
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
int MaxN = 2e5+5;

// Clase de nodo para tener mejor manejo de los datos relacionados con los nodos del grafo
class Nodo{
	public:
		int node;			//Identificador del nodo
		//int depth;		//profundidad del nodo en el DFS Tree
		//long long weight;	//peso del nodo
		//int dist;			//Distancia desde la raiz hasta el nodo actual
		//int pred;			//Identificador del nodo predecesor del nodo actual
		//int delta_p;		//Grado exterior del nodo, es decir, |delta^+|
		//int delta_m;		//Grado interior del nodo, es decirm |delta^-|
		//int disc;			//Discovery del nodo, es decir, el tiempo en que fue descubierto
		//int low;			//Low del nodo, es decir, el menor discovery encontrado en el DFS tree generado
		//int cnt_hijos;	//Contador de hijos que tiene un nodo
		//int num;			//Variable auxiliar que representa el kcolor asignado
		bool visited;		//Variable para indicar si un nodo fue visitado 
		//char color;		//Colores de nodos para marcar si fueron visitados. B (Blanco) indica que el nodo no ha sido visitado,
							//G (Gris) que ya fue visitado pero el recorrido aún no ha sido completado y N (Negro) que fue visitado y
							//la búsqueda ha sido terminada por completo.
		
		// Constructor de la clase Nodo. Si no se indica dist y num toma como valor default 0 y 1, además su color por defecto es B.
		//Por otro lado, si no se indica el nivel y su predecesor es 0, a parte de que como valor predeterminado visitado será falso
		Nodo(int n){
			this->node = n;
			//this->depth = 0;
			//this->weight = w;
			//this->dist = 0;
			//this->delta_p = 0;
			//this->delta_m = 0;
			//this->disc = 0;
			//this->low = 0;
			//this->cnt_hijos = 0;
			//this->num = 1;
			this->visited = false;
			//this->color = 'B';
			//this->pred = 0;
			
		}
		// Constructor de la clase nodo con valores predeterminados.
		Nodo(){
			this->node = 0;
			//this->depth = 0;
			//this->weight = 0;
			//this->dist = 0;
			//this->delta_p = 0;
			//this->delta_m = 0;
			//this->disc = 0;
			//this->low = 0;
			//this->cnt_hijos = 0;
			//this->num = 1;
			this->visited = false;
			//this->color = 'B';
			//this->pred = 0;
		}
};

// Clase de grafo implementado como una lista de adyacencia junto con funciones básicas para manipular el mismo
class Grafo_Ady_List{
	int nodes;							//Cantidad de nodos
	vector<vector<Nodo> > edges;		//Lista de adyacencia
	stack<Nodo> aux;					//pila para el funcionamiento del algoritmo Kosaraju
	int time;							//Variable global para actualizar el discovery
	
	public:
		vector<Nodo> list_of_nodes;			//Vector de nodos del grafo creado
		//vector<long long> delta;			//Vector de distancias de costo mínimo delta para cada nodo
		//vector<vector<int> > C_F_C;		//components fuertemente conexas del grafo descubierto por Tarjan
		//vector<pair<int, int> > puentes;	//vector de pares para almacenar los puentes encontrados por Tarjan
		//vector<bool> art_points;          //vector para marcar los puntos de articulación 
		
		//Constructor de la clase Grafo_Ady_List
		Grafo_Ady_List(int s){
			this->nodes = s;
			this->edges = vector<vector<Nodo> >(s+2);  //Para trabajar con 1-indexación, se crea la lista de adyacencia con número de nodos+2
			this->list_of_nodes = vector<Nodo>(s+2);
			//this->delta = vector<long long>(s+1, INF);	   //Inicialmente todos los nodos tienen distancia INF
			//this->C_F_C = vector<vector<int> >(s+1);
			//this->art_points = vector<bool>(s+1, false);
			//this->time = 0;
		}
		//Función para agregar un nuevo arco
		void new_edge(Nodo x, Nodo y){
			edges[x.node].push_back(y);
		}
		
		//Función para obtener el número de adyacentes que posee un nodo
		int get_num_ady(Nodo x){
			return edges[x.node].size();
		}
		//Función para obtener el valor del nodo adyacente i del nodo x
		int get_ith_ady_of(Nodo x, int i){
			return edges[x.node][i].node;
		}
		//Función para obtener el nodo adyacente i del nodo x
		Nodo get_ith_adynode_of(Nodo x, int i){
			return edges[x.node][i];
		}
	
		//Algoritmo de dfs modificado para que cuente aquellos nodos que son alcanzados entre sí, incluyendo
		//el nodo raíz
		void dfs(Nodo root, int *cnt){
			stack<Nodo> Q;
			//se agrega la raíz a la pila
			Q.push(root);
			//se cuenta la cantidad de nodos alcanzados, en este caso, se cuenta que el
			//nodo raíz es capaz de alcanzarse a sí mismo
			*(cnt)+=1;
			//marcamos como visitdo
			list_of_nodes[root.node].visited = true;
			
			//mientras la pila no esté vacía
			while(!Q.empty()){
				//se saca de la pila el nodo actual
				Nodo cur = Q.top();
				Q.pop();
				
				//se itera por sus adyacentes
				int num_ady = get_num_ady(cur);
				for(int i=0; i<num_ady; i++){
					int ady = get_ith_ady_of(list_of_nodes[cur.node], i);
					//y si no ha sido visitado
					if(!list_of_nodes[ady].visited){
						//se aumenta la cantidad de nodos alcanzados
						*(cnt)+=1;
						//se marca como visitado y se agrega a la pila
						list_of_nodes[ady].visited = true;
						Q.push(list_of_nodes[ady]);
					}
				}	
			}
		}
		
};

//PROGRAMA PRINCIPAL
int main(){
	// Modificación para la optimización de la entrada y salida de datos
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);
	
	//variables del programa
	int n,m,k,u,v,connect, new_nodes,
		total_connect = 0,
		max_connect = 0,
		edges_added = 0;
	
	//se ingresan el número de nodos, número de arcos actualmente en el grafo
	//y se agrega la cantidad de gobiernos que hay
	cin>>n>>m>>k;
	
	//se crea un grafo de tamaño n
	Grafo_Ady_List G(n);
	
	//se inicializa la lista de nodos
	for(int i=1; i<n+1; i++){
		G.list_of_nodes[i] = Nodo(i);
	}
	
	//se crea un vector de tamaño k y se almacenan los gobiernos
	vector<int> governments(k,0);
	for(int i=0; i<k; i++){
		cin>>governments[i];
	}
	
	//se agrega los arcos actuales del grafo
	for(int i=0; i<m; i++){
		cin>>u>>v;
		G.new_edge(Nodo(u), Nodo(v));
		G.new_edge(Nodo(v), Nodo(u));
	}
	
	//Se realiza dfs por el grafo partiendo de todos los nodos de gobierno, de esta forma
	//se asegura que cada gobierno pertenezca a una componente conexa separada
	for(int i=0; i<k; i++){
		//contador de nodos alcanzados en cada dfs realizado
		connect = 0;
		G.dfs(Nodo(governments[i]), &connect);
		
		//se calcula el máximo número de nodos conectados, es decir, aquellos nodos con arista asignada
		max_connect = max(connect, max_connect);
		//se aumenta la cantidad de nodos no disponibles para conectar entre sí, bien sea por ser gobiernos
		//o por ya estar con arco asignado
		total_connect+=connect;
		//se calcula la cantidad de arcos totales sabiendo que por cada sub-grafo la cantidad máxima de arcos 
		//que puede tener es, si n es el números de nodos, n*(n-1)/2
		edges_added+=(connect*(connect-1))/2;
	}
	//calculamos la cantidad de nodos disponibles restando los nodos ya apartados, sumado al máximo de números conectados
	//para poder realizar los arcos 
	new_nodes = n-total_connect+max_connect;
	
	//a la cantidad de arcos posibles se le debe restar la cantidad de arcos máxima que se pueden hacer con los nodos ya conectados
	edges_added-=(max_connect*(max_connect-1)/2);
	
	//de la misma forma, se le agrega la cantidad de arcos máxima que se pueden tener con la nueva cantidad de nodos disponibles
	edges_added+=(new_nodes*(new_nodes-1))/2;
	
	//finalmente, se debe restar las aristas ya agregadas desde un principio
	edges_added-=m;
	
	//se imprime el número de nodos a agregar
	cout<<edges_added<<endl;
	return 0;
}
