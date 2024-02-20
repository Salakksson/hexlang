#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>

#include "flags.h"
void interpret(char* code, uint32_t tags);

void interpret_debug(char* code, uint32_t tags);