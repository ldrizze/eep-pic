#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>

#define IN 999
#define N 9


//          GLOBALS
enum cores{R,G,B}; // Cores
char letter[9]; // Letras das cores do labirinto
char single_letter[3] = {'R','G','B'};

			 // R G B
int pesos[3] = {1,2,3}; // Peso das cores RGB respectivamente


// PROTOTYPES
int peso(int p);
void printMatrix(int m[][3]);
void printMatrixAsRGB(int m[][3]);
void printPesos(int p[3]){int i; for(i=0;i<3;i++) printf("%c = %d\n", single_letter[i], p[i]); }
void printAdj(int m[][9]);
int dijkstra(int cost[][N], int source, int target, int record_path[9]);
char *strrev(char *str);
void printPath(int path[9]);
void printColorPath(int c[9]);
void makeLetters(int m[3][3], char l[9]);
void makeColorPath(int p[9], int c[9]);
void reverse_array(int *a, int n);
int backtracking(int m[3][3], int p[9], int adj[9][9], int i, int j, int n, int* visited);
void randomize(int arr[], int n);
void swap(int *a, int *b);
void line(void){ getchar(); printf("---------------------\n\n"); }

int matriz[3][3] = { // Matriz do labirinto
	{R,G,B},
	{B,G,R},
	{G,B,R}
};

int main(){

    // MISTURA O LABIRINTO
    int colors_to_shuffle[9] = {R,R,R,G,G,G,B,B,B};
    randomize(colors_to_shuffle, 9);
    int i,j;
    for(i=0; i<3; i++){
        for(j=0; j<3; j++){
            matriz[i][j] = colors_to_shuffle[(i*3) + (j)];
        }
    }

    // ATRIBUI PESO RANDOM AS CORES
    int lower = 0, upper = 100;
    pesos[0] = (rand() % (upper - lower + 1)) + lower;
    pesos[1] = (rand() % (upper - lower + 1)) + lower;
    pesos[2] = (rand() % (upper - lower + 1)) + lower;

	int path[9] = {-1,-1,-1,-1,-1,-1,-1}; // Caminho
	int colorPath[9] = {-1,-1,-1,-1,-1,-1,-1}; // Caminho das cores
	makeLetters(matriz, letter); // Cria as letras conforme a matriz
	printf("Matriz em numeros:\n");
	printMatrix(matriz); // Imprime a matriz
	line();
	printf("Matriz em RGB:\n");
	printMatrixAsRGB(matriz);
	line();
	printf("Pesos das cores:\n");
	printPesos(pesos);

	int adjacencia[9][9] = {
		//	A 			B			C 			D 			E 			F 			G 				H 			I
		{	IN,			peso(1),	IN,			peso(3),	IN,			IN,			IN, 			IN,	 		IN		}, // A
		{	peso(0),	IN,			peso(2),	IN,			peso(4),	IN,			IN, 			IN, 		IN		}, // B
		{	IN,			peso(1),	IN,			IN,			IN,			peso(5),	IN, 			IN, 		IN		}, // C
		{	peso(0),	IN,			IN,			IN,			peso(4),	IN,			peso(6), 		IN, 		IN		}, // D
		{	IN,			peso(1),	IN,			peso(3),	IN,			peso(5),	IN, 			peso(7), 	IN		}, // E
		{	IN,			IN,			peso(2),	IN,			peso(4),	IN,			IN, 			IN, 		peso(8)	}, // F
		{	IN,			IN,			IN,			peso(3),	IN,			IN,			IN, 			peso(7), 	IN		}, // G
		{	IN,			IN,			IN,			IN,			peso(4),	IN,			peso(6), 		IN, 		peso(8)	}, // H
		{	IN,			IN,			IN,			IN,			IN,			peso(5),	IN, 			peso(7), 	IN		}  // I
	};

	line();
	printf("Matriz da adjacencia (99 = INFINITO):\n");
	printAdj(adjacencia);
	line();
	printf("Resultado do Dijkstra:\n");
	printf("\n%d\n", dijkstra(adjacencia, 6, 2, path));
	line();
	printf("Caminho na matriz:\n");
	printPath(path);
	printf("\n");
	printf("Caminho de cores:\n");
	makeColorPath(path, colorPath);
	printColorPath(colorPath);
	printf("\n");
	printf("Resultado do backtracking: %d.\n", backtracking(matriz, colorPath, adjacencia, 2, 0, 1, NULL));
}

// Print the matrix
void printMatrix(int m[][3]) {
	int x;
	int y;

	for(x = 0 ; x < 3 ; x++) {
		for(y = 0 ; y < 3 ; y++){
			printf("%d     ", m[x][y]);
		}
		printf("\n");
	}
}

void printMatrixAsRGB(int m[][3]) {
	int x;
	int y;

	for(x = 0 ; x < 3 ; x++) {
		for(y = 0 ; y < 3 ; y++){
			printf("%c     ", letter[x*3+y]);
		}
		printf("\n");
	}
}

void printAdj(int m[][9]) {
	int x;
	int y;

	for(x = 0 ; x < 9 ; x++) {
		for(y = 0 ; y < 9 ; y++){
			printf("%d     ", m[x][y]);
		}
		printf("\n");
	}
}

int peso(int p){
	return pesos[matriz[(p/3)][p%3]];
}

int dijkstra(int cost[][N],int source,int target, int record_path[9])
{
	int dist[N],prev[N],selected[N]={0},i,m,min,start,d,j;
	char path[N];
	for(i=1;i< N;i++)
	{
		dist[i] = IN;
		prev[i] = -1;
	}
	
	start = source;
	selected[start]=1;
	dist[start] = 0;
	while(selected[target] ==0)
	{
		min = IN;
		m = 0;
		for(i=1;i< N;i++)
		{
			d = dist[start] +cost[start][i];
			if(d< dist[i]&&selected[i]==0)
			{
				dist[i] = d;
				prev[i] = start;
			}
			if(min>dist[i] && selected[i]==0)
			{
				min = dist[i];
				m = i;
			}
		}
		start = m;
		selected[start] = 1;
	}
	start = target;
	j = 0;
	while(start != -1)
	{
		record_path[j] = start;
		path[j++] = start+65;
		start = prev[start];
	}
	path[j]='\0';
	strrev(path);
	printf("%s", path);
	reverse_array(record_path, N);
	return dist[target];
}

char *strrev(char *str)
{
	char *p1, *p2;

	if (! str || ! *str)
		return str;
	for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
	{
		*p1 ^= *p2;
		*p2 ^= *p1;
		*p1 ^= *p2;
	}
	return str;
}

void printPath(int path[9]){
	int i;
	for(i=0;i<8;i++){
		if(path[i] == -1) break;
		if(i > 0) printf("-> ");
		printf("%d ", path[i]);
	}
}

void makeLetters(int m[3][3], char l[9]){
	int i, j;
	for(i=0; i<3;i++)
		for(j=0; j<3;j++)
			l[(i*3)+j] = (m[i][j] == R ? 'R' : (m[i][j] == G ? 'G' : 'B'));
}

void reverse_array(int *a, int n)
{
	int x;
	for(x = 0; x < n; x++) if(a[x] == -1) break;
	int i = x - 1;
	int j = 0;
	while(i > j)
	{
		int temp = a[i];
		a[i] = a[j];
		a[j] = temp;
		i--;
		j++;
	}
}

void makeColorPath(int p[9], int c[9]){
	int i;
	for(i=0;i<9;i++){
		if(p[i] == -1) break;
		c[i] = matriz[p[i]/3][p[i]%3];
	}
}

void printColorPath(int c[9]){
	int i;
	for(i=0;i<9;i++){
		if(c[i] == -1) break;
		if(i > 0) printf("-> ");
		printf("%c ", (c[i] == R ? 'R' : (c[i] == G ? 'G' : 'B')));
	}
}

/**
*
* @param n inicio do path
* @param i linha atual
* @param j coluna atual
*/
int backtracking(int m[3][3], int p[9], int adj[9][9], int i, int j, int n, int* visited){

	int x,local,r;
	if(p[n] == -1) return 1; // Finalizou

	if(visited == NULL){ // Cria array de visitado
        visited = (int*)malloc(sizeof(int)*9);
        for(x=0;x<9;x++) visited[x] = 0;
	}

	for(x=0;x<9;x++){
		local = (i*3)+j; // Local atual do robô
		printf("Origem:%d\nTentativa: %d\nResultado: %d\n", adj[local][x], m[x/3][x%3], p[n]);

		// m[x/3][x%3] -> ESSE CALCULO DEVE SER SUBSTITUIDO PELA COR IDENTIFICADA PELO ROBO
		if(
            adj[local][x] != IN && // VERIFICA SE EXISTE ADJACENCIA
            m[x/3][x%3] == p[n] &&  // COMPARA A COR DA MATRIZ COM A COR QUE PRECISA ANDAR
            visited[local] == 0){ // Se a adjacência do local atual existe e se é a cor que eu quero e não foi visitado

			/* MOVE O ROBÔ */
			/*	  TODO	   */
			printf("Movendo para [%d][%d]\n---\n", x/3, x%3);

            visited[local] = 1;
			r = backtracking(m, p, adj, x/3, x%3, n+1, visited);

			if(r == 1){
                return 1;
			}
            visited[local] = 0;

			/* MOVE O ROBÔ PARA O LOCAL ANTERIOR */
			/*		TODO						 */
			printf("Movendo de volta para [%d][%d]\n---\n", i, j);
		}else{
            printf("Nao eh adjacente, ou da cor [%c] ou ja foi visitado [%d]\n---\n", letter[local], visited[local]);
		}

	}

	return 0;

}

// Swap in memory
void swap(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

// Shuffle the array
void randomize(int arr[], int n) {
    srand(time(NULL));
    int i;
    for(i = n-1; i > 0; i--) {
        int j = rand() % (i+1);
        swap(&arr[i], &arr[j]);
    }
}
