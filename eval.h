#pragma once
#include "types.h"
#include "SabaccModel.h"
#include "windows.h"

Action choisisActionMonteCarloSimple(JeuSabacc& jeu, uint32_t nbJeux, PCGAlea& rng);
float  evalActionMonteCarlo(JeuSabacc& j, Action& a, uint32_t nbJeux, PCGAlea& rng);