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
#include <ctype.h>
#include <stdlib.h>
#include <string.h>
#include "./headerFiles/Lexico.h"
#include "./headerFiles/SistemaEntrada.h"
#include "./headerFiles/Definiciones.h"
#include "./headerFiles/TablaSimbolos.h"
#include "./headerFiles/Errores.h"

int linea=1;

/////////////////// FUNCIONES PRIVADAS

//autómata cadenas alfanumeŕicas (para identificadores y palabras reservadas)
int automataAlfanumerico();
//autómata para comentarios. Lo ignora (recorre sus caracteres)
int automataComentario();
//autómata para StringLiteral
int automataString();
//autómata para numeros (puede ser entero o flotante).
//1-> entero
//0-> flotante
//-1 -> error
int automataNumeros(char primero);

///////////    Las definiciones de las funciones públicas se encuentran en el .h

tipoLexico siguienteComponente(){

    tipoLexico tl;
    int erro=0;
    int estado=0;
    char c;
    int aux;
    
    while(erro==0){
        switch (estado){


        //estado inicial
        case 0:
            c = siguienteChar();
            //obviamos espacios y tabulados
            if (c == ' ' || c == '\t'){  
                siguienteLexema();
                estado=0;
            }
            else if(c=='\n'){
                siguienteLexema();
                estado=0;
                linea++;
            }
            //identificadores o palabras reservadas
            else if(isalpha(c) || c=='_')  
                estado = 1;
            //números, pueden ser enteros o flotantes
            else if(isdigit(c))     
                estado = 2;
            //StringLiteral
            else if(c=='"')         
                estado = 3;
            //cuando entra '/' ,dependiendo del siguiente caracter, puede ser: comentario, división, o /=
            else if(c=='/')  
                estado=4;
            //cuando entra '=' ,dependiendo del siguiente caracter, puede ser: =, ==,  o =>
            else if(c=='=')
                estado=5;
            //cuando entra '+' ,dependiendo del siguiente caracter, puede ser: +, +=,  o ++
             else if(c=='+')
                estado=6;
            //cuando entra '-' ,dependiendo del siguiente caracter, puede ser: -, -=,  o --
             else if(c=='-')
                estado=7;
            //cuando entra '<' ,dependiendo del siguiente caracter, puede ser: < o <=
             else if(c=='<')
                estado=8;
            //cuando entra '*' ,dependiendo del siguiente caracter, puede ser: * o *=
             else if(c=='*')
                estado=9;
            //caracteres simples y que SOLO PUEDEN SER UN COMPONENTE LÉXICO.
            else if(c=='.' || c=='(' || c==')' || c=='[' || c==']' || 
                        c=='{' || c=='}' || c=='?' || c==',' || c==';' || c==':' || c=='$' ||
                        c== '#' || c=='@'){
                tl.lexema = siguienteLexema();
                tl.componenteLexico = (int) (tl.lexema[0]);
                return(tl);
            }
            else if(c==EOF){        //fin de fichero
                tl.lexema = siguienteLexema();
                tl.componenteLexico = _EOF;
                return(tl);
            }
            else{
                erro=1;
                imprimeError(6,linea);
                //indicando error.
                tl.componenteLexico=-1;
                tl.lexema = siguienteLexema();
            }
            break;

        // Estado1: detector de cadenas alfanuméricas (ejs: _asd23 iu8)
        case 1:
            if(automataAlfanumerico()){
                devolverCaracter(1);
                tl.lexema = siguienteLexema();
                tl.componenteLexico = insertarComponenteLexico(tl.lexema);
                return(tl);
            }
            break;
        
        //Estado 2: Discernir entre uno de los dos posibles significados de un dígito (entero, flotante)
        case 2:
            aux = automataNumeros(c);
            if(aux==1){
                tl.lexema = siguienteLexema();
                tl.componenteLexico = _ENTERO;
                return(tl);
            }else if(aux==0){
                tl.lexema = siguienteLexema();
                tl.componenteLexico = _FLOTANTE;
                return(tl);
            }
            break;

        //Estado 3: StringLiteral "alsd"
        case 3:
            if(automataString()){
                tl.lexema = siguienteLexema();
                tl.componenteLexico = _STRING;
                return(tl);
            }
            break;

        //Estado 4: Discernir entre uno de los tres posibles significados de / (comentario, /=, /)
        case 4:
            if(automataComentario())  //ya recorre todos los caracteres del comentario ignorándolos.
                estado=0;
            else if(siguienteChar()=='='){     // en caso de /=
                tl.lexema = siguienteLexema();
                tl.componenteLexico = _BARRA_IGUAL;
                return(tl);
            }
            else{   //ninguno de los anteriores -> solo queda la posibilidad de ser /
                devolverCaracter(1); //el leido previamente que no era.
                tl.lexema = siguienteLexema();
                tl.componenteLexico = (int) (tl.lexema[0]);
                return(tl);
            }
            break;

        //Estado 5: Discernir entre uno de los tres posibles significados de = (=,==,=>)
        case 5:
            c = siguienteChar();
            if(c=='='){
                tl.lexema = siguienteLexema();
                tl.componenteLexico = _DOBLE_IGUAL;
                return(tl);

            }
            else if(c=='>'){
                tl.lexema = siguienteLexema();
                tl.componenteLexico = _IGUAL_MAYOR;
                return(tl);

            }
            else{
                devolverCaracter(1);  //el leido previamente que no era.
                tl.lexema = siguienteLexema();
                tl.componenteLexico = (int) (tl.lexema[0]);  
                return(tl);
            }
            break;

        //Estado 6: Discernir entre uno de los tres posibles significados de + (+,++,+=)
        case 6:
            c = siguienteChar();
            if(c=='+'){
                tl.lexema = siguienteLexema();
                tl.componenteLexico = _DOBLE_MAS;
                return(tl);
            }
            else if(c=='='){
                tl.lexema = siguienteLexema();
                tl.componenteLexico = _MAS_IGUAL;
                return(tl);
            }
            else{
                devolverCaracter(1);  //el leido previamente que no era.
                tl.lexema = siguienteLexema();
                tl.componenteLexico = (int) (tl.lexema[0]);  
                return(tl);
            }
            break;

        //Estado 7: Discernir entre uno de los tres posibles significados de - (-,--,-=)
        case 7:
            c = siguienteChar();
            if(c=='-'){
                tl.lexema = siguienteLexema();
                tl.componenteLexico = _DOBLE_MENOS;
                return(tl);
            }
            else if(c=='='){
                tl.lexema = siguienteLexema();
                tl.componenteLexico = _MENOS_IGUAL;
                return(tl);
            }
            else{
                devolverCaracter(1);  //el leido previamente que no era.
                tl.lexema = siguienteLexema();
                tl.componenteLexico = (int) (tl.lexema[0]);  
                return(tl);
            }
            break;

        //Estado 8: Discernir entre uno de los dos posibles significados de < (<, <=)
        case 8:
            c = siguienteChar();
            if(c=='='){
                tl.lexema = siguienteLexema();
                tl.componenteLexico = _MENOR_O_IGUAL;
                return(tl);
            }
            else{
                devolverCaracter(1);  //el leido previamente que no era.
                tl.lexema = siguienteLexema();
                tl.componenteLexico = (int) (tl.lexema[0]);  
                return(tl);
            }
            break;

        //Estado 9: Discernir entre uno de los dos posibles significados de * (*, *=)
        case 9:
            c = siguienteChar();
            if(c=='='){
                tl.lexema = siguienteLexema();
                tl.componenteLexico = _POR_IGUAL;
                return(tl);
            }
            else{
                devolverCaracter(1);  //el leido previamente que no era.
                tl.lexema = siguienteLexema();
                tl.componenteLexico = (int) (tl.lexema[0]);  
                return(tl);
            }
            break;
        default:
            break;
        }
    }
    //nunca debería llegar aquí. En caso de que lo haga es por un error de caracter extraño.
    //Devolvemos tl con componente léxico -1.
    return(tl);
}


int automataAlfanumerico(){
    char c;
    do{
        c = siguienteChar();
    }while(isalpha(c) || c=='_' || isdigit(c));
    return(1);
}

int automataComentario(){
    char c = siguienteChar();
    char d = siguienteChar();
    //comentario del tipo /* */
    if(c=='*'){
        do{
            c = d;
            d = siguienteChar();
            siguienteLexema();
        }while(c!='*' || d!='/');
        return(1);
    }
    //comentario del tipo /+ +/
    if(c=='+'){
        do{
            c = d;
            d = siguienteChar();
            //Llamada recursiva. Como el primer elemento tiene que ser el siguiente a /. Retrocedemos 1 el puntero
            //del sistema de entrada.
            if(c=='/' && d=='+'){
                devolverCaracter(1);
                automataComentario();
            }
            siguienteLexema();
        }while(c!='+' || d!='/');
        return(1);
    }
    //Comentario en línea tipo //
    if(c=='/'){
        c=d;
        while(c!='\n'){
            c = siguienteChar();
            siguienteLexema();
        }
        return(1);
    }
    //si no era comentario, reseteamos.
    devolverCaracter(2);
    return(0);
}

int automataString(){
    char c;
    do{
        c=siguienteChar();
        if(c=='\\')
            siguienteChar();
    }while(c!='"');
    return(1);
}

int automataNumeros(char primero){
    int estado=0;
    char c = siguienteChar();
    int fin=0;
    int tipo=1; //1 entero, 0 flotante.
    int vecesMASoMENOS = 0;

    while(fin==0){
        switch (estado){
            //primer caracter leído.
            case 0:
                if(c=='.')        //flotante básico de inicio
                    estado=1;
                else if(primero=='0' && (c=='b' || c=='B'))   //entero binario (no existe binario flotante)
                    estado=2;
                else if(primero == '0' && (c=='x' || c=='X'))   //hexadecimal (discernir entre flotante o binario)
                    estado=3;
                else if(isdigit(c))   //entero normal o flotante
                    estado=4;
                else if(c=='_'){
                    c = siguienteChar();
                    break;
                }       
                else if(c=='e' || c=='E'){
                    estado=5;
                }
                else if(!isdigit(c)){
                    devolverCaracter(1);
                    return(1);
                }
                else{
                    fin=1;
                }
                break;

            //Estado1: flotante no hexadecimal
            case 1:
                do{
                    c = siguienteChar();
                    if(c=='e' || c=='E'){
                        estado=5;
                    }
                }while(isdigit(c) || c=='_');
                if(estado!=5){
                    devolverCaracter(1);
                    return(0);
                }
                break;

            //Estado2: entero binario
            case 2:
                do{
                    c = siguienteChar();
                }while(c=='0' || c=='1' || c=='_');
                devolverCaracter(1);
                return(1);

            //Estado3: Discernir entre hexadecimal entero o flotante (no incluido notación científica con p)
            case 3:
                do{
                    c = siguienteChar();
                    if(c=='.' && tipo==1){
                        c = siguienteChar();
                        tipo=0;
                    }
                }while(isdigit(c) || c=='a' || c=='A' || c=='b' || c=='B' || c=='c' || c=='C' || c=='d' || c=='D' ||
                            c=='e' || c=='E' || c=='f' || c=='F' || c=='_');
                devolverCaracter(1);
                return(tipo);
            
            //Estado4: entero normal o flotante no . inicial
            case 4:
                do{
                    c = siguienteChar();
                    if(c=='.')
                        estado=1;
                    else if(c=='e' || c=='E'){
                        estado=5;
                    }
                }while(isdigit(c) || c=='_');
                if(estado!=1 && estado!=5){
                    devolverCaracter(1);
                    return(1);
                }
                break;

            //Estado5: valor después de exponente, notación ciéntifica.
            case 5:
                c = siguienteChar();
                if(!isdigit(c) && c!='+' && c!='-'){
                    imprimeError(5,linea);
                    return(-1);
                }
                devolverCaracter(1);
                do{
                    c = siguienteChar();
                    if(c=='+' || c=='-'){
                        vecesMASoMENOS++;
                    }
                }while(isdigit(c) || c=='_' || ((c=='+' || c=='-') && vecesMASoMENOS<2));
                devolverCaracter(1);
                return(0);
                
            default:
                break;
        }
    }

    return(-1);
}
