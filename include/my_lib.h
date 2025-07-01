/************************************************************************************
 * Práctica 3 - Sistemas Operativos II                                              *
 * SSOOIIGLE - my_lib.h                                                             *
 * Autor: Diego García Álvarez                                                      *
 * 30/06/2025                                                                       *
 * Clase donde encontramos las funciones genéricas.                                 *
 ************************************************************************************/
#ifndef MY_LIB_H
#define MY_LIB_H
#include <iostream>
#include <bits/stdc++.h>
#include <string>
#include <fstream>
#include <ctype.h>

int count_lines(std::string file_name);
void word_to_upper(std::string &word);
void word_to_lower(std::string &word);
bool test_symbol(std::string word, std::string current_word);
void books_names();

std::vector<std::string> titles_vector;

/****************************************************
* Controla que se le han incluido todos los         *
* argumentos necesarios, si no salta un error.      *
* @param argc  Numero de argumentos                 *
* @return      No devuelve nada                     *
****************************************************/
void parse_args(int argc){
    if (argc != 4)
    {
        std::cerr << "[!] Error in args line. Usage: ./main <file_name> <word> <threads_number> " << std::endl;
        exit(EXIT_FAILURE);
    }
}

/****************************************************
* Lee el archivo y cuenta cuantas líneas tiene.     *
* @param file_name   Nombre del archivo.            *
* @return            Número de líneas.              *
****************************************************/
int count_lines(std::string file_name)
{
    std::string line;
    std::ifstream file(file_name);
    int num_lines = 0;

    if (file.is_open()){
        while (!file.eof()){
            getline(file, line);
            num_lines++;
        }
        file.close();
    }
    return num_lines;
}

/****************************************************
* Compara la palabra con la actual y esta con . , y;*
* para ver si una palabra contiene a otra.          *
* @param word           Palabra a buscar            *
* @param current_word   Palabra actual              *
* @return               True o False                *
****************************************************/
// Compara la palabra con la actual y esta con . , y ; para ver si una palabra contiene a otra.
bool test_symbol(std::string word, std::string current_word){
    bool bol = false;
    if (current_word == word || current_word == word + "." || current_word == word + "," || current_word == word + ";"){
        bol = true;
    }
    return bol;
}

/****************************************************
* Convierte una palabra a mayúsculas                *
* @param word  Palabra a transformar                *
* @return      La palabra en mayúsculas             *
****************************************************/
void word_to_upper(std::string &word){
    transform(word.begin(), word.end(), word.begin(), [](unsigned char c){ 
        return toupper(c); });
}

/****************************************************
* Convierte una palabra a minúsculas                *
* @param word  Palabra a transformar                *
* @return      La palabra en minusculas             *
****************************************************/
void word_to_lower(std::string &word){
    transform(word.begin(), word.end(), word.begin(), [](unsigned char c){ 
        return tolower(c); });
}

/****************************************************
* Añade a la variable todos los títulos             *
* @param .  No requiere                             *
* @return   No devuelve nada                        *
****************************************************/
void books_names(){
    std::string path = "8-libros-txt/";
    for (const auto &entry : std::filesystem::directory_iterator(path))
        titles_vector.push_back("8-libros-txt/"+entry.path().filename().string());
}
#endif