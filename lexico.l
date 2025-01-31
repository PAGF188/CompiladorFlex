%{
#include <stdio.h>
#include <ctype.h>
#include "./headerFiles/Definiciones.h"
#include "./headerFiles/Errores.h"
#include "./headerFiles/TablaSimbolos.h"
int lineas=1;  //variable global para hacer visible al lexico el nº de línea donde estamos.
%}
%option noyywrap

/*PATRONES PARA ENTEROS. El principal es INTEGER_LITERAL. (no incluimos hexadecimales)*/
INTEGER_LITERAL     {INTEGER}|({INTEGER}{INTEGER_SUFIX})
INTEGER_SUFIX       ([LuU]|(Lu)|(LU)|(uL)|(UL))
INTEGER             {BINARY_INTEGER}|{DECIMAL_INTEGER_NON_ZERO}
DECIMAL_INTEGER_NON_ZERO    (0|[1-9][0-9_]*)
BINARY_INTEGER      ((0b)|(0B))(([01][01_]*)|(_[01][01_]*))

/*PATRONES PARA FLOTANTES. El principal es FLOAT_LITERAL. (no incluimos hexadecimales)*/
FLOAT_LITERAL       {FLOAT}|({FLOAT}{SUFFIX})|({INTEGER}{FLOAT_SUFFIX})|({INTEGER}{IMAGINARY_SUFFIX})|({INTEGER}{FLOAT_SUFFIX}{IMAGINARY_SUFFIX})|({INTEGER}{REAL_SUFFIX}{IMAGINARY_SUFFIX})      
SUFFIX              {FLOAT_SUFFIX}|{REAL_SUFFIX}|{IMAGINARY_SUFFIX}|({FLOAT_SUFFIX}{IMAGINARY_SUFFIX})|({REAL_SUFFIX}{IMAGINARY_SUFFIX})
FLOAT_SUFFIX        [fF]
REAL_SUFFIX         L
IMAGINARY_SUFFIX    i
DECIMAL_INTEGER     [0-9][0-9_]*
FLOAT               ({DECIMAL_INTEGER_NON_ZERO}".")|({DECIMAL_INTEGER_NON_ZERO}"."{DECIMAL_INTEGER})|({DECIMAL_INTEGER_NON_ZERO}"."{DECIMAL_INTEGER}{DECIMAL_EXPONENT})|("."{DECIMAL_INTEGER})|("."{DECIMAL_INTEGER}{DECIMAL_EXPONENT})|({DECIMAL_INTEGER_NON_ZERO}{DECIMAL_EXPONENT})
DECIMAL_EXPONENT    ((e\+)|(E\+)|(e-)|(E-)|(e)|(E))(_?[0-9][0-9_]*)

IDENTIFICADOR       [_A-Za-z][A-Za-z0-9_]*

/*SOLO SE CONTEMPLA: DoubleQuotedString:*/

STRING              \"([^"\\]|\\.)*\"{STRINGPOSTFIX}?
STRINGPOSTFIX       [cwd]

/*COMENTARIOS*/
COMENTARIO_LINEA    \/\/.*
/*Referencia: https://blog.ostermiller.org/finding-comments-in-source-code-using-regular-expressions/ */
COMENTARIO_BLOQUE   \/\*([^*]|[\n]|(\*+([^*\/]|[\n])))*\*+\/
/*Comentario anidado no constituye un lenguaje regular -> no se puede resolver con autómata finito*/
/*solución-> rutina en c*/

/*TERMINACIONES INCORRECTAS PARA FLOTANTES Y INTEGERS (pueden existir más!!)*/
TERMINACIONES_INCORRECTAS_FI        [A-Za-z.&|!\[{]

%%

{INTEGER_LITERAL}                                   return(_ENTERO);
{INTEGER_LITERAL}{TERMINACIONES_INCORRECTAS_FI}     imprimeError(7,lineas,yytext);

{FLOAT_LITERAL}                                     return(_FLOTANTE);
{FLOAT_LITERAL}{TERMINACIONES_INCORRECTAS_FI}       imprimeError(8,lineas,yytext);

{STRING}                return(_STRING);    
    /*Identificadores y palabras reservadas!! (la tabla de símbolos resuelve el problema)*/        
{IDENTIFICADOR}         return(insertarComponenteLexico(strdup(yytext)));

"/+" {
    void comentario(){
        char c1 = '+';
        char c2 = input();
        do{
            c1=c2;
            c2 = input();
            if(c1==(char)0 || c2==(char)0){
                imprimeError(10,lineas,NULL);
                return;
            }
            if(c2=='\n')
                lineas++;
            if(c1=='/' && c2=='+'){
                unput(c2);
                comentario();
            }
        }while(c1!='+' || c2!='/');
    }
    comentario();
}
{COMENTARIO_LINEA}  ;
{COMENTARIO_BLOQUE} ;


" "       ;
\n        ++lineas;
"/="      return(_BARRA_IGUAL);
".."      return(_DOS_PUNTOS);
"..."     return(_TRES_PUNTOS);
"&="      return(_AMP_IGUAL);
"&&"      return(_AMP_AMP);
"|="      return(_VERT_IGUAL);
"||"      return(_VERT_VERT);
"-="      return(_MENOS_IGUAL);
"--"      return(_DOBLE_MENOS);
"+="      return(_MAS_IGUAL);
"++"      return(_DOBLE_MAS);
"<="      return(_MENOR_O_IGUAL);
"<<"      return(_MENOR_MENOR);
"<<="     return(_DMENOR_IGUAL);
">="      return(_MAYOR_IGUAL);
">>="     return(_DMAYOR_IGUAL);
">>>="    return(_TMAYOR_IGUAL);
">>"      return(_DMAYOR);
">>>"     return(_TMAYOR);
"!="      return(_ADMI_IGUAL);
"=="      return(_DOBLE_IGUAL);
"*="      return(_POR_IGUAL);
"%="      return(_PORCIEN_IGUAL);
"^="      return(_EXP_IGUAL);
"^^"      return(_EXP_EXP);
"^^="     return(_EXP_EXP_IGUAL);
"~="      return(_BIR_IGUAL);
"=>"      return(_IGUAL_MAYOR);
"-"       |
">"       |
"<"       |
"!"       |
"+"       | 
"|"       |
"&"       |
"."       |
"/"       |
"("       |
")"       |
"["       |
"]"       |
"{"       |
"}"       |
"?"       |
","       |
";"       |
":"       |
"*"       |
"$"       |
"%"       |
"^"       |
"~"       |
"@"       |
"#"       |
"="       return((int) yytext[0]);


. { /*regla por defecto*/
        if(isascii((int)yytext[0])){
            /*Si el carácter que sobro es " -> no se cerro STRING*/
            if((int)yytext[0] == '"')
                imprimeError(9,lineas,NULL);
            else
                imprimeError(6,lineas,yytext);
            }
        }

<<EOF>> {
            if(yyin!=NULL){
                if(fclose(yyin)!=0){
                    imprimeError(3,-1,NULL);
                }
            }
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
        imprimeError(2,-1,NULL);
        return(-1);
    }
    return(1);
}

