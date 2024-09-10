/*
Estudiante: Astrid Alvarado
User SPOJ: a_7565
Ejercicio: Capital City (https://www.spoj.com/problems/CAPCITY/)
Veredicto: Accepted

Solución:
La idea consiste en detectar todas las Componentes Fuertemente Conexas del grafo, de esta forma tener en consideración aquellos posibles
nodos candidatos a ser capital de las ciudades. Posterior a eso, calcular el grafo reducido con el fin de ver cual CFC es alcanzable desde
cualquier otra CFC, es así como todos los nodos que pertenezcan a esa CFC que es alcanzable desde el resto, serán los nodos elegidos a ser
candidatos a capitales.

*/
#include<iostream>
#include<vector>
#include<queue>
#include<stack>
#include<string>
#include<algorithm>

using namespace std;

//Variables globales del programa
int MaxN = 2000007;
int MaxM = 2000007;
vector<int> CFC_indx(MaxN, 0);
vector<vector<int> > CFC_k(MaxN);

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
		int cnt_hijos;		//Contador de hijos que tiene un nodo
		int num;			//Variable auxiliar que representa el kcolor asignado
		bool visited;		//Variable para indicar si un nodo fue visitado 
		char color;			//Colores de nodos para marcar si fueron visitados. B (Blanco) indica que el nodo no ha sido visitado,
							//G (Gris) que ya fue visitado pero el recorrido aún no ha sido completado y N (Negro) que fue visitado y
							//la búsqueda ha sido terminada por completo.
		
		// Constructor de la clase Nodo. Si no se indica dist y num toma como valor default 0 y 1, además su color por defecto es B.
		//Por otro lado, si no se indica el nivel y su predecesor es 0, a parte de que como valor predeterminado visitado será falso
		Nodo(int n, int d = 0, int p = 0, int dp = 0, int dm = 0, int ds = 0, int l = 0, int h = 0, int u = 1, bool vs = false, char c = 'B'){
			this->node = n;
			this->dist = d;
			this->delta_p = dp;
			this->delta_m = dm;
			this->disc = ds;
			this->low = l;
			this->cnt_hijos = h;
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
			this->cnt_hijos = 0;
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
		//vector<vector<int> > C_F_C;			//components fuertemente conexas del grafo descubierto por Tarjan
		//vector<pair<int, int> > puentes;	//vector de pares para almacenar los puentes encontrados por Tarjan
		//vector<bool> art_points;          //vector para marcar los puntos de articulación 
		
		//Constructor de la clase Grafo_Ady_List
		Grafo_Ady_List(int s){
			this->nodes = s;
			this->edges = vector<vector<Nodo> >(s+1);  //Para trabajar con 1-indexación, se crea la lista de adyacencia con número de nodos+1
			this->list_of_nodes = vector<Nodo>(s+1);
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
		//Implementqación recursiva de algortimo de recorrido DFS nmodoficada para que en su versión 0 almacene 
		//los nodos en una pila, y en versión 1 calcule las CFC del grafo, almacenándola un vector de vectores
		void dfs_recursive(Nodo root, int vers, int num){
			
			if(vers == 1){
				//Se marca el número de CFC al que pertenece cada nodo
				CFC_indx[root.node] = num;
				//y en esa misma posición se guarda los nodos pertenecientes a esa misma CFC
				CFC_k[num].push_back(root.node); 
			}
			
			//se recorre por los adyacentes del nodo raiz
			int num_ady = get_num_ady(root);
			for(int i=0; i<num_ady; i++){
				Nodo r = get_ith_adynode_of(root, i);
				
				//Si no ha sido visitado
				if(!list_of_nodes[r.node].visited){
					//se marca como tal y se llama otra vez a la función DFS con el nodo adyacente
					list_of_nodes[r.node].visited = true;
					
					dfs_recursive(r, vers, num);
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
					dfs_recursive(list_of_nodes[i], 0, 0);
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
					dfs_recursive(cur, 1, num_cfc);
				}
			}
			//revertimos el grafo nuevamente para tener el grafo original
			inverse();
			
			return num_cfc;
		}
};

//PROGRAMA PRINCIPAL
int main(){
	// Modificación para la optimización de la entrada y salida de datos
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);
	
	//Variables del programa
	int N,M,A,B,cfc, hlp, n, capitals = 0, access_from_cap = 0;
	
	//Se ingresa el número de nodos y el número de lados
	cin>>N>>M;
	
	//se crea el grafo correspondiente
	Grafo_Ady_List G(N);
	
	//almacenamos los nodos del grafo en la lista
	for(int i=1; i<N+1; i++){
		G.list_of_nodes[i] = Nodo(i);
	}
	
	for(int i=0; i<M; i++){
		//se ingresan los arcos del grafo
		cin>>A>>B;
		//y se almacenan en el grafo de tal forma que es un grafo dirigido
		G.new_edge(Nodo(A), Nodo(B));
		//actualizamos el grado exterior del nodo de salida y el grado interior de llegada
		G.list_of_nodes[A].delta_p++;
		G.list_of_nodes[B].delta_m++;
	}
	
	//en caso de que algun nodo no tenga grado exterior o interior, quiere decir que no es alcanbale 
	//por el resto de ciudades, por lo tanto no hay candidata posible a ser capital.
	for(int i=1; i<N+1; i++){
		if(G.list_of_nodes[i].delta_p == 0 && G.list_of_nodes[i].delta_m == 0){
			cout<< 0 <<endl;
			return 0;
		}
	}
	
	//se realiza kosaraju y obtenemos el numero de CFC
	cfc = G.kosaraju();
	
	//creamos el grafo reducido
	Grafo_Ady_List Grafo_reducido(cfc);
	for(int i=1; i<cfc+1; i++){
		for(int j=0; j<CFC_k[i].size(); j++){
			//iteramos por los nodos que pertenecen a una misma CFC y obtenemos sus
			//adyacentes en el grafo original
			hlp = G.get_num_ady(G.list_of_nodes[CFC_k[i][j]]);
			for(int k=0; k<hlp; k++){
				n = G.get_ith_ady_of(G.list_of_nodes[CFC_k[i][j]],k);
				
				//si el indice de CFC del nodo adyacente es distinto a i, se agrega al grafo reducido,
				//actualizando os grados exteriores e interiores correspondientes.
				//Esto es para simular que un nodo equivole a una CFC y evitar agregar noos demás
				if(CFC_indx[n] != i){
					Grafo_reducido.new_edge(Nodo(i), Nodo(CFC_indx[n]) );
					Grafo_reducido.list_of_nodes[i].delta_p++;
					Grafo_reducido.list_of_nodes[CFC_indx[n]].delta_m++;
				}
			}
		}
	}
	
	//Luego, si se encuentra una CFC que tenga grado exterior igual a 0, se escoge como 
	//CFC cuyos nodos son candidatos a capital. Sin embargo, si se encuentra más de una 
	//CFC de este estilo, se debe retornar 0 pues no todos los nodos son alcanzables
	for(int i = 1; i<cfc+1; i++){
		if(Grafo_reducido.list_of_nodes[i].delta_p == 0){
			capitals = i;
			access_from_cap++;
		}
		if(access_from_cap > 1){
			cout << 0 << endl;
			return 0;
		}
	}
	//Una vez hecho esto, imprimimos el tamaño de la CFC
	cout<<CFC_k[capitals].size()<<endl;
	//ordenamos el mismo
	sort(CFC_k[capitals].begin(), CFC_k[capitals].end());
	//e imprimomo caa nodo perteneciente a esta
	for(int i=0; i<CFC_k[capitals].size(); i++){
		cout<<CFC_k[capitals][i]<< " ";
	}
	cout<<endl;
	
	return 0;
}
