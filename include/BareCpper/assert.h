#ifndef CROG_ENCODER_ASSERT_H
#define CROG_ENCODER_ASSERT_H

#include "serial.h"

#define __ASSERT_USE_STDERR
#include <assert.h>
// handle diagnostic informations given by assertion and abort program execution:

void __assert(const char *__func, const char *__file, int __lineno, const char *__sexp)
{
    // transmit diagnostic informations through serial link. 
    /*Serial.print("ASSERT:");
    Serial.println(__sexp);
    Serial.print('@');
    Serial.print(__file);
    Serial.print('|');
    Serial.print(__func);
    Serial.print('|');
    Serial.println(__lineno, DEC);
    Serial.flush();*/
    // abort program execution.
    abort();
}

#endif


