#ifndef ZYGISK_DUMP_ORIGINAL_FUNCOES_H
#define ZYGISK_DUMP_ORIGINAL_FUNCOES_H

#include <fstream>
#include <string>

struct Funcoes {
    bool attackSpeed = false, maxHp = false, setSpeed = false, hpZero = false, increasedDamage = false, vida = false,
            testando = false,clearPerson = false, onDamage = false;
};

Funcoes funcoes;

#endif //ZYGISK_DUMP_ORIGINAL_FUNCOES_H