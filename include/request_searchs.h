/************************************************************************************
 * Práctica 3 - Sistemas Operativos II                                              *
 * SSOOIIGLE - request_searchs.h                                                    *
 * Autor: Diego García Álvarez                                                      *
 * 29/06/2025                                                                       *
 * Contiene las peticiones de búsqueda y la info del cliente.                       *
 ************************************************************************************/

#include <iostream>

struct sh_request{
    int client_id;
    int client_type;
    int balance;
    std::string word;
};