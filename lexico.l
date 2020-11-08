%{
#include <stdio.h>
#include "./headerFiles/Definiciones.h"
#include "./headerFiles/Errores.h"
#include "./headerFiles/TablaSimbolos.h"
int lineas=0;  //variable global para hacer visible al lexico el nº de línea donde estamos.
%}
%option noyywrap

/*PATRONES PARA ENTEROS. El principal es INTEGER_LITERAL. (no incluimos hexadecimales)*/
INTEGER_LITERAL     {INTEGER}|({INTEGER}{INTEGER_SUFIX})
INTEGER_SUFIX       ([LuU]|(Lu)|(LU)|(uL)|(UL))
INTEGER             {BINARY_INTEGER}|{DECIMAL_INTEGER_NON_ZERO}
DECIMAL_INTEGER_NON_ZERO    (0|[1-9][0-9_]*)
BINARY_INTEGER      ((0b)|(0B))(([01][01_]*)|(_[01][01_]*))

/*PATRONES PARA FLOTANTES. El principal es FLOAT_LITERAL. (no incluimos hexadecimales)*/
FLOAT_LITERAL       {FLOAT}|({FLOAT}|{SUFFIX})|({INTEGER}{FLOAT_SUFFIX})|({INTEGER}{IMAGINARY_SUFFIX})|({INTEGER}{FLOAT_SUFFIX}{IMAGINARY_SUFFIX})|({INTEGER}{REAL_SUFFIX}{IMAGINARY_SUFFIX})      
SUFFIX              {FLOAT_SUFFIX}|{REAL_SUFFIX}|{IMAGINARY_SUFFIX}|({FLOAT_SUFFIX}{IMAGINARY_SUFFIX})|({REAL_SUFFIX}{IMAGINARY_SUFFIX})
FLOAT_SUFFIX        [fF]
REAL_SUFFIX         L
IMAGINARY_SUFFIX    i
DECIMAL_INTEGER     [0-9][0-9_]*
FLOAT               ({DECIMAL_INTEGER_NON_ZERO}\.)|({DECIMAL_INTEGER_NON_ZERO}\.{DECIMAL_INTEGER})|({DECIMAL_INTEGER_NON_ZERO}\.{DECIMAL_INTEGER}{DECIMAL_EXPONENT})|(\.{DECIMAL_INTEGER})|(\.{DECIMAL_INTEGER}{DECIMAL_EXPONENT})|({DECIMAL_INTEGER_NON_ZERO}{DECIMAL_EXPONENT})
DECIMAL_EXPONENT    ((e\+)|(E\+)|(e-)|(E-)|(e)|(E))(_[0-9][0-9]*|[0-9][0-9_]*)

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

%%

{INTEGER_LITERAL}       return(_ENTERO);
{FLOAT_LITERAL}         return(_FLOTANTE);

{STRING}                return(_STRING);            
{IDENTIFICADOR}         return(insertarComponenteLexico(strdup(yytext)));

"/+" {
    void comentario(){
        char c1 = '+';
        char c2 = input();
        do{
            c1=c2;
            c2 = input();
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
"/"       return((int) yytext[0]);
"/="      return(_BARRA_IGUAL);
"."       return((int) yytext[0]);
".."      return(_DOS_PUNTOS);
"..."     return(_TRES_PUNTOS);
"&"       return((int) yytext[0]);
"&="      return(_AMP_IGUAL);
"&&"      return(_AMP_AMP);
"|"       return((int) yytext[0]);
"|="      return(_VERT_IGUAL);
"||"      return(_VERT_VERT);
"-"       return((int) yytext[0]);
"-="      return(_MENOS_IGUAL);
"--"      return(_DOBLE_MENOS);
"+"       return((int) yytext[0]);
"+="      return(_MAS_IGUAL);
"++"      return(_DOBLE_MAS);
"<"       return((int) yytext[0]);
"<="      return(_MENOR_O_IGUAL);
"<<"      return(_MENOR_MENOR);
"<<="     return(_DMENOR_IGUAL);
">"       return((int) yytext[0]);
">="      return(_MAYOR_IGUAL);
">>="     return(_DMAYOR_IGUAL);
">>>="    return(_TMAYOR_IGUAL);
">>"      return(_DMAYOR);
">>>"     return(_TMAYOR);
"!"       return((int) yytext[0]);
"!="      return(_ADMI_IGUAL);
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
"$"       |
"="       return((int) yytext[0]);
"=="      return(_DOBLE_IGUAL);
"*"       return((int) yytext[0]);
"*="      return(_POR_IGUAL);
"%"       return((int) yytext[0]);
"%="      return(_PORCIEN_IGUAL);
"^"       return((int) yytext[0]);
"^="      return(_EXP_IGUAL);
"^^"      return(_EXP_EXP);
"^^="     return(_EXP_EXP_IGUAL);
"~"       return((int) yytext[0]);
"~="      return(_BIR_IGUAL);
"@"       return((int) yytext[0]);
"=>"      return(_IGUAL_MAYOR);
"#"       return((int) yytext[0]);

<<EOF>> {
            if(yyin!=NULL){
                if(fclose(yyin)!=0){
                    imprimeError(3,-1);
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
        imprimeError(2,-1);
        return(-1);
    }
    return(1);
}

