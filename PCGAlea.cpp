#include "PCGAlea.h"

PCGAlea::PCGAlea(uint64_t graine) : state(graine), inc(54 | 1)
{
}

//Méthode pour générer un nombre aléatoire
uint32_t PCGAlea::alea()
{
    uint64_t oldState = state;
    state = oldState * 6364136223846793005ULL + inc;
    uint32_t xorshifted = (uint32_t)(((oldState >> 18u) ^ oldState) >> 27u);
    uint32_t rot = oldState >> 59u;
    return (xorshifted >> rot) | (xorshifted << ((-(int32_t)rot) & 31));
}

// Méthode pour générer un nombre borné dans [0, borne[
uint32_t PCGAlea::aleaInferieurA(uint32_t borne)
{
    uint32_t threshold = (~borne + 1u) % borne;
    uint32_t r;
    do {
        r = alea();
    } while (r < threshold);
    return r % borne;
}
