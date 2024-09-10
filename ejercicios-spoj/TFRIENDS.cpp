/*
Estudiante: Astrid Alvarado
User SPOJ: a_7565
Ejercicio: True Friends (https://www.spoj.com/problems/TFRIENDS/)
Veredicto: Accepted

Solución
La idea consiste en recorrer el grqfo dado, y detectar las Componentes Fuertemente Conexas (CFC) del mismo. Aquellos nodos que
pertenezcan a una misma CFC, será considerado un grupo de amigos verdaderos, por lo que el resultado será la cantidad de CFC
que existan en el grafo. 

Para detectar las CFC, se emplea el algoritmo de Kosaraju, contando las veces que se realiza por segunda vez DFS para obtener 
la cantidad de las CFC que posee el grafo.
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
		int num;			//Variable auxiliar que representa el kcolor asignado
		bool visited;		//Variable para indicar si un nodo fue visitado 
		char color;			//Colores de nodos para marcar si fueron visitados. B (Blanco) indica que el nodo no ha sido visitado,
							//G (Gris) que ya fue visitado pero el recorrido aún no ha sido completado y N (Negro) que fue visitado y
							//la búsqueda ha sido terminada por completo.
		
		// Constructor de la clase Nodo. Si no se indica dist y num toma como valor default 0 y 1, además su color por defecto es B.
		//Por otro lado, si no se indica el nivel y su predecesor es 0, a parte de que como valor predeterminado visitado será falso
		Nodo(int n, int d = 0, int p = 0, int u = 1, bool vs = false, char c = 'B'){
			this->node = n;
			this->dist = d;
			this->num = u;
			this->visited = vs;
			this->color = c;
			this->pred = p;
		}
		// Constructor de la clase nodo con valores predeterminados.
		Nodo(){
			this->node = 0;
			this->dist = 0;
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
		
		//Implementqación recursiva de algortimo de recorrido DFS nmodoficada para que en su versión 0 almacene 
		//los nodos en una pila 
		void dfs_recursive(Nodo root, int vers){
			//se recorre por los adyacentes del nodo raiz
			int num_ady = get_num_ady(root);
			for(int i=0; i<num_ady; i++){
				Nodo r = get_ith_adynode_of(root, i);
				
				//Si no ha sido visitado
				if(!list_of_nodes[r.node].visited){
					//se marca como tal y se llama otra vez a la función DFS con el nodo adyacente
					list_of_nodes[r.node].visited = true;
					
					dfs_recursive(r, vers);
				}
			}
			//si se indica la versión 0, almacenamos en la pila los nodos antes de devolvernos en la llamada
			if(vers == 0){
				aux.push(root);
			}
		}
		//Función para resetear el estado de visitados de los nodos
		void clear_visit(){
			for(int i=1; i<nodes+1; i++){
				list_of_nodes[i].visited = false;
			}
		}
		//Función para obtener el grafo invertido
		void inverse(){
			//se crea una nueva lista de adyacencia
			vector<vector<Nodo> > edges_inverse(nodes + 1, vector<Nodo>());
			//e iteramos por todos los nodos del grafo y sus adyacentes
			for(int i=1; i<nodes+1; i++){
				int num_ady = get_num_ady(Nodo(i));
				for(int j=0; j<num_ady; j++){
					int ady = get_ith_ady_of(Nodo(i), j);
					//Así, en la nueva lista se almacena el arco invertido
					edges_inverse[ady].push_back(Nodo(i));
				}
			}
			//Finalmente, se renombra la lista de adyacencia con la nueva creada
			edges = edges_inverse;
		}
		//Implementación de Kosaraju para calcular componentes fuertemente conexas
		int kosaraju(){
			int num_cfc = 0;
			
			//No aseguramos de que los nodos no hayan sido marcados como visitados
			clear_visit();
			
			//Iteramos por todos los nodos
			for(int i=1; i<nodes+1; i++){
				//Si no han sido visitados
				if(!list_of_nodes[i].visited){
					//Se marca como tal y s llama a la funcion DFS en la version 0
					list_of_nodes[i].visited = true;
					dfs_recursive(list_of_nodes[i], 0);
				}
			}
			
			//desmarcamos como visitados a los nodos
			clear_visit();
			
			//Invertimos el grafo
			inverse();
			//Y mientras que la pila no est=e vacía
			while(!aux.empty()){
				//Sacamos de la pila
				Nodo cur = aux.top();
				aux.pop();
				//Y si no ha sido visitdo
				if(!list_of_nodes[cur.node].visited){
					//Se marca como tal, aumentamos el contador de CFC y llamamos a la función DFS
					//para que recorra el grafo invertido calculado
					list_of_nodes[cur.node].visited = true;
					num_cfc++;
					dfs_recursive(cur, 1);
				}
			}
			
			return num_cfc;
		}
		
		
};

//PROGRAMA PRINCIPAL
int main(){
	// Modificación para la optimización de la entrada y salida de datos
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);
	
	//Variables del programa
	string known;
	int t, n;
	
	//Se ingresa el número de casos
	cin>>t;
	
	for(int i = 0; i<t; i++){
		//Se incresa el número de nodos
		cin>>n;
		//Creamos el grafo donde se almacenará la información dada
		Grafo_Ady_List G(n);
		
		for(int j = 0; j<n; j++){
			//Se ingresa la cadena de relaciones
			cin>>known;
			//a la vez que guardamos los nodos del grafo
			G.list_of_nodes[j+1] = Nodo(j+1);
			for(int k = 0; k<n; k++){
				//cuando se encuentre un Y
				if(known[k] == 'Y'){
					//agregamosun arco al grafo de tal foma que el grafo resultante sea dirigido
					G.new_edge(Nodo(j+1), Nodo(k+1));
				}
			}
		}
		//Imprimimos el resultado dado por kosaraju
		cout<< G.kosaraju() << endl;
	}
	return 0;
}
