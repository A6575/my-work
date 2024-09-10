#include<iostream>
#include<assert.h>
#include<string>
#include<vector>
#include<cstdlib>
#include<cmath>

using namespace std;

//Implementación de Stack

class Stack {
	// Variables de clase
	vector<char> L;
	int last, n;

	public:
	// Funciones / Operaciones
	Stack() {
	    L = vector<char>(2);
		last = -1;
		n = 2;
	}
	void push(int);
	int  top();
	void pop();
	bool empty();
};

void Stack::push(int v) {
	
	if (n-1 == last) {
		vector<char> Lnew;
		
		Lnew = vector<char>(2 * n);
		for (int i=0 ; i<n ; i++) {
			Lnew[i] = L[i];
		}
		L = Lnew;
		n *= 2;
	}

	L[last+1] = v;
	last++;
}

int Stack::top() {
	if (last == -1) {
		cerr << "Error: stack vacío" << endl;
		exit(0);
	}
	return L[last];
}

void Stack::pop() {
	if (last == -1) {
		cerr << "Error: stack vacío" << endl;
		exit(0);
	}
	last--;
}

bool Stack::empty() {
	return (last == -1);
}


//Variables del programa

int replacement = 0;
string RBS;
Stack saver;

bool has_pair_RBS(int i){
	/*
	Función que verifica si un bracket tiene su pareja.
	
	Arg:
	   i: número entero mayor o igual a 0 y menos que la longitud del string
	Return:
	   Retorna True si el i-ésimo bracket del string tiene su pareja una vez que 
	   se haya guardado en el stack. En caso contrario, retorna False.
	*/
	
	//Precondición:
	//El número debe ser menor que la longitud del string ingresado y mayor o igual a 0.
	//assert (0<=i<RBS.length());
	
    if(saver.top() == '(' && RBS[i] == ')'){
		return true;
	} else if(saver.top() == '[' && RBS[i] == ']'){
		return true;
	} else if(saver.top() == '{' && RBS[i] == '}'){
		return true;
	} else if(saver.top() == '<' && RBS[i] == '>'){
		return true;
    }
    return false;
}

//Programa Principal 

int main() {
	//Variables que cuentan la cantidad de brackets que abren y cierran.
	int opc = 0, closc = 0;
	
	cin >> RBS;
	
	//Precondición:
	//La longitud del string ingresado debe ser menor or igual a 10^6. 
	//assert (0<RBS.length()<=pow(10,6));
	
	if(RBS[0]=='}' || RBS[0]==')' || RBS[0]=='>' || RBS[0]==']'){
		cout<<"Impossible"<<endl;
	} else{
	    for(int i = 0; i<RBS.length(); i++){
	    	if(RBS[i] == '(' || RBS[i] == '{' || RBS[i] == '<' || RBS[i] == '['){
	    		saver.push(RBS[i]);
	    		opc++;
	    	} else if(!saver.empty() && has_pair_RBS(i)){
	    		saver.pop();
	    		closc++;
	    	} else if(!saver.empty() && (RBS[i] == ')' || RBS[i] == '}' || RBS[i] == '>' || RBS[i] == ']') ){
	    		replacement++;
	    		saver.pop();
	    		closc++;
	    	} else{
	    		closc++;
	    	}
	    }
	    if (saver.empty() && opc == closc){
	    	cout<<replacement;
        }else{
        	cout<<"Impossible"<<endl;
        }
		
	}
    
	return 0;
}
