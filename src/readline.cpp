///////////////////////////// IMPORTAÇÕES /////////////////////////////////////

// STD
using namespace std;

// Bibliotecas-padrão
#include <cstdio>
#include <string>

// Bibliotecas locais
#include "../lib/readline.hpp"

/////////////////////////////// FUNÇÕES ///////////////////////////////////////

/* Realiza leitura de uma linha de um arquivo. */
string read_line_from_file(FILE *source)
{

    // EXCEÇÃO: ponteiro vazio
    if (!source)
        return NULL;

    // Variáveis locais
    string output;
    char validator = (char)fgetc(source);

    // Limpeza de buffer
    if (validator > LAST_UNREADABLE || validator < EOF)
    {
        output.push_back(validator);
    }
    else if (validator == EOF)
    {
        return NULL;
    }

    // Leitura
    do
    {
        validator = (char)fgetc(source);
        output.push_back(validator);
    } while (output.back() > LAST_UNREADABLE || output.back() < EOF);

    return output;
}