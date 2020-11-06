%{
#include <stdio.h>
#include "./headerFiles/Definiciones.h"
#include "./headerFiles/Errores.h"
#include "./headerFiles/TablaSimbolos.h"
int lineas=0;  //variable global para hacer visible al lexico el nº de línea donde estamos.
%}
%option noyywrap

IDENTIFICADOR       [_A-Za-z][A-Za-z0-9_]*
COMENTARIO_LINEA    \/\/.*
COMENTARIO_BLOQUE   \/\*(.|\n)*\*\/


%%

{IDENTIFICADOR}     return(insertarComponenteLexico(strdup(yytext)));
{COMENTARIO_LINEA}  printf("\ncomentario linea: %s\n\n", yytext);
{COMENTARIO_BLOQUE} printf("\ncomentario: %s\n\n", yytext);

\n        ++lineas;
"/"       return((int) '/');
"/="      return(_BARRA_IGUAL);
"."       return((int) '.');
".."      return(_DOS_PUNTOS);
"..."     return(_TRES_PUNTOS);
"&"       return((int) '&');
"&="      return(_AMP_IGUAL);
"&&"      return(_AMP_AMP);
"|"       return((int) '|');
"|="      return(_VERT_IGUAL);
"||"      return(_VERT_VERT);
"-"       return((int) '-');
"-="      return(_MENOS_IGUAL);
"--"      return(_DOBLE_MENOS);
"+"       return((int) '+');
"+="      return(_MAS_IGUAL);
"++"      return(_DOBLE_MAS);
"<"       return((int) '<');
"<="      return(_MENOR_O_IGUAL);
"<<"      return(_MENOR_MENOR);
"<<="     return(_DMENOR_IGUAL);
">"       return((int) '>');
">="      return(_MAYOR_IGUAL);
">>="     return(_DMAYOR_IGUAL);
">>>="    return(_TMAYOR_IGUAL);
">>"      return(_DMAYOR);
">>>"     return(_TMAYOR);
"!"       return((int) '!');
"!="      return(_ADMI_IGUAL);
"("       return((int) '(');
")"       return((int) ')');
"["       return((int) '[');
"]"       return((int) ']');
"{"       return((int) '{');
"}"       return((int) '}');
"?"       return((int) '?');
","       return((int) ',');
";"       return((int) ';');
":"       return((int) ':');
"$"       return((int) '$');
"="       return((int) '=');
"=="      return(_DOBLE_IGUAL);
"*"       return((int) '*');
"*="      return(_POR_IGUAL);
"%"       return((int) '%');
"%="      return(_PORCIEN_IGUAL);
"^"       return((int) '^');
"^="      return(_EXP_IGUAL);
"^^"      return(_EXP_EXP);
"^^="     return(_EXP_EXP_IGUAL);
"~"       return((int) '~');
"~="      return(_BIR_IGUAL);
"@"       return((int) '@');
"=>"      return(_IGUAL_MAYOR);
"#"       return((int) '#');

<<EOF>> {
            //finArchivo();
            return(_EOF);
        }




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
