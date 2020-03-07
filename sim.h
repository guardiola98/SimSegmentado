#include <stdio.h>


#define ESPACIO

//DEFINIMOS LAS OPERACIONES
#define     ADD     0
#define     SUB     1
#define     ADDI    2
#define     SUBI    3
#define     MULT    4
#define     LW      5
#define     SW      6
#define     NOP     7
#define     RAM     512
#define     TAM     40


#define     RIESGO_ARITMETICO_A 1
#define     RIESGO_ARITMETICO_B 2
#define     RIESGO_ARITMETICO_AB 3

typedef struct {
    int operacion;
    int ra;//lo que valga ra, rb o rc, sera el identificativo del registro, el valor estara en el banco de registros
    int rb;
    int rdest;
    int inmediato;
}instruccion;

struct registro_segmentacion{
    int cod;
    int ra;
    int rb;
    int rdest;
    int mem;
    int opra;
    int oprb;
    int inmediato;
    int resultado;
};

/*
 IF->   -lectura instrucción en mem inst (lee del vector de instrucciones)
		-almacenamiento en RS_IF_ID
		-Incremento PC
*/
void etapa_if(int i);


/*
ID->    -Fuente: RS_IF_ID
		-Decodificacion y lectura de operandos
		-almacenamiento en RS_ID_EX, se pasa la informacion del registro anterior más lo nuevo

*/
void etapa_id();

/*
EX->    -Fuente: RS_ID_EX
		-realiza la operacón
		-almacenamiento en RS_EX_MEM, se pasa la informacion del registro anterior más lo nuevo

*/

void etapa_ex();

/*
MEM->   -Fuente: RS_EX_MEM
		-solo la utilizan lw y sw
		-almacenamiento en RS_MEM_WB, se pasa la informacion del registro anterior más lo nuevo (si no es lw o sw, simplemente se pasa lo anterior pq no hay nada nuevo)

*/

void etapa_mem();

/*
WB->-Fuente: RS_MEM_WB
		-escribe en el registro correspondiente, en el banco de registros 

*/

void etapa_wb();
