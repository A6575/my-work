/*
Estudiante: Astrid Alvarado
User CodeForces: A_6575
Ejercicio: Gift (https://codeforces.com/problemset/problem/76/A)
Veredicto: Accepted

Solución:
La idea consiste en generar un Minimum Spanning Tree (MST) tal que el mínimo costo de este represente el 
mínimo costo en tugriks del regalo requerido para que así no se realicen más robos. Por lo tanto, se crea
un MST buscando conseguir realizar esta conversión con la mayor cantidad de plata y la mínima cantidad 
de oro. Por lo que, se aplica una versión del algoritmo de Kruskal para hallar el MST condicionado a dos
costos.  
*/
#include<iostream>
#include<vector>
#include<queue>
#include<stack>
#include<string>
#include<algorithm>
#include<set>
#include<climits>

using namespace std;

#define INF LLONG_MAX

// Clase de nodo para tener mejor manejo de los datos relacionados con los nodos del grafo
class Nodo{
	public:
		int node;			//Identificador del nodo
		//int depth;		//profundidad del nodo en el DFS Tree
		long long int weight;	//peso del nodo
		long long int weight2;
		//int dist;			//Distancia desde la raiz hasta el nodo actual
		//int pred;			//Identificador del nodo predecesor del nodo actual
		//int delta_p;		//Grado exterior del nodo, es decir, |delta^+|
		//int delta_m;		//Grado interior del nodo, es decirm |delta^-|
		//int disc;			//Discovery del nodo, es decir, el tiempo en que fue descubierto
		//int low;			//Low del nodo, es decir, el menor discovery encontrado en el DFS tree generado
		//int cnt_hijos;	//Contador de hijos que tiene un nodo
		//int num;			//Variable auxiliar que representa el kcolor asignado
		//bool visited;		//Variable para indicar si un nodo fue visitado 
		//char color;		//Colores de nodos para marcar si fueron visitados. B (Blanco) indica que el nodo no ha sido visitado,
							//G (Gris) que ya fue visitado pero el recorrido aún no ha sido completado y N (Negro) que fue visitado y
							//la búsqueda ha sido terminada por completo.
		
		// Constructor de la clase Nodo. Si no se indica dist y num toma como valor default 0 y 1, además su color por defecto es B.
		//Por otro lado, si no se indica el nivel y su predecesor es 0, a parte de que como valor predeterminado visitado será falso
		Nodo(int n, long long int w = 0, long long int w2 = 0){
			this->node = n;
			//this->depth = 0;
			this->weight = w;
			this->weight2 = w2;
			//this->dist = 0;
			//this->delta_p = 0;
			//this->delta_m = 0;
			//this->disc = 0;
			//this->low = 0;
			//this->cnt_hijos = 0;
			//this->num = 1;
			//this->visited = false;
			//this->color = 'B';
			//this->pred = 0;
			
		}
		// Constructor de la clase nodo con valores predeterminados.
		Nodo(){
			this->node = 0;
			//this->depth = 0;
			this->weight = 0;
			this->weight2 = 0;
			//this->dist = 0;
			//this->delta_p = 0;
			//this->delta_m = 0;
			//this->disc = 0;
			//this->low = 0;
			//this->cnt_hijos = 0;
			//this->num = 1;
			//this->visited = false;
			//this->color = 'B';
			//this->pred = 0;
		}
};
//Nueva clase Edges para los arcos con dos costos
class Edges{
	public:
		int salida;					//nodo de salida del arco
		int llegada;				//nodo de llegada del arco
		long long int weight_g;		//peso 1 del arco (oro)
		long long int weight_s;		//peso 2 del arco (plata)
		
		//Constructor de la clase
		Edges(int s, int ll, long long int wg, long long int ws){
			this->salida = s;
			this->llegada = ll;
			this->weight_g = wg;
			this->weight_s = ws;
		}
		//Constructor de la clase para valores predeterminados
		Edges(){
			this->salida = 0;
			this->llegada = 0;
			this->weight_g = 0;
			this->weight_s = 0;
		}
		//definición del operador < basado en el primer costo del arco
		bool operator<(const Edges &_edge) const{
			return weight_g < _edge.weight_g;
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
		vector<int> repr;
		//vector<long long> delta;			//Vector de distancias de costo mínimo delta para cada nodo
		//vector<vector<int> > C_F_C;		//components fuertemente conexas del grafo descubierto por Tarjan
		//vector<pair<int, int> > puentes;	//vector de pares para almacenar los puentes encontrados por Tarjan
		//vector<bool> art_points;          //vector para marcar los puntos de articulación 
		
		//Constructor de la clase Grafo_Ady_List
		Grafo_Ady_List(int s){
			this->nodes = s;
			this->edges = vector<vector<Nodo> >(s+2);  //Para trabajar con 1-indexación, se crea la lista de adyacencia con número de nodos+2
			this->list_of_nodes = vector<Nodo>(s+2);
			this->repr = vector<int>(s);
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
		
		//Función recursiva para encontrar los representates de los nodos
		//pertenecientes a un mismo conjunto
		int find_set(Nodo v){
			if(v.node == repr[v.node]){
				return v.node;
			}else{
				return find_set(Nodo(repr[v.node]));
			}
		}
		//función para unir los conjuntos de dos nodos, esto se hace
		//asignando el mismo representante a ambos nodos. 
		void join(Nodo u, Nodo v){
			repr[u.node] = repr[v.node];
		}
		//Implementación de kruskal para calcular el Minimum Spanning Tree (MST) de un grafo
		//con dos costos en sus nodos
		long long int kruskal(int m, long long int g, long long int s, vector<Edges> &list_of_edges){
			//se ordena la lista de arcos de menor a mayor en función de el primer costo (costo de oro)
			sort(list_of_edges.begin(), list_of_edges.end());
			
			//se crea un MST en el cual será llenado en función de segundo costo (costo de plata) 
			vector<Edges> spanning_tree(nodes);
			
			long long int MST_cost = INF;   //costo mínimo del MST
			long long int MaxS;             //número máximo de plata del MST
			int count,						//contador auxiliar para calular los nodos en el MST actual
				connect = 0;      			//número de nodos conectados en el MST
			
			//se itera por el arreglo de arcos 
			for(int i=0; i<m; i++){
				//se agrega el arco al MST en función a los nodos conectados 
				spanning_tree[connect] = list_of_edges[i];
				connect++;
				//Marcamos -1 como MaxS para iniciar la búsqueda del maximo valor de plata
				MaxS = -1; 
				
				//se realiza n conjuntos disjunstos con un único nodo como elemento
				for(int k=0; k<nodes; k++){
					repr[k] = k;	
				}
				
				//Organizamos los arcos del MST en función del mayor valor que se posea de plata
				for(int k=connect-1; k>0; k--){
					if(spanning_tree[k].weight_s< spanning_tree[k-1].weight_s){
						swap(spanning_tree[k], spanning_tree[k-1]);
					}
				}
				 //iniciamos la cuenta de los nodos en el MST actual
				count = 0;
				
				//se itera por los nodos conectados
				for(int j=0; j<connect; j++){
					//extraemos del MST los nodos de salida y entrada del arco
					int n1 = spanning_tree[j].salida, n2 = spanning_tree[j].llegada;
					
					//si no poseen el mismo representante, quiere decir que pertenecen al mismo conjunto
					// por lo que es posible hacer la unión de estos
					if(find_set(Nodo(n1)) != find_set(Nodo(n2))){
							//unión de los conjuntos
							join(find_set(Nodo(n1)), find_set(Nodo(n2)));
							//se busca el valor máximo de plata en el MST
							MaxS = max(MaxS, spanning_tree[j].weight_s);
							//y el arco acutal es agregado al MST 
							spanning_tree[count] = spanning_tree[j];
							count++;
							//en caso de haber iterado por todos los nodos, detenemos el ciclo
							if(count == nodes - 1){
								break;
							}					
					}

				}
				//si ya se agreron todos los nodos, se actualiza el costo del MST buscando la menor
				//suma de la mínima cantidad de oro y la máxima cantidad de plata con sus respectivas conversiones
				if(count == nodes - 1){
					MST_cost = (long long int)min(MST_cost, (list_of_edges[i].weight_g*g)+(MaxS*s)); 
				}
				//se acutliza la cantidad de nodos conectados en el MST
				connect = count;
			}
			//Luego, si los nodos conectados en total no son el número de nodos, se retorna -1 para indicar que es 
			//imposible hacer un MST
			if(connect != nodes - 1){
				return -1;
			}
			//Finalmente se retorna el costo del MST generado
			return MST_cost;
		}
};

//PROGRAMA PRINCIPAL
int main(){
	// Modificación para la optimización de la entrada y salida de datos
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);
	
	//Variables del programa
	int N, M, G, S, x, y;
	long long int g, s;
	
	//se agrega en número de nodos, arcos, y la equivalencia de oro y plata en tugriks
	cin>>N>>M>>G>>S;
	//se crea un grafo de tamaño N
	Grafo_Ady_List Graph(N);
	//se crea una lista de arcos
	vector<Edges> list_of_edges(M);
	
	//se ingresa los arcos con sus pesos correspondientes
	//se resta -1 para 0-idexación
	for(int i=0; i<M; i++){
		cin>>x>>y>>g>>s;
		Graph.new_edge(Nodo(x-1), Nodo(y-1, g, s));
		list_of_edges[i] = Edges(x-1, y-1, g, s);
	}
	
	//se aplica el algorimo de kruskal sobre el grafo y se retorna el costo del MST
	cout<<Graph.kruskal(M, G, S, list_of_edges)<<endl;
	
	return 0;
}
