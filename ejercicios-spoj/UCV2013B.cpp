/*
Estudiante: Astrid Alvarado
User SPOJ: a_7565
Ejercicio: Alice in Amsterdan, I mean Wonderland (https://www.spoj.com/problems/UCV2013B/)
Veredicto: Accepted

Solución:
La idea consiste en aplicar el algoritmo de Floyd Warshall para encontrar el camino de costo mínimo entre cada par de nodos,
verificando además si el grafo tiene un ciclo con costo negativo. Para esto, una vez que se consigue el camino de costo 
mínimo entre todos los nodos, se vuelve a iterar buscando un nodo cuyo delta sea negativo que pertenezca a un camino entre
otro par de nodos
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
const int MaxN = 107;
long long _delta[MaxN][MaxN];
bool neg_cycle[MaxN][MaxN];
string monuments[MaxN];

// Clase de nodo para tener mejor manejo de los datos relacionados con los nodos del grafo
class Nodo{
	public:
		int node;			//Identificador del nodo
		long long weight;	//peso del nodo
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
		Nodo(int n, long long w = 0){
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
		//vector<vector<int> > C_F_C;		//components fuertemente conexas del grafo descubierto por Tarjan
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
		
		//Algoritmo de Floyd-Warshall para calcular el camino de costo minimo entre cada par de nodos del grafo
		void floyd_warshall(){
			Nodo ady;
			int num_ady;
			//inicializamos la matriz de distancias delta y la matriz de ciclo negativo 
			for(int i=0; i<MaxN; i++){
				for(int j=0; j<MaxN; j++){
					_delta[i][j] = INF;
					neg_cycle[i][j] = false;
				}
			}
			
			//Iteramos por todos los nodos, los cuales srán nodo raíz
			for(int r=0; r<nodes; r++){
				//marcamos en la matriz delta al nodo raiz como 0
				_delta[r][r] = 0;
				
				//recorremos el grafo la misma cantidad de nodos para así consegui
				//caminos de tamaño i
				for(int i = 0; i<nodes; i++){
					
					//recorremos por todos los nodos
					for(int j=0; j<nodes; j++){
						
						//recorremos por todos sus adyacentes
						num_ady = get_num_ady(list_of_nodes[j]);
						for(int k=0; k<num_ady; k++){
							ady = get_ith_adynode_of(list_of_nodes[j], k);
							
							//si la distancia entre el nodo raiz y los nodos existe, además de que la
							//distancia del nodo adyacente sea mayor al nodo actual sumado con el costo
							//de ese arco, se relaja el mismo actalizando su costo
							if(_delta[r][j] != INF && _delta[r][ady.node] > _delta[r][j] + ady.weight){
								_delta[r][ady.node] = _delta[r][j] + ady.weight;
							}
						}
					}
				}
			}
			
			//iteramos por todos los nodos verificando si existe un nodo n cuyo delta es negativo que sea parte del
			//camino entre dos nodos i y j cuyos delta haya sido actualizado. En caso de encontrarlo, decimos que los
			//nodos i y j opertenecen a un ciclo negativo, por lo que se marcata true en la matriz correspondiente
			for(int i=0; i<nodes; i++){
				
				for(int j=0; j<nodes; j++){
					
					num_ady = get_num_ady(list_of_nodes[j]);
					for(int k=0; k<num_ady; k++){
						ady = get_ith_adynode_of(list_of_nodes[j], k);
						
						if(_delta[i][ady.node] < INF && _delta[ady.node][ady.node] < 0 && _delta[ady.node][j] < INF){
							neg_cycle[i][j] = true;
						}
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
	long long kj;
	int N, Q, U, V,
	    tc = 1;
	
	while(true){
		//se ingresa el número de nodos
		cin>>N;
		
		//Si es igual a 0, se detiene el programa
		if(N==0){
			break;
		}
		
		//se genera el grafo con los nodos dados
		Grafo_Ady_List G(N);
		
		//Se almacenan los nodos en la lista de nodos
		for(int i=0; i<N; i++){
			G.list_of_nodes[i] = Nodo(i);
		}
		
		//Leemos los arcos y monumentos
		for(int i=0; i<N; i++){
			cin>>monuments[i];
			
			//por cada monumento tomamos el distancias (peso) entre cada monumento
			for(int j=0; j<N; j++){
				cin>>kj;
				
				//si hay un arco a un mismo nodo
				if(i == j){
					//si el peso es positivo, asignamos este con valor 0 de tal forma que 
					//no se cuente al momento de armar el grafo
					if(kj > 0){
						kj = 0;
					//en caso de que sea de peso negativo, le asignamos el menor valor posible
					// y así contarlo como ciclo negativo
					}else if(kj < 0){
						kj = -INF;
					}
				}
				//agregamos un arco cada vez que el peso sea distinto de 0 o que los nodos sean iguales
				if(i == j || kj != 0){
					G.new_edge(Nodo(i), Nodo(j, kj));
				}
			}
		}
		//se ejecuta Floyd-Warshall
		G.floyd_warshall();
		
		//se ingresa el número de arcos a probar
		cin>>Q;
		cout<< "Case #"<<tc<<":"<<endl;
		tc++;
		
		for(int i=0; i<Q; i++){
			//se ingresa los arcos
			cin>>U>>V;
			//y si estos arcos fueron marcados como ciclos negativos se imprime "NEGATIVE CYCLE"
			if(neg_cycle[U][V]){
				cout<<"NEGATIVE CYCLE"<<endl;
			}else{
				//en caso contrario, se imprime la distancia entre nodos
				cout<<monuments[U]<<"-"<<monuments[V]<<" ";
				
				//en caso de que la distancia delta sea igual a INF se imorime que no es alcanzable
				if(_delta[U][V] == INF){
					cout<<"NOT REACHABLE"<<endl;
				}else{
					cout<<_delta[U][V]<<endl;
				}
				
			}
		}
	}
	

	return 0;
}
