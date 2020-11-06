%{
#include <stdio.h>
#include "./headerFiles/Definiciones.h"
#include "./headerFiles/Errores.h"
#include "./headerFiles/TablaSimbolos.h"
%}

%option noyywrap

DIGITO [0-9]
ID [a-z][a-z0-9]*
%%

%%

/**
 * Función para iniciar el sistema de entrada. 
 * Comprueba que existe el archivo.
 * Introduce en flex el archivo a analizar. 
 * @param path, ruta al archivo a compilar.
 * @return int, código de error (-1 fallo en el inicio, 1 inicio correcto)
 */ 
int iniciaArchivo(char *path){
    yyin = fopen(path,"r");
    if(yyin==NULL){
        imprimeError(2,-1);
        return(-1);
    }
    return(1);
}

/**
 * Cierra el archivo al finalizar la compilación.
 * @return int, código de éxito (-1 error en cierre, 1 cierre correcto).
 */
int finArchivo(){
    if(yyin!=NULL){
        if(fclose(yyin)!=0){
            imprimeError(3,-1);
            return(-1);
        }
    }
    return(1);
}
