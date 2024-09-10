/*
Estudiante: Astrid Alvarado
User CodeForce: A_6575
Ejercicio: Boy or Girl (https://codeforces.com/problemset/problem/236/A)
Veredicto: Accepted
*/

#include<iostream>
#include<string>

using namespace std;

int main(){
	
	string name;
	cin>> name;
	
	int letters = 0;
	int len = name.size();
	int i = 0;
	
	while (i < len){
		int pos = name.find(name[i]);
		if (pos >= i){
			letters++;
		}
		i++;
	}
	
	if (letters % 2 == 0){
		cout << "CHAT WITH HER!" << endl;
	} else{
		cout << "IGNORE HIM!" << endl;
	}
	return 0;
}
