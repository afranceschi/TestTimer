// Do not remove the include below
#include "TestTimer.h"
#include "Motor.h"

void Interprete(String Command, String Arg);
void Timer_Start();
int32_t String_to_Integer(String VAR);

bool FLAG_COMANDO;
bool FLAG_COM_R;
bool FLAG_FIN;

char Dato_Leido;
String COMANDO;
String ARGUMENTO;

Motor mx;

//The setup function is called once at startup of the sketch
void setup()
{
	pinMode(13,OUTPUT);
	pinMode(X_STEP_PIN, OUTPUT);
	pinMode(X_DIR_PIN, OUTPUT);
	pinMode(X_ENABLE_PIN, OUTPUT);

	digitalWrite(X_STEP_PIN,LOW);
	digitalWrite(X_DIR_PIN,LOW);
	digitalWrite(X_ENABLE_PIN,HIGH);

	Serial.begin(9600);

	X_STEP_BUFF = 0;
	X_DELAY_BUFF = 100;
	FLAG_COMANDO = false;
	FLAG_COM_R = false;
	FLAG_FIN = false;
	Dato_Leido = 0;

	COMANDO = "";
	ARGUMENTO = "";

	mx.Init(X_STEP_PIN,X_DIR_PIN,X_ENABLE_PIN,0);

	mx.SetDelay(5);

	//mx.NuevaTarea(3200);
	//mx.NuevaTarea(-3200);
	//mx.NuevaTarea(2);
	//mx.NuevaTarea(3);
	//mx.NuevaTarea(4);
	//mx.NuevaTarea(5);

	/*mx.CargarTarea();
	mx.CargarTarea();
	mx.CargarTarea();
	mx.CargarTarea();
	mx.CargarTarea();
	mx.CargarTarea();
	mx.CargarTarea();
	mx.CargarTarea();*/

	Timer_Start();
}

// The loop function is called in an endless loop
void loop()
{

	if(Serial.available()){
		if(!FLAG_FIN) Dato_Leido = Serial.read();
		//Serial.write(Dato_Leido);
		if(Dato_Leido != '\n' && !FLAG_FIN){
			if(!FLAG_COMANDO){
				if(Dato_Leido != 32) COMANDO = COMANDO + Dato_Leido; else FLAG_COMANDO = true;
			}else{
				if(Dato_Leido != 32) ARGUMENTO = ARGUMENTO + Dato_Leido; else FLAG_FIN = true;
			}
		}else{
			FLAG_COM_R = true;
			//Serial.flush();
			//Serial.println("OK");
		}
	}

	if(FLAG_COM_R){
		Interprete(COMANDO,ARGUMENTO);
		COMANDO = "";
		ARGUMENTO = "";
		FLAG_COM_R = false;
		FLAG_COMANDO = false;
		FLAG_FIN = false;
	}
	//Mover_Motores();

	/*digitalWrite(13,!digitalRead(X_ENABLE_PIN));
	mx.Step();
	if(!mx.Ocupado()) mx.CargarTarea();
*/

}

void Interprete(String Command, String Arg){
	Command.toUpperCase();
	Arg.toUpperCase();

	int32_t argnum = String_to_Integer(Arg);

	#ifdef DEBUG
		Serial.print("Comando: ");
		Serial.print(Command);
		Serial.print(" (");
		Serial.print(String_to_Integer(Command));
		Serial.print(" ) ");
		Serial.print("   Argumento: ");
		Serial.print(Arg);
		Serial.print(" (");
		Serial.print(argnum);
		Serial.println(" )");
	#endif

	if(Command == "V1" && argnum != -1){
		Serial.print("Variando velocidad de motor 1 a: ");
		Serial.println(argnum);
		mx.SetDelay(argnum);
	}else if(Command == "G1" && argnum != -1){
		/*Serial.print("Moviendo motor 1 ");
		Serial.print(argnum);
		Serial.println(" pasos.");*/
		mx.NuevaTarea(argnum);
		//mx.Mover(argnum);
	}else if(Command == "POS1"){
		Serial.print("Posicion de Motor 1: ");
		Serial.println(mx.getPosicion());
	}else if(Command == "SET1"){
		mx.setZero();
		Serial.println("Motor 1 en posicion inicial.");
	}else if(Command == "S"){
		Timer_Start();
	}else{
		Serial.print("Comando Incorrecto");
	}
}

void Timer_Start(){
	TCCR5B = 0;
	TCCR5B = 0;
	TCNT5 = 0;
	noInterrupts();
	OCR5A = 16;
	interrupts();

	TCCR5B |= (1 << WGM12);
	TCCR5B |= (1 << CS10) ;
	TIMSK5 |= (1 << OCIE1A);
}

int32_t String_to_Integer(String VAR){
	int32_t z = 0;
	int aux = 0;
	unsigned int i = 0;

	while(i < VAR.length()){
		aux = VAR.c_str()[i];
		if(i == 0 && aux == '-'){
			i++;
			aux = VAR.c_str()[i];
		}
		if(aux >= 0x30 && aux <= 0x39 && z != -1){
			z *= 10;
			z += (aux - 0x30);
		}else{
			z = -1;
		}
		i++;
	}
	if(VAR.c_str()[0] != '-') return z; else return z * -1;
}

ISR(TIMER5_COMPA_vect){
	mx.Step();
	if(!mx.Ocupado()) mx.CargarTarea();
}
