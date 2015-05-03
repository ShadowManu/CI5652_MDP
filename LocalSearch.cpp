/*
	Descripcion: Busqueda Local para el problema de maxima diversidad (MDP)
	Autores: Cristian Medina
			 Manuel Pacheco
	Ult. Modificacion: 03/05/2015
*/

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <algorithm>
#include <vector>
#include <queue>
#include <utility>
#include <ctime>
#include <set>

using namespace std;

const long MAXITR = 10000000;		//Numero maximo de iteraciones a realizar en total
const long ITERNOCHANGE = 100000;	//Numero maximo de iteraciones a realizar en una vecindad
const bool DEBUG = true;			//Valor para mostrar datos de la corrida

int numNodes;						//Numero de Nodos grafo
int solutionSize;					//Numero de Nodos de la solucion
int numEdges;						//Numero de arcos del problema
long GlobalIter;					//Numero de iteraciones realizadas
double *matriz;						//Arreglo con los arcos
double *coefDistance;				//Coeficiente de distancia entre arcos para cada nodo
int *solution;						//Vector con los nodos usados en la solucion parcial
vector<int> notChosen;				//Vector con los nodos no usados en la solucion parcial

/*
	Comparador de pares para la construccion de la solucion inicial
*/
class pairComp
{
public:
	bool operator() (const pair<int, double> a, const pair<int, double> b) const{
		return a.second < b.second;
	}
};

/*
	Constructor de la solucion inicial. Elije los M nodos con mayor coeficiente de distancia,
	el cual se define como, para cada nodo, la suma de las distancias entre ese nodos con todos
	los demas, dividido por el numero total nodos.
*/
void initSolution(){

	//Cola de prioridad para los nodos en funcion de su coeficiente de distancia.
	priority_queue <pair<int, double>,vector<pair<int,double>>,pairComp > pq;
	
	//Inicializa la cola de prioridad
	for (int i = 0; i < numNodes; i++){
		pair <int, double> dummy = make_pair(i, coefDistance[i]);
		pq.push(dummy);
	}

	//Elige los primeros M elementos de la cola
	for (int i = 0; i < numNodes; i++){
		if (i < solutionSize){
			solution[i] = pq.top().first;
		}
		else {
			notChosen.push_back(pq.top().first);
		}
		
		pq.pop();
	}
}

/*
	A partir de dos nodos lados a y b, calcula la posicion en el arreglo matriz donde se encuentra
	almacenada la distancia del arco entre a y b.
*/
int getPos(int a, int b){
	if (a < b){
		return numEdges - (((numNodes - a)*((numNodes - a) - 1)) / 2) + (b - a) - 1;
	}
	else {
		return numEdges - (((numNodes - b)*((numNodes - b) - 1)) / 2) + (a - b) - 1;
	}
	return 0;
}

/*
	Imprime los arcos leidos del archivo, su posicion en el arreglo y los coeficientes de distancia de cada nodo.
*/
void printInput(){

	cout << "Nodes #: " << numNodes << " Solution #: " << solutionSize << "\n";
	
	for (int i = 0; i < numNodes; i++) {
		for (int j = 0; j < numNodes; j++) {
			if (i < j){
				cout << "Arco: "<<i << " " << j << " " << matriz[getPos(i, j)] << " | Pos: " << getPos(i, j) << "\n";
			}
		}
	}

	cout << "\n";

	for (int i = 0; i < numNodes; i++){
		cout << "Node: " << i << " Coef: " << coefDistance[i] << "\n";
	}

	cout << "\n";
}

/*
	A partir del nombre de un archivo dado, lee el numero de nodos, el numero de nodos de la solucion
	y los arcos para el problema. Construye entonces las estructuras de datos correspondientes.
*/
void readInput(char* fileName){

	ifstream in(fileName);
	streambuf *cinBuffer = cin.rdbuf();
	cin.rdbuf(in.rdbuf());

	//Lee numero de Nodos y numeros de nodos de la solucion
	cin >> numNodes >> solutionSize;
	numEdges = (numNodes*(numNodes - 1)) / 2;
	int from, to;
	double length;

	//Asigna el espacio de memoria exacto a la arrelgo de vectores, el vector de coeficientes y el de solucion.
	matriz = (double*) malloc(numEdges*sizeof(double));
	coefDistance = (double*) malloc(numNodes*sizeof(double));
	solution = (int*)malloc(solutionSize*sizeof(int));
	
	for (int i = 0; i < numNodes; i++){
		coefDistance[i] = 0;
	}

	//Lee cada uno de los arcos
	for (int i = 0; i < numEdges; i++){
		cin >> from >> to >> length;
		coefDistance[from] += length;
		coefDistance[to] += length;
		matriz[i] = length;
	}

	//Realiza el calculo final para los coeficientes
	for (int i = 0; i < numNodes; i++){
		coefDistance[i] = coefDistance[i]/numNodes;
	}

	cin.rdbuf(cinBuffer);
}

/*
	Calcula la suma de las distancias de la solucion actual representada en el vector solucion
*/
double calculateDistance(){
	
	double distance = 0;

	for (int i = 0; i < solutionSize; i++){
		for (int j = i+1; j < solutionSize; j++){
			distance += matriz[getPos(solution[i], solution[j])];
		}
	}

	return distance;
}

/*
	Procedimiento que imprime el vector de la solucion actual y la suma de distancias actual dada.
*/
void printDebug(double currentDistance){
	if (DEBUG){

		cout << "-------------------------------------------------------------------------------\n";
		cout << "Current Solution: ";

		for (int i = 0; i < solutionSize; i++){
			cout << solution[i] << " ";
		}

		cout << "\nCurrent Distance: " << currentDistance << "\n";
	}
}

/*
	Procedimiento que imprieme datos sobre la iteración actual a ejecutar.
*/
void printDebugIter(double currentDistance, int localIter, int totalIter, int Pivot, int newNode){
	cout << "===============================================================================\n";
	cout << "TotalIter: " << totalIter << " | LocalIter: " << localIter <<
		"\nPivot Node: " << Pivot << " | New Node: " << newNode << "\n";
	printDebug(currentDistance);
}

/*
	Procedimiento de aplica la busqueda local al problema. Itera sobre una solucion inicial recorriendo las
	distintas vecindades sin escederse de un numero de iteraciones por vencidad y un numero de iteraciones total.
	Se define una vecindad como los posible valores que puede tomar un elemento del vector de solucion. Cuando se hayan
	agotados las posibilidades o se haya alcanzado un maximo de iteraciones ne la vecindad, se procede a evaluar otro
	elemento del vector de solucion.
*/
double localSearch(){

	bool stay = true;
	long totalIter = 0, localIter = 0;
	int pivotNode = solutionSize-1, compareNode, oldNode, ncSize = numNodes-solutionSize;
	set <int> lastTries;
	
	double currentDistance = calculateDistance(), newDistance;

	if (DEBUG){
		printDebug(currentDistance);
	}

	while (stay) {

		//Se verifica las iteraciones por vecindad
		if ((localIter < (numNodes - solutionSize)) && (localIter < ITERNOCHANGE)){

			//Se elige un elemento de los no elegidos en la solucion para intercambiarlo con el pivote actual
			compareNode = rand() % (ncSize);

			//Se verifica que el elemento no se haya probado antes
			if (lastTries.find(compareNode)!=lastTries.end()){
				continue;
			}

			lastTries.insert(compareNode);

			if (DEBUG){
				printDebugIter(currentDistance, localIter, totalIter, pivotNode, notChosen[compareNode]);
			}

			//Se inbtercambia el elemento pivote con el nuevo nodo a probar
			oldNode = solution[pivotNode];
			solution[pivotNode] = notChosen[compareNode];
			newDistance = calculateDistance();

			//Se procede a evaluar si se obtuvo una mejora con respecto a la solucion actual
			if (currentDistance < newDistance){
				currentDistance = newDistance;
				notChosen[compareNode] = oldNode;
			}
			else {
				solution[pivotNode] = oldNode;
			}
			
			localIter++;
			totalIter++;
		}
		else {
			//Cambia de vencidad, al cambiar el nodo pivote de la solucion.
			lastTries.clear();
			localIter = 0;
			pivotNode--;
		}

		//Condicion de salida
		if ((totalIter >= MAXITR) || (pivotNode < 0)) {
			stay = false;
		}

	}

	GlobalIter = totalIter;
	return currentDistance;
}

/*
	Main
*/
int main(int argc, char *argv[]){

	char *fileName;
	int nodeNum = 0;

	//Inicializacion del nombre del archivo
	if (argc < 2) {
		fileName = "prueba1.in";
	}
	else {
		fileName = argv[1];
	}

	readInput(fileName);

	if (DEBUG) {
		printInput();
	}

	
	clock_t begin = clock();

	initSolution();
	double result = localSearch();

	//Calculo del tiempo transcurrido
	clock_t end = clock();
	double seconds = double(end - begin) / CLOCKS_PER_SEC;

	cout << "\n###############################################################################\n\n";
	cout << "Universe size: " << numNodes << " | Solution size: " << solutionSize << "\n";
	cout << "Result: " << result << " | Time: " << seconds << " | Total Iterations: " << GlobalIter << "\n";
	cout << "\n###############################################################################\n";

	free(matriz);
	free(coefDistance);
	free(solution);

	while (1){
	}
}