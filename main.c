
#include "Bibliotecas/library.h"

//Inicialização do programa
int init = 1;

//Indica Game Over(Tela de fim de jogo)
int theEnd = 0;

//Timer (FPS)
int timer =24;

//Movimento:
int MovimentarOn=0; //Define se o movimento esta ativo (Tecla C) (TESTE)
int ColisaoObjeto=0; //(IMPLEMENTAR)


//Pulo:
int PularOn = 0;
float oldpulo;
float pulo = 5; //o quanto eu vou pra cima no y (variação do y)
float incPulo = 0.5; //incremento no y (variação da variação)



//Spawn:
//OBS: Atentar à função mudaCorrida que pega valores determinados
int rSpawn = 50; //Random Spawn (Determina o limite dos numeros randômicos)
float dSpawn = 100; //Distância do Spawn (Determina a distância minima para o Spawn)

float velocidade = 3;


//Nivel, Posição Y do chão
int groundlevel =30;

float aspect; //Ainda não mexi, para arrumar o AspectRatio(TESTE).

//Declarar variaveis to tipo Struct Quadrado
quadrado q1,q2,q3,q4;





void verificaColisao(quadrado *a){
  if(q1.x+q1.tx >= a->x && q1.y<=a->y+a->ty){
    MovimentarOn = 0; //Mudar de Movimentar para Colisao
    theEnd = 1;

  }

  if(q1.y<= a->y+a->ty && q1.x+q1.tx>= a->x){
    MovimentarOn = 0;
    theEnd = 1;
  }

}


//Essa função retorna a posição X do objeto mais distante( Mais a Direita do jogador), util para as funções Respawn
float objMaisDistante(){
    float n =q2.x;
    quadrado *i = &(q2);
    do{
        if( i->x > n)
            n = i->x;
        i = i->proxquadrado;
    }
    while ( (i) != NULL);
    //while( (i)!= (q4.proxquadrado) );

return n;
}



//Respawn do Objeto
void eventoRespawn(quadrado *a){
    //Primeiramente: Descobre a posição do objeto mais distante;
    float posMaior = objMaisDistante();
    //Atribui um valor da variação para ser somado à posição;
    float valorVariacao = rand()%rSpawn+dSpawn;
    //Determina a nova Posição do Quadrado;
    posQuadrado(a, posMaior+valorVariacao ,groundlevel);


}

// Muda o tipo de corrida(mexendo nos valores rSpawn e dSpawn)- X: Valor recebido pelo glutTimerFunc( Tempo para chamar a função)
void mudaCorrida(int X){
    printf("Mudando tipo de corrida:\n");
    if(dSpawn = 100){
        X*=2; //Teste dobrar o tempo para chamar a função
        dSpawn = 50;
        rSpawn = 300;
    }
    else{
        X/=2; //Teste - dividir o tempo para chamar a função
        dSpawn = 100;
        rSpawn = 60;
    }
    glutTimerFunc(X,mudaCorrida,X);
}








//Reposicionar o objeto caso ele tenha saido de Cena (para os quadrados "inimigos"),
//Para um mundo começando no eixo X.
void verificaPosicao(quadrado *a){
//Quando o quadrado percorrer até o final do mundo, (tendo percorrido todo seu tamanho);
  if( a->x < - (a->tx) )
    eventoRespawn(a);

}



//             (quadrado, velocidade)
void movimenta(){
  q2.x-=velocidade;
  q3.x-=velocidade;
  q4.x-=velocidade;

}


void puloBackup(){
  oldpulo = pulo;
}


void resetPulo(){
  pulo = oldpulo;
  PularOn = 0;
}


void pular(){
    q1.y+=pulo;
    pulo-=incPulo;

//Verifica se o jogador chegou na altura do chão para parar o pulo
    if(q1.y<=groundlevel)
        resetPulo();

}





void atualiza(int timer){
//MovimentarOn para controlar se o movimento está ativo ou não
  if (MovimentarOn==1){
      movimenta();

//PularOn para controlar se o pulo está ativo ou não
  if(PularOn==1)
    pular();
  }

  glutPostRedisplay();
  glutTimerFunc(timer,atualiza,timer);

}

void desenhaQuadrado(quadrado* a){
    glColor3f(a->r,a->g,a->b);


    //Para nao verificar a colisão no próprio q1(MODIFICAR)
    // if(a!=&q1)
    // verificaColisao(a);
    verificaPosicao(a);

    glBegin(GL_TRIANGLE_FAN);

      glVertex2d(a->x , a->y);
      glVertex2d(a->x + a->tx, a->y);
      glVertex2d(a->x + a->tx, a->y + a->ty);
      glVertex2d(a->x , a->y + a->ty);

    glEnd();


}



void desenhaCena(){
  glClear(GL_COLOR_BUFFER_BIT);

  glEnable(GL_BLEND); // teste pra opacidade
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  desenhaQuadrado(&q1);
  desenhaQuadrado(&q2);
     desenhaQuadrado(&q3);
     desenhaQuadrado(&q4);

//Verifica o se o jogador perdeu ou não, indo pro tela do fim (GAME-OVER) (MODIFICAR)
 if (theEnd==1)
     telaFim();

  glutSwapBuffers();

}


void redimensiona(int width, int height){
  aspect = width/height;
  glViewport(0,0,width,height);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0,100*aspect,0,100*aspect,-1,1);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();


}


void initQuadrado(){
//Para inicializar o quadrado somente uma vez. Podendo atualizar ele sem preocupações
  if (init==1){
    puloBackup(); //Guarda o valor inicial de pulo
  init++;
  tamQuadrado(&q1,10,15);
  posQuadrado(&q1,0,groundlevel);
  corQuadrado(&q1,.8,.1,.1);

  nomeQuadrado(&q2,"q2");
  tamQuadrado(&q2,10,10);
  posQuadrado(&q2,90,groundlevel);
  corQuadrado(&q2,.1,.3,.9);
  linkQuadrado(&q2,&q3);

  nomeQuadrado(&q3,"q3");
  tamQuadrado(&q3,10,10);
  posQuadrado(&q3,180,groundlevel);
  corQuadrado(&q3,.8,.5,.1);
  // corQuadrado(&q3,.1,.3,.9);
  // linkEnd(&q3);
  linkQuadrado(&q3,&q4);

  nomeQuadrado(&q4,"q4");
  tamQuadrado(&q4,10,10);
  posQuadrado(&q4,270,groundlevel);
  corQuadrado(&q4,.8,.3,.9);
  // corQuadrado(&q4,.1,.3,.9);
  // linkEnd(&q4);
  linkQuadrado(&q4,NULL);

  }
}


void resetAll(){    //Da para colocar um variavel para avisar se volta em movimento ou nao
  theEnd = 0;
  resetPulo();
  init = 1;
  MovimentarOn = 0;
  initQuadrado();
  glutPostRedisplay();
}




void keyboard(unsigned char key, int x, int y){

  switch (key){
    case 27:
      exit(0);
      break;

    case 'c':
    case 'C':
        verificarLiberarMovimento(&theEnd, &MovimentarOn );
    break;

      // Tela de Fim de Jogo
    case 13: //Enter
        if(theEnd==1){
            resetAll();
        }
    break;

    //Reset em qualquer momento
    case 'r':
    case 'R':
      resetAll();
    break;

    case 32: //Barra de Espaço
    if(MovimentarOn){
      if(PularOn==0)
        PularOn=1;
    }
      break;

    default:
      break;


  }

}

void inicializa(){
  glClearColor(.3,.3,.3,0);
}


int main(int argc, char**argv){
  //Inicializar o glut
  glutInit(&argc,argv);

  //Configurações de inicialização do glut
  glutInitContextVersion(1,1);
  glutInitContextProfile(GLUT_COMPATIBILITY_PROFILE);

  //Configurações da Janela
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
  glutInitWindowSize(400,400);
  glutInitWindowPosition(100,100);

  //Abrir(criar) Janela
  glutCreateWindow("Teste");
  inicializa();

  srand(time(NULL));

  //Callbacks
  initQuadrado();
  glutDisplayFunc(desenhaCena);
  glutReshapeFunc(redimensiona);
  glutKeyboardFunc(keyboard);
  glutTimerFunc(timer,atualiza,timer);
  glutTimerFunc(6000,mudaCorrida,6000); //Teste...

  glutMainLoop();
  return 0;

}
