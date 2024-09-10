/*
Estudiante: Astrid Alvarado
User CodeForce: A_6575
Ejercicio: Friendly Spiders (https://codeforces.com/problemset/problem/1775/D)
Veredicto: Accepted

Solución:
La idea consiste en particionar el grafo en dos conjuntos, de tal forma en que un conjunto corresponda al número de patas de cada araña,
y el otro que corresponda a aquellos números primos que componen al número de patas. Esto se hace de forma implícita con dos grafos, almacenando
en uno la descomposición del número de patas para cada araña y en el otro las arañas múltiplos de estos números primos.

Una vez armado los dos grafos, se realiza un recorrido por ambos grafos usando el algortimo de bfs, hasta encontrar el camino óptimo. 
*/

#include<iostream>
#include<vector>
#include<queue>
#include<stack>

using namespace std;

//Variables globales del programa
long long MAXN = 3*1e5 + 5;
vector<long long> vis_div(MAXN, 1);		//arreglo de divisores visitados

// Clase de nodo para tener mejor manejo de los datos relacionados con los nodos del grafo
class Nodo{
	public:
		long long node;		//Identificador del nodo
		long long dist;		//Distancia desde la raiz hasta el nodo actual
		long long pred;
		int num;			//Variable auxiliar que representa el kcolor asignado
		char color;			//Colores de nodos para marcar si fueron visitados. B (Blanco) indica que el nodo no ha sido visitado,
							//G (Gris) que ya fue visitado pero el recorrido aún no ha sido completado y N (Negro) que fue visitado y
							//la búsqueda ha sido terminada por completo.
		
		// Constructor de la clase Nodo. Si no se indica dist y num toma como valor default 0 y 1, además su color por defecto es B.
		//Además, si no se indica el nivel y su predecesor es 0
		Nodo(long long n, long long d = 0, long long p = 0, int u = 1, char c = 'B'){
			this->node = n;
			this->dist = d;
			this->num = u;
			this->color = c;
			this->pred = p;
		}
		Nodo(){
			this->node = 0;
			this->dist = 0;
			this->num = 1;
			this->color = 'B';
			this->pred = 0;
		}
};

//lista de nodos del grafo que contene la descomposición de las patas
vector<Nodo> list_of_nodes(MAXN);

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
		
		//Función para obtener el número de adyacentes que posee un nodo
		int get_num_ady(Nodo x){
			return edges[x.node].size();
		}
		//Función para obtener el nodo adyacente i del nodo x
		long long get_ith_ady_of(Nodo x, int i){
			return edges[x.node][i].node;
		}
		
		//BFS modificado para recorrer un grafo condicionado a otro. Al finalizar retorna un vector con el camino óptio
		vector<long long> path_i_to_j(long long i, long long j, Grafo_Ady_List Gp){
			vector<long long> path;
			queue<Nodo> Q;
			
			//si el punto de salida es el mismo de entrada, simplemente se agrega uno de los dos al camino
			if(i == j){
				path.push_back(i);
				return path;
			}
			
			//Almacenamos el nodo raíz en la cola y se marca como visitado
			Q.push(Nodo(i));
			visited[i] = 1;
			
			//mientras que la cola no esté vacia 
			while(!Q.empty()){
				Nodo cur = Q.front();
				Q.pop();
				
				//sacamos de la cola y recorremos por los adyacentes del nodo
				int num_ady = get_num_ady(cur);
				for(int r = 0; r<num_ady; r++){
					long long ady = get_ith_ady_of(cur, r);
					
					//Si el nodo no ha sido visitado del grafo de números primos
					if(Gp.visited[ady] != 1){
						
						//se marca como visitado en este grafo y recorremos por los adyacentes del mismo
						Gp.visited[ady] = 1;
					
						int num_ady_gp = Gp.get_num_ady(Nodo(ady));
						for(long long q = 0; q<num_ady_gp; q++){
							long long ady_gp = Gp.get_ith_ady_of(Nodo(ady), q);
							
							//si en el grafo que contiene las descomposiciones de las patas no ha sido vistiado el nodo
							if(visited[ady_gp] == -1){
								//se marca como visitado y actualizamos su nivel (dist) y predecesor (pred)
								visited[ady_gp] = 1;
								list_of_nodes[ady_gp].dist = cur.dist+1;
								list_of_nodes[ady_gp].pred = cur.node;
								Q.push(Nodo(ady_gp, cur.dist+1, cur.node));
								
								//en caso de encontar el punto de llegada buscamos vamos iterando por los predecesores de los nods y
								//almacenando en un vector
								if(ady_gp == j){
									long long pth = list_of_nodes[j].dist, 
											  aux = j;
									path.resize(pth+1);
									for(long long f = pth; f>=0; f--){
										path[f] = aux;
										aux = list_of_nodes[aux].pred;
									}
									return path;
								}
							}
						}
					}
				}
			}
			//en caso de no encotra camino, se retorna un vector vacío
			return path;
		}
};

Grafo_Ady_List G(MAXN);
Grafo_Ady_List GPrime(MAXN);

//Función que permite inicializar ambos grafos usando el algoritmo de Cribas de Eratóstenes
void init_with_prime_graph(vector<long long> arr){
	long long _div, lgs, aux;
	
	//recorremos por la cantidad máxima almacenando el menor divisor primo 
	for(long long i=2; i<MAXN+1; i++){
		if(vis_div[i] == 1){
			vis_div[i] = i;
			for(long long k = i*i; k<MAXN+1; k+=i){
				if(vis_div[k] == 1){
					vis_div[k] = i;
				}
			}
		}
	}

	//luego, armamos los grafos buscando los demás divisores primos  
	for(long long sp = 1; sp<arr.size(); sp++){
		aux = 1;
		
		lgs = arr[sp];
		while(lgs > 1){
			_div = vis_div[lgs];
			if(_div != aux){
				G.new_edge(Nodo(sp), Nodo(_div));
				GPrime.new_edge(Nodo(_div), Nodo(sp));
			}
			lgs /= _div;
			aux = _div;
		}
	}
}

//PROGRAMA PRINCIPAL
int main(){
	// Modificación para la optimización de la entrada y salida de datos
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);
	
	long long n, s, t;
	//se ingresa el número de arañas
	cin>>n;
	
	vector<long long> a(n+1);
	//se almacena las patas de cada araña
	for(long long i=1; i<n+1; i++){
		cin>>a[i];
	}
	//inicializamos los grafos
	init_with_prime_graph(a);
	
	//ingtresamos el punto de salida y llegada
	cin>>s>>t;
	
	//y obtenemos el camino óptimo, y dependiendo del tamaño del vector retornado se impirme la respuesta
	vector<long long> path = G.path_i_to_j(s, t, GPrime);
	
	if(path.size() > 1){
		cout<< list_of_nodes[t].dist+1<<endl;
		for(long long h=0; h<list_of_nodes[t].dist+1; h++){
			cout<<path[h]<<" ";
		}
		cout<<endl;
	}else if(path.size() == 1){
		cout<< 1 << "\n" << s <<endl;
	}else{
		cout<< -1 <<endl;
	}
	return 0;
}
