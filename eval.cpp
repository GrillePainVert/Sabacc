#include "eval.h"
#include "logger.h"

Action choisisActionMonteCarloSimple(JeuSabacc& jeu, uint32_t nbJeux, PCGAlea& rng) {
	std::vector<Action> actions;
	jeu.listerActionsLegales(actions);
	uint32_t meilleureActionIndex = 0;
	float meilleureActionScore = -1.0;
	uint32_t index = 0;
	LOGDEBUG.setActif(false);
	for (Action a : actions) {
		float score = 100.0f * evalActionMonteCarlo(jeu, a, static_cast<uint32_t>(nbJeux / actions.size()), rng);
		LOGWARNING << a << " " << score << "%" << std::endl;
		if (score > meilleureActionScore) {
			meilleureActionScore = score;
			meilleureActionIndex = index;
		}
		index++;
	}
	return actions[meilleureActionIndex];
}

float  evalActionMonteCarlo(JeuSabacc& j, Action& a, uint32_t nbJeux, PCGAlea& rng) {
	float successRate = 0;
	std::vector<Action> actionsJcopy;
	for (uint32_t i = 0; i < nbJeux; i++) {
		JeuSabacc jcopy(j);
		jcopy.m_pListener = NULL;
		jcopy.appliqueConnaissanceJoueur(jcopy.m_positionJActuel, rng);
		jcopy.executeAction(a, rng);
		while (!jcopy.m_jeuTermine) {
			jcopy.listerActionsLegales(actionsJcopy);
			uint32_t i = rng.aleaInferieurA(static_cast<uint32_t>(actionsJcopy.size()));
			jcopy.executeAction(actionsJcopy[i], rng);
		}
		if (jcopy.m_vainqueur == j.m_positionJActuel) {
			successRate++;
		}
	}
	successRate /= (float)nbJeux;
	return successRate;
}


