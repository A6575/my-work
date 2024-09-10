/*
Estudiante: Astrid Alvarado
User SPOJ: a_7565
Ejercicio: A Bug's Life (https://www.spoj.com/problems/BUGLIFE/)
Veredicto: Accepted

Soluci�n:
La idea consiste en recorrer el grafo y marcar cada nodo con los g�nero 1 y 2, de tal forma que los nodos no tengan 
adyacentes del mismo g�nero. Por esto, basta con ver si el grafo de interacciones entre los insectos es un grafo 
bipartito mediante una versi�n modificada del algoritmo iterativo de DFS. De tal forma que si se encuentra que un 
grafo no es bipartito, quiere decir que existe un insecto que interact�a con otro del mismo g�nero.
*/

#include<iostream>
#include<vector>
#include<queue>
#include<stack>

using namespace std;

// Clase de nodo para tener mejor manejo de los datos relacionados con los nodos del grafo
class Nodo{
	public:
		int node;		//Identificador del nodo
		int dist;		//Distancia desde la raiz hasta el nodo actual
		int num;		//Variable auxiliar
		char color;		//Colores de nodos para marcar si fueron visitados. B (Blanco) indica que el nodo no ha sido visitado,
						//G (Gris) que ya fue visitado pero el recorrido a�n no ha sido completado y N (Negro) que fue visitado y
						//la b�squeda ha sido terminada por completo.
		
		// Constructor de la clase Nodo. Si no se indica dist y num toma como valor default 0 y 1, adem�s su color por defecto es B
		Nodo(int n, int d = 0, int u = 1, char c = 'B'){
			this->node = n;
			this->dist = d;
			this->num = u;
			this->color = c;
		}
};

// Clase de grafo implementado como una lista de adyacencia junto con funciones b�sicas para manipular el mismo
class Grafo_Ady_List{
	int nodes;							//Cantidad de nodos
	vector<vector<Nodo> > edges;		//Lista de adyacencia
	vector<int> visited;				//Arreglo para marcar como visitados a los nodos
	public:
		//Constructor de la clase Grafo_Ady_List
		Grafo_Ady_List(int s){
			this->nodes = s;
			this->edges = vector<vector<Nodo> >(s+1);  //Para trabajar con 1-indexaci�n, se crea la lista de adyacencia con n�mero de nodos+1
			this->visited = vector<int>(s+1, -1);	   //se inicializa con -1 el vector para indicar que no ha sido visitado
		}
		//Funci�n para agregar un nuevo arco
		void new_edge(Nodo x, Nodo y){
			edges[x.node].push_back(y);
		}
		//Funci�n para agregar un nuevo nodo
		Nodo new_node(int x, int d, int u){
			nodes++;
			edges.push_back(vector<Nodo> ());
			return Nodo(x, d, u);
		}
		//Funci�n para obtener el n�mero de adyacentes que posee un nodo
		int get_num_ady(Nodo x){
			return edges[x.node].size();
		}
		//Funci�n para obtener el nodo adyacente i del nodo x
		int get_ith_ady_of(Nodo x, int i){
			return edges[x.node][i].node;
		}
		
		//Funci�n para detectar si un grafo es Bipartito usando una versi�n modificada de DFS
		bool Bipartito(vector<Nodo> arr, int N){
			//Reorremos por todos los nodos del grafo
			for(int i=1; i<N+1; i++){
				//Y empezamos el recorrido si el nodo no ha sido visitado
				if(arr[i].color == 'B'){
					//Se agrega el nodo ra�z a la pila
					stack<Nodo> Q;
					Q.push(arr[i]);
					
					//Y mientras que la pila no est� vac�a
					while(!Q.empty()){
						//Scamos y elminamos el nodo de la pila
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
								//Vemos el n�mero (g�nero) marcado del nodo actual para as� marcar su adyacente 
								//con un n�mero distinto y agregarlo a la pila 
								if(cur.num == 1){
									arr[ady].color = 'G';
									arr[ady].num = 2;
									Q.push(arr[ady]);
								}else{
									arr[ady].color = 'G';
									arr[ady].num = 1;
									Q.push(arr[ady]);
								}
							//En caso de que haya sido visitado, vemos si los n�meros asignados del nodo actual y su adyacente 
							//son iguales, y en caso de serlo retornamos FALSE
							}else if(cur.num == arr[ady].num){
								return false;
							}
						}	
					}
					//Y cerramos el recorrido
					arr[i].color = 'N';
				}
			}
			return true;
		}
		
};

//PROGRAMA PRINCIPAL
int main(){
	// Modificaci�n para la optimizaci�n de la entrada y salida de datos
	ios::sync_with_stdio(false);
	cin.tie(0); cout.tie(0);
	
	//Variables del programa (en orden): n�mero de casos, n�mero de insectos, n�mero de interacciones y variables auxiliares a y b
	int scn, bugs, interac, a, b;
	//Se ingresa el n�mero de casos
	cin>>scn;
	for(int i=1; i<scn+1; i++){
		//Y se ingresa el n�mero de insectos (nodos) e interacciones (arcos)
		cin>>bugs>>interac;
		//Creamos un grafo de tama�o bugs
		Grafo_Ady_List G(bugs);
		//Se crea un vector para almacenar los nodos del grafo y poder determinar la bipartici�n
		vector<Nodo> list_of_nodes;
		for(int j=0; j<interac; j++){
			//Luego se ingresa las interacciones
			cin>>a>>b;
			//Y se almacenan en el grafo como arcos de un grafo no dirigido
			G.new_edge(Nodo(a), Nodo(b));
			G.new_edge(Nodo(b), Nodo(a));
		}
		//Luego, se inicializa el vector de los nodos de tal forma que la posici�n del nodo y su valor coincidan
		for(int n=0; n<bugs+1; n++){
			list_of_nodes.push_back(Nodo(n));
		}
		//Finalmente, revisamos si el grafo es bipartito, y en caso de serlo, indicar que no hay insectos sospechosos.
		//De lo contrario se indica que hay insectos sospechosos
		if(G.Bipartito(list_of_nodes,bugs)){
			cout<<"Scenario #"<<i<<":\nNo suspicious bugs found!"<<endl;
		}else{
			cout<<"Scenario #"<<i<<":\nSuspicious bugs found!"<<endl;
		}
	}

	return 0;
}
