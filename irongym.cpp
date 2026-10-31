#include <iostream>
#include <conio.h>
#include <windows.h>
#include <winspool.h>
#include <string>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <ctime>
using namespace std;

// plan: 1 = Basico, 2 = Premium, 3 = Black VIP
struct Cliente {
	string nombre;
	string telefono;
	int plan;
	double pendiente; // monto que debe
};

const int MAX_CLIENTES = 100;
Cliente clientes[MAX_CLIENTES];
int totalClientes = 0;

// la defino mas abajo, pero la necesito desde registrarCliente
void comprobantePago(int posicion);

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

void mostrarCursor() {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = TRUE;
    SetConsoleCursorInfo(hConsole, &info);
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

// letras UMG en bloques, cada letra ocupa unas columnas de la fila
string letras[] = {
"#   # #   #  ###  ",
"#   # ## ## #     ",
"#   # # # # #  ## ",
"#   # #   # #   # ",
" ###  #   #  ###  "
};

// dibuja solo las columnas de inicio a fin (para dibujar una letra a la vez)
void dibujarLetras(int inicio, int fin){
	setColor(6);
	for(int i = 0; i < 5; i++){
		for(int j = inicio; j < fin; j++){
			irACoordenada(51 + j, 6 + i);
			if(letras[i][j] == '#'){
				cout<<(char)219; // bloque solido
			}
		}
	}
}

void load(){

	ocultarCursor();
	char l = 219;
	int total = 30;
	for(int i = 0; i<=total; i++){
		// las letras aparecen segun el porcentaje de carga
		if(i == total/3){
			dibujarLetras(0, 5); // U
		}else if(i == total*2/3){
			dibujarLetras(6, 11); // M
		}else if(i == total){
			dibujarLetras(12, 18); // G
		}
		if(i == total){
			setColor(10); // verde al completar
		}else{
			setColor(6);
		}
		irACoordenada(0, 14); // la barra queda en medio, debajo de las letras
		cout<<"\t\t\t\t   "; // 4 tabs y 3 espacios para centrarla con las letras
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

// devuelve false si presionan ESC (para cerrar el programa), true con cualquier otra tecla
bool portada(){
	//SetConsoleOutputCP(437);
	borrar();
	ocultarCursor();
	dibujarLetras(0, 18); // mismas letras de la carga, en el mismo lugar
	irACoordenada(0, 11);
	setColor(7);
	cout<<"\n\t\t\t\t Universidad Mariano Galvez de Guatemala - Sede Mazatenango"<<endl;
	cout<<"\t\t\t\t\t     Catedratico: Ing. Mynor Lapoyeu"<<endl;
	setColor(8);
	cout<<"\n\t\t\t\t\t    Presione una tecla para continuar";
	cout<<"\n\t\t\t\t\t\t    ESC para salir";
	setColor(7);
	int tecla = _getch();
	if(tecla == 27){
		return false;
	}
	return true;
}

// lee la clave mostrando * en lugar de las letras
string leerClave(){
	string clave = "";
	char tecla;
	while(true){
		tecla = _getch();
		if(tecla == 13){
			break;
		}else if(tecla == 8){
			if(clave.size() > 0){
				clave.erase(clave.size() - 1); // borra el ultimo caracter
				cout<<"\b \b";
			}
		}else{
			clave = clave + tecla;
			cout<<"*";
		}
	}
	cout<<endl;
	return clave;
}

// revisa usuario y clave en usuarios.txt (cada linea: usuario clave)
bool login(){
	// si el archivo no existe se crea con un usuario por defecto
//	ifstream prueba("usuarios.txt");
//	if(!prueba){
//		ofstream nuevo("usuarios.txt");
//		nuevo<<"admin 1234"<<endl;
//		nuevo.close();
//	}
//	prueba.close();

	string usuario, clave, u, c;
	for(int intento = 1; intento <= 3; intento++){
		borrar();
		mostrarCursor();
		cout<<"\n";
		espaciosT();
		cout<<"INICIO DE SESION"<<endl<<endl;
		espaciosT();
		cout<<"Usuario: ";
		getline(cin, usuario);
		espaciosT();
		cout<<"Clave: ";
		clave = leerClave();

		ifstream archivo("usuarios.txt");
		while(archivo>>u>>c){
			if(u == usuario && c == clave){
				archivo.close();
				ocultarCursor();
				return true;
			}
		}
		archivo.close();

		ocultarCursor();
		setColor(12);
		cout<<endl;
		espaciosT();
		cout<<"Usuario o clave incorrectos. Intento "<<intento<<" de 3"<<endl;
		setColor(7);
		getch();
	}
	return false;
}

// lee clientes.txt: cada cliente ocupa 3 lineas (nombre, telefono, "plan pendiente")
void cargarClientes(){
	ifstream archivo("clientes.txt");
	if(!archivo){
		return; // si no existe todavia, empezamos sin clientes
	}
	string nombre, telefono;
	while(totalClientes < MAX_CLIENTES && getline(archivo, nombre)){
		getline(archivo, telefono);
		archivo>>clientes[totalClientes].plan>>clientes[totalClientes].pendiente;
		archivo.ignore(); // salto de linea que queda despues del numero
		clientes[totalClientes].nombre = nombre;
		clientes[totalClientes].telefono = telefono;
		totalClientes++;
	}
	archivo.close();
}

// reescribe clientes.txt con todos los clientes del arreglo
void guardarClientes(){
	ofstream archivo("clientes.txt");
	for(int i = 0; i < totalClientes; i++){
		archivo<<clientes[i].nombre<<endl;
		archivo<<clientes[i].telefono<<endl;
		archivo<<clientes[i].plan<<" "<<clientes[i].pendiente<<endl;
	}
	archivo.close();
}

const int NUM_PLANES = 3;

string nombrePlan(int plan){
	if(plan == 1){
		return "Basico";
	}else if(plan == 2){
		return "Premium";
	}else{
		return "Black VIP";
	}
}

// lo que cuesta cada plan
double precioPlan(int plan){
	if(plan == 1){
		return 150;
	}else if(plan == 2){
		return 250;
	}else{
		return 300;
	}
}

// margen izquierdo del formulario de registro (mas a la izquierda que el menu)
void margen(){
	cout<<"\t\t";
}

void registrarCliente(){
	string linea(50, '=');
	string guiones(50, '-');

	// encabezado
	// el titulo va centrado (como en las otras pantallas), el formulario a la izquierda
	cout<<"\n";
	setColor(6);
	espaciosT();
	cout<<linea<<endl;
	espaciosT();
	cout<<"             REGISTRAR NUEVO CLIENTE"<<endl;
	espaciosT();
	cout<<linea<<endl<<endl;
	setColor(7);
	if(totalClientes >= MAX_CLIENTES){
		margen();
		cout<<"No hay espacio para mas clientes."<<endl;
		getch();
		return;
	}

	Cliente c;

	// paso 1: datos personales
	setColor(6);
	margen();
	cout<<"1. DATOS PERSONALES"<<endl;
	setColor(8);
	margen();
	cout<<guiones<<endl;
	setColor(7);
	mostrarCursor();
	margen();
	cout<<"Nombre   : ";
	getline(cin, c.nombre);
	margen();
	cout<<"Telefono : ";
	getline(cin, c.telefono);
	cout<<endl;

	// paso 2: membresia, muestro cada plan con su precio (setw alinea las columnas)
	setColor(6);
	margen();
	cout<<"2. MEMBRESIA"<<endl;
	setColor(8);
	margen();
	cout<<guiones<<endl;
	setColor(7);
	cout<<fixed<<setprecision(2);
	margen();
	cout<<left<<setw(6)<<"Op."<<setw(16)<<"Plan"<<"Precio"<<endl;
	for(int i = 1; i <= NUM_PLANES; i++){
		margen();
		cout<<left<<setw(6)<<i<<setw(16)<<nombrePlan(i)<<"Q"<<precioPlan(i)<<endl;
	}
	cout<<endl;
	margen();
	cout<<"Elija una opcion: ";
	char tecla;
	do{
		tecla = _getch();
	}while(tecla < '1' || tecla > '0' + NUM_PLANES);
	c.plan = tecla - '0';
	cout<<c.plan<<endl<<endl;

	// paso 3: pago
	setColor(6);
	margen();
	cout<<"3. PAGO"<<endl;
	setColor(8);
	margen();
	cout<<guiones<<endl;
	setColor(7);
	margen();
	cout<<"Plan elegido     : "<<nombrePlan(c.plan)<<endl;
	margen();
	cout<<"Precio del plan  : Q"<<precioPlan(c.plan)<<endl;
	setColor(8);
	margen();
	cout<<"(escriba 0 si ya pago todo)"<<endl;
	setColor(7);
	margen();
	cout<<"Monto pendiente  : Q";
	while(!(cin>>c.pendiente) || c.pendiente < 0 || c.pendiente > precioPlan(c.plan)){
		cin.clear();
		cin.ignore(1000, '\n');
		setColor(12);
		margen();
		cout<<"Monto invalido, debe estar entre 0 y "<<precioPlan(c.plan)<<endl;
		setColor(7);
		margen();
		cout<<"Monto pendiente  : Q";
	}
	cin.ignore(1000, '\n');

	clientes[totalClientes] = c;
	totalClientes++;
	guardarClientes();

	ocultarCursor();
	setColor(10);
	cout<<endl;
	margen();
	cout<<"Cliente registrado correctamente!"<<endl;
	setColor(7);
	// saco el comprobante del cliente que acabo de guardar
	comprobantePago(totalClientes - 1);
}

// dibuja la tabla de los clientes que tienen el plan que le paso
void tablaPorPlan(int plan){
	int cantidad = 0;

	espaciosT();
	cout<<"Plan "<<nombrePlan(plan)<<":"<<endl;
	// encabezado de la tabla, setw le da un ancho fijo a cada columna
	espaciosT();
	cout<<left<<setw(5)<<"No."<<setw(20)<<"Nombre"<<setw(12)<<"Telefono"<<"Pendiente"<<endl;
	espaciosT();
	cout<<"------------------------------------------------"<<endl;

	// recorro el arreglo y solo muestro los del plan
	for(int i = 0; i < totalClientes; i++){
		if(clientes[i].plan == plan){
			cantidad++;
			espaciosT();
			cout<<left<<setw(5)<<cantidad<<setw(20)<<clientes[i].nombre<<setw(12)<<clientes[i].telefono<<"Q"<<clientes[i].pendiente<<endl;
		}
	}

	if(cantidad == 0){
		espaciosT();
		cout<<"Sin clientes en este plan"<<endl;
	}
	espaciosT();
	cout<<"Total: "<<cantidad<<endl<<endl;
}

void totalDeClientes(){
	cout<<"\n";
	espaciosT();
	cout<<"TOTAL DE CLIENTES"<<endl<<endl;
	espaciosT();
	cout<<"Clientes registrados: "<<totalClientes<<endl<<endl;

	// una tabla por cada plan
	for(int i = 1; i <= NUM_PLANES; i++){
		tablaPorPlan(i);
	}
	setColor(8);
	cout<<endl;
	espaciosT();
	cout<<"Presione una tecla para volver";
	setColor(7);
	getch();
}

// el ticket tiene 48 caracteres de ancho (lo que cabe en una impresora termica de 80 mm)
const int ANCHO_TICKET = 48;

// esta funcion le agrega espacios a la izquierda para que el texto quede centrado
string centrar(string texto){
	int espacios = (ANCHO_TICKET - texto.length()) / 2;
	return string(espacios, ' ') + texto;
}

// cuenta las lineas de pases.txt para saber que numero de ticket sigue
int siguienteTicket(){
	ifstream archivo("pases.txt");
	string linea;
	int cantidad = 0;
	while(getline(archivo, linea)){
		cantidad++;
	}
	archivo.close();
	return cantidad + 1;
}

// manda el texto directo a la impresora predeterminada de Windows (la termica)
// devuelve true si se pudo imprimir
bool imprimirTicket(string texto){
	char impresora[256];
	DWORD tamano = 256;
	if(!GetDefaultPrinterA(impresora, &tamano)){
		return false; // no hay impresora predeterminada
	}

	HANDLE h;
	if(!OpenPrinterA(impresora, &h, NULL)){
		return false;
	}

	// "RAW" significa que le mando el texto tal cual, sin que Windows lo adapte
	DOC_INFO_1A doc;
	doc.pDocName = (LPSTR)"Ticket IronGym";
	doc.pOutputFile = NULL;
	doc.pDatatype = (LPSTR)"RAW";

	DWORD escritos = 0;
	StartDocPrinterA(h, 1, (LPBYTE)&doc);
	StartPagePrinter(h);
	WritePrinter(h, (LPVOID)texto.c_str(), texto.length(), &escritos);
	EndPagePrinter(h);
	EndDocPrinter(h);
	ClosePrinter(h);

	return escritos == texto.length();
}

void paseUnDia(){
	cout<<"\n";
	espaciosT();
	setColor(6);
	cout<<"PASE POR UN DIA"<<endl<<endl;
	setColor(7);

	string visitante;
	mostrarCursor();
	espaciosT();
	cout<<"Nombre del visitante: ";
	getline(cin, visitante);
	ocultarCursor();
	if(visitante == ""){
		visitante = "Visitante";
	}
	// el nombre tiene que caber en el ticket
	if(visitante.length() > 30){
		visitante = visitante.substr(0, 30);
	}

	// saco la fecha y la hora de la computadora
	time_t ahora = time(0);
	tm *f = localtime(&ahora);
	char fecha[20], hora[10];
	strftime(fecha, sizeof(fecha), "%d/%m/%Y", f);
	strftime(hora, sizeof(hora), "%H:%M", f);

	int numero = siguienteTicket();

	// guardo el pase vendido para que el numero de ticket no se repita
	ofstream archivo("pases.txt", ios::app);
	archivo<<numero<<" "<<fecha<<" "<<hora<<" "<<visitante<<endl;
	archivo.close();

	// el numero del ticket con ceros a la izquierda (00001)
	ostringstream convertir;
	convertir<<setfill('0')<<setw(5)<<numero;
	string num = convertir.str();

	// armo todo el ticket en un solo texto, asi lo muestro y lo imprimo igual
	string linea(ANCHO_TICKET, '=');
	string guiones(ANCHO_TICKET, '-');
	string total = "TOTAL A PAGAR:";
	string precio = "Q45.00";
	// los espacios del medio hacen que el precio quede pegado a la derecha
	string ticket = "";
	ticket += linea + "\n";
	ticket += centrar("I R O N   G Y M") + "\n";
	ticket += centrar("Universidad Mariano Galvez") + "\n";
	ticket += linea + "\n";
	ticket += centrar("PASE POR UN DIA") + "\n";
	ticket += guiones + "\n";
	ticket += "Ticket No.: " + num + "\n";
	ticket += string("Fecha:      ") + fecha + "\n";
	ticket += string("Hora:       ") + hora + "\n";
	ticket += "Visitante:  " + visitante + "\n";
	ticket += guiones + "\n";
	ticket += "Incluye acceso a:\n";
	ticket += " - Area de pesas y maquinas\n";
	ticket += " - Clases grupales\n";
	ticket += " - Vestidores y duchas\n";
	ticket += guiones + "\n";
	ticket += total + string(ANCHO_TICKET - total.length() - precio.length(), ' ') + precio + "\n";
	ticket += guiones + "\n";
	ticket += "Valido solo hoy hasta las 23:59.\n";
	ticket += "No transferible ni reembolsable.\n";
	ticket += linea + "\n";
	ticket += centrar("Gracias por tu visita!") + "\n";
	ticket += centrar("Ven y entrena con fuerza") + "\n";
	ticket += linea + "\n";

	cout<<endl<<ticket;

	// lo que mando a la impresora: el ticket, unas lineas en blanco para que
	// el papel salga completo y el comando que corta el papel (ESC/POS: 29 86 66 0)
	string corte = "";
	corte += char(29);
	corte += 'V';
	corte += char(66);
	corte += char(0);
	bool impreso = imprimirTicket(ticket + "\n\n\n\n" + corte);

	cout<<endl;
	espaciosT();
	if(impreso){
		setColor(10);
		cout<<"Ticket impreso!"<<endl;
	}else{
		setColor(12);
		cout<<"No se pudo imprimir. Revise la impresora."<<endl;
	}
	setColor(8);
	espaciosT();
	cout<<"Presione una tecla para volver";
	setColor(7);
	getch();
}

// arma el comprobante de pago como texto (diseno de mi companera)
string armarFactura(int numeroFactura, string codigoCliente, string nombre, string telefono, string membresia, double precio, double pendiente, string metodoPago){
	time_t ahora = time(0);
	tm *fecha = localtime(&ahora);

	// uso ostringstream para ir armando el texto igual que con cout
	ostringstream t;
	// los titulos son los mismos que en el pase por un dia (mismo ancho y misma forma de centrar)
	string linea(ANCHO_TICKET, '=');
	string guiones(ANCHO_TICKET, '-');
	t<<linea<<"\n";
	t<<centrar("I R O N   G Y M")<<"\n";
	t<<centrar("Universidad Mariano Galvez")<<"\n";
	t<<linea<<"\n";
	t<<centrar("COMPROBANTE DE PAGO")<<"\n";
	t<<guiones<<"\n";
	t<<"Factura No.: "<<numeroFactura<<"\n";
	t<<"Fecha: "<<setfill('0')<<setw(2)<<fecha->tm_mday<<"/"<<setw(2)<<fecha->tm_mon + 1<<"/"<<fecha->tm_year + 1900<<setfill(' ')<<"\n";
	t<<guiones<<"\n";
	t<<centrar("DATOS DEL CLIENTE")<<"\n";
	t<<guiones<<"\n";
	t<<"Codigo: "<<codigoCliente<<"\n";
	t<<"Nombre: "<<nombre<<"\n";
	t<<"Telefono: "<<telefono<<"\n";
	t<<guiones<<"\n";
	t<<centrar("DETALLE DEL PAGO")<<"\n";
	t<<guiones<<"\n";
	t<<"Membresia: "<<membresia<<"\n";
	t<<fixed<<setprecision(2);
	t<<"Precio: Q"<<precio<<"\n";
	t<<guiones<<"\n";
	t<<"TOTAL A PAGAR: Q"<<precio<<"\n";
	t<<"Metodo de pago: "<<metodoPago<<"\n";
	if(pendiente > 0){
		t<<"Estado: PENDIENTE - Debe Q"<<pendiente<<"\n";
	}else{
		t<<"Estado: PAGADO"<<"\n";
	}
	t<<linea<<"\n";
	t<<centrar("GRACIAS POR ELEGIR IRON GYM")<<"\n";
	t<<centrar("TU ESFUERZO DE HOY ES TU FUERZA DE MANANA")<<"\n";
	t<<linea<<"\n";
	return t.str();
}

// muestra e imprime el comprobante del cliente que esta en esa posicion del arreglo
void comprobantePago(int posicion){
	double precio = precioPlan(clientes[posicion].plan);

	// el codigo del cliente sale de su posicion (C-001, C-002...)
	ostringstream codigo;
	codigo<<"C-"<<setfill('0')<<setw(3)<<posicion + 1;

	string factura = armarFactura(posicion + 1, codigo.str(), clientes[posicion].nombre, clientes[posicion].telefono, nombrePlan(clientes[posicion].plan), precio, clientes[posicion].pendiente, "Efectivo");
	cout<<endl<<factura;

	// mismo envio a la impresora que el pase por un dia: 4 lineas en blanco y el corte
	string corte = "";
	corte += char(29);
	corte += 'V';
	corte += char(66);
	corte += char(0);
	bool impreso = imprimirTicket(factura + "\n\n\n\n" + corte);

	cout<<endl;
	if(impreso){
		setColor(10);
		cout<<"Comprobante impreso!"<<endl;
	}else{
		setColor(12);
		cout<<"No se pudo imprimir. Revise la impresora."<<endl;
	}
	setColor(8);
	cout<<"Presione una tecla para volver";
	setColor(7);
	getch();
}

void pendientesDePago(){
	int cantidad = 0;
	double suma = 0;
	cout<<"\n";
	setColor(6);
	espaciosT();
	cout<<"PENDIENTES DE PAGO"<<endl<<endl;
	setColor(7);

	// encabezado de la tabla, con setw cada columna tiene su ancho
	cout<<fixed<<setprecision(2);
	espaciosT();
	cout<<left<<setw(5)<<"No."<<setw(20)<<"Nombre"<<setw(12)<<"Plan"<<"Pendiente"<<endl;
	setColor(8);
	espaciosT();
	cout<<"------------------------------------------------"<<endl;
	setColor(7);

	// recorro el arreglo y solo muestro los que deben algo
	for(int i = 0; i < totalClientes; i++){
		if(clientes[i].pendiente > 0){
			cantidad++;
			espaciosT();
			cout<<left<<setw(5)<<cantidad<<setw(20)<<clientes[i].nombre<<setw(12)<<nombrePlan(clientes[i].plan)<<"Q"<<clientes[i].pendiente<<endl;
			suma = suma + clientes[i].pendiente;
		}
	}
	if(cantidad == 0){
		espaciosT();
		cout<<"No hay pagos pendientes."<<endl;
	}else{
		setColor(8);
		espaciosT();
		cout<<"------------------------------------------------"<<endl;
		setColor(7);
		espaciosT();
		cout<<left<<setw(37)<<"Total pendiente"<<"Q"<<suma<<endl;
	}
	setColor(8);
	cout<<endl;
	espaciosT();
	cout<<"Presione una tecla para volver";
	setColor(7);
	getch();
}

// muestra el staff del proyecto en una tabla (sin acentos para la consola)
void integrantes(){
	const int TOTAL = 6;
	string nombres[TOTAL] = {
		"Stefany Liliana Sanchez Alvizurez",
		"Kelly Jeannette Cervantes Ortiz",
		"Edison Daniel Escobar Meneses",
		"Brayan Fernando Santay Montejo",
		"Josue Isaias Yac Cobox",
		"Angel Eduardo Godinez Calderon"
	};
	string carnes[TOTAL] = {
		"3090-26-7980",
		"3090-26-2986",
		"3090-26-4499",
		"3090-26-3884",
		"3090-26-5017",
		"3090-26-18"
	};

	cout<<"\n\n\n";
	setColor(6);
	espaciosT();
	cout<<" Ingenieria en Sistemas Segundo Semestre Seccion \"A\""<<endl;
	espaciosT();
	cout<<"\t\t  STAFF DEL PROYECTO"<<endl<<endl;
	setColor(7);

	// encabezado de la tabla
	espaciosT();
	cout<<left<<setw(5)<<"No."<<setw(36)<<"Nombre"<<"Carne"<<endl;
	setColor(8);
	espaciosT();
	cout<<"------------------------------------------------------"<<endl;
	setColor(7);

	// una fila por integrante
	for(int i = 0; i < TOTAL; i++){
		espaciosT();
		cout<<left<<setw(5)<<i + 1<<setw(36)<<nombres[i]<<carnes[i]<<endl;
	}

	setColor(8);
	cout<<endl;
	espaciosT();
	cout<<"Presione una tecla para volver";
	setColor(7);
	getch();
}

void menu(){

	borrar();
	ocultarCursor();
	int posicion = 0;
	const int nOpciones = 6;
	bool enMenu = true;

	string menu[nOpciones] = {
	" REGISTRAR CLIENTE ",
	" TOTAL DE CLIENTES ",
	" PASE POR UN DIA ",
	" PENDIENTES DE PAGO ",
	" STAFF ",
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
				break; // salgo del menu y main me regresa a la portada
			}else{
				borrar();
				if(posicion == 0){
					registrarCliente();
				}else if(posicion == 1){
					totalDeClientes();
				}else if(posicion == 2){
					paseUnDia();
				}else if(posicion == 3){
					pendientesDePago();
				}else if(posicion == 4){
					integrantes();
				}
				borrar();
				nameGym();
			}
		}
	}

}

int main(){
	cargarClientes();
	load();
	// portada -> login -> menu; al salir del menu se vuelve a la portada
	bool seguir = true;
	while(seguir){
		if(!portada()){
			seguir = false; // ESC en la portada cierra el programa
		}else if(login()){
			menu();
		}else{
			borrar();
			cout<<"\n\n";
			espaciosT();
			cout<<"Demasiados intentos fallidos. Saliendo..."<<endl;
			Sleep(1500);
			seguir = false;
		}
	}
	borrar();
	return 0;
}
