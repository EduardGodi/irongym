#include <iostream>
#include <conio.h>
#include <windows.h>
#include <string>
using namespace std;

void borrar(){
	system("cls");
}

void espaciosT(){
	cout<<"\t\t\t\t\t";
}

void irACoordenada(int x, int y) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    COORD pos = { (SHORT)x, (SHORT)y };
    SetConsoleCursorPosition(hConsole, pos);
}

void ocultarCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE; // Oculta el cursor blanco
    SetConsoleCursorInfo(hConsole, &info);
}

void setColor(int colorCode) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorCode);
}

void nameGym(){
	cout<<"\n";
	cout<<"\t\t\t\t --------------------------------------"<<endl;
	cout<<"\t\t\t\t   Bienvenido al sistema IronGym v1.0"<<endl;
	cout<<"\t\t\t\t --------------------------------------"<<endl;
}

void menu(){
	
	borrar();
	ocultarCursor();
	int posicion = 0;
	int nOpciones = 5;
	bool enMenu = true;
	
	string menu[nOpciones]{
	" REGISTRAR CLIENTE ",
	" TOTAL DE CLIENTES ",
	" CLIENTES VIP ",
	" PENDIENTES DE PAGO ",
	" SALIR "
	};
	nameGym();
	espaciosT();
	while(enMenu){
		irACoordenada(0,5);
		for(int i = 0; i < nOpciones; i++){
			if(i == posicion){
				setColor(6);
				espaciosT();
				cout<<"["<<menu[i]<<"]"<<endl;
			}else{
				setColor(7);
				espaciosT();
				cout<<" "<<menu[i]<<" "<<endl;
			}
		}
		int tecla = _getch();
		
		if(tecla == 224){
			tecla = _getch();
			if(tecla == 72){
				posicion--;
				if(posicion<0){
					posicion = nOpciones-1;
				}
			}else if(tecla == 80){
				posicion++;
				if(posicion>=nOpciones){
					posicion = 0;
				}
			}
		}else if(tecla == 13){
			if(posicion == (nOpciones-1)){
				break;
			}else{
				borrar();
				cout<<"Entraste a "<<menu[posicion]<<endl;
				getch();
				borrar();
			}
		}
		
			
	}
	
}

int main(){
	
	menu();
	return 0;
}
