#include "eval.h"
#include "logger.h"

//choisit l'action la plus gagnante après (nbJeux) parties aléatoires
Action choisisActionMonteCarloSimple(JeuSabacc& jeu, uint32_t nbJeux, PCGAlea& rng) {
	std::vector<Action> actions;
	jeu.listerActionsLegales(actions);
	uint32_t meilleureActionIndex = 0;
	float meilleureActionScore = -1.0;
	uint32_t index = 0;
	LOGDEBUG.setActif(false);
	for (Action a : actions) {
		float score = 100.0f * evalActionMonteCarloSimple(jeu, a, static_cast<uint32_t>(nbJeux / actions.size()), rng);
		LOGWARNING << a << " " << score << "%" << std::endl;
		if (score > meilleureActionScore) {
			meilleureActionScore = score;
			meilleureActionIndex = index;
		}
		index++;
	}
	return actions[meilleureActionIndex];
}

//retourne le % de parties aléatoires gagnées après avoir appliqué l'action a à l'etat jeu
float  evalActionMonteCarloSimple(JeuSabacc& jeu, Action& a, uint32_t nbJeux, PCGAlea& rng) {
	float successRate = 0;
	std::vector<Action> actionsJcopy;
	for (uint32_t i = 0; i < nbJeux; i++) {
		JeuSabacc jcopy(jeu);
		jcopy.m_pListener = NULL;
		jcopy.appliqueConnaissanceJoueur(jcopy.m_positionJActuel, rng);
		jcopy.executeAction(a, rng);
		while (!jcopy.m_jeuTermine) {
			jcopy.listerActionsLegales(actionsJcopy);
			uint32_t i = rng.aleaInferieurA(static_cast<uint32_t>(actionsJcopy.size()));
			jcopy.executeAction(actionsJcopy[i], rng);
		}
		if (jcopy.m_vainqueur == jeu.m_positionJActuel) {
			successRate++;
		}
	}
	successRate /= (float)nbJeux;
	return successRate;
}


