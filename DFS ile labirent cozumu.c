/* Emircan K�REZ - Ch3rry */
/* Last Update: 02/06/2022 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#define MAX 255

/* Kullan�lan Matris TEK Bo�lukludur
+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
|     |     |                             |
+ +-+ + +-+-+ +-+-+-+-+-+-+-+-+-+ +-+-+-+ +
|   |   |     |   |   |         |   |     |
+-+ + +-+ +-+-+ + + + + +-+-+-+ +-+ + +-+-+
|   | |   |     |   | |     | | |   |   | |
+ +-+ + +-+ +-+-+-+-+-+-+ +-+-+ +-+-+-+ +-+
| |   | | | |         | | |   |     | |   |
+ +-+-+ +-+ + +-+-+-+ + + + + +-+ + +-+-+ +
|     | |   |   |   | | |   |   | |       |
+ +-+ + + +-+-+ +-+ + +-+-+-+-+ + +-+-+-+-+
| |   | |     |   | |         | |       | |
+ + +-+ +-+-+-+-+ + +-+-+-+-+ + +-+ +-+ +-+
| | |             |         | |   | | |   |
+ + + +-+-+-+-+-+-+-+ +-+-+ + +-+ + + +-+ +
| |     |   |             | |   | |   |   |
+ +-+-+ + + + +-+ +-+-+ +-+-+ + + +-+ + +-+
| |     | | |   |     | |     | | | | |   |
+ + +-+-+ + +-+ + +-+-+-+ +-+-+-+ + + +-+ +
| | |     |   | |   |   |       | |   | | |
+ + + +-+-+-+-+ +-+ + +-+-+-+-+ + +-+-+-+ +
| |               | |   | | |   |     | | |
+ + +-+-+-+-+-+-+-+-+-+ +-+-+ +-+-+-+ +-+ +
| |     |   | |     | | | |         | |   |
+-+-+-+ + + + + +-+ +-+ +-+ +-+ +-+-+ + +-+
|   |   | | | | | |     | | |   |   | |   |
+ +-+ +-+ + + + +-+-+-+-+-+ + +-+ + + +-+-+
|       | | | |           | | |   | |     |
+-+-+-+ +-+ + +-+-+-+-+-+ + + +-+-+-+-+-+ +
|b          |           |   |         |  c|
+---+-+-+-+-+---+-+-+-+-+-+-+-+-+-+-+-+-+-+
*/

//Dosyadan okudu�um sat�rlar� saklad���m linkli liste
typedef struct lineNode{
	char line[100];
	struct lineNode *next;
}LINES; 

//x ve y indislerini birlikte tutmak i�in olu�turdu�um struct
typedef struct indices{
	int x, y;
}INDICES;

//Ziyaret etti�im d���mleri tuttu�um linkli liste 
typedef struct visitedIndices{
	INDICES* indices;
	struct visitedIndices *next;
}VISITED;

//DFS i�in kulland���m stack yap�s� - �stteki linkli listede INDICES* tuttu�um i�in kolayl�k olsun diye burada da �yle tutuyorum
typedef struct {
	INDICES* item[MAX]; //0 .. MAX-1
	int top;
}STACK;

//Function Prototypes (18)
void initialize(STACK *s);
void push(STACK *s, INDICES* indices);
int isEmpty(STACK *s);
INDICES* pop(STACK *s);
INDICES* top(STACK *s);
int readFromFile(LINES **head, char *fileName, int *mazeHeight);
LINES *createLineNode(char *line);
void addLineNode(LINES **head, char *line);
int findWidth(char *line);
char **createMaze(int mazeHeight, int mazeWidth);
void initializeMaze(LINES *head, char **maze, int mazeHeight, int mazeWidth, int *startingX, int *startingY);
void findMazeExit(VISITED **visitedhead, int startingX, int startingY, STACK *s, char **maze, int mazeHeight, int mazeWidth);
void printMaze(char **maze, int mazeHeight, int mazeWidth, int score);
INDICES* createIndices(int x, int y);
VISITED* createVisitedIndices(INDICES *indices);
void addVisitedNode(VISITED **head, VISITED* newVisited);
void deleteVisitedNode(VISITED **head);
void deallocateMaze(char **maze, int mazeHeight, int mazeWidth);

int main(){
	srand(time(NULL));
	LINES *lineHead = NULL;
	VISITED *visitedHead = NULL;
	char fileName[20];
	char **maze; //labirenti saklad���m matris yap�m
	STACK s;
	int startingX, startingY, mazeWidth, mazeHeight;
	initialize(&s); //Stack'i initialize eder. Stack bo� ise top de�erimiz -1'i g�sterir


	printf("Labirenti okumak istediginiz dosyayi uzantisiyla beraber yaziniz: ");
	scanf("%s", fileName);
	
	if(!readFromFile(&lineHead, fileName, &mazeHeight)){
		printf("Dosya okunamadigi icin program kapatilmistir.");
		return -1;
	}
	
	mazeWidth = findWidth(lineHead->line);
	maze = createMaze(mazeHeight, mazeWidth);
	initializeMaze(lineHead, maze, mazeHeight, mazeWidth, &startingX, &startingY);
	findMazeExit(&visitedHead, startingX, startingY, &s, maze, mazeHeight, mazeWidth);
	
	/* Program� kapatmadan �nce silme i�lemleri yapar */
	
	//Stack'i bo�alt�r
	while(!isEmpty(&s)){
		INDICES *tmp = pop(&s);
	}
	
	//Linkli listelerin head'lerini s�f�rlar
	visitedHead = NULL;
	lineHead = NULL;
	
	//Olu�turulan labirenti deallocate eder
	deallocateMaze(maze, mazeHeight, mazeWidth);
	return 0;
}

/* STACK Fonksiyonlar� */
void initialize(STACK *s){
	s->top = -1;
}

//stack'i yeteri kadar b�y�k ayarlay�p dolu olup olmad���na bakmad�m
void push(STACK *s, INDICES* indices){ 
	s->item[++s->top] = indices; //�nce artt�r yeni yeri g�stersin, ondan sonra ekle 
}

int isEmpty(STACK *s){
	return s->top == -1 ? 1 : 0;
}

INDICES* pop(STACK *s){ //Bo� olup olmad���na bakmad�m ��nk� DFS'te stack bo� olmad��� s�rece pop yap�yoruz
	return s->item[s->top--]; //toptaki de�eri al, sonra bir azalt
}

INDICES* top(STACK *s){
	return s->item[s->top];
}

//Dosyadan sat�r sat�r okuyup, onlar� LINES linkli listesine kaydeder ve labirentin y�ksekli�ini bulur.
int readFromFile(LINES **head, char *fileName, int *mazeHeight){
	FILE *fPtr;
	char line[100];
	
	if((fPtr = fopen(fileName, "r")) == NULL){
		printf("Dosya acilamadi!!\n");
		return 0;
	}
	

	while(fgets(line, 100, fPtr) != NULL){
		addLineNode(head, line);
		(*mazeHeight)++;
	}

	fclose(fPtr);
	return 1;
}

//Dosyadan sat�r sat�r okurken elde etti�imiz line'lar� saklamak i�in node olu�turur
LINES *createLineNode(char *line){
	LINES *newLineNode = (LINES *)malloc(sizeof(LINES));
	strcpy(newLineNode->line, line);
	newLineNode->next = NULL;
	return newLineNode;
}

//Sat�rlar� i�inde tutan bir node olu�turur ve linkli listeye sondan ekler
void addLineNode(LINES **head, char *line){
	LINES *newLineNode = createLineNode(line);
	if(*head == NULL){
		*head = newLineNode;
	}else{
		LINES *tmp = *head;
		while(tmp->next != NULL){
			tmp = tmp->next;
		}
		tmp->next = newLineNode;
	}
}

//Labirentin geni�li�i bir sat�r�n uzunlu�una bakarak belirler
int findWidth(char *line){
	int i = 0;
	while(i < 100 && line[i] != '\n'){
		i++;
	}
	return i;
}

//Labirentin y�ksekli�i ve geni�li�ine g�re bir matris olu�turup onu geri d�nd�r�r
char **createMaze(int mazeHeight, int mazeWidth){
	char **maze;
	int i;
	maze = (char **)malloc(mazeHeight * sizeof(char *));
	for(i = 0; i < mazeHeight; i++){
		maze[i] = (char *)malloc(mazeWidth * sizeof(char));
	}
	return maze;
}

//Daha �nceden dosyadan okudu�um ve LINES linkli listesinden sat�rlar� teker teker alarak matrisi initialize eder. Ayn� zamanda ba�lang�� noktas�n�n indislerini bulur
void initializeMaze(LINES *head, char **maze, int mazeHeight, int mazeWidth, int *startingX, int *startingY){
	int i, j, randomX, randomY;
	LINES *tmp = head;
	for(i = 0; i < mazeHeight; i++){
		for(j = 0; j < mazeWidth; j++){
			maze[i][j] = tmp->line[j];
			if(maze[i][j] == 'b'){
				*startingX = i;
				*startingY = j;
			}
		}
		tmp = tmp->next;
	}
	
	//10 adet elma eklemek istedim
	i = 0;
	while(i < 10){
		randomX = rand() % mazeHeight;
		randomY = rand() % mazeWidth;
		if(maze[randomX][randomY] == ' '){
			maze[randomX][randomY] = 'O';
			i++;
		} 
	}
}

//
void findMazeExit(VISITED **visitedHead, int startingX, int startingY, STACK *s, char **maze, int mazeHeight, int mazeWidth){
	//sol - a�a��- sa� - yukar� �eklinde kontrol eder. Bu �ekilde stackte yukar� - sa� - a�a�� - sol yap�s� kurulur
	int directions[][4] = {{0, 1, 0, -1}, {-1, 0, 1, 0}};  
	//int directions[][4] = {{-1, 0, 0, 1}, {0, -1, 1, 0}}; //a�a�� - sa� - yukar� - sol pdf'teki �rnek i�in
	int i, x, y, counter, score = 0;
	INDICES *tmp = createIndices(startingX, startingY);
	INDICES *current;
	
	//Ba�lang�� noktas�n� VISITED linkli listesine koyuyorum ve kom�ular�n� stack'e at�yorum. Ondan sonra DFS uyguluyorum
	addVisitedNode(visitedHead, createVisitedIndices(tmp));
	for(i = 0; i < 4; i++){
		x = startingX + directions[0][i];
		y = startingY + directions[1][i];
		
		if(maze[x][y] == ' ' || maze[x][y] == 'O'){ //bo� yerse ya da elmaysa stack'e ekle
			tmp = createIndices(x, y);
			addVisitedNode(visitedHead, createVisitedIndices(tmp));
			push(s, tmp);
		}
	}
	
	printMaze(maze, mazeHeight, mazeWidth, score);
	while(!isEmpty(s)){
		tmp = pop(s);
		if(maze[tmp->x][tmp->y] == 'O'){ //elmay� yerse +10 puan
			score += 10;
		}
		maze[tmp->x][tmp->y] = '*';
		printMaze(maze, mazeHeight, mazeWidth, score);
		counter = 0;
		i = 0;
		while(i < 4){
			x = tmp->x + directions[0][i];
			y = tmp->y + directions[1][i];
			if(maze[x][y] == 'c'){
				printf("\nLabirentten cikis puaniniz: %d", score);
				current = createIndices(x, y);
				addVisitedNode(visitedHead, createVisitedIndices(current));
				return;
			}else if(maze[x][y] == ' ' || maze[x][y] == 'O'){ //e�er bo�sa
				current = createIndices(x, y);
				addVisitedNode(visitedHead, createVisitedIndices(current));
				push(s, current);
				counter++;
			}
			i++;
		}
		

		if(counter == 0){ 
		//counter == 0 ise demekki hi� bir yere gidilememi�. Bu da demek oluyor ki ��kmaz sokak. O y�zden skor'dan 5 ��kar�p, stack'teki son de�er'e kadar olan *'lar� kald�r�yorum.
		//Bu �ekilde gitti�im yoldan geri d�nm�� oluyorum. Bunun i�in de VISITED linkli listesinden yararlan�yorum
			score -= 5;
			current = top(s);
			while((*visitedHead)->indices->x != current->x || (*visitedHead)->indices->y != current->y){
				maze[(*visitedHead)->indices->x][(*visitedHead)->indices->y] = ' '; //geri geliyorum
				printMaze(maze, mazeHeight, mazeWidth, score);
				deleteVisitedNode(visitedHead); //Ba�tan ekledi�im linkli listeden silerek ilgili noktaya gelmeye �al���yorum
			}
		}
	}
}

void printMaze(char **maze, int mazeHeight, int mazeWidth, int score){
	int i, j;
	printf("Anlik Puaniniz: %d\n", score);
	for(i = 0; i < mazeHeight; i++){
		for(j = 0; j < mazeWidth; j++){
			printf("%c", maze[i][j]);
		}
		printf("\n");
	}
	printf("\n");
	usleep(500000); 
}

//x ve y de�erlerine g�re INDICES olu�turur
INDICES* createIndices(int x, int y){
	INDICES* newIndices = (INDICES*)malloc(sizeof(INDICES));
	newIndices->x = x;
	newIndices->y = y;
	return newIndices;
}

//Ziyaret etti�imiz noktalar�n kordinat bilgilerini i�eren node olu�turur
VISITED* createVisitedIndices(INDICES *indices){
	VISITED* newVisited = (VISITED*)malloc(sizeof(VISITED));
	newVisited->indices = indices;
	newVisited->next = NULL;
	return newVisited;
}

//�lgili node'u linkli listeye ba�tan ekler ��nk� silme i�lemi yapacaksak en son ekledi�imizi silece�imiz i�in en sona kadar s�rekli gitmektense ba�a eklemek daha mant�kl�
void addVisitedNode(VISITED **head, VISITED *newVisited){
	newVisited->next = *head;
	*head = newVisited;
}

//Ba�tan silme yapar
void deleteVisitedNode(VISITED **head){
	VISITED *tmp = *head;
	*head = (*head)->next;
	free(tmp);
}

//Dynamic olarak olu�turulan matrisi deallocate eder
void deallocateMaze(char **maze, int mazeHeight, int mazeWidth){
	int i;
	for(i = 0; i < mazeHeight; i++){
		free(maze[i]);
	}
	free(maze);
}
