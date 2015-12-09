/*
 * Motor.h
 *
 *  Created on: 27 de oct. de 2015
 *      Author: agustin
 */

#ifndef MOTOR_H_
#define MOTOR_H_

#include "Arduino.h"

class Motor{

	public:
		void Mover(int PASOS);
		void Step(void);
		void SetPin(int STEP_PIN_, int DIR_PIN_, int ENABLE_PIN_);
		void Init(int STEP_PIN_, int DIR_PIN_, int ENABLE_PIN_, int32_t posicion);
		void SetDelay(int TIME);
		int32_t getPosicion();
		void NuevaTarea(int32_t PASOS);
		bool Ocupado();
		void CargarTarea();
		void setZero();

	private:

		struct tarea{
			int32_t Steps;
			tarea *proximo;
		};

		tarea *pri = NULL;
		tarea *ult = NULL;

		int ENABLE_PIN;
		int STEP_PIN;
		int32_t STEP_BUFF;
		int32_t POSICION;
		int DIR_PIN;
		int DELAY_BUFF;
		int DELAY;
		bool INVERTIR;

		tarea* AddTarea(tarea* ultimo, int32_t PASOS);
		int32_t getTarea();

};

#endif /* TIMER_H_ */
