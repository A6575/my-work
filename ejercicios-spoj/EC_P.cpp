/*
Estudiante: Astrid Alvarado
User SPOJ: a_7565
Ejercicio: Critical Edges (https://www.spoj.com/problems/EC_P/)
Veredicto: Accepted

Solución:
La idea consiste en recorrer el grafo haciendo uso del algoritmo de Tarjan para calcular los puentes que posee el grafo.
En caso de que hayan, se imprime los mismos asegurándonos de que los arcos estén en orden decreciente, ademas de que el
primer nodo sea menor que el segundo impreso. En caso d no haber puentes en e grafo, se retorna "Sin bloqueos".

*/
#include<iostream>
#include<vector>
#include<queue>
#include<stack>
#include<string>
#include<algorithm>

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
		int disc;			//Discovery del nodo, es decir, el tiempo en que fue descubierto
		int low;			//Low del nodo, es decir, el menor discovery encontrado en el DFS tree generado
		int num;			//Variable auxiliar que representa el kcolor asignado
		bool visited;		//Variable para indicar si un nodo fue visitado 
		char color;			//Colores de nodos para marcar si fueron visitados. B (Blanco) indica que el nodo no ha sido visitado,
							//G (Gris) que ya fue visitado pero el recorrido aún no ha sido completado y N (Negro) que fue visitado y
							//la búsqueda ha sido terminada por completo.
		
		// Constructor de la clase Nodo. Si no se indica dist y num toma como valor default 0 y 1, además su color por defecto es B.
		//Por otro lado, si no se indica el nivel y su predecesor es 0, a parte de que como valor predeterminado visitado será falso
		Nodo(int n, int d = 0, int p = 0, int dp = 0, int dm = 0, int ds = 0, int l = 0, int u = 1, bool vs = false, char c = 'B'){
			this->node = n;
			this->dist = d;
			this->delta_p = dp;
			this->delta_m = dm;
			this->disc = ds;
			this->low = l; 
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
			this->disc = 0;
			this->low = 0;
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
	int time;							//Variable global para actualizar el discovery
	
	public:
		vector<Nodo> list_of_nodes;			//Vector de nodos del grafo creado
		vector<vector<int> > C_F_C;			//components fuertemente conexas del grafo descubierto por Tarjan
		vector<pair<int, int> > puentes;	//vector de pares para almacenar los puentes encontrados por Tarjan
		
		//Constructor de la clase Grafo_Ady_List
		Grafo_Ady_List(int s){
			this->nodes = s;
			this->edges = vector<vector<Nodo> >(s+1);  //Para trabajar con 1-indexación, se crea la lista de adyacencia con número de nodos+1
			this->list_of_nodes = vector<Nodo>(s+1);
			this->C_F_C = vector<vector<int> >(s+1);
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
		
		//Algoritmo de Tarjan para detectar CFC. En particular, se agrega la condici=on para calcular
		//puentes en un grafo no dirigido 
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
				
				//en caso de que el adyancente sea el padre del nodo raiz, obviamos este y seguimos con el recorrido
				if(ady == list_of_nodes[root].pred){
					continue;
				}
				
				//si no ha sido visitado el nodo adyacente
				if(!list_of_nodes[ady].visited){
					//guardamos su predecesor
					list_of_nodes[ady].pred = root;
					//y llamamos otra vez al algoritmo
					tarjan(ady);
					
					//Para detectar puentes en un grafo no dirigido, es necesario que el discovery 
					//de la raiz sea menor que el low de su adyacente. Esto es para asegurar que 
					//en el subarbol del DFS tree generado no haya la existencia de back-edges
					//hacia el vértice que se visita o a un nodo anterior 
					if(list_of_nodes[root].disc < list_of_nodes[ady].low){
						//revisamos si la raiz es menor al adyacente para poder cumplir con la condición del problema
						if(root<ady){
							puentes.push_back(make_pair(root, ady));
						}else{
							puentes.push_back(make_pair(ady, root));
						}
					}
					//finalmente, actualizamos el low del nodo raiz
					list_of_nodes[root].low = min(list_of_nodes[root].low, list_of_nodes[ady].low);
 				}else{
 					//en caso de que el nodo ya haya sido visitado, revisamos que el adyacente no haya concluido su recorrido
 					//para así poder actualizar el low del nodo raiz
 					if(list_of_nodes[ady].color == 'G'){
 						list_of_nodes[root].low = min(list_of_nodes[root].low, list_of_nodes[ady].disc);
 					}
 				}
			}
			//finalmente, se calula las componentes fuertemente conexas verificando si el low y el discovery del
			//nodo raiz son iguales
			if(list_of_nodes[root].disc == list_of_nodes[root].low){
				cc++;
				//y mientras que la pila no esté vacía y que el nodo raiz no sea igual al tope de la pila
				while(!Q.empty() && root != Q.top()){
					//se cierra el recorrido de los nodos y se guarda la componente fuertemente conexa
					int cur = Q.top();
					Q.pop();
					list_of_nodes[cur].color = 'N';
					C_F_C[cc].push_back(cur);
				}
				
				//se saca de la pila el nodo faltante y se guarda el nodo raiz en su CFC respectiva
				Q.pop();
				C_F_C[cc].push_back(root);
				//finalmente se cierra el recorrido del nodo coloreándolo de Negro
				list_of_nodes[root].color = 'N';
			}
			
		}
};

//PROGRAMA PRINCIPAL
int main(){
	// Modificación para la optimización de la entrada y salida de datos
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);

	//Variables del programa
	int NC, N, M, a, b;
	
	//Se ingresa el número de casos
	cin>>NC;
	for(int i=0; i<NC; i++){
		//Se ingresa el número de nodos y número de lados
		cin>>N>>M;
		//Se genera el grafo
		Grafo_Ady_List G(N);
		
		//Se guardan los nodos del grafo
		for(int j=1; j<N+1; j++){
			G.list_of_nodes[j] = Nodo(j);
		}
		
		for(int j=0; j<M; j++){
			//se ingresa los lados del grafo
			cin>>a>>b;
			//y se guardan los arcos de tal forma qu el grafo resultante es no dirigido
			G.new_edge(Nodo(a), Nodo(b));
			G.new_edge(Nodo(b), Nodo(a));
		}
		
		//se aplica el algoritmo de Tarjan
		G.tarjan(1);
		
		//en caso de que el vector de puentes esté vacio, se retorna "Sin bloqueos"
		cout<<"Caso #"<<i+1<<endl;
		if(G.puentes.empty()){
			cout<<"Sin bloqueos"<<endl;
		}else{
			//sino, se imprime el tamaño del vector, se ordena el mismo y se imprime los arcos hallados
			cout<<G.puentes.size()<<endl;
			sort(G.puentes.begin(), G.puentes.end());
			for(int j=0; j<G.puentes.size(); j++){
				cout<<G.puentes[j].first<<" "<<G.puentes[j].second<<endl;
			}
		}
		
	}
	
	return 0;
}
