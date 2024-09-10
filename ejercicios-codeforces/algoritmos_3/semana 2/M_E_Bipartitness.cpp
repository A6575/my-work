/*
Estudiante: Astrid Alvarado
User CodeForce: A_6575
Ejercicio: Mahmoud and Ehab and the bipartiteness (https://codeforces.com/problemset/problem/862/B)
Veredicto: Accepted

Solucion:
La idea consiste en recorrer el árbol ingresado para así dividr los nodos del mismo en dos conjuntos disjuntos (bipartición),
además de obtener la cardinalidad de los mismos. Ahora, para determinar la máxima cantidad de arcos a agregar, se debe calcular
todos los posibles arcos que puede tener un grafo bipartito, quitando aquellos arcos que ya tiene el árbol.

Un árbol tiene exactamente n-1 arcos, donde n es el número de nodos. Por otro lado, sea h y k las cardinalidades de los conjuntos
resultantes de la bipartición, se tiene que él máximo número de arcos que puede tener un grafo bipartito es h*k, por lo que los 
arcos a agregar vienen siendo h*k - (n-1) 
*/

#include<iostream>
#include<vector>
#include<queue>
#include<stack>

using namespace std;

//Variables globales del programa
typedef pair<long long, long long> pll;

// Clase de nodo para tener mejor manejo de los datos relacionados con los nodos del grafo
class Nodo{
	public:
		int node;		//Identificador del nodo
		int dist;		//Distancia desde la raiz hasta el nodo actual
		int num;		//Variable auxiliar que representa el kcolor asignado
		char color;		//Colores de nodos para marcar si fueron visitados. B (Blanco) indica que el nodo no ha sido visitado,
						//G (Gris) que ya fue visitado pero el recorrido aún no ha sido completado y N (Negro) que fue visitado y
						//la búsqueda ha sido terminada por completo.
		
		// Constructor de la clase Nodo. Si no se indica dist y num toma como valor default 0 y 1, además su color por defecto es B
		Nodo(int n, int d = 0, int u = 1, char c = 'B'){
			this->node = n;
			this->dist = d;
			this->num = u;
			this->color = c;
		}
};

// Clase de grafo implementado como una lista de adyacencia junto con funciones básicas para manipular el mismo
class Grafo_Ady_List{
	int nodes;							//Cantidad de nodos
	vector<vector<Nodo> > edges;		//Lista de adyacencia
	vector<int> visited;				//Arreglo para marcar como visitados a los nodos
	public:
		//Constructor de la clase Grafo_Ady_List
		Grafo_Ady_List(int s){
			this->nodes = s;
			this->edges = vector<vector<Nodo> >(s+1);  //Para trabajar con 1-indexación, se crea la lista de adyacencia con número de nodos+1
			this->visited = vector<int>(s+1, -1);	   //se inicializa con -1 el vector para indicar que no ha sido visitado
		}
		//Función para agregar un nuevo arco
		void new_edge(Nodo x, Nodo y){
			edges[x.node].push_back(y);
		}
		//Función para agregar un nuevo nodo
		Nodo new_node(int x, int d, int u){
			nodes++;
			edges.push_back(vector<Nodo> ());
			return Nodo(x, d, u);
		}
		//Función para obtener el número de adyacentes que posee un nodo
		int get_num_ady(Nodo x){
			return edges[x.node].size();
		}
		//Función para obtener el nodo adyacente i del nodo x
		int get_ith_ady_of(Nodo x, int i){
			return edges[x.node][i].node;
		}
		
		//Función para armar un grafo Bipartito y calcular el tamaño de ambos conjuntos usando una versión modificada de DFS
		pll Bipartito(vector<Nodo> arr, int N){
			//par de elementos del tipo long long inicializados en 0.
			//La primera posición corresponde a la cardinalidad del conjunto con kcolor 1
			//La segunda posición corresponde a la cardinalidad del conjunto con kcolor 2
			pll card_conj(0, 0);
			
			//Reorremos por todos los nodos del grafo
			for(int i=1; i<N+1; i++){
				//Y empezamos el recorrido si el nodo no ha sido visitado
				if(arr[i].color == 'B'){
					//Se agrega el nodo raíz a la pila
					stack<Nodo> Q;
					Q.push(arr[i]);
					
					//Y mientras que la pila no esté vacía
					while(!Q.empty()){
						//Sacamos y elminamos el nodo de la pila
						Nodo cur = Q.top();
						Q.pop();
						
						//Se marca como visitado
						cur.color = 'G';
						
						//Y recorremos los adyacntes del nodo actual
						int num_ady = get_num_ady(cur);
						for(int j=0; j<num_ady; j++){
							int ady = get_ith_ady_of(cur, j);
							//Si el nodo adyacente no ha sido visitado
							if(arr[ady].color == 'B'){
								//Vemos el número (kcolor) marcado del nodo actual para así marcar su adyacente 
								//con un número distinto y agregarlo a la pila 
								if(cur.num == 1){
									arr[ady].color = 'G';
									arr[ady].num = 2;
									card_conj.second++;			//cada vez que marcamos un nodo con kcolor 2, se aumenta la cardinalidad del conjunto
									Q.push(arr[ady]);
								}else{
									arr[ady].color = 'G';
									arr[ady].num = 1;
									Q.push(arr[ady]);
								}
							}
						}	
					}
					//Y cerramos el recorrido
					arr[i].color = 'N';
				}
			}
			//calculamos la cardinalidad del primer conjunto restando el número de nodos y la cardinalidad del segundo conjunto
			card_conj.first += (long long)N - card_conj.second;
			return card_conj;
		}
		
};

//PROGRAMA PRINCIPAL
int main(){
	// Modificación para la optimización de la entrada y salida de datos
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);
	
	//Variables del programa: lista de nodos, par de elementos long long, número de nodos, variables auxiliares u y v
	vector<Nodo> list_of_nodes;
	pll card_conj;
	int n, u, v;
	
	//se ingresa el número de nodos
	cin>>n;
	//Y se crea un grafo del mismo tamaño
	Grafo_Ady_List G(n);
	for(int i=0; i<n-1; i++){
		//se ingresan los arcos perteneientes al grafo para obtener un grafo no dirigido
		cin>>u>>v;
		G.new_edge(Nodo(u), Nodo(v));
		G.new_edge(Nodo(v), Nodo(u));
	}
	//Se inicializa una lista de nodos que están en el grafo, donde el nodo j coincide con la posición i 
	for(int i=0;i<n+1;i++){
		list_of_nodes.push_back(Nodo(i));
	}
	//Obtenemos la cardinalidad de ambos conjuntos generados
	card_conj = G.Bipartito(list_of_nodes, n);
	
	//En imprimimos el cálculo correspondiente
	cout<<card_conj.first*card_conj.second - (n-1)<<endl;
	
	return 0;
}
