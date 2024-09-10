/*
Estudiante: Astrid Alvarado
User SPOJ: a_7565
Ejercicio: Topological Sort (https://www.spoj.com/problems/TOPOSORT/)
Veredicto: Accepted

Solución
La idea consiste en recorrer el grafo dirigido siguiendo el ordenamiento topológico del mismo. Para esto, se realiza
el algoritmo de Kahn en el grafo usando una priority queue como conjunto de nodos sin precedencia, para así cumplir
con obtener el orden topológico en forma creciente.
*/
#include<iostream>
#include<vector>
#include<queue>
#include<stack>
#include<string>


using namespace std;

//Variables globales del programa

// Clase de nodo para tener mejor manejo de los datos relacionados con los nodos del grafo
class Nodo{
	public:
		int node;			//Identificador del nodo
		int dist;			//Distancia desde la raiz hasta el nodo actual
		int pred;			//Identificador del nodo predecesor del nodo actual
		int delta_p;		//Grado exterior del nodo, es decir, |delta^+|
		int delta_m;		//Grado interior del nodo, es decirm |delta^-|
		int num;			//Variable auxiliar que representa el kcolor asignado
		bool visited;		//Variable para indicar si un nodo fue visitado 
		char color;			//Colores de nodos para marcar si fueron visitados. B (Blanco) indica que el nodo no ha sido visitado,
							//G (Gris) que ya fue visitado pero el recorrido aún no ha sido completado y N (Negro) que fue visitado y
							//la búsqueda ha sido terminada por completo.
		
		// Constructor de la clase Nodo. Si no se indica dist y num toma como valor default 0 y 1, además su color por defecto es B.
		//Por otro lado, si no se indica el nivel y su predecesor es 0, a parte de que como valor predeterminado visitado será falso
		Nodo(int n, int d = 0, int p = 0, int dp = 0, int dm = 0, int u = 1, bool vs = false, char c = 'B'){
			this->node = n;
			this->dist = d;
			this->delta_p = dp;
			this->delta_m = dm;
			this->num = u;
			this->visited = vs;
			this->color = c;
			this->pred = p;
		}
		// Constructor de la clase nodo con valores predeterminados.
		Nodo(){
			this->node = 0;
			this->dist = 0;
			this->delta_p = 0;
			this->delta_m = 0;
			this->num = 1;
			this->visited = false;
			this->color = 'B';
			this->pred = 0;
		}
};

// Clase de grafo implementado como una lista de adyacencia junto con funciones básicas para manipular el mismo
class Grafo_Ady_List{
	int nodes;							//Cantidad de nodos
	vector<vector<Nodo> > edges;		//Lista de adyacencia
	stack<Nodo> aux;					//pila para el funcionamiento del algoritmo Kosaraju
	public:
		vector<Nodo> list_of_nodes;		//Vector de nodos del grafo creado
		
		//Constructor de la clase Grafo_Ady_List
		Grafo_Ady_List(int s){
			this->nodes = s;
			this->edges = vector<vector<Nodo> >(s+1);  //Para trabajar con 1-indexación, se crea la lista de adyacencia con número de nodos+1
			this->list_of_nodes = vector<Nodo>(s+1);
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
		
		//Algoritmo de kahn para obtener el ordenamiento topológico de un grafo dirigido modificado
		//para obtener el mismo en orden creciente
		vector<int> kahn(priority_queue<int, vector<int>, greater<int> > &S, int E){
			//vector para guardar el orden topológico
			vector<int> topo_sort;
			//Número de lados del grafo
			int card_edges = E;
			
			//mientras que el priority queue no esté vacío
			while(!S.empty()){
				//Se saca de la cola
				Nodo cur = S.top();
				S.pop();
				//se guarda en vector que almacena el orden topológico
				topo_sort.push_back(cur.node);
				
				//recorremos por los adyacentes del nodo
				int num_ady = get_num_ady(cur);
				for(int i=0; i<num_ady; i++){
					int ady = get_ith_ady_of(cur, i);
					
					//y reducimos el grado interior. Esto es para emular remover el arco del grafo
					list_of_nodes[ady].delta_m--;
					//además, se reduce el número de lados del grafo por lo antes explicado
					card_edges--;
					if(list_of_nodes[ady].delta_m == 0){
						//y en caso de que este sea 0, se agrega a la cola de prioridad
						S.push(list_of_nodes[ady].node);
					}
				}
			}
			//si la cola queda vacía pero el número de arcos no es 0
			if(card_edges != 0){
				//signfica que el grafo tiene un ciclo, por lo tanto no es posible realizar el ordenamiento topológico
				//por lo que se le asigna un vector vacío
				vector<int> empt;
				topo_sort = empt; 
			}
			
			return topo_sort;
		}
};

//PROGRAMA PRINCIPAL
int main(){
	// Modificación para la optimización de la entrada y salida de datos
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);
	
	//Vriables del programa
	int n,m,x,y;
	//vector para almacnar el ordenamiento topológico
	vector<int> topo_sort;
	//priority queue para el funcionamiento de Kahn
	priority_queue<int, vector<int>, greater<int> > S;
	
	//se ingresa el número de nodos y el número de arcos
	cin>>n>>m;
	//e crea el respectivo grafo
	Grafo_Ady_List G(n);
	
	//y almacenamos en la lista los nodos del mismo
	for(int i=1; i<n+1; i++){
		G.list_of_nodes[i] = Nodo(i);
	}
	
	for(int i=0; i<m; i++){
		//ingresamos los arcos del grafo
		cin>>x>>y;
		//y los guardamos para formar el grafo dirigido
		G.new_edge(Nodo(x), Nodo(y));
		//además, se aumenta el grado interior y enxterior de los mismos
		G.list_of_nodes[x].delta_p++;
		G.list_of_nodes[y].delta_m++;
	}
	
	//luego, iteramos por la lista de nodos, y almacenamos en la cola de prioridad aquellos nodos cuyo grado
	//interior es 0. Es decir, que no tienen precedencia desde un principio
	for(int i=1; i<n+1; i++){
		if(G.list_of_nodes[i].delta_m == 0){
			S.push(G.list_of_nodes[i].node);
		}
	}
	//calculamos el oren topológico
	topo_sort = G.kahn(S, m);
	
	//y si la lista retornada es vacía, se indicqaque Sandro falló
	if(topo_sort.empty()){
		cout<<"Sandro fails."<<endl;
	}else{
		//en otro caso, se imprime el orden topológico hallado
		for(int i=0; i<topo_sort.size(); i++){
			cout<<topo_sort[i]<<" ";
		}
		cout<<endl;
	}
	
	return 0;
}
