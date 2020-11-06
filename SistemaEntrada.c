/**
 * Archivo: SistemaEntrada.c
 * Versión: 1.0
 * Descripción: Implementa el sistema de entrada del compilador.
 * Objetivos:
 *       - Devolver el siguiente caracter.
 *       - Devolver el siguiente lexema.
 */

/**
 * Nota: No es exactamente el método del doble centinela. No se incluye EOF al final de un bloque, 
 * pero aún así no se pierden suns ventajas pués solo se realiza una única comparación.
 */

#include <stdio.h>
#include <stdlib.h>
#include "./headerFiles/SistemaEntrada.h"
#include "./headerFiles/Errores.h"

//Define el tamaño de los bloques.
#define MAX 64


//Variables globales.
FILE *ptr;  //fichero
char bloque1[MAX];
char bloque2[MAX];

// Punteros centinela. Toman valores de 0 a 2*MAX-1
long inicio=0;   
long fin=0;
// Para saber tam del lexema y devolver error de tam excesivo cuando iguala a MAX.
int t_lexema=0;

// Al devolver un caracter, para detectar el caso en el que retrocedo de bloque. Lo que implicaría 
// no volver a cargarlo. 
int puedo_b1 = 1;
int puedo_b2 = 1;

//saber línea en la que estamos
int lineas=0;


////////// FUNCIONES PRIVADAS

//Si bloque es 1, carga MAX-1 caracteres en bloque1.
//Si bloque es 2, carga MAX-1 caracteres en bloque2.
void cargarBloque(int bloque);


//Función de depuración. OBVIAR
void imprimeBloque(int b){
    printf("Bloque %d: ", b);
    for(int i=0;i<MAX;i++){
        if(b==1){
            if(bloque1[i]==EOF)
                printf("EOF");
            else
                printf("%c",bloque1[i]);
            
        }
        else
        {if(bloque2[i]==EOF)
                printf("EOF");
            else
                printf("%c",bloque2[i]);
        }
    }
}

////////// Las definiciones de las funciones públicas se encuentran en el .h

int iniciaSistemaEntrada(char* archivo){
    //Lo intentamos abrir en modo lectura.
    ptr = fopen(archivo,"r");
    if(ptr==NULL){
        imprimeError(2,-1);
        return(-1);
    }
    //Cargamos el 1 bloque
    cargarBloque(1);
    return(1);
}

char siguienteChar(){
    char c;
    //estamos en bloque 2
    if(fin>=MAX){
        c = bloque2[fin-MAX];
        // si estamos en la última posición y no hemos retrocedido de bloque 
        // (al ejecutar devolverCaracter), cargamos el siguiente bloque.
        if(fin==2*MAX-1 && puedo_b1){
            cargarBloque(1);
            fin=0;
        }else{
            puedo_b1=1;
            fin++;
        }
    }
    //estamos en bloque 1
    else{
        c = bloque1[fin];
        // si estamos en la última posición y no hemos retrocedido de bloque 
        // (al ejecutar devolverCaracter), cargamos el siguiente bloque.
        if(fin==MAX-1 && puedo_b2){
            cargarBloque(2);
            fin=MAX;
        }else{
            puedo_b2=1;
            fin++;
        }
    }
    t_lexema++;
    // Si el tamaño del lexema es demasiado grande (recordar que está limitado por el tamaño del bloque)
    // devolver error
    if(t_lexema==MAX){
        imprimeError(4,lineas);
        //exit(-1);
    }
    //printf("\n!!!Caracter: %c  Fin: %ld\n", c,fin);
    if(c=='\n'){
        lineas++;
    }
    return(c);
}

char * siguienteLexema(){
    if(fin!=inicio){
        char * lexema;
        lexema = (char *) malloc(t_lexema+1);
        int i=0;
        // vamos moviendo inicio hasta que alcance fin y almacenando lso caracteres leidos
        // en lexema.
        while (inicio!=fin){
            if(inicio>=MAX){
                lexema[i] = bloque2[inicio-MAX];
            }
            else{
                lexema[i] = bloque1[inicio];
            }
            i++;
            inicio=(inicio+1)%(MAX*2);
        }
        lexema[t_lexema] = '\0';
        t_lexema=0;
        return(lexema);
    }else{
        return(NULL);
    }
}

void devolverCaracter(int nposiciones){
    //Ojo, si pasamos de un bloque a otro evitar vovler a cargarlo.
    int aux = fin;
    fin=fin-nposiciones;
    if(fin<0){
        //volvimos al bloque2
        fin = MAX-1+fin;
        //no volver a cargar bloque1
        puedo_b1=0;
    }

    if(aux==MAX && fin <MAX){
        //volvimos al bloque1
        //no volver a cargar bloque2
        puedo_b2=0;
    }
    t_lexema--;
}

int finSistemaEntrada(){
    //Si el archivo está abierto lo intentamos cerrar.
    if(ptr!=NULL){
        if(fclose(ptr)!=0){
            imprimeError(3,-1);
            return(-1);
        }
    }
    return(1);
}

//////// IMPLEMENTACION FUNCIONES PRIVADAS

void cargarBloque(int bloque){
    if(ptr!=NULL){
        if(bloque==1){
            int leidos;
            leidos = fread(bloque1,1,MAX,ptr);
            // Si leemos menos de MAX caracteres, hemos llegado al final de archivo,
            // insertamos EOF al final para que pueda ser devuelto al léxico.
            if(leidos<MAX){
                bloque1[leidos] = EOF;
            }
        }
        else{
            int leidos;
            leidos = fread(bloque2,1,MAX,ptr);
            // si leemos menos de MAX caracteres, hemos llegado al final de archivo,
            // insertamos EOF al final para que pueda ser devuelto al léxico.
            if(leidos<MAX){
                bloque2[leidos] = EOF;
            }
        }
    }
}
