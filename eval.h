#pragma once
#include "types.h"
#include "SabaccModel.h"
#include "windows.h"

//choisit l'action la plus gagnante après (nbJeux) parties aléatoires
Action choisisActionMonteCarloSimple(JeuSabacc& jeu, uint32_t nbJeux, PCGAlea& rng);

//retourne le % de parties aléatoires gagnées après avoir appliqué l'action a à l'etat jeu
float  evalActionMonteCarloSimple(JeuSabacc& j, Action& a, uint32_t nbJeux, PCGAlea& rng);