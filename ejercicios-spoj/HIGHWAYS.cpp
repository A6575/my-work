/*
Estudiante: Astrid Alvarado
User SPOJ: a_7565
Ejercicio: Highways (https://www.spoj.com/problems/HIGHWAYS/)
Veredicto: Accepted

Solución:
La idea consiste en recorrer el grafo aplicando el algortimo de Dijkstra para obtener el camino de costo mínimo entre los nodos ingresados
De esta forma, se consigue el menor tiempo para llegar de una ciudad ni a una ciudad nf

*/
#include<iostream>
#include<vector>
#include<queue>
#include<stack>
#include<string>
#include<algorithm>
#include<set>

using namespace std;

#define INF 1e9

//Variables globales del programa

// Clase de nodo para tener mejor manejo de los datos relacionados con los nodos del grafo
class Nodo{
	public:
		int node;			//Identificador del nodo
		int weight;			//peso del nodo
		/*
		int dist;			//Distancia desde la raiz hasta el nodo actual
		int pred;			//Identificador del nodo predecesor del nodo actual
		int delta_p;		//Grado exterior del nodo, es decir, |delta^+|
		int delta_m;		//Grado interior del nodo, es decirm |delta^-|
		int disc;			//Discovery del nodo, es decir, el tiempo en que fue descubierto
		int low;			//Low del nodo, es decir, el menor discovery encontrado en el DFS tree generado
		int cnt_hijos;		//Contador de hijos que tiene un nodo
		int num;			//Variable auxiliar que representa el kcolor asignado
		bool visited;		//Variable para indicar si un nodo fue visitado 
		char color;			//Colores de nodos para marcar si fueron visitados. B (Blanco) indica que el nodo no ha sido visitado,
							//G (Gris) que ya fue visitado pero el recorrido aún no ha sido completado y N (Negro) que fue visitado y
							//la búsqueda ha sido terminada por completo.
		*/
		// Constructor de la clase Nodo. Si no se indica dist y num toma como valor default 0 y 1, además su color por defecto es B.
		//Por otro lado, si no se indica el nivel y su predecesor es 0, a parte de que como valor predeterminado visitado será falso
		Nodo(int n, int w = 0){
			this->node = n;
			this->weight = w;
			
			/*
			this->dist = 0;
			this->delta_p = 0;
			this->delta_m = 0;
			this->disc = 0;
			this->low = 0;
			this->cnt_hijos = 0;
			this->num = 0;
			this->visited = false;
			this->color = 'B';
			this->pred = 0;
			*/
		}
		// Constructor de la clase nodo con valores predeterminados.
		Nodo(){
			this->node = 0;
			this->weight = 0;
			
			/*
			this->dist = 0;
			this->delta_p = 0;
			this->delta_m = 0;
			this->disc = 0;
			this->low = 0;
			this->cnt_hijos = 0;
			this->num = 1;
			this->visited = false;
			this->color = 'B';
			this->pred = 0;
			*/
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
		vector<int> delta;					//Vector de distancias de costo mínimo delta para cada nodo
		//vector<vector<int> > C_F_C;			//components fuertemente conexas del grafo descubierto por Tarjan
		//vector<pair<int, int> > puentes;	//vector de pares para almacenar los puentes encontrados por Tarjan
		//vector<bool> art_points;          //vector para marcar los puntos de articulación 
		
		//Constructor de la clase Grafo_Ady_List
		Grafo_Ady_List(int s){
			this->nodes = s;
			this->edges = vector<vector<Nodo> >(s+1);  //Para trabajar con 1-indexación, se crea la lista de adyacencia con número de nodos+1
			this->list_of_nodes = vector<Nodo>(s+1);
			this->delta = vector<int>(s+1, INF);	   //Inicialmente todos los nodos tienen distancia INF
			//this->C_F_C = vector<vector<int> >(s+1);
			//this->art_points = vector<bool>(s+1, false);
			this->time = 0;
		}
		//Función para agregar un nuevo arco
		void new_edge(Nodo x, Nodo y){
			edges[x.node].push_back(y);
		}
		
		//Función para agregar un nuevo nodo
		Nodo new_node(int x, int d, int u){
			nodes++;
			edges.push_back(vector<Nodo> ());
			return Nodo(x, d);
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
		
		//Algoritmo de Dijkstra para calcular el camino de costo de mínimo desde una raiz 
		//implementado usando sets como cola de prioridad
		void dijkstra(Nodo root){
			//set de pares para el funcionamiento de dijkstra. Aquí, la primera posición 
			//representa el peso del nodo y la segunda posición el id del mismo
			set< pair<int, int> > Q;
			
			//guardamos el nodo raiz junto a su peso inicializado en 0
			Q.insert(make_pair(root.weight, root.node));
			//almacenamos la distancia inicial de raiz en el arreglo delta
			delta[root.node] = 0;
			
			//Mientras que el set no esté vacío
			while(!Q.empty()){
				//sacamos del set el nodo y se borra del mismo
				pair<int, int> cur = *(Q.begin());
				Q.erase(Q.begin());
				
				//luego, iteramos por los adyacentes del nodo actual
				int num_ady = get_num_ady(list_of_nodes[cur.second]);
				for(int i=0; i<num_ady; i++){
					Nodo ady = get_ith_adynode_of(list_of_nodes[cur.second], i);
					
					int v = list_of_nodes[ady.node].node,
						w = ady.weight;
					
					//si el camino del nodo adyacente es menor que el nodo actual más el peso del arco
					if(delta[v] > delta[cur.second] + w){
						
						//si el camino del nodo adyacente es distinto de INF, quiere decir que encontramos un camino
						//de menor costo que el ya ncontrado, por lo que es necesario remover el anterior y actualizar corectamente
						//el costo
						if(delta[v] != INF){
							Q.erase(Q.find(make_pair(delta[v], v)));
						}
						
						//Finalemte se actualiza delta y se guarda en el set
						delta[v] = delta[cur.second] + w;
						Q.insert(make_pair(delta[v], v));
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
	
	//Variables del programa
	int t, n, m, ni, nf, a, b, c;
	
	//se ingresa el númro de casos
	cin>>t;
	for(int r=0; r<t; r++){
		//Se ingresa el número de nodos, número de lados, nodo inicial y nodo final
		cin>>n>>m>>ni>>nf;
		
		//se genera un grafo
		Grafo_Ady_List G(n);
		
		//se inicializa la lista de nodos
		for(int i=1; i<n+1; i++){
			G.list_of_nodes[i] = Nodo(i);
		}
		
		for(int i=0; i<m; i++){
			//se ingresa los lados del grafo y el costo del mismo
			cin>>a>>b>>c;
			//se guarda en el grafo los nodos con su costo, generando un grafo no dirigido
			G.new_edge(Nodo(a), Nodo(b, c));
			G.new_edge(Nodo(b), Nodo(a, c));
		}
		//aplicamos dijkstra desde el nodo inicial
		G.dijkstra(G.list_of_nodes[ni]);
		
		//luego, si el delta del nodo final es INF, quiere decir que no fue alcanzado por el nodo de partida
		//por lo tanto, se dice imprime NONE
		if(G.delta[nf] == INF){
			cout<<"NONE"<<endl;
		}else{
			//en caso contrario, se imprime el delta actualiaado del nodo de llegada
			cout<<G.delta[nf]<<endl;
		}
		
	}
	
	return 0;
}
