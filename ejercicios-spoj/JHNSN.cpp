/*
Estudiante: Astrid Alvarado
User SPOJ: a_7565
Ejercicio: Johnson's Algorithm (https://www.spoj.com/problems/JHNSN/)
Veredicto: Accepted

Solución:
la idea consiste en imoplementar el agloritmo de Johnson para calcular el camino de costo mínimo entre cada par de nodos
de un grafo dirigido, además de detectar posibles ciclos negativos en el grafo.

El algortimo de Johnson emplea las funciones de Bellman-Ford y Dijkstra como subrutinas. Con Bellman-Ford se detecta los
ciclos negativos que posea el grafo, y en caso de no haberlos se calcula el vector h de distancias entre un nodo temporal
que conecta con todos los nodos con arcos de costo 0. Posteriormente, se reasignan los pesos de cada arco empleando
la siguiente fórmula: w(u,v) = h[u] - h[v] + w(u,v)

Esta modificación de los pesos nos permie eliminar costos negativos y así pode aplicar Dijkstra sobre todos los nodos del
grafo para obtener las distancias entre cada para de nodos 
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
const int MaxN = 1017;
long long _delta[MaxN][MaxN];

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
		//vector<long long> delta;			//Vector de distancias de costo mínimo delta para cada nodo
		//vector<vector<int> > C_F_C;		//components fuertemente conexas del grafo descubierto por Tarjan
		//vector<pair<int, int> > puentes;	//vector de pares para almacenar los puentes encontrados por Tarjan
		//vector<bool> art_points;          //vector para marcar los puntos de articulación 
		
		//Constructor de la clase Grafo_Ady_List
		Grafo_Ady_List(int s){
			this->nodes = s;
			this->edges = vector<vector<Nodo> >(s+1);  //Para trabajar con 1-indexación, se crea la lista de adyacencia con número de nodos+1
			this->list_of_nodes = vector<Nodo>(s+1);
			//this->delta = vector<long long>(s+1, INF);	   //Inicialmente todos los nodos tienen distancia INF
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
		
		//Algoritmo de Bellman-Ford para calcular el camino de costo mínimo entre un nodo raiz 
		//y el resto de los nodos del grafo
		bool bellman_ford(Nodo root, vector<long long> &h){
			Nodo ady;
			int num_ady;
			bool not_negCycle;
			
			//marcamos en el vector de distancias al nodo raiz con 0
			h[root.node] = 0;
			
			//iteramos n veces el grafo para así general caminos de tamaño i
			for(int i=0; i<nodes+1; i++){
				not_negCycle = true;
				
				//iteramos por todos los nodos del grafo y sus adyacentes
				for(int j=0; j<nodes+1; j++){
					num_ady = get_num_ady(list_of_nodes[j]);
					for(int k=0; k<num_ady; k++){
						ady = get_ith_adynode_of(list_of_nodes[j], k);
						//si la distancia del nodo adyacente es mayor a la del nodo actal sumado al peso del arco que los conecta
						if(h[ady.node] > h[list_of_nodes[j].node] + ady.weight){
							//se marca como falso la existencia de ciclos negativos
							not_negCycle = false;
							//y se relaja el costo del nodo
							h[ady.node] = h[list_of_nodes[j].node] + ady.weight;
						}
					}
				}
				//en dado caso de que en el camino de tamaño i no se hayan relajado más arcos, se retorna que no hay ciclos negativos
				if(not_negCycle){
					return true;
				}
			}
			//en caso de haber relajado todos los nodos hasta la última iteración, se indica que hay un ciclo negativo
			return false;
		}
		//Algoritmo de Dijkstra para el funcionamiento de Johnson
		void dijkstra_jhnsn(Nodo root, vector<long long> &h){
			int num_ady;
			Nodo ady;
			vector<vector<Nodo> >G_rw(nodes + 1);																			//Vector de vectores de nodos para crear el nuevo grafo
			priority_queue<pair<long long, int>, vector<pair<long long, int> >, greater<pair<long long, int> > > Q;			//Priority queue de pares
			vector<bool> visited(nodes+1, false);																			//vector de visitados
			
			//Se crea un nuevo grafo usando como dato el vector de distancias generado por Bellman-Ford
			for(int i=1; i<nodes+1; i++){
				num_ady = get_num_ady(list_of_nodes[i]);
				for(int j=0; j<num_ady; j++){
					ady = get_ith_adynode_of(list_of_nodes[i], j);
					//el nuevo costo asignado está dado por w(u, v) = h[u]-h[v] + w(u, v)
					G_rw[i].push_back(Nodo(ady.node, h[i]-h[ady.node]+ady.weight));
				}
			}
			//se agrega a la cola de prioridad el nodo raiz con el costo 0
			Q.push(make_pair(0, root.node));
			//se marca en la matriz de distancias delta el valor de 0 en el nodo raiz
			_delta[root.node][root.node] = 0;
			
			//mientras que la cola no esté vacía
			while(!Q.empty()){
				//se saca el nodo y se elimina de la cola
				pair<long long, int> cur = Q.top();
				Q.pop();
				
				//si ya fue visitado, se sigue iterando
				if(visited[cur.second]){
					continue;
				}
				
				//se marca como vistado al nodo
				visited[cur.second] = true;
				
				//e iteramos por los adyacents del nuevo grafo generado
				for(int i=0; i<G_rw[cur.second].size(); i++){
					ady = G_rw[cur.second][i];
					//en caso de que la distancia del nodo adyacente sea mayor a la distancia del nodo actual sumado al costo del arco
					if(_delta[root.node][ady.node] > _delta[root.node][cur.second] + ady.weight){
						//se relaja la distancia del nodo y se sgrega a la cola de prioridad
						_delta[root.node][ady.node] = _delta[root.node][cur.second]  + ady.weight;
						Q.push(make_pair(_delta[root.node][ady.node], ady.node));
					}
						
				}
				
				
			}
			//una vez calculado los caminos de costo mínimo, se itera por dicha fila y se resta lo antes agregado, así se consigue
			//la distancia corecta
			for(int i=1; i<nodes+1; i++){
				if(_delta[root.node][i] < INF){
					_delta[root.node][i] -= (h[root.node] - h[i]);
				}
			}
			
		}
		//Algoritmo de Johnson para calcular caminos de costo mínimo entre cada par de nodos
		bool johnson(vector<long long> &h){
			
			//al grafo inicial se le grega un nodo temporar que tnedrá arcos de costo 0 entre todos los nodos
			for(int i=1; i<nodes+1; i++){
				new_edge(Nodo(0), Nodo(i, 0));
			}
			//Luego, se aplica Bellman-Ford para detectar ciclos de costo negativo. En caso de no
			//encotrar uno, se crea el vector de distancias h
			if(!bellman_ford(0, h)){
				return false;
			}
			//se inicaliza la matriz de distancias delta con el valor INF
			for(int i=0; i<MaxN; i++){
				for(int j=0; j<MaxN; j++){
					_delta[i][j] = INF;
				}
			}
			//luego, se aplica por todos los nodos Dijkstra
			for(int k=1; k<nodes+1; k++){
				dijkstra_jhnsn(list_of_nodes[k], h);
			}
			
			return true;
		}
		
};

//PROGRAMA PRINCIPAL
int main(){
	// Modificación para la optimización de la entrada y salida de datos
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);
	
	//variables del programa
	long long w;
	int t, n, m, u, v;
	
	//se ingresa el número de casoso
	cin>>t;
	for(int r=1; r<t+1; r++){
		//se ingresa el número de nodos y el número de lados
		cin>>n>>m;
		
		//se genera el grafo con el número de nodos
		Grafo_Ady_List G(n);
		//se inicializa el vector h con los valores INF
		vector<long long> h(n+1, INF);
		
		//se almacenan los nodos en la lista de nodos
		for(int i=0; i<n+1; i++){
			G.list_of_nodes[i] = Nodo(i);
		}
		
		for(int i=0; i<m; i++){
			//se ingresan los arcos con su peso
			cin>>u>>v>>w;
			//y se agregan al grafo generando un grafo dirigido
			G.new_edge(Nodo(u), Nodo(v, w));
		}
		
		cout<< "graph " << r << " ";
		
		//se aplica Johnson en el grafo, y en caso de retornar true, quiere decir
		//que no se encontró ningún ciclo negativo
		if(G.johnson(h)){
			cout<<"yes\n"<<endl;
			
			//imprimimos el vector de distancias h
			for(int i=1; i<n+1; i++){
				cout<<h[i]<<" ";
			}
			cout<<h[0]<<"\n\n";
			
			//y la matriz de distancias delta
			for(int i=1; i<n+1; i++){
				for(int j=1; j<n+1; j++){
					//en caso de que una distancia sea igual a INF, se imprime # para indicar que no es alcanzable
					if(_delta[i][j] == INF){
						cout<<"# ";
						continue;
					}
					cout<<_delta[i][j]<<" ";
				}
				cout<<endl;
			}
			cout<<endl;
		}else{
			//en caso contrario se imprime "no"
			cout<<"no\n\n";
		}
		
	}

	return 0;
}
