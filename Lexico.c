/**
 * Archivo: Lexico.c
 * Versión: 1.0
 * Descripción: Implementa enl analizador léxico del compilador.
 * Objetivos:
 *       - Pedirle al sistema de entrada el siguiente caŕacter a procesar.
 *       - Procesar los caracteres obtenidos para identificar lexemas y sus 
 *         correspondientes componentes léxicos.
 *       - Devolverleselos al sintáctico.
 *       - Interactura con la tabla de símbolos para almacenar identificadores y
 *         distinguir palabras reservadas.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./headerFiles/Lexico.h"
#include "./headerFiles/Definiciones.h"
#include "./headerFiles/TablaSimbolos.h"
#include "./headerFiles/Errores.h"
#include "lex.yy.c"

///////////    Las definiciones de las funciones públicas se encuentran en el .h

tipoLexico siguienteComponente(){
    tipoLexico tl;
    tl.componenteLexico = yylex();
    tl.lexema = strdup(yytext);
    return(tl);
}

int inicia(char *path){
    return(iniciaArchivo(path));
}
