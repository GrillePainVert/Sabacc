#pragma once
#include <cstdint>

//g�n�rateur de nombres al�atoires
class PCGAlea {
private:
	uint64_t state; // �tat interne du g�n�rateur
	uint64_t inc;   // Incr�ment utilis� pour la g�n�ration

public:
	PCGAlea(uint64_t  graine);
	uint32_t alea();
	uint32_t aleaInferieurA(uint32_t borne);
};
