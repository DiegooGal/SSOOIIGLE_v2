/************************************************************************************
 * Práctica 3 - Sistemas Operativos II                                              *
 * SSOOIIGLE - request_struct.h                                                     *
 * Autores: Diego García Álvarez                                                    *
 *          Pablo Mollá GIl                                                         *
 * 31/04/2025                                                                       *
 * Contiene el saldo de la cuenta y un mutex para el acceso                         *
 ************************************************************************************/

#include <mutex>
struct request{
    double balance;
    std::mutex mtx;
};