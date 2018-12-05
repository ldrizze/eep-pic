/*
  Projeto Interdisciplinar de Computação
  Carrinho para matriz de cores
  Prof. Odahyr
*/
#include <AFMotor.h>

#define MOTOR_1_PIN 1
#define MOTOR_2_PIN 3

#define ROTATE_TIME 650
#define IDENTIFY_COLOR_TIME 460
#define MOVE_CELL_TIME 460

#define DELAY_STEP_TIME 1000

const int s0 = A0;
const int s1 = A1;
const int s2 = A2;
const int s3 = A3;
const int out = A5;

enum dir{NONE,UP,RIGHT,DOWN,LEFT}; // Enumerador das direções
enum colors{R,G,B}; // Enumerador de cores

int adjacencia[9][9] = {
		//	A 			B			    C 			D 			  E 			F 			G 				H 			I
		{	NONE,		  RIGHT,	  NONE,		DOWN,	    NONE,		NONE,		NONE, 		NONE,		NONE		}, // A
		{	/*LEFT*/NONE,	    NONE,		  RIGHT,  NONE,			DOWN,   NONE,		NONE,			NONE,		NONE		}, // B
		{	NONE,			/*LEFT*/NONE,     NONE,		NONE,	    NONE,		DOWN,   NONE,			NONE,		NONE		}, // C
		{	UP,	      NONE,		  NONE,		NONE,	    RIGHT,	NONE,		DOWN, 		NONE,		NONE		}, // D
		{	NONE,			UP,   	  NONE,		/*LEFT*/NONE,	    NONE,		RIGHT,  NONE,			DOWN, 	NONE		}, // E
		{	NONE,		  NONE,		  UP,	    NONE,			/*LEFT*/NONE,   NONE,		NONE,			NONE, 	DOWN    }, // F
		{	NONE,		  NONE,		  NONE,		UP,	      NONE,		NONE,		NONE, 		RIGHT,  NONE		}, // G
		{	NONE,		  NONE,		  NONE,		NONE,			UP,	    NONE,		/*LEFT*/NONE, 		NONE, 	RIGHT   }, // H
		{	NONE,		  NONE, 		NONE,		NONE,			NONE,	  UP,	    NONE, 		/*LEFT*/NONE,   NONE		}  // I
}; // Matriz da adjacência

int posicao_atual = 6; // Posicao G da matriz [3][1]
int olhando_para  = UP; // O carrinho inicial olhando para "cima"

int caminho[9]  = {G, R, G, R, G, -1, -1, -1, -1};
int finalizado  = 0;
int resultado   = 0;

char msg[200];
char color_letter[3] = {'R', 'G', 'B'};

AF_DCMotor left_motor(MOTOR_1_PIN);
AF_DCMotor right_motor(MOTOR_2_PIN);

void setup() {
  left_motor.setSpeed(255);
  right_motor.setSpeed(255);

  pinMode(s0, OUTPUT);
  pinMode(s1, OUTPUT);
  pinMode(s2, OUTPUT);
  pinMode(s3, OUTPUT);
  pinMode(out, INPUT);
  Serial.begin(9600);
  digitalWrite(s0, HIGH);
  digitalWrite(s1, LOW);
}

void loop() {
  if(finalizado) return; // Se finalizado não faz mais nada
  Serial.write("Iniciando\n");
  delay(5000);
  resultado = backtracking(caminho, adjacencia, 2, 0, 1, NULL);
  finalizado = 1;
  Serial.write("Resultado: ");
  Serial.print(resultado, DEC);
  Serial.write("\n");
  Serial.write("Finalizado\n");
}

void rotate(int to){
  
  int motor_1, motor_2, times = 0, i;
  
  switch(olhando_para){
    case UP: // Se ele estiver olhando para cima
      if(to == LEFT){ // Girar para a esquerda
        motor_1 = FORWARD;
        motor_2 = BACKWARD;
        times = 1;
      }else if(to == RIGHT){ // Girar para a direita
        motor_1 = BACKWARD;
        motor_2 = FORWARD;
        times   = 1;
      }else if(to == DOWN){ // Girar para baixo
        motor_1 = BACKWARD;
        motor_2 = FORWARD;
        times   = 2;
      }
    break;
    
    case LEFT: // Se estiver olhando para esquerda
    if(to == UP){ // Girar para a esquerda
        motor_1 = BACKWARD;
        motor_2 = FORWARD;
        times = 1;
      }else if(to == RIGHT){ // Girar para direita
        motor_1 = FORWARD;
        motor_2 = BACKWARD;
        times   = 2;
      }else if(to == DOWN){ // Girar para baixo
        motor_1 = FORWARD;
        motor_2 = BACKWARD;
        times   = 1;
      }
    break;
    
    case RIGHT: // Se estiver olhando para a direita
    if(to == UP){ // Girar para cima
        motor_1 = FORWARD;
        motor_2 = BACKWARD;
        times = 1;
      }else if(to == LEFT){ // Girar para esquerda
        motor_1 = FORWARD;
        motor_2 = BACKWARD;
        times   = 2;
      }else if(to == DOWN){ // Girar para baixo
        motor_1 = BACKWARD;
        motor_2 = FORWARD;
        times   = 1;
      }
    break;
    
    case DOWN:
    if(to == UP){ // Girar para cima
        motor_1 = BACKWARD;
        motor_2 = FORWARD;
        times = 2;
      }else if(to == LEFT){ // Girar para esquerda
        motor_1 = FORWARD;
        motor_2 = BACKWARD;
        times   = 1;
      }else if(to == RIGHT){ // Girar para baixo
        motor_1 = BACKWARD;
        motor_2 = FORWARD;
        times   = 1;
      }
    break;
  }
  
  for(i=0;i<times;i++) applyRotate(motor_1, motor_2); 
  olhando_para = to;
}

void applyRotate(int motor_1, int motor_2){
  left_motor.run(motor_1);
  right_motor.run(motor_2);
  delay(ROTATE_TIME);
  left_motor.run(RELEASE);
  right_motor.run(RELEASE);

  Serial.write("Rotacionando\n");
  delay(DELAY_STEP_TIME);
}

int identifyColor(int cell){
  int readedColor;
  Serial.write("Identificando cor\n");
  rotate(cell);
  left_motor.run(FORWARD);
  right_motor.run(FORWARD);
  delay(IDENTIFY_COLOR_TIME);
  left_motor.run(RELEASE);
  right_motor.run(RELEASE);
  
  readedColor = readColor();

  if(readedColor != -1) sprintf(msg, "Cor identificada: %c\n", color_letter[readedColor]);
  else sprintf(msg, "Cor nao identificada!\n");
  Serial.write(msg);
  
  delay(DELAY_STEP_TIME);
  return readedColor;
}

void backFromIdentify(){
  left_motor.run(BACKWARD);
  right_motor.run(BACKWARD);
  delay(IDENTIFY_COLOR_TIME);
  left_motor.run(RELEASE);
  right_motor.run(RELEASE);

  Serial.write("Voltando da identificacao de cor\n");
  delay(DELAY_STEP_TIME);
}

void moveCell(){
  left_motor.run(FORWARD);
  right_motor.run(FORWARD);
  delay(MOVE_CELL_TIME);
  left_motor.run(RELEASE);
  right_motor.run(RELEASE);

  Serial.write("Movendo celula\n");
  delay(DELAY_STEP_TIME);
}

void backCell(){
  left_motor.run(BACKWARD);
  right_motor.run(BACKWARD);
  delay(MOVE_CELL_TIME);
  left_motor.run(RELEASE);
  right_motor.run(RELEASE);

  Serial.write("Voltando celula\n");
  
  delay(DELAY_STEP_TIME);
}

int readColor()
{
  int red = 0, blue = 0, green = 0;
  //Rotina que le o valor das cores
  digitalWrite(s2, LOW);
  digitalWrite(s3, LOW);
  //count OUT, pRed, RED
  red = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s3, HIGH);
  //count OUT, pBLUE, BLUE
  blue = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);
  digitalWrite(s2, HIGH);
  //count OUT, pGreen, GREEN
  green = pulseIn(out, digitalRead(out) == HIGH ? LOW : HIGH);

  Serial.print("Vermelho :");
  Serial.print(red, DEC);
  Serial.print(" Verde : ");
  Serial.print(green, DEC);
  Serial.print(" Azul : ");
  Serial.print(blue, DEC);
  Serial.println();
  
  if (red < blue && red < green && red < 150){
    return R;
  }
  
  if (green < red && green < blue && green < 150){
    return G;
  }
  
  if (blue < red && blue < green && blue < 150){ // Se for a cor azul
    return B;
  }
  
  return -1;
}


/**
*
* @param n inicio do path
* @param i linha atual
* @param j coluna atual
*/
int backtracking(int p[9], int adj[9][9], int i, int j, int n, int* visited){

	int x,local,r,color,visited_local;
	if(p[n] == -1) return 1; // Finalizou

	if(visited == NULL){ // Cria array de visitado
        visited = (int*)malloc(sizeof(int)*9);
        for(x=0;x<9;x++) visited[x] = 0;
        visited[6] = 1;
	}

	for(x=0;x<9;x++){
		local = (i*3)+j; // Local atual do robô
		//printf("Origem:%d\nTentativa: %d\nResultado: %d\n", adj[local][x], m[x/3][x%3], p[n]);		

    // Se a adjacência do local atual existe e se é a cor que eu quero e não foi visitado
    if(adj[local][x] == NONE || visited[x] == 1) continue; // VERIFICA SE EXISTE ADJACENCIA
		
		// m[x/3][x%3] -> ESSE CALCULO DEVE SER SUBSTITUIDO PELA COR IDENTIFICADA PELO ROBO
		if(
        identifyColor(adj[local][x]) == p[n]  // COMPARA A COR DA MATRIZ COM A COR QUE PRECISA ANDAR
        ){ 

			/* MOVE O ROBÔ */
			/*	  TODO	   */
			sprintf(msg, "Movendo para [%d][%d]\n", x/3, x%3);
      Serial.write(msg);

      sprintf(msg, "Visitando local: %d\n", x);
      Serial.write(msg);
      Serial.write("---\n");
      moveCell();
      visited[x] = 1;

      //if(x == 1) visited[0] = 1; // Se está no local[1], então marque o local[0] como visitado para evitar tilt
      //if(x == 5) visited[4] = 1; // Se está no local[5], então marque o local[4] como visitado para evitar tilt
      
			r = backtracking(p, adj, x/3, x%3, n+1, visited);

			if(r == 1){
        return 1;
			}
      
      visited[local] = 0;

			/* MOVE O ROBÔ PARA O LOCAL ANTERIOR */
			/*		TODO						 */
			sprintf(msg, "Movendo de volta para [%d][%d]\n---\n", i, j);
      Serial.write(msg);
			rotate(adj[local][x]);
			moveCell();
		}else{
      backFromIdentify();
		}

	}

	return 0;

}
