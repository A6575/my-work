/*
Estudiante: Astrid Alvarado
User CodeForces: A_6575
Ejercicio: Gregor and the Pawn Game (https://codeforces.com/problemset/problem/1549/B)
Veredicto: Accepted

Solución:
La idea consiste en crear un grafo bipartito marcando los posibles emparejamientos posibles entre los peones de Gregor y los peones enemigos.
Luego, se aplica el algoritmo de Hopcroft Karp para calcular en maximum matching

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
		int depth;			//profundidad del nodo en el DFS Tree
		//long long weight;	//peso del nodo
		int dist;			//Distancia desde la raiz hasta el nodo actual
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
		Nodo(int n){
			this->node = n;
			this->depth = 0;
			//this->weight = w;
			this->dist = 0;
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
			this->depth = 0;
			//this->weight = 0;
			this->dist = 0;
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
		
		//Algoritmo de bfs implementado para el funcionamiento del algoritmo de Hopcroft Karp
		bool bfs_hk(int n, vector<int> &set_1){
			queue<int> Q;
			
			//Se agrega a la cola aquellos nodos pertenecientes al primer conjunto del grafo bipartito
			//asignando su distancia como 0 en caso de cumplir la condición. Por el contrario, en caso de que
			//los vectores en el primer conjunto sea distinto de 0, su distancia será igual a INF
			for(int vert=1; vert<n + 1; vert++){
				if(set_1[vert] == 0){
					list_of_nodes[vert].dist = 0;
					Q.push(vert);
				}else{
					list_of_nodes[vert].dist = INF;
				}
			}
			
			//iniciamos la búsqueda con el nodo '0' con distanci INF. El nodo '0' representa un nodo fantasma
			list_of_nodes[0].dist = INF;
			
			//Mientras la cola no esté vacía
			while(!Q.empty()){
				//se sacan de la cola los elementos
				int cur = Q.front();
				Q.pop();
				
				//en caso de que la distacia sea menor a la del nodo '0'
				if(list_of_nodes[cur].dist < list_of_nodes[0].dist){
					//recorremos por los adyacentes del nodo
					int num_ady = get_num_ady(Nodo(cur));
					for(int i=0; i<num_ady; i++){
						int ady = get_ith_ady_of(Nodo(cur), i);
						
						//si la distancia es igual a INF
						if(list_of_nodes[set_1[ady]].dist == INF){
							//actualizamos la misma y agregamos a la cola
							list_of_nodes[set_1[ady]].dist = list_of_nodes[cur].dist + 1;
							Q.push(set_1[ady]);
						}
					}
				}
			}
			//finalmente se retorna true en caso de que el nodo '0' sea distinto de INF, en caso contrario se retorna false
			return (list_of_nodes[0].dist != INF);
		
		}
		//Algoritmo de dsf implementado para el correcto funcionamiento de Hopcroft Karp
		bool dfs_hk(int root, vector<int> &set_1){
			//Si la raíz es distinta de 0
			if(root != 0){
				//se itera por los adyacentes del nodo raíz
				int num_ady = get_num_ady(Nodo(root));
				for(int i=0; i<num_ady; i++){
					int ady = get_ith_ady_of(Nodo(root), i);
					
					//en caso de que la distancia del adyacente sea igual a la distancia del nodo raíz + 1
					if(list_of_nodes[set_1[ady]].dist == list_of_nodes[root].dist + 1){
						
						//llamamos recursivamente a la función y en caso de retornar true
						if(dfs_hk(set_1[ady], set_1)){
							//realizamos el emparejamiento respectivo, y se retorna true
							set_1[ady] = root;
							set_1[root] = ady;
							return true;
						}
					}
				}
				//en caso de no haber realizdo el emparejamiento, se establece la distancia del nodo raiz como INF
				list_of_nodes[root].dist = INF;
				return false;
			}
			//Si la raíz es igual a 0, se retorna true
			return true;
		}
		
		//Algoritmo de Hopcroft Karp que calula el maximun matching
		int hopcroftKarp(int n, int m){
			//se crea un vector conjunto donde se representará el grafo bipartito de match, todo inicialmente en 0
			vector<int> set_1(m+n+2, 0);
			int maximum_matching = 0;
			
			//se aplica el algoritmo de bfs y mientras sea true, es decir, mientras que el nodo '0' es distinto de INF
			while(bfs_hk(n, set_1)){
				//iteramos por los nodos del primer conjunto
				for(int vert = 1; vert < n+1; vert++){
					//mientras aún no se haya realizado ningún emparejamiento y el dfs respectivo retorne true, es decir,
					//se consigue hacer un match o el nodo raíz es '0', se aumenta la cantidad de maximum_matching
					if(set_1[vert] == 0 && dfs_hk(vert, set_1)){
						maximum_matching++;
					}
				}
			}
			
			return maximum_matching;
		}
		
};

//PROGRAMA PRINCIPAL
int main(){
	// Modificación para la optimización de la entrada y salida de datos
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);
	
	//Variables del programa
	string enemies, gregor;
	int T, n;
	
	//Se ingresa el número de casos
	cin>>T;
	for(int t=0; t<T; t++){
		//se ingresa el número de peones, los peones de los enemigos y de Gregor
		cin>>n>>enemies>>gregor;
		
		//se crea un grafo de tamaño  que representa un grafo bipartito.
		Grafo_Ady_List G(2*n);
		
		//Agregamos al grafo todos los posibles match siguiendo las condiciones
		for(int i=0; i<n; i++){
			//si no hay peón de Gregor en la posión i se obvia
			if(gregor[i] == '0'){
				continue;
			}
			
			//Si i y i+n es una posición válida y hay un peón enemigo en la posición i-1 de la fila enemiga
			if(i>0 && i+n>n && enemies[i-1] == '1'){
				//se marca el peón como emparejado
				enemies[i-1] = 'x';
				//y se agrega al grafo el posible match con el peón de Gregor en la posición i+1 con el peón enemigo en la posición i+n
				G.new_edge(Nodo(i+1), Nodo(i+n));
				G.new_edge(Nodo(i+n), Nodo(i+1));
			}else if(enemies[i] == '0'){  //en cambio, si se tiene que en la posición i hay un espacio vacío en la fila enemiga
				//se marca la casilla como emparejada
				enemies[i] = 'x';
				//y se agrega al grafo el posible match con el peón de Gregor en la posición i+1 con el peón enemigo en la posición i+n+1
				G.new_edge(Nodo(i+1), Nodo(i+n+1));
				G.new_edge(Nodo(i+n+1), Nodo(i+1));
			}else if(i+1<n && i+n+2<2*n+1 && enemies[i+1] == '1'){ //si i+1 y i+n+2 es una posición válida y hay un peón en la posición i+1 de la fila enemiga
				//se marca al peón como emparejado
				enemies[i+1] = 'x';
				////y se agrega al grafo el posible match con el peón de Gregor en la posición i+1 con el peón enemigo en la posición i+n+2
				G.new_edge(Nodo(i+1), Nodo(i+n+2));
				G.new_edge(Nodo(i+n+2), Nodo(i+1));	
			}
		}
		//se aplica el algoritmo de hopcroft Karp para obtener el maximum matching
		cout << G.hopcroftKarp(n,n) << endl;
	}
	return 0;
}
