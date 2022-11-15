
#include "ledDisplay.h"

tipo_pantalla pantalla_inicial = {
	.matriz = {
			{0,0,0,0,0,0,0,0},
			    {0,1,1,0,0,1,1,0},
			    {0,1,1,0,0,1,1,0},
			    {0,0,0,0,0,0,0,0},
			    {0,1,0,0,0,0,1,0},
			    {0,0,1,1,1,1,0,0},
			    {0,0,0,0,0,0,0,0},
			    }
			};

			tipo_pantalla pantalla_final = {
			    .matriz = {
			    {0,0,0,0,0,0,0,0},
			    {0,1,1,0,0,1,1,0},
			    {0,1,1,0,0,1,1,0},
			    {0,0,0,0,0,0,0,0},
			    {0,0,1,1,1,1,0,0},
			    {0,1,0,0,0,0,1,0},
			    {0,0,0,0,0,0,0,0},
			    }
			};


// Maquina de estados: lista de transiciones
// {EstadoOrigen, CondicionDeDisparo, EstadoFinal, AccionesSiTransicion }
fsm_trans_t fsm_trans_excitacion_display[] = {
	{ DISPLAY_ESPERA_COLUMNA, CompruebaTimeoutColumnaDisplay, DISPLAY_ESPERA_COLUMNA, ActualizaExcitacionDisplay },
	{-1, NULL, -1, NULL },
};

//------------------------------------------------------
// PROCEDIMIENTOS DE INICIALIZACION DE LOS OBJETOS ESPECIFICOS
//------------------------------------------------------

void InicializaLedDisplay (TipoLedDisplay *led_display) {
	// A completar por el alumno...
	// ...

	      led_display->pantalla = pantalla_inicial;

			// Inicialmente no hay columna pulsada
			led_display->p_columna = -1;


			pinMode (GPIO_LED_DISPLAY_ROW_1, OUTPUT);
			digitalWrite (GPIO_LED_DISPLAY_ROW_1, HIGH);;


			pinMode (GPIO_LED_DISPLAY_ROW_2, OUTPUT);
			digitalWrite (GPIO_LED_DISPLAY_ROW_2, HIGH);


			pinMode (GPIO_LED_DISPLAY_ROW_3, OUTPUT);
			digitalWrite (GPIO_LED_DISPLAY_ROW_3, HIGH);


			pinMode (GPIO_LED_DISPLAY_ROW_4, OUTPUT);
			digitalWrite (GPIO_LED_DISPLAY_ROW_4, HIGH);


			pinMode (GPIO_LED_DISPLAY_ROW_5, OUTPUT);
			digitalWrite (GPIO_LED_DISPLAY_ROW_5, HIGH);


			pinMode (GPIO_LED_DISPLAY_ROW_6, OUTPUT);
			digitalWrite (GPIO_LED_DISPLAY_ROW_6, HIGH);


			pinMode (GPIO_LED_DISPLAY_ROW_7, OUTPUT);
			digitalWrite (GPIO_LED_DISPLAY_ROW_7, HIGH);

			pinMode (GPIO_LED_DISPLAY_COL_1, OUTPUT);
			digitalWrite (GPIO_LED_DISPLAY_COL_1, LOW);

			pinMode (GPIO_LED_DISPLAY_COL_2, OUTPUT);
			digitalWrite (GPIO_LED_DISPLAY_COL_2, LOW);

			pinMode (GPIO_LED_DISPLAY_COL_3, OUTPUT);
			digitalWrite (GPIO_LED_DISPLAY_COL_3, LOW);

			//pinMode (GPIO_LED_DISPLAY_COL_4, OUTPUT);
			//digitalWrite (GPIO_LED_DISPLAY_COL_4, LOW);

			led_display->tmr_refresco_display = tmr_new (timer_refresco_display_isr);
			tmr_startms((tmr_t*)(led_display->tmr_refresco_display), TIMEOUT_COLUMNA_DISPLAY);

			//fflush(stdout);
}

//------------------------------------------------------
// OTROS PROCEDIMIENTOS PROPIOS DE LA LIBRERIA
//------------------------------------------------------

void ApagaFilas (TipoLedDisplay *led_display){
	// A completar por el alumno...
	// ...

		digitalWrite (GPIO_LED_DISPLAY_ROW_1, HIGH);
		digitalWrite (GPIO_LED_DISPLAY_ROW_2, HIGH);
		digitalWrite (GPIO_LED_DISPLAY_ROW_3, HIGH);
		digitalWrite (GPIO_LED_DISPLAY_ROW_4, HIGH);
		digitalWrite (GPIO_LED_DISPLAY_ROW_5, HIGH);
		digitalWrite (GPIO_LED_DISPLAY_ROW_6, HIGH);
		digitalWrite (GPIO_LED_DISPLAY_ROW_7, HIGH);


}

void ExcitaColumnas(int columna) {


	switch(columna) {
		// A completar por el alumno...
		// ...
	    case DISPLAY_COLUMNA_1:

			digitalWrite (GPIO_LED_DISPLAY_COL_1, LOW);
			digitalWrite (GPIO_LED_DISPLAY_COL_2, LOW);
			digitalWrite (GPIO_LED_DISPLAY_COL_3, LOW);


			break;

		case DISPLAY_COLUMNA_2:
			digitalWrite (GPIO_LED_DISPLAY_COL_1, HIGH);
			digitalWrite (GPIO_LED_DISPLAY_COL_2, LOW);
			digitalWrite (GPIO_LED_DISPLAY_COL_3, LOW);


		    break;

		case DISPLAY_COLUMNA_3:
			digitalWrite (GPIO_LED_DISPLAY_COL_1, LOW);
			digitalWrite (GPIO_LED_DISPLAY_COL_2, HIGH);
			digitalWrite (GPIO_LED_DISPLAY_COL_3, LOW);

		    break;

		case DISPLAY_COLUMNA_4:
			digitalWrite (GPIO_LED_DISPLAY_COL_1, HIGH);
			digitalWrite (GPIO_LED_DISPLAY_COL_2, HIGH);
			digitalWrite (GPIO_LED_DISPLAY_COL_3, LOW);

		    break;

		case DISPLAY_COLUMNA_5:
			digitalWrite (GPIO_LED_DISPLAY_COL_1, LOW);
			digitalWrite (GPIO_LED_DISPLAY_COL_2, LOW);
			digitalWrite (GPIO_LED_DISPLAY_COL_3, HIGH);

			    break;

		case DISPLAY_COLUMNA_6:
			digitalWrite (GPIO_LED_DISPLAY_COL_1, HIGH);
			digitalWrite (GPIO_LED_DISPLAY_COL_2, LOW);
			digitalWrite (GPIO_LED_DISPLAY_COL_3, HIGH);

			    break;

		case DISPLAY_COLUMNA_7:
			digitalWrite (GPIO_LED_DISPLAY_COL_1, LOW);
			digitalWrite (GPIO_LED_DISPLAY_COL_2, HIGH);
			digitalWrite (GPIO_LED_DISPLAY_COL_3, HIGH);

			    break;

		case DISPLAY_COLUMNA_8:
			digitalWrite (GPIO_LED_DISPLAY_COL_1, HIGH);
			digitalWrite (GPIO_LED_DISPLAY_COL_2, HIGH);
			digitalWrite (GPIO_LED_DISPLAY_COL_3, HIGH);

			    break;

	}
}

void ActualizaLedDisplay (TipoLedDisplay *led_display) {
	// A completar por el alumno...
	// ...
	ApagaFilas(led_display);
	ExcitaColumnas(led_display->p_columna);
	int j=0;


		for(j=0;j<NUM_FILAS_DISPLAY;j++) {
			if((led_display->pantalla.matriz[j][led_display->p_columna])){
			digitalWrite (led_display->filas[j], LOW);
			}

		}

}

void PintaPantallaPorTerminal (tipo_pantalla *p_pantalla) {
	/*int i=0, j=0;

	printf("\n[PANTALLA]\n");
	fflush(stdout);
	for(j=0;j<NUM_FILAS_DISPLAY;j++) {
		for(i=0;i<NUM_COLUMNAS_DISPLAY;i++) {
			printf("%d", p_pantalla->matriz[j][i]);
			fflush(stdout);
		}
		printf("\n");
		fflush(stdout);
	}
	fflush(stdout);*/
}

//------------------------------------------------------
// FUNCIONES DE ENTRADA O DE TRANSICION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

int CompruebaTimeoutColumnaDisplay (fsm_t* this) {
	int result = 0;
	TipoLedDisplay *p_ledDisplay;
	p_ledDisplay = (TipoLedDisplay*)(this->user_data);

	// A completar por el alumno...
	// ...
	piLock (SYSTEM_FLAGS_KEY);
	result = (flags & FLAG_TIMEOUT_COLUMNA_DISPLAY);
	piUnlock (SYSTEM_FLAGS_KEY);

	return result;
}

//------------------------------------------------------
// FUNCIONES DE SALIDA O DE ACCION DE LA MAQUINA DE ESTADOS
//------------------------------------------------------

void ActualizaExcitacionDisplay (fsm_t* this) {
	// A completar por el alumno...
	// ...
	TipoLedDisplay *p_ledDisplay;
	p_ledDisplay = (TipoLedDisplay*)(this->user_data);

	    piLock(SYSTEM_FLAGS_KEY);
	    flags  &= (~FLAG_TIMEOUT_COLUMNA_DISPLAY);
		piUnlock(SYSTEM_FLAGS_KEY);

		piLock(MATRIX_KEY);
		p_ledDisplay->p_columna= p_ledDisplay->p_columna +1;
			if (p_ledDisplay->p_columna>DISPLAY_COLUMNA_8){
				p_ledDisplay->p_columna=DISPLAY_COLUMNA_1;
			}
		ActualizaLedDisplay(p_ledDisplay);
		tmr_startms((tmr_t*)(p_ledDisplay->tmr_refresco_display), TIMEOUT_COLUMNA_DISPLAY);
		piUnlock(MATRIX_KEY);

}

//------------------------------------------------------
// SUBRUTINAS DE ATENCION A LAS INTERRUPCIONES
//------------------------------------------------------

void timer_refresco_display_isr (union sigval value) {
	// A completar por el alumno...
	// ...
	piLock (SYSTEM_FLAGS_KEY);
	flags|=FLAG_TIMEOUT_COLUMNA_DISPLAY;
	piUnlock (SYSTEM_FLAGS_KEY);

}
