#pragma once

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

#define printfln(fmt, ...)  (fprintf(stdout, fmt "\n", ##__VA_ARGS__))
#define eprintfln(fmt, ...) (fprintf(stderr, fmt "\n", ##__VA_ARGS__))
#define die(fmt, ...)       (eprintfln("Line %d " fmt, __LINE__, ##__VA_ARGS__), exit(EXIT_FAILURE))



