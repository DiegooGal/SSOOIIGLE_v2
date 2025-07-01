/************************************************************************************
 * Práctica 3 - Sistemas Operativos II                                              *
 * SSOOIIGLE - thread_struct.h                                                      *
 * Autor: Diego García Álvarez                                                      *
 * 28/06/2025                                                                       *
 * Distintas estructuras que se van a utilizar.                                     *
 *          result_struct: Contiene las soluciones.                                 *
 *          thread_struct: Contiene los hilos que se crean.                         *
 ************************************************************************************/

#include <iostream>
#include <thread>
#include <string>
#include <queue>
#include <mutex>

#ifndef THREAD_STRUCT_H
#define THREAD_STRUCT_H

struct result_struct{
    std::string pre_word;
    std::string post_word;
    int line;
};

struct thread_struct{
    int thread_id;
    int initial_line;
    int final_line;
    std::string word;
    std::queue<result_struct> result;
};

#endif

