/*
Estudiante: Astrid Alvarado
User CodeForce: A_6575
Ejercicio: Bit++ (https://codeforces.com/problemset/problem/282/A)
Veredicto: Accepted
*/

#include<iostream>
#include<string>

using namespace std;

int main(){
	
	int n, x = 0;
	string statement;

	cin>>n;
	
	while (n--){
		cin>>statement;
		if (statement[1] == '+'){
			++x;
		} else {
			--x;
		}
	}
	
	cout<<"\n"<<x<<endl;
	return 0;
}
