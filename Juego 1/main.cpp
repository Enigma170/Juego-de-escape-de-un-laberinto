//Autor de esta version: Daniel Josue Vallejo Coss y Hector Gustavo Hernandez Reyes
//Fecha de ultima modificacion 3/11/2019 //Es una version de mi editor de sprites con modificaciones, me parecio mas comodo y funcional
//26/11/2019 Solo falta a�adir unas cuantas funcionalidades
//2/12/2019 Termine el juego :3 //Quedo Hermoso, supero mis expectativas

#include <iostream>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <winbgim.h>
#define N 15//Tama�o de casilla en pixeles
#define TAM 2//de cuanto se imprimira cada pixel de una casilla en pantalla por pixeles :v
#define R 20//numero de renglones del juego completo
#define C 30//numero de columnas del juego completo
#define NS 20//numero de archivos de texto de sprites
#define NE 7//Numero de enemigos por nivel

using namespace std;

//Estructura de lista de casillas del juego
typedef struct nodoC
{
    //Apuntadores de las casillas siguientes y anteriores en dos dimensiones
    struct nodoC *up,*down;
    struct nodoC *left,*right;

    //Variable que dice el tipo de imagen que se va a cargar;
    int tipo;//el tipo de casilla, para ser precisos el numero en el arreglo tridimensional con los datos sobre la imagen que debe cargarse (n-1)
    char point;
    bool luz;
    int vel;
}*CAS;
//funcion que contiene los datos sobre un personaje, ya sea el jugador o un enemigo
typedef struct personaje{
    int vida;//La vida en enteros
    CAS pos;//direccion de memoria de la casilla en donde esta
    int tipo;//que tipo de casilla es?
    int piso;//el tipo de casilla en donde esta antes de realizar un movimiento
    bool llave;
}PSJ;

void pideNombre(char Nombre[20]){
    system("cls");
    printf("Escriba su nombre para preservarlo en los records:\n");
    scanf("%s",Nombre);
}

//Crea los botones que se usaran de un alto de 50p y largo de 100p, texto incluido
void boton(int x,int y, char *texto){
    int alto=50;//define el alto en y del boton a crearse
    int ancho=140;//define el ancho  en x del boton a crearse
    setcolor(3);//define el color de las impresiones siguientes
    rectangle(x,y,x+ancho,y+alto);//crea el boton en s�
    settextstyle(0,0,2);//define los parametros del texto a imprimirse
    outtextxy(x+20,y+20,texto);//imprime el texto dentro del boton
}

//Funcion para inicializar las casillas
void iniCasillas(CAS *cab){
    *cab=NULL;
}

//Funcion para crear una casilla y ponerla en NULL en todos los sus apuntadores regresa un 1 si pudo asignar memoria y 0 si no
int creaCasilla(CAS*nuevo,int tipo)
{
    int res=0;

    *nuevo=(CAS)malloc(sizeof(struct nodoC));//da memoria a la casilla nueva como elemento de una lista

    if(*nuevo)
    {
        res=1;
        (*nuevo)->up=(*nuevo)->down=(*nuevo)->left=(*nuevo)->right=NULL;//coloca en NULL los apuntadores de la casilla nueva
        (*nuevo)->tipo=tipo;//coloca el tipo de casila que es
        (*nuevo)->luz=false;
    }
    return(res);//regresa el resultado de la funcion
}

//esta funcion llena la malla tridimensional con el contenido de cada tipo de casilla
int cargar(int malla[NS][N][N]){
    char Nombre[4];//se declara un array que se usara para colocar el resultado de la funcion itoa
    int res=0;//se usara para guardar el resultado de la funcion actual
    int cont,i,j;//se declaran los indices de los bucles a usar
    FILE *f;//se declara un file como apuntador pues se usara a continuacion
    for(cont=0;cont<NS;cont++){//este ciclo nos dara los valores de cada archivo txt con los sprites, ademas de hacer su guardado NS veces
        itoa(cont,Nombre,10);//transforma el valor de cont a array para poder ser usado en la siguiente funcion
        f=fopen(strcat(Nombre,".txt"),"r");//esta funcion abre el archivo txt con el nombre que previamente formamos de manera que sea de solo lectura
        if(f){//verifica si se pudo abrir
            res=1;
            for(i=0;i<N;i++)//recorre de manera mas lenta para formar las columnas osea en x
                for(j=0;j<N;j++)//es el que se recorre primero de manera que forma los renglones en y
                    fscanf(f,"%d",&malla[cont][i][j]);//guarda en la malla de sprites pixel por pixel cada uno de estos
            fclose(f);//cierra el archivo despues de usarlo
        }
    }
    return res;//regresa el resultado de la funcion actua
}

//Funcion que inicializa cualquier personaje, ya sea jugador o enemigo
void IniPsj(PSJ*jugador,int tipo,CAS pos,int vidas){
    jugador->vida=vidas;
    jugador->tipo=tipo;
    jugador->pos=pos;
    jugador->piso=12;
    jugador->pos->luz=true;
    jugador->llave=false;
}

//funcion que se encarga de hacer movimiento de un personaje a la Derecha
int movimientoR(PSJ *pj)
{
    int res=0,aux;
    if(pj->pos->right)
    {
       res=1;
       aux=pj->pos->tipo;//Guarda el tipo de personaje en un aux
       pj->pos->tipo=pj->piso; //coloca el antiguo piso en donde el personaje esta
       pj->pos=pj->pos->right;//se cambia la posicion del personaje
       pj->piso=pj->pos->tipo;//Guarda el tipo de piso sobre el que esta el personaje
       pj->pos->tipo=aux;//
    }
    return(res);
}

//funcion que se encarga de hacer movimiento de un personaje a la Izquierda
int movimientoL(PSJ *pj)
{
    int res=0,aux;
    if(pj->pos->left)
    {
       res=1;
       aux=pj->pos->tipo;//Guarda el tipo de personaje en un aux
       pj->pos->tipo=pj->piso; //guarda el antiguo piso en donde el personaje esta
       pj->pos=pj->pos->left;//se cambia la posicion del personaje
       pj->piso=pj->pos->tipo;//Guarda el tipo de piso sobre el que esta el personaje
       pj->pos->tipo=aux;//
    }
    return(res);
}

//funcion que se encarga de hacer movimiento de un personaje a arriba
int movimientoU(PSJ *pj)
{
    int res=0,aux;
    if(pj->pos->up)
    {
       res=1;
       aux=pj->pos->tipo;//Guarda el tipo de personaje en un aux
       pj->pos->tipo=pj->piso; //guarda el antiguo piso en donde el personaje esta
       pj->pos=pj->pos->up;//se cambia la posicion del personaje
       pj->piso=pj->pos->tipo;//Guarda el tipo de piso sobre el que esta el personaje
       pj->pos->tipo=aux;//
    }
    return(res);
}

//funcion que se encarga de hacer movimiento de un personaje a abajo
int movimientoD(PSJ *pj)
{
    int res=0,aux;
    if(pj->pos->down)
    {
       res=1;
       aux=pj->pos->tipo;//Guarda el tipo de personaje en un aux
       pj->pos->tipo=pj->piso; //guarda el antiguo piso en donde el personaje esta
       pj->pos=pj->pos->down;//se cambia la posicion del personaje
       pj->piso=pj->pos->tipo;//Guarda el tipo de piso sobre el que esta el personaje
       pj->pos->tipo=aux;//
    }
    return(res);
}

//Funcion que ilumina las 8 casillas alrededor del personaje, e ilumina una casilla extra a sus cuatro ejes
void iluminacion(PSJ jugador)
{
    CAS auxL;
    int cont;
    jugador.pos->luz=jugador.pos->left->luz=jugador.pos->right->luz=jugador.pos->up->luz=jugador.pos->down->luz=true;
    jugador.pos->left->up->luz=jugador.pos->right->down->luz=jugador.pos->up->right->luz=jugador.pos->down->left->luz=true;
    for(auxL=jugador.pos->left,cont=0;auxL&&auxL->tipo>=12 && cont<2;auxL->luz=true,auxL=auxL->left,cont++);
    for(auxL=jugador.pos->right,cont=0;auxL&&auxL->tipo>=12 && cont<2;auxL->luz=true,auxL=auxL->right,cont++);
    for(auxL=jugador.pos->up,cont=0;auxL&&auxL->tipo>=12 && cont<2;auxL->luz=true,auxL=auxL->up,cont++);
    for(auxL=jugador.pos->down,cont=0;auxL&&auxL->tipo>=12 && cont<2;auxL->luz=true,auxL=auxL->down,cont++);
}


//Funcion que verifica si se puede hacer movimiento del jugador, recibe la tecla oprimida y el jugador
void movimientoJugador(PSJ *jugador,int tecla,int *puntaje,int punt,int *mov)
{
    switch(tecla)
    {
    //Derecha
    case 77:
        if(jugador->pos->right->tipo>11&&jugador->pos->right->tipo<16&&jugador->pos->right->tipo!=14){
            movimientoR(jugador);
            *puntaje+=punt;
            (*mov)++;
        }
        break;
    //Izquierda
    case 75:
        if(jugador->pos->left->tipo>11&&jugador->pos->left->tipo<16&&jugador->pos->right->tipo!=14){
            movimientoL(jugador);
            *puntaje+=punt;
            (*mov)++;
        }
        break;
    //Arriba
    case 72:
        if(jugador->pos->up->tipo>11&&jugador->pos->up->tipo<16){
            movimientoU(jugador);
            *puntaje+=punt;
            (*mov)++;
        }
        break;
    //Abajo
    case 80:
        if(jugador->pos->down->tipo>11&&jugador->pos->down->tipo<16){
            movimientoD(jugador);
            *puntaje+=punt;
            (*mov)++;
        }
        break;
    }
}

void movimientoFantasma(PSJ *fantasma)
{
    int direccion = rand()%4;
    switch(direccion)
    {
    //Derecha
    case 1:
        if(fantasma->pos->right!=NULL&&fantasma->pos->right->tipo<15){
            movimientoR(fantasma);
        }
        break;
    //Izquierda
    case 2:
        if(fantasma->pos->left!=NULL&&fantasma->pos->left->tipo<15){
            movimientoL(fantasma);
        }
        break;
    //Arriba
    case 3:
         if(fantasma->pos->up!=NULL&&fantasma->pos->up->tipo<15){
            movimientoU(fantasma);
        }
            break;
    //Abajo
    case 0:
         if(fantasma->pos->down!=NULL&&fantasma->pos->down->tipo<15){
            movimientoD(fantasma);
        }
        break;
    }
    fantasma->pos->luz=true;
}

void movimientoZombie(PSJ*zombie)
{
    int direccion = rand()%4;
      switch(direccion)
    {
    //Derecha
    case 1:
        if(zombie->pos->right&&zombie->pos->right->tipo==12)
        {
            movimientoR(zombie);
        }
        break;
    //Izquierda
    case 2:
        if(zombie->pos->left&&zombie->pos->left->tipo==12)
        {
            movimientoL(zombie);
        }
        break;
    //Arriba
    case 3:
        if(zombie->pos->up&&zombie->pos->up->tipo==12)
        {
            movimientoU(zombie);
        }
        break;
    //Abajo
    case 0:
        if(zombie->pos->down&&zombie->pos->down->tipo==12)
        {
            movimientoD(zombie);
        }
        break;
    }
    zombie->pos->luz=true;
}

void movimientoAnimal(PSJ *animal)
{
    int direccion = rand()%4;
    switch(direccion)
    {
    //Derecha
    case 1:
        if(animal->pos->right)
        {
            if(animal->pos->right->right&&animal->pos->right->right->tipo==12)
            {
                movimientoR(animal);
                movimientoR(animal);
            }
        }
        break;
    //Izquierda
    case 2:
        if(animal->pos->left)
        {
            if(animal->pos->left->left&&animal->pos->left->left->tipo==12)
            {
                movimientoL(animal);
                movimientoL(animal);
            }
        }
        break;
    //Arriba
    case 3:
        if(animal->pos->up->up)
        {
            if(animal->pos->up->up&&animal->pos->up->up->tipo==12)
            {
                movimientoU(animal);
                movimientoU(animal);
            }
        }
        break;
    //Abajo
    case 0:
        if(animal->pos->down->down)
        {
            if(animal->pos->down->down&&animal->pos->down->down->tipo==12)
            {
                movimientoD(animal);
                movimientoD(animal);
            }
        }
        break;
    }
    animal->pos->luz=true;
}

//esta funcion crea la lista casilla por casilla para de manera formal crear el juego en s�
int creaEscenario(CAS *esc,char *nivel,PSJ*jugador,PSJ enemigos[]){
    CAS nuevo, ant, antCol;//se definen apuntadores a casilla auxiliares
    int res=1,i,j,tipo;//se declaran indices de recorrido, el resultado de la funcion actual y un auxiliar de tipo de casilla
    int cont=0;//el contador de �enemigos?
    FILE *f;//se declara apuntador a archivo
    f=fopen(nivel,"r");//se abre el archivo que contiene los tipos de casilla en el nivel indicado en el array nivel en modo solo lectura
    if(f){//verifica si se abrio el archivo
        for(i=0;i<R&&res;i++){//recorre la lista en renglones en y
            for(j=0;j<C&&res;j++){//recorre la lista en columnas en x
                fscanf(f,"%d",&tipo);//se escanea el tipo de casilla que se creara
                res=creaCasilla(&nuevo,tipo);//se manda llamar la funcion que crea las casillas
                if(res){//verifica si se pudo crear la casilla
                    if(i==0){//si la casilla es la primera en el renglon
                        if(j==0){//si la casilla esta en la primer columna tambien es porque es la primera en la lista
                            *esc=nuevo;//se enlaza la casilla a la cabecera dando inicio a la lista
                            ant=nuevo;//la variable ant se inicia con esta casilla :)
                            antCol=nuevo;//tambien la variable de la columna anterior se inicia
                        }
                        else{//es porque esta en el primer renglon pero no en la primer columna
                            ant->right=nuevo;//el apuntador a la derecha de la casilla anaterior la se�ala
                            nuevo->left=ant;//el apuntador a la izquierda de la casilla nueva se�ala a la anterior
                            ant=nuevo;     //se actualiza el anterior
                        }
                    }
                    else{//es porque no esta en el primer renglon
                        if(j==0){//indica que es la primera en la columna
                            antCol->down=nuevo;//el apuntador a abajo de la casilla de arriba apunta a la nueva casilla
                            nuevo->up=antCol;//el apuntador de arriba de la nueva casilla se�ala a la de arriba
                            antCol=nuevo;//se actualiza anterior columna
                            ant=nuevo;//se actualiza anterior
                        }
                        else{//es porque no es la primer columna ni el primer renglon :v
                            nuevo->up=ant->up->right;//la casilla nueva apunta a la casilla que esta arriba de esta :3
                            ant->up->right->down=nuevo;//la casilla arriba de la nueva ahora apunta a nuestra casilla nueva :3
                            ant->right=nuevo;//la casilla a la izquierda de la nueva ahora apunta hacia esta
                            nuevo->left=ant;//la nueva casilla apunta a la de su izquierda
                            ant=nuevo;
                        }
                    }
                }
                 if(tipo>15){//si la casilla contiene un enemigo
                        IniPsj(&enemigos[cont],tipo,nuevo,0);//crea una estructura para el enemigo
                        cont++;//avanza el contador de enemigos
                    }
                    else{//si la casilla no es enemigo
                        if(!tipo){//si la casilla es 0 osea el jugador
                            IniPsj(jugador,tipo,nuevo,5);//crea una estructura para el jugador
                        }
                    }
                     if(nuevo->tipo==15 || nuevo->tipo==14)
                        nuevo->luz=true;
            }
        }
    }
    else{//si no se abrio el txt del nivel :(
        printf("No se pudo abrir el nivel deseado :(");//nos indica que no se pudo
        res=0;//F
    }
    fclose(f);//cierra el archivo como debe ser
    return res;//regresa el resultado de la funcion actual
}

//esta funcion rellena la lista de listas con un nuevo nivel, de esta manera reciclamos la estructura de datos
void rellenaEscenario(CAS *cab,char *nivel,PSJ*jugador,PSJ enemigos[]){
    CAS aux,aux2;//se definen apuntadores a casilla auxiliares
    int i,j;//se declaran indices de recorrido
    int cont=0;//el contador de �enemigos?
    aux=*cab;
    aux2=*cab;
    FILE *f;//se declara apuntador a archivo
    f=fopen(nivel,"r");//se abre el archivo que contiene los tipos de casilla en el nivel indicado en el array nivel en modo solo lectura
    if(f){//verifica si se abrio el archivo
        for(i=0;i<R;i++){//recorre por renglones
            for(j=0;j<C;j++){//recorre por columnas
                fscanf(f,"%d",&aux->tipo);//se escanea el tipo de casilla que se creara
                if(aux->tipo>15){//si la casilla contiene un enemigo
                        IniPsj(&enemigos[cont],aux->tipo,aux,0);//rellena la estructura de enemigos
                        cont++;//avanza el contador de enemigos
                    }
                    else{//si la casilla no es enemigo
                        if(!aux->tipo){//si la casilla es 0 osea el jugador
                            IniPsj(jugador,aux->tipo,aux,8);//rellena la estructura para el jugador, osea lo reinicia
                        }
                    }
                    if(aux->tipo==15 || aux->tipo==14)
                        aux->luz=true;
                aux=aux->right;
            }
            aux2=aux2->down;
            aux=aux2;
        }
    }
    else{//si no se abrio el txt del nivel :(
        printf("No se pudo abrir el nivel deseado :(");//nos indica que no se pudo
    }
    fclose(f);//cierra el archivo como debe ser
}

//imprime una casilla pixel por pixel y segun su tipo se toman los datos del arreglo de sprites
void ImpImagen(int malla[NS][N][N],int tipo,int x,int y){
    for(int i=0;i<N;i++){//recorre por renglones los pixeles de la casilla (y)
        for(int j=0;j<N;j++){//recorre por columnas los pixeles de la casilla (x)
            setcolor(malla[tipo][i][j]);//define el color que tendra nuestro pixel
            setfillstyle(1,malla[tipo][i][j]);//indica que rellenara un espacio por completo
            bar(x,y,x+TAM,y+TAM);//se crea la barra en la posicion del tama�o en pixeles indicado por TAM
            y+=TAM;//se avanza en y la posicion de inicio para que la siguiente barra se imprima abajo y no en el mismo lugar
        }
        x+=TAM;//se avanza en x la posicion de inicio para que la siguiente barra se imprima a la derecha y no en la misma columna
        y=y-N*TAM;//reinicia la posicion de y para que vuelva a empezar desde arriba
    }
}

//imprime la lista y otros elementos de la pantalla del juego como tal
void imprimePantalla(CAS *cab,int malla[NS][N][N]){
//int actual=0;
//setactivepage(actual);
    cleardevice();//se limpia lo que existe
    CAS aux,aux2;//se declaran auxiliares de recorrido
    aux=*cab;//se inicia el auxiliar para columnas
    aux2=*cab;//se inicia el auxiliar para renglones
    int x=20,y=20,r,c;//se declaran e inician variables de recorrido, se puede hacer de otra manera con un solo for, pero la hare +luego
    for(r=0;r<R;r++){//recorre  por renglones
        for(c=0;c<C;c++){//recorre por columnas
            if(aux->luz){
                ImpImagen(malla,aux->tipo,x,y);//se imprime una casilla mandando la posicion de inicio de esta, y el tipo de casilla a imprimir
            }
            aux=aux->right;//se desplaza a la sig a la derecha
            x+=N*TAM;//se recorre el inicio de la casilla en x
        }
        aux=aux2->down;//se baja al siguiente renglon
        aux2=aux;//se actualiza el apuntador a renglon
        y=y+N*TAM;//se recorre el inicio de la casilla en y
        x=20;//se vuelve a iniciar x
    }
    //boton(950,500,"Salir");//pone el boton de salir en el lado derecho de la pantalla(x,y,texto)
    //aqui falta agregar valores como la vida, puntos, etc.
}

void imprimeVida(int malla[NS][N][N],PSJ jugador){
    setcolor(5);
    settextstyle(0,0,2);//define los parametros del texto a imprimirse
    outtextxy(950,100,"Vida:");//imprime el texto dentro del boton
    int x=950,y=150,i;
    for(i=0;i<jugador.vida;i++){
        ImpImagen(malla,19,x,y);
        x+=30;
    }
}

void imprimePuntaje(int malla[NS][N][N],PSJ jugador,int puntaje){
    char punt[20];
    itoa(puntaje,punt,10);
    setcolor(2);
    settextstyle(0,0,2);//define los parametros del texto a imprimirse
    outtextxy(950,20,"Puntaje:");//imprime el texto dentro del boton
    outtextxy(950,50,punt);//imprime el texto dentro del boton
}

//funcion que imprime los records guardados
void imprimeRecords(){
    int x,y;
    cleardevice();
    settextstyle(10,0,5);//define tipo  fuente, orientacion y tama�o
    setcolor(14);//define el color
    outtextxy(450,25,"Records");
    FILE *f;
    char nombre[30],puntuacion[10];
 	f=fopen("records.txt","r");
 	if (f){
        settextstyle(10,0,1);//define tipo  fuente, orientacion y tama�o
        setcolor(15);//define el color
        fgets(nombre,30,f);
        outtextxy(100,100,nombre);
        fgets(puntuacion,10,f);
        setcolor(10);//define el color
        outtextxy(700,100,puntuacion);
        fgets(nombre,30,f);
        setcolor(15);//define el color
        outtextxy(100,150,nombre);
        fgets(puntuacion,10,f);
        setcolor(10);//define el color
        outtextxy(700,150,puntuacion);
        fgets(nombre,30,f);
        setcolor(15);//define el color
        outtextxy(100,200,nombre);
        fgets(puntuacion,10,f);
        setcolor(10);//define el color
        outtextxy(700,200,puntuacion);
        boton(400,300,"Volver");//coloca el boton para regresar al menu
        fclose(f);
        do{
            delay(50);
            getmouseclick(WM_LBUTTONDOWN,x,y);//este ciclo mantiene el menu en espera de que regrese al del inicio
            clearmouseclick(WM_LBUTTONDOWN);
        }while(!ismouseclick(WM_LBUTTONDOWN) && !(x>400 && x<500 && y>300 && y<350));
    }
    else
        printf("No se abrio correctamente");
}

//imprime las instrucciones del juego :v
void menuAyuda()
{
    int x,y;
    cleardevice();//limpia lo que este en la pantalla
    settextstyle(10,0,5);//define tipo  fuente, orientacion y tama�o para el titulo
    setcolor(10);//define el color del titulo
    outtextxy(250,50,"Menu de ayuda");//imprime el titulo
    settextstyle(10,0,1);//define tipo  fuente, orientacion y tama�o para el contenido generak
    setcolor(15);//define el color del contenido
    outtextxy(25,100,"El objetivo del juego es encontar la llave en cada nivel y salir por la puerta.");
    outtextxy(25,125,"Te mueves con las flechas direccionales.");
    outtextxy(25,150,"Cuentas con una farol que ilumina tus alrededores, solo puedes ver eso");                   //el contenido en s�
    outtextxy(25,175,"Algunos enemigos los podras ver, pero otros estaran escondidos");
    outtextxy(25,200,"Recuerda que cada ves que te toque algo desconocido, perderas vida");
    outtextxy(25,225,"Si te vuelves loco, seras parte de la mansion.");
    boton(400,300,"Volver");//coloca el boton para regresar al menu
    do
    {
        delay(50);
        getmouseclick(WM_LBUTTONDOWN,x,y);//este ciclo mantiene el menu en espera de que regrese al del inicio
        clearmouseclick(WM_LBUTTONDOWN);
    }while(!ismouseclick(WM_LBUTTONDOWN) && !(x>400 && x<500 && y>300 && y<350));
}
void desiluminacion(CAS *cab)
{
    CAS aux,aux2;//se declaran auxiliares de recorrido
    aux=*cab;//se inicia el auxiliar para columnas
    aux2=*cab;//se inicia el auxiliar para renglones
    int x=20,y=20,r,c;//se declaran e inician variables de recorrido, se puede hacer de otra manera con un solo for, pero la hare +luego
    for(r=0;r<R;r++){//recorre  por renglones
        for(c=0;c<C;c++){//recorre por columnas
            if(aux->tipo!=15 && aux->tipo!=14)
                        aux->luz=false;
            aux=aux->right;//se desplaza a la sig a la derecha
        }
        aux=aux2->down;//se baja al siguiente renglon
        aux2=aux;//se actualiza el apuntador a renglon
    }
}

void verificaEntorno(PSJ *personaje){
    if(personaje->pos->up->right->tipo>15)
        (personaje->vida)--;
    else
        if(personaje->pos->up->tipo>15)
            (personaje->vida)--;
        else
            if(personaje->pos->up->left->tipo>15)
                (personaje->vida)--;
            else
                if(personaje->pos->left->tipo>15)
                    (personaje->vida)--;
                else
                    if(personaje->pos->right->tipo>15)
                        (personaje->vida)--;
                    else
                        if(personaje->pos->down->tipo>15)
                            (personaje->vida)--;
                        else
                            if(personaje->pos->down->left->tipo>15)
                                (personaje->vida)--;
                            else
                                if(personaje->pos->down->right->tipo>15)
                                    (personaje->vida)--;
}

void verificaLLave(PSJ *personaje){
    if(personaje->piso==15){
        personaje->llave=true;
        personaje->piso=12;
    }
}

int verificaSalida(PSJ *personaje){
    int res=1;
    if(personaje->pos->right->tipo==14&&personaje->llave==true){
            res=0;
    }
    return res;
}

void verificaPuntaje(int puntaje_hecho){
    int puntaje_primero,puntaje_segundo,puntaje_tercero,posicion;
    char nombre_primero[30],nombre_segundo[30],nombre_tercero[30];
    FILE *f;
 	f=fopen("records.txt","w+");
 	if (f){
        fscanf(f,"%s\n%d\n",nombre_primero,&puntaje_primero);
        //fscanf(f,"%d",&puntaje_guardado);
        posicion=ftell(f);
        if(puntaje_primero<puntaje_hecho){
            rewind(f);
            closegraph();
            pideNombre(nombre_primero);
            puntaje_primero=puntaje_hecho;
            fscanf(f,"%s\n%d\n%s\n%d",nombre_segundo,&puntaje_segundo,nombre_tercero,&puntaje_tercero);
        }
        else{
            fscanf(f,"%s\n%d\n",nombre_segundo,&puntaje_segundo);
            if(puntaje_segundo<puntaje_hecho){
                closegraph();
                fseek(f,posicion,SEEK_SET);
                pideNombre(nombre_segundo);
                puntaje_segundo=puntaje_hecho;
                fscanf(f,"%s\n%d\n",nombre_tercero,&puntaje_tercero);
            }
            else{
                fscanf(f,"%s\n%d\n",nombre_tercero,&puntaje_tercero);
                if(puntaje_tercero<puntaje_hecho){
                    closegraph();
                    pideNombre(nombre_tercero);
                    puntaje_tercero=puntaje_hecho;
                }
            }
        }
        rewind(f);
        initwindow(1300,700);
        fprintf(f,"%s\n%d\n%s\n%d\n%s\n%d\n",nombre_primero,puntaje_primero,nombre_segundo,puntaje_segundo,nombre_tercero,puntaje_tercero);
    }
}

//esta funcion es para que el juego se lleve a cabo como tal, osea mantiene el chequeo del click y cualquier pulsacion de teclas
void juego(CAS *cab,int malla[NS][N][N],PSJ *personaje,PSJ enemigos[],int *puntaje,int mov_max){
    int i,mov=0,puntos=20,res=1;
    int x=0,y=0,tecla=0;//se declaran variables de posicion del mouse, y del teclado
    do{
        //Quite el boton de salida porque nadie lo usaba :`c
        /*if(x>950&&x<1050&&y>500&&y<550)
            break;
        getmouseclick(WM_LBUTTONDOWN,x,y);
        clearmouseclick(WM_LBUTTONDOWN);
        while(!ismouseclick(WM_LBUTTONDOWN)&&!kbhit());//esto espera que se pulse el teclado o el raton*/
        if(tecla=getch()){//si le dan al teclado se captura la tecla presionada en entero
            if(mov>mov_max){
                puntos=-10;
            }
            movimientoJugador(personaje,tecla,puntaje,puntos,&mov);//aqui iria el llamado a las funciones del movimiento
            desiluminacion(cab);
            for(i=0;i<NE;i++){
                switch(enemigos[i].pos->tipo){
                    case 16:
                        movimientoFantasma(&enemigos[i]);
                    break;
                    case 17:
                        movimientoAnimal(&enemigos[i]);
                    break;
                    case 18:
                        movimientoZombie(&enemigos[i]);
                    break;
                }
            }
            verificaEntorno(personaje);
            verificaLLave(personaje);
            res=verificaSalida(personaje);
            iluminacion(*personaje);
            imprimePantalla(cab,malla);//imprime la pantalla del juego
            imprimeVida(malla,*personaje);
            imprimePuntaje(malla,*personaje,*puntaje);
        }
    }while(personaje->vida>0&&res);
}
//esto recibe la posicion del click y hace lo que corresponde de acuerdo lo que piko :V

void pantallaGameOver(){
    cleardevice();
    settextstyle(10,0,5);//define tipo  fuente, orientacion y tama�o
    setcolor(4);//define el color
    outtextxy(450,300,"GAME OVER");
    settextstyle(10,0,7);//define tipo  fuente, orientacion y tama�o
    setcolor(15);//define el color
    outtextxy(580,350,".");
    delay(1000);
    outtextxy(600,350,".");
    delay(1000);
    outtextxy(620,350,".");
    delay(1000);
}

void pantallaWin(){
    cleardevice();
    settextstyle(10,0,5);//define tipo  fuente, orientacion y tama�o
    setcolor(10);//define el color
    outtextxy(450,200,"FELICIDADES");
    outtextxy(300,300,"Lograste Salir del Laberinto");
    settextstyle(10,0,7);//define tipo  fuente, orientacion y tama�o
    setcolor(15);//define el color
    outtextxy(580,350,".");
    delay(1000);
    outtextxy(600,350,".");
    delay(1000);
    outtextxy(620,350,".");
    delay(1000);
}

int menuInicio(int x,int y,CAS *cab,int malla[NS][N][N],PSJ*jugador,PSJ enemigos[]){
    if(x>350&&x<490&&y>245&&y<295){//si le dio a jugar
        //cout <<"Jugar fue presionado"<<endl;//Inicia el Juego
        if(creaEscenario(cab,"nivel1.txt",jugador,enemigos)){//crea la lista del nivel 1
            //imprimeEscenario(cab);
            int puntaje=0;
            iluminacion(*jugador);
            imprimePantalla(cab,malla);
            imprimeVida(malla,*jugador);
            imprimePuntaje(malla,*jugador,puntaje);
            juego(cab,malla,jugador,enemigos,&puntaje,43);//que comienze el juego ;)
            if(jugador->vida>0){
                puntaje+=jugador->vida*100;
                rellenaEscenario(cab,"nivel2.txt",jugador,enemigos);
                juego(cab,malla,jugador,enemigos,&puntaje,105);//Avanzo al nivel 2 ;)
                if(jugador->vida>0){
                    puntaje+=jugador->vida*100;
                    rellenaEscenario(cab,"nivel3.txt",jugador,enemigos);
                    juego(cab,malla,jugador,enemigos,&puntaje,122);//Avanzo al nivel 3 ;)
                    if(jugador->vida>0)
                        pantallaWin();//Gano el juego :3
                    else
                        pantallaGameOver();//Perdio en el nivel 3
                }
                else
                    pantallaGameOver();//Perdio en el nivel 2
            }
            else
                pantallaGameOver();//Perdio en el nivel 1 XD
            verificaPuntaje(puntaje);
            imprimeRecords();
        }
        else//si no pudo crear la lista
            return 0;
    }
    else
        //cout <<"El mouse fue presionado en:"<<endl<<"x="<<x<<"y="<<y<<endl;
        if(x>350&&x<490&&y>325&&y<375){//Carga un archivo de records
            imprimeRecords();
            verificaPuntaje(100);
            imprimeRecords();
        }
        else{
            if(x>350&&x<490&&y>405&&y<455){
                menuAyuda();
            }
            else{
                if(x>350&&x<490&&y>485&&y<535){//Se sale del programa
                    return 0;
                }
            }
        }
    cleardevice();//limpia la pantalla
    clearmouseclick(WM_LBUTTONDOWN);//elimina que se haya dado click al mouse
    return 1;//regresa el resultado de la funcion
}

//imprime el menu de inicio literalmente uwu
int pantallaInicio(int *x,int *y){
    *x=*y=0;//inicia la posicion del raton en el origen
    settextstyle(10,0,5);//define la fuente, la orientacion y el tama�o del texto de arriba
    setcolor(10);//define el color del texto de arriba
    outtextxy(280,100,"La mansion\n");//imprime el titulo del juego
    boton(350,245,"Jugar");
    boton(350,325,"Records");       //se imprimen los 4 botones
    boton(350,405,"Ayuda");
    boton(350,485,"Salir");
    while(!ismouseclick(WM_LBUTTONDOWN))
        delay(100);//mientras no se presione el mouse
    getmouseclick(WM_LBUTTONDOWN,*x,*y);//obtiene la posicion donde se preiono el mouse
    clearmouseclick(WM_LBUTTONDOWN);//limpia que se haya presionado el mouse
}

//es la funcion principal del programa
int main(){
    int x,y,res=1;//se declaran las variables en donde se guardara la posicion del mouse
    srand(time(0));
    int sprites[NS][N][N];//se declara el array en donde estara la informacion de los sprites
    PSJ jugador,enemigos[NE];//se declara la cabecera de las estructuras de los distintos personajes
    if(cargar(sprites)){// llena la malla de sprites y despues checa si se pudo
        CAS escenario;//se declara la cabecera a la lista
        iniCasillas(&escenario);//se inicia la cabecera a la lista
        initwindow(1300,700);//se abre una ventana de 1200 x 700
        while(res){//mientras no se quieran salir o suceda un error sigue el juego
            pantallaInicio(&x,&y);//se imprime el menu de inicio y se obtiene la posicion del mouse cuando presionen algo
            res=menuInicio(x,y,&escenario,sprites,&jugador,enemigos);//se checa que eligio el usuario
        }
    }
}


//Estas dos funciones que siguen fueron de apoyo en el desarrollo del juego

void muestraSprite(int malla[NS][N][N]){
    int i,j,k;
    for(k=0;k<NS;k++){
        for(i=0;i<N;i++){
            for(j=0;j<N;j++){
                printf("%d",malla[k][i][j]);
            }
            cout<<endl;
        }
        cout<<endl;
    }
}

/*imprime los valores guardados en la lista */
void muestraEscenario(CAS *cab){
    int i,j;
    CAS aux,aux2;
    aux=*cab;
    aux2=*cab;
    for(i=0;i<R;i++){
        for(j=0;j<C;j++){
            printf(" %d",aux->tipo);
            aux=aux->right;
        }
        printf("\n");
        aux2=aux2->down;
        aux=aux2;
    }
}
