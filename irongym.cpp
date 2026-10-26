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

void load(){
	
	ocultarCursor();
	char l = 219;
	int total = 30;
	cout<<"\n\n\n\n\n\n\n\n";
	for(int i = 0; i<=total; i++){
		if(i == total){
			setColor(10); // verde al completar
		}else{
			setColor(6);
		}
		cout<<"\r";
		espaciosT();
		if(i == total){
			cout<<"Listo!       ";
		}else{
			cout<<"Cargando...  ";
		}
		cout<<"[";
		for(int j = 0; j<total; j++){
			if(j < i){
				cout<<l;
			}else{
				cout<<" ";
			}
		}
		cout<<"] "<<i*100/total<<"%  ";
		Sleep(80);
	}
	Sleep(500);
	borrar();
}

void portada(){
	string letras[] = {
	"#   # #   #  ###  ",
	"#   # ## ## #     ",
	"#   # # # # #  ## ",
	"#   # #   # #   # ",
	" ###  #   #  ###  "
	};
	int nLineas = 5;

	//SetConsoleOutputCP(437); 
	borrar();
	ocultarCursor();
	cout<<"\n\n\n\n\n\n";
	setColor(6);
	for(int i = 0; i < nLineas; i++){
		cout<<"\t\t\t\t\t\t   ";
		for(int j = 0; j < (int)letras[i].size(); j++){
			if(letras[i][j] == '#'){
				cout<<(char)219; // bloque solido
			}else{
				cout<<letras[i][j];
			}
		}
		cout<<endl;
	}
	setColor(7);
	cout<<"\n\t\t\t\t\t Universidad Mariano Galvez de Guatemala"<<endl;
	cout<<"\t\t\t\t\t     Catedratico: Ing. Mynor Lapoyeu"<<endl;
	setColor(8);
	cout<<"\n\t\t\t\t\t    Presione una tecla para continuar";
	setColor(7);
	_getch();
}

void menu(){

	borrar();
	ocultarCursor();
	int posicion = 0;
	int nOpciones = 5;
	bool enMenu = true;

	string menu[nOpciones]	{
	" REGISTRAR CLIENTE ",
	" TOTAL DE CLIENTES ",
	" CLIENTES VIP ",
	" PENDIENTES DE PAGO ",
	" SALIR "
	};
	espaciosT();
	nameGym();
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

		setColor(8);
		cout<<endl;
		cout<<"\t\t\t\t  Flechas: mover  |  ENTER: seleccionar"<<endl;
		setColor(7);

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
				nameGym();
			}
		}
	}

}

int main(){
	load();
	portada();
	menu();
	return 0;
}
