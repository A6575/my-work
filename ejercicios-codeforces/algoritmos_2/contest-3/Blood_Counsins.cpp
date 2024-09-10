/*
Estudiante: Astrid Alvarado
User CodeForce: A_6575
Ejecicio: Blood counsins (https://codeforces.com/problemset/problem/208/E)
Veredicto: Time Limit Exceeded on test 9

Solución:
La idea consiste en, usando la implementación de LCA, ir viendo
la cantidad de primos que tiene v a la altura p. Para ello almacenamos
en una matriz ancestors todos los ancestros que tiene el nodo n y en
un vector de arreglos los nodos por su altura en el árbol, luego 
vamos contando si para cada nodo en la altura del nodo v, tiene el mismo
ancestro a la altura p.
*/


#include<iostream>
#include<vector>

using namespace std;

const int max_n = 100001;
const int max_saltos = 20;

vector<int> depth(max_n, -1); //Para almacenar la altura de cada nodo
vector<int> tree[max_n]; //Para almacenar los nodos que se encuentren en la misma altura
int ancestors[max_n][max_saltos]; //Para almacenar los ancestros de cada nodo

void setear_ancestro(int nodo, int salto) {
	    //Almacena en una matriz todos los ancestros del nodo en la altura saltos
        int ancestro_salto_1 = ancestors[nodo][salto-1];

        if(ancestro_salto_1 != -1 && ancestors[ancestro_salto_1][salto-1] != -1) {
                ancestors[nodo][salto] = ancestors[ancestro_salto_1][salto-1]; 
        }
}

void construir(int N) {
        for(int i = 1; i< max_saltos; i++) {
                for(int j =1 ; j <= N; j++ ) {
                        setear_ancestro(j, i);
                }
        }
}

int Primos(int v, int p){
	//Cuenta la cantidad de primos del nodo v a la altura p 
	int primos = 0;
	if(depth[v]>=p){
		    //Si la altura del nodo v es mayor o igual p buscamos su ancestro a la altura p. 
			int prd_v = ancestors[v][p];
			for(int k:tree[depth[v]]){
				//Y vamos buscando en el árbol todos los nodos que compartan dicho ancestro
				if((k != v) && (ancestors[k][p] == prd_v)){
					primos++;
				}
			}
	}
	return primos;
}

int main(){
	//Variables del programa
	int n, m, r, v, p;
	
	cin>>n;
	//Configuramos la matriz de ancestros con todos sus valores a -1.
	//Dicho valor representa NULL
	for(int i = 0; i<=n; i++){
		for(int j = 0; j<20; j++){
			ancestors[i][j] = -1;
		}
	}
    
    //Almacenamos la altura de cada nodo, los padres, y además guardamos
    //cada nodo por su altura.
	for(int i = 1; i<=n; i++){
		cin>>r;
		depth[i] = depth[r] + 1;
		tree[depth[i]].push_back(i);
		ancestors[i][1] = r;
	}
    
    //Construimos la matriz ancestros
    construir(n);
    
	cin>>m;
	int ans[m];
	for(int i=1; i<=m; i++){
		//Y por cada par ingresado contamos la cantidad de primos y almacenamos los valores
		cin>>v>>p;
		ans[i] = Primos(v, p);
	}
	
	//Finalmente retornamos
	for(int i=1; i<=m; i++){
		cout<< ans[i] << " ";
	}

	return 0;
}
