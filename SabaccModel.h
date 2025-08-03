#pragma once
#include <iostream>
#include <vector>
#include "types.h"
#include "PCGAlea.h"

//taille historique
// jeton, pioche, defausse 12 fois
// résolution imposteur 4 fois.
const uint32_t HISTORIQUE_TAILLE = 16;

// nb Jetons reserve
const uint32_t RESERVE_TAILLE = 8;

//distribution initiale des paquets
const uint32_t DISTRIBUTION_PAQUET_INITIAL[NB_CARTES] = { 1,3,3,3,3,3,3,3 };

// paquet de 22 cartes de Kessel Sabacc
class PaquetSabacc {
private:
	uint32_t m_distribution[NB_CARTES];
	uint32_t m_nbCartesRestantes = 0;

public:
	void reset();
	void appliqueDistribution(const uint32_t dist[NB_CARTES]);
	uint32_t getNbCartes() const;
	Carte pioche(PCGAlea& rng);
};

class DefausseSabacc {
private:
	Carte cartesDefausse[22];
	uint32_t nbCartes = 0;

public:
	void vide();
	void defausse(Carte c);
	uint32_t getNbCartes() const;
	Carte pioche();
	Carte regarde() const;
};

class ConnaissanceJoueur {
public:	

	//applique connaissance:
	// remplace paquets par distributions
	// pour chaque joueur AUTRE, remplace carte inconnue par tirage

	uint32_t m_distributionSable[NB_CARTES];
	uint32_t m_distributionSang[NB_CARTES];

	bool m_carteSableConnueDesAutres;
	bool m_carteSangConnueDesAutres;
	
	bool m_aPiocheDefausse;//pour se rappeller si le joueur a pioché dans la défausse.

	void reset();
};


class JoueurSabacc {
public:	
	TYPE_JOUEUR m_type;	
	Carte m_carteSable = C_INDEFINI; //main carte sable
	Carte m_carteSang = C_INDEFINI;	//main carte sang
	Carte m_carteSablePiochee = C_INDEFINI; //si le joueur a pioché et doit défausser, sinon C_INDEFINI
	Carte m_carteSangPiochee = C_INDEFINI; //si le joueur a pioché et doit défausser, sinon C_INDEFINI
	JetonInfluence m_jetonsInfluence[3] = { JI_INDEFINI,JI_INDEFINI,JI_INDEFINI};
	uint32_t m_de1 = 0, m_de2 = 0; //les dés à 6 faces lorsque le joueur les a lancés

	uint32_t m_reserve=0;
	uint32_t m_pot=0;
	bool m_aPasseCeTour = false;
	bool m_enJeu = false;	//joueur pas encore éliminé

	//effets des jetons d'action
	bool m_doitPasser = false;	//effet de EMBARGO
	bool m_immunite = false;	//effet de IMMUNITE
	bool m_piocheGratuite = false; //effet de PIOCHE_GRATUITE
		
	void tireDes(PCGAlea& rng);
	
};
std::ostream& operator<<(std::ostream& os, const JoueurSabacc& joueur);


class JeuSabaccListener {
public:
	virtual void surPhaseResolution() = 0;
	virtual void surFinTourJoueur() = 0;
	virtual void surFinInitJeu() = 0;
	virtual void surJeuTermine() = 0;
	virtual void surFinInitManche() = 0;
	virtual void surMAJReserveJoueur(Position p, uint32_t reserveActuelle, int delta) = 0;
	virtual void surTirageDesAnnonce() = 0;
	virtual void surTirageDesResultat() = 0;
};

class JeuSabacc {

public:
	JeuSabaccListener* m_pListener = NULL;
	uint32_t m_manche = 0;	// compte de la manche
	uint32_t m_tour = 0;	// 3 tours par manche
	PaquetSabacc m_paquetSable, m_paquetSang;
	DefausseSabacc m_defausseSable, m_defausseSang;
	Position m_positionJ1Manche = Position::P_INDEFINI; //pos premier joueur de la manche
	Position m_positionJActuel = Position::P_INDEFINI;	//pos joueur actuel
	PhaseTour m_phaseActuelle = PhaseTour::PHASE_INDEFINIE; //phase actuelle du tour: PHASE_JETON puis PHASE_PIOCHE puis PHASE_DEFAUSSE si necessaire
	TourJoueur m_historiqueToursJoueurs[HISTORIQUE_TAILLE]; //historique des tours dans la manche
	uint32_t m_nbToursDansHistorique = 0;

	JoueurSabacc m_joueurs[NB_POSITIONS]; //donnees des joueurs
	ConnaissanceJoueur m_connaissanceJoueurs[NB_POSITIONS]; //connaissance de chaque joueur sur le jeu
	bool m_suitConnaissanceJoueurs = true;
	Position m_positionConnaissanceAppliquee = Position::P_INDEFINI;

	//Jetons d'influence communs
	JetonInfluence m_jetonsTiresAuSort[3] = {JI_INDEFINI, JI_INDEFINI, JI_INDEFINI};

	//effets des Jetons d'influence
	bool m_sylopsAZero = false;			//effet de DEVALORISATION
	bool m_imposteurASix = false;		//effet de FRAUDE_MAJEURE
	bool m_sabaccInverse = false;		//effet de CUISINER_LIVRES
	Carte m_sabaccPrime = Carte::C_INDEFINI;		//carte désignée premier des sabacc par SABACC_PRIME

	//fin du jeu
	bool m_jeuTermine = true; // Indique si la partie est terminée
	Position m_vainqueur = Position::P_INDEFINI; // Stocke le vainqueur si m_jeuTermine est vrai

	void setListener(JeuSabaccListener* pListener) { m_pListener = pListener; };
	bool finTourJoueurActuel(PCGAlea& rng);
	bool finManche();
	
	void defausseCartePaquet(Famille f, PCGAlea& rng);
	void defausseCarteMain(Position pJoueur, Famille f);
	void defausseCartePiochee(Position pJoueur, Famille f);
	void piocheCartePaquet(Position pJoueur, Famille f, bool directEnMain, PCGAlea& rng);
	void piocheCarteDefausse(Position pJoueur, Famille f);

	void initJeu(PCGAlea& rng); //reset des scores, jetons et choix influence
	void initManche(PCGAlea& rng);
	void initTour();
	bool estTermine() const { return m_jeuTermine; };
	Position getVainqueur() const { return m_vainqueur; };
	void appliqueConnaissanceJoueur(Position pj, PCGAlea& rng);

	void listerActionsLegales(std::vector<Action> &actions) const;
	bool executeAction(Action& a, PCGAlea& rng);

	//friend std::ostream& operator<<(std::ostream& os, const JeuSabacc& jeu);
};
std::ostream& operator<<(std::ostream& os, const JeuSabacc& jeu);
