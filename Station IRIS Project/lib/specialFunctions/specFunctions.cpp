// Inclusões
#include "specFunctions.h"

// Variáveis
hw_timer_t *wTimer = NULL;

// Funções
void IRAM_ATTR resetModule() {ESP.restart();}  // Reinicia dispositivo