/*
Estudiante: Astrid Alvarado
User CodeForces: A_6575
Ejercicio: Scheme (https://codeforces.com/problemset/problem/22/E)
Veredicto: Accepted

Solución
La idea consiste en recorrer el grafo de tal forma que se consigan aquellos arcos tales que al ser agregados formen sólo una componentemente fuertemente conexa.
Para esto, se usa el algoritmo de Tarjan modificado para que alamcene aquellos nodos talesque tenga un arco hacia su ancestro, es decir, aquellos nodos que
sean nodos de salida de un backedge. Una vez determinado esto, buscamos los arcos necesarios para convertir el grafo en una única CFC
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
//vector para almacenar los nodos de salida de un backedge. Estos serán los candidatos a realizar las llamadas a otras personas 
vector<int> cllout_i(MaxN, 0);
//vector para marcar a los nodos anteriores como visitados
vector<bool> cllout_i_visited(MaxN, false);
//vector para guardar a los nodos de salida del arco (persona que tiene que llamar otras)
vector<int> callout;
//vector para almaenar los nodos de llegada del arco (personas que serán llamadas por las anteriores)
vector<int> callin;

// Clase de nodo para tener mejor manejo de los datos relacionados con los nodos del grafo
class Nodo{
	public:
		int node;			//Identificador del nodo
		//long long weight;	//peso del nodo
		int dist;			//Distancia desde la raiz hasta el nodo actual
		int pred;			//Identificador del nodo predecesor del nodo actual
		//int delta_p;		//Grado exterior del nodo, es decir, |delta^+|
		int delta_m;		//Grado interior del nodo, es decirm |delta^-|
		int disc;			//Discovery del nodo, es decir, el tiempo en que fue descubierto
		int low;			//Low del nodo, es decir, el menor discovery encontrado en el DFS tree generado
		int cnt_hijos;		//Contador de hijos que tiene un nodo
		//int num;			//Variable auxiliar que representa el kcolor asignado
		bool visited;		//Variable para indicar si un nodo fue visitado 
		char color;			//Colores de nodos para marcar si fueron visitados. B (Blanco) indica que el nodo no ha sido visitado,
							//G (Gris) que ya fue visitado pero el recorrido aún no ha sido completado y N (Negro) que fue visitado y
							//la búsqueda ha sido terminada por completo.
		
		// Constructor de la clase Nodo. Si no se indica dist y num toma como valor default 0 y 1, además su color por defecto es B.
		//Por otro lado, si no se indica el nivel y su predecesor es 0, a parte de que como valor predeterminado visitado será falso
		Nodo(int n){
			this->node = n;
			//this->weight = w;
			this->dist = 0;
			//this->delta_p = 0;
			this->delta_m = 0;
			this->disc = 0;
			this->low = 0;
			this->cnt_hijos = 0;
			//this->num = 1;
			this->visited = false;
			this->color = 'B';
			this->pred = 0;
			
		}
		// Constructor de la clase nodo con valores predeterminados.
		Nodo(){
			this->node = 0;
			//this->weight = 0;
			this->dist = 0;
			//this->delta_p = 0;
			this->delta_m = 0;
			this->disc = 0;
			this->low = 0;
			this->cnt_hijos = 0;
			//this->num = 1;
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
		
		//Algoritmo de Tarjan para detectar CFC. En particular, se agrega la condición para calcular
		//puentes y puntos de articulacón en un grafo no dirigido, además de caluclar los nodos de salida de un backedge 
		void tarjan(int root){
			stack<int> Q;
			int cc = 0;
			
			//actualizamos los valores del nodo raiz
			list_of_nodes[root].disc = time++;
			list_of_nodes[root].low = list_of_nodes[root].disc;
			list_of_nodes[root].visited = true;
			//se colorea el nodo de Gris para denotar que el nodo inició su recorrido, es decir que este es abierto
			list_of_nodes[root].color = 'G';
			//almacenamos en la pila el nodo raiz
			Q.push(root);
			
			//recorremos por los adyacentes del nodo
			int num_ady = get_num_ady(list_of_nodes[root]);
			for(int i=0; i<num_ady; i++){
				int ady = get_ith_ady_of(list_of_nodes[root], i);
				
				
				//si no ha sido visitado el nodo adyacente
				if(!list_of_nodes[ady].visited){
					//guardamos su predecesor
					list_of_nodes[ady].pred = root;
					list_of_nodes[root].cnt_hijos++;
					//y llamamos otra vez al algoritmo
					tarjan(ady);
					
					//finalmente, actualizamos el low del nodo raiz
					list_of_nodes[root].low = min(list_of_nodes[root].low, list_of_nodes[ady].low);
 				}else{
 					//en caso de que el nodo ya haya sido visitado, revisamos que el adyacente no haya concluido su recorrido
 					//para así poder actualizar el low del nodo raiz
 					//cout<<"check"<<endl;
 					if(list_of_nodes[ady].color == 'G'){
 						list_of_nodes[root].low = min(list_of_nodes[root].low, list_of_nodes[ady].disc);
 					}
 				}
 				//marcamos al nodo de salida
 				cllout_i[root] = cllout_i[ady];
 				//y si este es igual a 0 es porque el nodo de salida es sí mismo
 				if(cllout_i[root] == 0){
 					cllout_i[root] = root;
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
	int n, f_i, cnt;
	
	//ingresamos el número de nodos
	cin>>n;
	
	//generamos un grafo
	Grafo_Ady_List G(n);
	
	//almacenamos los nodos en la lista de nodos
	for(int i=1; i<n+1; i++){
		G.list_of_nodes[i] = Nodo(i);
	}
	
	//Ingresamos lo arcos y los guardamos en el grafo dirigido, además de aumentar el grado interior a los nodos de llegada
	for(int i=1; i<n+1; i++){
		cin>>f_i;
		G.new_edge(Nodo(i), Nodo(f_i));
		G.list_of_nodes[f_i].delta_m++;
	}
	
	//Mientras que los nodos no hayan sido visitados, se ejecuta Tarjan
	for(int i=1; i<n+1; i++){
		if(!G.list_of_nodes[i].visited){
			G.tarjan(i);
		}
	}
	
	//se crean los arcos con aquellos nodos cuyo grado interior sea igual a 0 y con su
	//nodo de salida correspondiente
	callin.push_back(0);				//reservamos la primera posición para posteriormente setear e forma correcta su arco
	for(int i=1; i<n+1; i++){
		if(G.list_of_nodes[i].delta_m == 0){
			callout.push_back(cllout_i[i]);
			callin.push_back(i);
			cllout_i_visited[cllout_i[i]] = true;
		}
	}
	//en caso de que exista más d una CFC se busca de conectarlas con aquellos nodos no visitados
	//y que su nodo de salida sea el mismo
	for(int i=1; i<n+1; i++){
		if(cllout_i[i] == i && !cllout_i_visited[i]){
			callout.push_back(i);
			callin.push_back(i);
		}
	}
	callin[0] = callin[callin.size()-1];
	
	//obtenemos el tamaño de los nodos de salida de los nuevos arcos
	cnt = callout.size();
	//si este tamaño es igual a 1 pero el arco creado es un bucle, se tiene que el grafo ya es una única CFC
	if(cnt == 1 && callout[0] == callin[0]){
		cnt = 0;
	}
	//imprimimos el números de arxos a agregar e imprimimos los arcos
	cout<<cnt<<endl;
	for(int i=0; i<cnt; i++){
		cout<<callout[i]<<" "<<callin[i]<<endl;
	}
	
	return 0;
}
