/************************************************************************************
 * Práctica 3 - Sistemas Operativos II                                              *
 * SSOOIIGLE - request_struct.h                                                     *
 * Autor: Diego García Álvarez                                                      *
 * 29/06/2025                                                                       *
 * Contiene el saldo de la cuenta y un mutex para el acceso                         *
 ************************************************************************************/

#include <mutex>
struct request{
    double balance;
    std::mutex mtx;
};