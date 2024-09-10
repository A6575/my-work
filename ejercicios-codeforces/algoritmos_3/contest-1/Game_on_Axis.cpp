/*
Estudiante: Astrid Alvarado
User CodeForces: A_6575
Ejercicio: Game on Axis (https://codeforces.com/problemset/problem/1787/D)
Veredicto: Accepted

Soluci�n:
La idea consiste en recorrer el grafo aplicando DFS dando pasos de tama�o i+a_i y ver si se encuentra en el rango (1, n) del camino, en este caso
es posible seguir saltando, y en caso de no estarlo, el programa se termina. Adem�s del grafo, se tiene en cuenta un arreglo con 
permutaciones de n�meros entre -n y n, para as�, el �nidice de este arreglo representa los nodos encontrados en el grafo, y aquellos que 
nos hagan ganar en el juego se marca como INF. 

Finalmente, se analiza todas las combinaciones de pares posibles que hagan que el juego termine en un n�mero finito de pasos 
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
							//G (Gris) que ya fue visitado pero el recorrido a�n no ha sido completado y N (Negro) que fue visitado y
							//la b�squeda ha sido terminada por completo.
		
		// Constructor de la clase Nodo. Si no se indica dist y num toma como valor default 0 y 1, adem�s su color por defecto es B.
		//Por otro lado, si no se indica el nivel y su predecesor es 0, a parte de que como valor predeterminado visitado ser� falso
		Nodo(int n){
			this->node = n;
			this->depth = 0;
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
			this->depth = 0;
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

// Clase de grafo implementado como una lista de adyacencia junto con funciones b�sicas para manipular el mismo
class Grafo_Ady_List{
	int nodes;							//Cantidad de nodos
	vector<vector<Nodo> > edges;		//Lista de adyacencia
	stack<Nodo> aux;					//pila para el funcionamiento del algoritmo Kosaraju
	int time;							//Variable global para actualizar el discovery
	
	public:
		vector<Nodo> list_of_nodes;			//Vector de nodos del grafo creado
		//vector<long long> delta;			//Vector de distancias de costo m�nimo delta para cada nodo
		//vector<vector<int> > C_F_C;		//components fuertemente conexas del grafo descubierto por Tarjan
		//vector<pair<int, int> > puentes;	//vector de pares para almacenar los puentes encontrados por Tarjan
		//vector<bool> art_points;          //vector para marcar los puntos de articulaci�n 
		
		//Constructor de la clase Grafo_Ady_List
		Grafo_Ady_List(int s){
			this->nodes = s;
			this->edges = vector<vector<Nodo> >(s+1);  //Para trabajar con 1-indexaci�n, se crea la lista de adyacencia con n�mero de nodos+1
			this->list_of_nodes = vector<Nodo>(s+1);
			//this->delta = vector<long long>(s+1, INF);	   //Inicialmente todos los nodos tienen distancia INF
			//this->C_F_C = vector<vector<int> >(s+1);
			//this->art_points = vector<bool>(s+1, false);
			this->time = 0;
		}
		//Funci�n para agregar un nuevo arco
		void new_edge(Nodo x, Nodo y){
			edges[x.node].push_back(y);
		}
		
		/*//Funci�n para agregar un nuevo nodo
		Nodo new_node(int x, int d, int u){
			nodes++;
			edges.push_back(vector<Nodo> ());
			return Nodo(x, d);
		}*/
		
		//Funci�n para obtener el n�mero de adyacentes que posee un nodo
		int get_num_ady(Nodo x){
			return edges[x.node].size();
		}
		//Funci�n para obtener el valor del nodo adyacente i del nodo x
		int get_ith_ady_of(Nodo x, int i){
			return edges[x.node][i].node;
		}
		//Funci�n para obtener el nodo adyacente i del nodo x
		Nodo get_ith_adynode_of(Nodo x, int i){
			return edges[x.node][i];
		}
		
		//Funci�n reursiva del DFS para recorrer el grafo seg�n un vector dado
		void dfs(Nodo root, vector<long long int> &v){
			//marcamos como visitado al nodo ra�z
			list_of_nodes[root.node].visited = true;
			//obtenemos el adyacente del vector dado
			long long int ady = v[root.node];
			
			//si est� equivale a INF
			if(ady == INF){
				//llamamos a la funci�n con el nodo 0
				dfs(Nodo(0), v);
				return;
			}
			//si no ha sido visitado y el adyacente no es 0, llamamos a la funci�n con el nodo adyacente
			if(ady != 0 && !list_of_nodes[ady].visited){
				dfs(list_of_nodes[ady], v);
			}
		}
		//Funci�n de dfs para medir la longitud de un nodo hasta la ra�z
		int dfs_dpth(Nodo root){
			//marcamos como 1 la longitud del nodo ra�z 
			list_of_nodes[root.node].depth = 1;
			
			//iteramos por los adyacentes
			int num_ady = get_num_ady(root);
			for(int i=0; i<num_ady; i++){
				Nodo ady = get_ith_adynode_of(root, i);
				//y se va sumando cada llamada recursiva del nodo adyacente
				list_of_nodes[root.node].depth += dfs_dpth(ady); 
			}
			
			return list_of_nodes[root.node].depth;
		}
		
};

//PROGRAMA PRINCIPAL
int main(){
	// Modificaci�n para la optimizaci�n de la entrada y salida de datos
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);
	
	//variables del programa
	int t;
	long long int n, a_i, ans;
	//ingresamos el n�mero de casos
	cin>>t;
	for(int r=0; r<t; r++){
		//ingresmos el n�meor de puntos (nodos del grafo)
		cin>>n;
		//se crea un grafo con el n�mero de nodos
		Grafo_Ady_List G(n);
		//se crea un vector de para almacenar los nodos que har�n terminar el juego
		vector<long long int> wnodes(n+1, 0);
		 
		 //guardamos los nodos en un a lista de nodos 
		for(int k=0; k<n+1; k++){
			G.list_of_nodes[k] = Nodo(k);
		}
		
		//ingresamos el n�mero en los puntos
		for(int i=1; i<n+1; i++){
			cin>>a_i;
		 	
		 	//si la suma de i+a_i es menor a 1 o mayor que n, autom�ticamente el juego termina
		 	if(i+a_i < 1 || i+a_i > n){
		 		wnodes[i] = INF;
		 		G.new_edge(Nodo(0), Nodo(i));
		 		continue;
		 	}
		 	
		 	//el otro caso, guardamos en el grafo el arco qwu va desde i+a_i a i
		 	wnodes[i] = i+a_i;
		 	G.new_edge(Nodo(i+a_i), Nodo(i));
		}
		//se ejecuta dfs partiendo desde el nodos 1 para verficar si un nodo est� en el camino de 
		//un nodo ganador (arco desde el nodo 0)
		G.dfs(G.list_of_nodes[1], wnodes);
		//luego, calculamos el tama�o del camino que nos permite salir del rango del camino
		G.dfs_dpth(Nodo(0));
		
		//Si el nodo de llegada fue visitado quiere decir que el juego puede terminar con un n�mero finito de pasos
		ans = 0;
		if(G.list_of_nodes[0].visited){
			//tenemos que los m�ximo n�mero pares posibles a realizar es de 2*n+1, tantas veces como puntos se tenga
			ans = n*(2*n+1);
			
			//se comienza a eliminar los pares
			long long int d = 1;
			while(d != INF){
				//Se elimina el tama�o del camino, esto es porque cualquier opuesto n_i (-n_i) causa 
				//que no sea posible terminar el juego en un n�mero finito de pasos
				ans -= G.list_of_nodes[d].depth;
				
				//se elmina el tama�o de los subcaminos desde el nodo de llegada que provoquen no ganar el juego
				ans -= (n+1-G.list_of_nodes[0].depth);
				
				//actualizamos d
				d = wnodes[d];
				
			}
		}else{
			//si el nodo de llegada no fue visitado entonces existe alg�n ciclo que cause perder el juego desde el nodo de partida
			//por lo que a cada par de dicho camino se le suma el tama�o del subcamino de los nodos con nodo de salida 0
			for(int k=1; k<n+1; k++){
				if(G.list_of_nodes[k].visited){
					ans += n + G.list_of_nodes[0].depth;
				}
			}
		}
		//finalmente se imprime la respuesta
		cout<<ans<<endl;
	}
	
	return 0;
}
