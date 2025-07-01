/************************************************************************************
 * Práctica 3 - Sistemas Operativos II                                              *
 * SSOOIIGLE - request_searchs.h                                                    *
 * Autor: Diego García Álvarez                                                      *
 * 24/06/2025                                                                       *
 * Contiene las variables globales para todo el código.                             *
 ************************************************************************************/
#include <iostream>
#include <thread>

#define MAX_CLIENTS 5
#define MAX_SEARCHERS 4
#define NUMBER_OF_THREADS std::thread::hardware_concurrency()
#define WORDS_NUM 5