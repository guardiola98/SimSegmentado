#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "sim.h"


struct registro_segmentacion RS_IF_ID;
struct registro_segmentacion RS_ID_EX;
struct registro_segmentacion RS_EX_MEM;
struct registro_segmentacion RS_MEM_WB;
instruccion mem_ins[TAM];
int mem_principal[RAM];

int banco_registros[10];

//Cuando se carga en los registros el dato inmediato??? suponog que solo puede en la etapa IF
void etapa_if(int i){
    //mem_ins[i];
    RS_IF_ID.cod=mem_ins[i].operacion;
    RS_IF_ID.ra=mem_ins[i].ra;
    RS_IF_ID.rb=mem_ins[i].rb;
    RS_IF_ID.rdest=mem_ins[i].rdest;   char str[80] = "This is - www.tutorialspoint.com - website";
    RS_IF_ID.inmediato=mem_ins[i].inmediato;

    //los operandos no los queremos en esta etapa
}

void etapa_id(){
    //leemos de RS_IF_ID
    
    RS_ID_EX.cod=RS_IF_ID.cod;
    RS_ID_EX.ra=RS_IF_ID.ra;
    RS_ID_EX.rb=RS_IF_ID.rb;
    RS_ID_EX.rdest=RS_IF_ID.rdest;
    RS_ID_EX.inmediato=RS_IF_ID.inmediato;


    //DECODIFICAMOS y leemos operando, con esto le pasamos los operandos a la etapa
    RS_ID_EX.opra=banco_registros[RS_ID_EX.ra];
    RS_ID_EX.oprb=banco_registros[RS_ID_EX.rb];

}

void etapa_ex(){
    //leemos de RS_ID_EX
    //Antes de nada miramos por ejemplo si RS_ID_EX.ra== RS_EX_MEM.rdest ---> RS_ID_EX.opra == RS_EX_MEM.rdest
    

    //Riesgo de datos del registro a en la anterior instruccion
    if(RS_ID_EX.ra == RS_EX_MEM.rdest)
    {
        RS_ID_EX.opra == RS_EX_MEM.rdest;
    }else
    {
        RS_EX_MEM.opra=RS_ID_EX.opra;

    }
    
    
    //Riesgo de datos del registro b en las anterior instruccion

    if (RS_ID_EX.rb == RS_EX_MEM.rdest)
    {
        RS_ID_EX.oprb == RS_EX_MEM.rdest;

    }else
    {
        RS_EX_MEM.oprb=RS_ID_EX.oprb;

    }
    
    RS_EX_MEM.cod=RS_ID_EX.cod;
    RS_EX_MEM.ra=RS_ID_EX.ra;
    RS_EX_MEM.rb=RS_ID_EX.rb;
    RS_EX_MEM.rdest=RS_ID_EX.rdest;
    RS_EX_MEM.inmediato=RS_ID_EX.inmediato;

    switch (RS_EX_MEM.cod){
        case ADD:
            RS_EX_MEM.resultado=RS_EX_MEM.opra + RS_EX_MEM.inmediato;
            break;
        case SUB:
            RS_EX_MEM.resultado=RS_EX_MEM.opra - RS_EX_MEM.inmediato;
            break;
        case ADDI:
            RS_EX_MEM.resultado=RS_EX_MEM.opra + RS_EX_MEM.inmediato;
            //SUMA CON EL INMEDIATO
            break;
        case SUBI:
            RS_EX_MEM.resultado=RS_EX_MEM.opra - RS_EX_MEM.inmediato;
            //RESTA CON EL INMEDIATO
            break;
        case MULT:
            RS_EX_MEM.resultado=RS_EX_MEM.opra * RS_EX_MEM.inmediato;
            break;
        case LW:
            RS_EX_MEM.resultado=RS_EX_MEM.oprb + RS_EX_MEM.inmediato;
            break;
        case SW:
            RS_EX_MEM.resultado=RS_EX_MEM.oprb + RS_EX_MEM.inmediato;
            break;
            
    }
        





}

void etapa_mem(){
    RS_MEM_WB.cod=RS_EX_MEM.cod;
    RS_MEM_WB.ra=RS_EX_MEM.ra;
    RS_MEM_WB.rb=RS_EX_MEM.rb;
    RS_MEM_WB.rdest=RS_EX_MEM.rdest;
    RS_MEM_WB.opra=RS_EX_MEM.opra;
    RS_MEM_WB.oprb=RS_EX_MEM.oprb;
    RS_MEM_WB.inmediato=RS_EX_MEM.inmediato;

    if(RS_MEM_WB.cod==LW) {
        //Actualizamos el valor del resultado, de manera que el resultado de la etapa anterior
        //es la direccion de memoria de  la que leemos el valor que vamos a almacenar en
        //el resultado de esta etapa
        RS_MEM_WB.resultado = mem_principal[RS_EX_MEM.resultado];
    } else if (RS_MEM_WB.cod == SW) {
        mem_principal[RS_MEM_WB.resultado] = RS_MEM_WB.opra;
    }
}

void etapa_wb(){
    banco_registros[RS_MEM_WB.rdest] = RS_MEM_WB.resultado;
}

/*
void generar_instrucciones(char * fichero, instruccion mem_ins[TAM])
{

}
*/

int main(int argc,char* argv[] ){

    int n_ins=10;
    int i=0;
    int riesgo=0;
    char * fichero;


    fichero = argv[1];
    char linea[128];

    FILE *fich;
    int contador =0;
    char* delim=" ";
    
    fich = fopen(fichero, "r");
    char *puntero;
    char *iteracion;
    while(fgets(linea, 128, (FILE*) fich)){


        char *p1, *p2;
        p1 = linea;

        int iteracion=0;
        while(p2 = strchr(p1, ' '))//vamos leyendo los datos de la instruccion.
        {
            *p2++ = '\0';

            if(iteracion==0)
            {
                printf("Iteracion 0\n");
                printf("valor de p1 %s\n", p1);
                if(!strcmp(p1, "add")){//ADD
                    printf("Estoy en add ---> ");
                    mem_ins[contador].operacion=ADD;
                    printf("%d\n",mem_ins[contador].operacion);
                }else if (!strcmp(p1, "sub"))
                {
                    printf("Estoy en sub --->");

                    mem_ins[contador].operacion=SUB;
                    printf("%d\n",mem_ins[contador].operacion);
                }else if(!strcmp(p1, "addi"))
                {
                    mem_ins[contador].operacion=ADDI;
                }else if (!strcmp(p1, "subi"))
                {
                    mem_ins[contador].operacion=SUBI;
                }else if (!strcmp(p1, "mult"))
                {
                    mem_ins[contador].operacion=MULT;
                }else if (!strcmp(p1, "lw"))
                {
                    mem_ins[contador].operacion=LW;
                }else if (!strcmp(p1, "sw"))
                {
                    mem_ins[contador].operacion=SW;
                }else if (!strcmp(p1, "nop") || !strcmp(p1, "NOP") )
                {
                    mem_ins[contador].operacion=NOP;
                }
                
            }else if (iteracion==1)
            {
                printf("Iteracion 1\n");
                //asumimos que tiene como maximo 9 registros
                mem_ins[contador].rdest=(int) p1[1];
                if(mem_ins[contador].operacion==LW || mem_ins[contador].operacion==SW)
                {
                    p1=p2;
                    iteracion++;
                }
            }
            if (iteracion==2)
            {
                printf("Iteracion 2\n");
                if(mem_ins[contador].operacion!=LW && mem_ins[contador].operacion!=SW)
                {
                    mem_ins[contador].ra=(int) p1[1];
                    p1=p2;
                    iteracion++;
                }else{

                    //Leemos dato inmediato
                    p2=strchr(p1,'(');
                    *p2++='\0';
                    mem_ins[contador].inmediato=atoi(p1);
                    //actualizamos el puntero
                    p1=p2;
                    *p1++;
                    //leemos el numero de registro
                    mem_ins[contador].rb=atoi(p1);
                }
            } 
            if(iteracion==3){//no fa aquesta iteració
                if(mem_ins[contador].operacion!=LW && mem_ins[contador].operacion!=SW)
                {
                    printf("Iteracion 3\n");
                    //Comprobamos si es o no, dato inmediato
                    if(p1[0]=='r')
                    {
                        mem_ins[contador].rb=(int) p1[1];

                    }else
                    {
                        *p1++;
                        mem_ins[contador].inmediato=atoi(p1);                       
                    }
                    
                }  

            }
            
            
            p1=p2;
            iteracion++;
            
        }
        
        printf("OPERACION: %d\n", mem_ins[contador].operacion);

        contador++;
        
        
    }
    /*
    while(i<n_ins){

        etapa_wb();
        etapa_mem();
        etapa_ex();
         if(RS_IF_ID.ra == RS_ID_EX.rdest && RS_IF_ID.rb == RS_EX_MEM.rdest) {
            RS_IF_ID.opra == RS_ID_EX.resultado;
            RS_IF_ID.oprb == RS_EX_MEM.resultado;
            riesgo = RIESGO_ARITMETICO_AB;

        } else if (RS_IF_ID.rb == RS_ID_EX.rdest && RS_IF_ID.ra == RS_EX_MEM.rdest){
            RS_IF_ID.oprb == RS_ID_EX.resultado;
            RS_IF_ID.opra == RS_EX_MEM.resultado;
            riesgo = RIESGO_ARITMETICO_AB;
        } else if ()
        {
            
        } 
        
        
        etapa_id();
        etapa_if(i);
        i++;

    }
    */
    //EL WHILE DEBEMOS HACERLO DE WB A IF

}