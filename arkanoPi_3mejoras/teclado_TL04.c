#include "teclado_TL04.h"

char tecladoTL04[4][4] = {
	{'1', '2', '3', 'C'},
	{'4', '5', '6', 'D'},
	{'7', '8', '9', 'E'},
	{'A', '0', 'B', 'F'}
};
int debounceTime[NUM_FILAS_TECLADO] = {0,0,0,0};

// Maquina de estados: lista de transiciones
// {EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
fsm_trans_t fsm_trans_excitacion_columnas[] = {
	{ TECLADO_ESPERA_COLUMNA, CompruebaTimeoutColumna, TECLADO_ESPERA_COLUMNA, TecladoExcitaColumna },
	{-1, NULL, -1, NULL },
};

fsm_trans_t fsm_trans_deteccion_pulsaciones[] = {
	{ TECLADO_ESPERA_TECLA, CompruebaTeclaPulsada, TECLADO_ESPERA_TECLA, ProcesaTeclaPulsada},
	{-1, NULL, -1, NULL },
};

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaTeclado(TipoTeclado *p_teclado) {
	// A completar por el alumno...
	// ...


		// Comenzamos excitacion por primera columna
		p_teclado->columna_actual = COLUMNA_1;

		// Inicialmente no hay tecla pulsada
		p_teclado->teclaPulsada.col = -1;
		p_teclado->teclaPulsada.row = -1;

		pinMode (GPIO_KEYBOARD_ROW_1, INPUT);
		pullUpDnControl(GPIO_KEYBOARD_ROW_1, PUD_DOWN);
		wiringPiISR(GPIO_KEYBOARD_ROW_1, INT_EDGE_RISING, teclado_fila_1_isr);

		pinMode (GPIO_KEYBOARD_ROW_2, INPUT);
		pullUpDnControl(GPIO_KEYBOARD_ROW_2, PUD_DOWN);
		wiringPiISR (GPIO_KEYBOARD_ROW_2, INT_EDGE_RISING, teclado_fila_2_isr);

		pinMode (GPIO_KEYBOARD_ROW_3, INPUT);
		pullUpDnControl(GPIO_KEYBOARD_ROW_3, PUD_DOWN);
		wiringPiISR (GPIO_KEYBOARD_ROW_3, INT_EDGE_RISING, teclado_fila_3_isr);

		pinMode (GPIO_KEYBOARD_ROW_4, INPUT);
		pullUpDnControl(GPIO_KEYBOARD_ROW_4, PUD_DOWN);
		wiringPiISR (GPIO_KEYBOARD_ROW_4, INT_EDGE_RISING, teclado_fila_4_isr);

		pinMode (GPIO_KEYBOARD_COL_1, OUTPUT);
		digitalWrite (GPIO_KEYBOARD_COL_1, HIGH);

		pinMode (GPIO_KEYBOARD_COL_2, OUTPUT);
		digitalWrite (GPIO_KEYBOARD_COL_2, LOW);

		pinMode (GPIO_KEYBOARD_COL_3, OUTPUT);
		digitalWrite (GPIO_KEYBOARD_COL_3, LOW);

		pinMode (GPIO_KEYBOARD_COL_4, OUTPUT);
		digitalWrite (GPIO_KEYBOARD_COL_4, LOW);

		p_teclado->tmr_duracion_columna = tmr_new (timer_duracion_columna_isr);
		tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), TIMEOUT_COLUMNA_TECLADO);

		printf("\nComienza el juego!! Pulsa 0 para empezar\n");
		fflush(stdout);


}

//------------------------------------------------------
// OTROS PROCEDIMIENTOS PROPIOS DE LA LIBRERIA
//------------------------------------------------------

void ActualizaExcitacionTecladoGPIO (int columna) {
	// A completar por el alumno
	// ...



	switch(columna){
		// ...
	case COLUMNA_1:
		digitalWrite (GPIO_KEYBOARD_COL_4, LOW);
		digitalWrite (GPIO_KEYBOARD_COL_1, HIGH);
		digitalWrite (GPIO_KEYBOARD_COL_2, LOW);
		digitalWrite (GPIO_KEYBOARD_COL_3, LOW);


		break;

	case COLUMNA_2:
		digitalWrite (GPIO_KEYBOARD_COL_1, LOW);
		digitalWrite (GPIO_KEYBOARD_COL_2, HIGH);
		digitalWrite (GPIO_KEYBOARD_COL_3, LOW);
		digitalWrite (GPIO_KEYBOARD_COL_4, LOW);

	    break;

	case COLUMNA_3:
		digitalWrite (GPIO_KEYBOARD_COL_2, LOW);
		digitalWrite (GPIO_KEYBOARD_COL_3, HIGH);
		digitalWrite (GPIO_KEYBOARD_COL_4, LOW);
		digitalWrite (GPIO_KEYBOARD_COL_1, LOW);

	    break;

	case COLUMNA_4:
		digitalWrite (GPIO_KEYBOARD_COL_3, LOW);
		digitalWrite (GPIO_KEYBOARD_COL_4, HIGH);
		digitalWrite (GPIO_KEYBOARD_COL_1, LOW);
		digitalWrite (GPIO_KEYBOARD_COL_2, LOW);

	    break;

	}



}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaTimeoutColumna (fsm_t* this) {
	int result = 0;
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completar por el alumno...
	// ...
	piLock (SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_TIMEOUT_COLUMNA_TECLADO);
	piUnlock (SYSTEM_FLAGS_KEY);


	return result;
}

int CompruebaTeclaPulsada (fsm_t* this) {
	int result = 0;
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completar por el alumno
	// ...
	piLock (SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_TECLA_PULSADA);
	piUnlock (SYSTEM_FLAGS_KEY);


	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LAS MAQUINAS DE ESTADOS
//------------------------------------------------------

void TecladoExcitaColumna (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completar por el alumno
	// ...
	piLock(SYSTEM_FLAGS_KEY);
	flags &= (~FLAG_TIMEOUT_COLUMNA_TECLADO);
	piUnlock(SYSTEM_FLAGS_KEY);

	piLock(KEYBOARD_KEY);
	tmr_startms((tmr_t*)(p_teclado->tmr_duracion_columna), TIMEOUT_COLUMNA_TECLADO);
	p_teclado->columna_actual= p_teclado->columna_actual +1;
	if (p_teclado->columna_actual>COLUMNA_4){
		p_teclado->columna_actual=COLUMNA_1;
	}
	ActualizaExcitacionTecladoGPIO(p_teclado->columna_actual);
	piUnlock(KEYBOARD_KEY);
	// Llamada a ActualizaExcitacionTecladoGPIO con columna a activar como argumento
}

void ProcesaTeclaPulsada (fsm_t* this) {
	TipoTeclado *p_teclado;
	p_teclado = (TipoTeclado*)(this->user_data);

	// A completar por el alumno
	// ...


	    piLock(SYSTEM_FLAGS_KEY);
		flags &= (~FLAG_TECLA_PULSADA);
		 piUnlock(SYSTEM_FLAGS_KEY);

		 piLock(KEYBOARD_KEY);
		switch(p_teclado->teclaPulsada.col){
			case COLUMNA_1:
				if(p_teclado->teclaPulsada.row==FILA_1){
				     // printf ("\nKeypress1!!!\n");
						//fflush(stdout);
						p_teclado->teclaPulsada.row =-1;
						p_teclado->teclaPulsada.col =-1;
						}
				if(p_teclado->teclaPulsada.row==FILA_2){
				      //printf ("\nKeypress4!!!\n");
						//fflush(stdout);
						p_teclado->teclaPulsada.row =-1;
						p_teclado->teclaPulsada.col =-1;
						}
				if(p_teclado->teclaPulsada.row==FILA_3){
				      //printf ("\nKeypress7!!!\n");
						//fflush(stdout);
						p_teclado->teclaPulsada.row =-1;
						p_teclado->teclaPulsada.col =-1;
						}

				if(p_teclado->teclaPulsada.row==FILA_4){
					   piLock(SYSTEM_FLAGS_KEY);
					   flags |= (FLAG_MOV_IZQUIERDA);
					   piUnlock(SYSTEM_FLAGS_KEY);
				      // printf ("\nKeypressA!!!\n");
						//fflush(stdout);
 						p_teclado->teclaPulsada.row =-1;
						p_teclado->teclaPulsada.col =-1;
						}
			break;
			case COLUMNA_2:
				if(p_teclado->teclaPulsada.row==FILA_1){
				     // printf ("\nKeypress2!!!\n");
						//fflush(stdout);
						p_teclado->teclaPulsada.row =-1;
						p_teclado->teclaPulsada.col =-1;
						}
				if(p_teclado->teclaPulsada.row==FILA_2){
				     // printf ("\nKeypress5!!!\n");
						//fflush(stdout);
						p_teclado->teclaPulsada.row =-1;
						p_teclado->teclaPulsada.col =-1;
						}
				if(p_teclado->teclaPulsada.row==FILA_3){
				      //printf ("\nKeypress8!!!\n");
						//fflush(stdout);
						p_teclado->teclaPulsada.row =-1;
						p_teclado->teclaPulsada.col =-1;
						}

				if(p_teclado->teclaPulsada.row==FILA_4){
					piLock(SYSTEM_FLAGS_KEY);
					flags |= (FLAG_BOTON);
					piUnlock(SYSTEM_FLAGS_KEY);
				     // printf ("\nKeypress0!!!\n");
						//fflush(stdout);
						p_teclado->teclaPulsada.row =-1;
						p_teclado->teclaPulsada.col =-1;
						}
			break;

			case COLUMNA_3:
				if(p_teclado->teclaPulsada.row==FILA_1){
				      //printf ("\nKeypress3!!!\n");
						//fflush(stdout);
						p_teclado->teclaPulsada.row =-1;
						p_teclado->teclaPulsada.col =-1;
						}
				if(p_teclado->teclaPulsada.row==FILA_2){
				      //printf ("\nKeypress6!!!\n");
						//fflush(stdout);
						p_teclado->teclaPulsada.row =-1;
						p_teclado->teclaPulsada.col =-1;
						}
				if(p_teclado->teclaPulsada.row==FILA_3){
				      //printf ("\nKeypress9!!!\n");
						//fflush(stdout);
						p_teclado->teclaPulsada.row =-1;
						p_teclado->teclaPulsada.col =-1;
						}

				if(p_teclado->teclaPulsada.row==FILA_4){
				      //printf ("\nKeypressB!!!\n");
						//fflush(stdout);
						p_teclado->teclaPulsada.row =-1;
						p_teclado->teclaPulsada.col =-1;
						}
			break;

			case COLUMNA_4:
				if(p_teclado->teclaPulsada.row==FILA_1){
				      //printf ("\nKeypressC!!!\n");
						//fflush(stdout);
						p_teclado->teclaPulsada.row =-1;
						p_teclado->teclaPulsada.col =-1;
						}
				if(p_teclado->teclaPulsada.row==FILA_2){
			    	piLock(SYSTEM_FLAGS_KEY);
			    	flags |= (FLAG_MOV_DERECHA);
			    	piUnlock(SYSTEM_FLAGS_KEY);
				      //printf ("\nKeypressD!!!\n");
						//fflush(stdout);
						p_teclado->teclaPulsada.row =-1;
						p_teclado->teclaPulsada.col =-1;
						}
				if(p_teclado->teclaPulsada.row==FILA_3){
				      //printf ("\nKeypressE!!!\n");
						//fflush(stdout);
						p_teclado->teclaPulsada.row =-1;
						p_teclado->teclaPulsada.col =-1;
						}

				if(p_teclado->teclaPulsada.row==FILA_4){
				      //printf ("\nKeypressF!!!\n");
						//fflush(stdout);
						p_teclado->teclaPulsada.row =-1;
						p_teclado->teclaPulsada.col =-1;
						}
			break;

			default:
				printf("\nERROR!!!! invalid number of column (%d)!!!\n", p_teclado->teclaPulsada.col);
				fflush(stdout);

				p_teclado->teclaPulsada.row = -1;
				p_teclado->teclaPulsada.col = -1;

				break;
		}
		piUnlock(KEYBOARD_KEY);


}


//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void teclado_fila_1_isr (void) {
	// A completar por el alumno
	// ...
	if (millis () < debounceTime[FILA_1]) {
		debounceTime[FILA_1] = millis () + DEBOUNCE_TIME ;
			return;
		}

		piLock (KEYBOARD_KEY);

		teclado.teclaPulsada.row = FILA_1;
		teclado.teclaPulsada.col = teclado.columna_actual;
		piUnlock (KEYBOARD_KEY);

		piLock (SYSTEM_FLAGS_KEY);
		flags |= FLAG_TECLA_PULSADA;
		piUnlock (SYSTEM_FLAGS_KEY);



		debounceTime[FILA_1] = millis () + DEBOUNCE_TIME ;
	}


void teclado_fila_2_isr (void) {
	// A completar por el alumno
	// ...
	if (millis () < debounceTime[FILA_2]) {
		debounceTime[FILA_2] = millis () + DEBOUNCE_TIME ;
				return;
			}

			piLock (KEYBOARD_KEY);

			teclado.teclaPulsada.row = FILA_2;
			teclado.teclaPulsada.col = teclado.columna_actual;
			piUnlock (KEYBOARD_KEY);

			piLock (SYSTEM_FLAGS_KEY);
			flags |= FLAG_TECLA_PULSADA;
			piUnlock (SYSTEM_FLAGS_KEY);

			debounceTime[FILA_2] = millis () + DEBOUNCE_TIME ;
		}


void teclado_fila_3_isr (void) {
	// A completar por el alumno
	// ...
	if (millis () < debounceTime[FILA_3]) {
		debounceTime[FILA_3] = millis () + DEBOUNCE_TIME ;
				return;
			}

			piLock (KEYBOARD_KEY);

			teclado.teclaPulsada.row = FILA_3;
			teclado.teclaPulsada.col = teclado.columna_actual;
			piUnlock (KEYBOARD_KEY);

			piLock (SYSTEM_FLAGS_KEY);
			flags |= FLAG_TECLA_PULSADA;
			piUnlock (SYSTEM_FLAGS_KEY);


			debounceTime[FILA_3] = millis () + DEBOUNCE_TIME ;
		}


void teclado_fila_4_isr (void) {
	// A completar por el alumno
	// ...
	if (millis () < debounceTime[FILA_4]) {
		debounceTime[FILA_4] = millis () + DEBOUNCE_TIME ;
				return;
			}

			piLock (KEYBOARD_KEY);
			teclado.teclaPulsada.row = FILA_4;
			teclado.teclaPulsada.col = teclado.columna_actual;
			piUnlock (KEYBOARD_KEY);

			piLock (SYSTEM_FLAGS_KEY);
			flags |= FLAG_TECLA_PULSADA;
			piUnlock (SYSTEM_FLAGS_KEY);


			debounceTime[FILA_4] = millis () + DEBOUNCE_TIME ;
		}


void timer_duracion_columna_isr (union sigval value) {
	// A completar por el alumno
	// ...
	piLock (SYSTEM_FLAGS_KEY);
	flags |= FLAG_TIMEOUT_COLUMNA_TECLADO;
	piUnlock (SYSTEM_FLAGS_KEY);
}
