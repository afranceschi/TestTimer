/*
 * Motor.cpp
 *
 *  Created on: 27 de oct. de 2015
 *      Author: agustin
 */

#include "Motor.h"

void Motor::Mover(int PASOS){
	STEP_BUFF = PASOS * 16;
}

void Motor::Step(void){
	if(STEP_BUFF != 0) digitalWrite(ENABLE_PIN,LOW); else digitalWrite(ENABLE_PIN,HIGH);
	//Serial.println(digitalRead(ENABLE_PIN));
	digitalWrite(13,!digitalRead(ENABLE_PIN));
	if(STEP_BUFF >= 1){
		if(INVERTIR == 0) digitalWrite(DIR_PIN,HIGH); else digitalWrite(DIR_PIN,LOW);
		if(DELAY_BUFF >= DELAY){
			digitalWrite(STEP_PIN,HIGH);
			digitalWrite(STEP_PIN,LOW);
			//Serial.println(STEP_BUFF);
			POSICION++;
			DELAY_BUFF = 0;
			STEP_BUFF--;
		}else{
			DELAY_BUFF++;
		}
	}else if(STEP_BUFF <= -1){
		if(INVERTIR == 1) digitalWrite(DIR_PIN,HIGH); else digitalWrite(DIR_PIN,LOW);
		if(DELAY_BUFF >= DELAY){
			digitalWrite(STEP_PIN,HIGH);
			digitalWrite(STEP_PIN,LOW);
			POSICION--;
			DELAY_BUFF = 0;
			STEP_BUFF++;
		}else{
			DELAY_BUFF++;
		}
	}
}

void Motor::SetPin(int STEP_PIN_, int DIR_PIN_, int ENABLE_PIN_){
	STEP_PIN = STEP_PIN_;
	DIR_PIN = DIR_PIN_;
	ENABLE_PIN = ENABLE_PIN_;
}

void Motor::Init(int STEP_PIN_, int DIR_PIN_, int ENABLE_PIN_, int32_t posicion){
	STEP_PIN = STEP_PIN_;
	DIR_PIN = DIR_PIN_;
	ENABLE_PIN = ENABLE_PIN_;
	POSICION = posicion;
}

void Motor::setZero(){
	POSICION = 0;
}

void Motor::SetDelay(int TIME){
	DELAY = TIME;
}

void Motor::NuevaTarea(int32_t PASOS){
	//noInterrupts();
	TIMSK5 = 0;
	//Serial.println("Timer desactivado");
	//delay(3000);
	//Serial.print("Agregando tarea: ");
	//Serial.println(PASOS);
	//Serial.print("Ultimo: ");
	//Serial.println((long) ult);

	ult = AddTarea(ult,PASOS);
	//Serial.print("Ultimo Despues: ");
	//Serial.println((long) ult);

	//Serial.print("Primero: ");
	//Serial.println((long) pri);
	if(pri == NULL) pri = ult;
	//Serial.print("Ultimo: ");
	//Serial.print((int32_t) ult->Steps);
	//Serial.print(" en ");
	//Serial.println((int32_t) ult);
	//interrupts();
	//Serial.println("Timer activado");
	TIMSK5 |= (1 << OCIE1A);
}

void Motor::CargarTarea(){
	STEP_BUFF = getTarea();
}

Motor::tarea* Motor::AddTarea(Motor::tarea* ultimo, int32_t PASOS){
	tarea* p;
	p = (tarea*) malloc(sizeof(tarea));
	(*p).Steps = PASOS;
	(*p).proximo = NULL;
	//Serial.print("Dato a guardar: ");
	//Serial.println(p->Steps);
	if(ultimo!=NULL) (*ultimo).proximo = p;
	Serial.print("Guardando datos en cola: ");
	Serial.print(PASOS);
	Serial.print(" en ");
	Serial.println((int)p);
	return p;
}


int32_t Motor::getTarea()
{
    tarea *aux;
    int32_t z = -1;
    if(pri!=NULL){
    	aux=pri;
    	z = (*aux).Steps;

    	Serial.print("Recuperando datos en cola: ");
    	Serial.print(z);
    	Serial.print(" en ");
    	Serial.print((long)aux);
    	Serial.print(" proximo ");
    	Serial.println((int)(*aux).proximo);

    	//Serial.print("Direccion de pri: ");
    	//Serial.println((int)aux);
        pri=(*aux).proximo;
        if(pri == NULL) ult = pri;
        //Serial.print("Direccion de pri camb.: ");
        //Serial.println((int)pri);
        free(aux);
    }else{
    	ult = NULL;
    	z = 0;
    	//Serial.println("No hay datos en cola");
    }
    //Serial.print("Recuperando datos en cola: ");
    //Serial.println(z);
    return z;
}

int32_t Motor::getPosicion(){
	return POSICION;
}

bool Motor::Ocupado(){
	if(!digitalRead(ENABLE_PIN)) return true; else return false;
}
