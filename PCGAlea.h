#pragma once
#include <cstdint>

//générateur de nombres aléatoires
class PCGAlea {
private:
	uint64_t state; // État interne du générateur
	uint64_t inc;   // Incrément utilisé pour la génération

public:
	PCGAlea(uint64_t  graine);
	uint32_t alea();
	uint32_t aleaInferieurA(uint32_t borne);
};
