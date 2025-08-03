#pragma once
#include <ostream>

//types de jetons d'influence
enum JetonInfluence {
	JI_INDEFINI = -1,
	PIOCHE_GRATUITE = 0,		// Piochez sans payer de JETON
	REMBOURSEMENT_2,			// Récupérez jusqu'à 2 JETONS du POT vers votre RESERVE
	REMBOURSEMENT_3,			// Récupérez jusqu'à 3 JETONS du POT vers votre RESERVE
	TARIF_GENERAL,				// Tous les adversaires placent 1 JETON dans leur POT
	TARIF_CIBLE,				// Un joueur ciblé place 2 JETONS dans son POT
	EMBARGO,					// Le joueur suivant doit PASSER
	DEVALORISATION,				// Valeur SYLOPS = 0 jusqu'à la prochaine RESOLUTION
	IMMUNITE,					// Protection contre tous les JETONS D'ACTION jusqu'à la prochaine RESOLUTION
	AUDIT_GENERAL,				// Joueurs ayant PASSE placent 2 JETONS dans leur POT
	AUDIT_CIBLE,				// Un joueur ciblé ayant PASSE place 3 JETONS dans son POT
	FRAUDE_MAJEURE,				// Valeur IMPOSTEUR = 6 jusqu'à la prochaine RESOLUTION	
	DETOURNEMENT,				// Prenez 1 JETON du POT de chaque joueur dans votre POT
	CUISINER_LIVRES,			// Inverse l'ordre des SABACC à la prochaine RESOLUTION
	EPUISEMENT,					// Un joueur défausse et pioche une nouvelle MAIN
//	TRANSACTION,				// Echangez votre MAIN avec un autre joueur
	SABACC_PRIME,				// Lance les 2 dés, la valeur choisie devient le meilleur des SABACC
	NB_JETONS_INFLUENCE
};
std::ostream& operator<<(std::ostream& os, const JetonInfluence& j);

const std::string  DESCRIPTION_JI[NB_JETONS_INFLUENCE] = {
	u8"Piochez sans payer de jeton",								//	PIOCHE_GRATUITE
	u8"Récupérez jusqu'à 2 jetons du pot vers votre réserve",		//	REMBOURSEMENT_2
	u8"Récupérez jusqu'à 3 jetons du pot vers votre réserve",		//	REMBOURSEMENT_3
	u8"Tous les adversaires placent 1 jeton dans leur pot",			//	TARIF_GENERAL
	u8"Un joueur ciblé place 2 jetons dans son pot",				//	TARIF_CIBLE
	u8"Le joueur suivant doit passer",								//	EMBARGO
	u8"Les Sylops valent 0 jusqu'à la prochaine résolution",		//	DEVALORISATION
	u8"Immunité aux jetons d'influence jusqu'à la résolution",		//	IMMUNITE
	u8"Les joueurs ayant passé placent 2 jetons dans leur pot",		//	AUDIT_GENERAL
	u8"Un joueur ciblé ayant passé place 3 jetons dans son pot",	//	AUDIT_CIBLE
	u8"Les Imposteurs valent 6 jusqu'à la prochaine résolution",	//	FRAUDE_MAJEURE
	u8"Prenez 1 jeton du pot de chaque joueur dans votre pot",		//	DETOURNEMENT
	u8"Inverse l'ordre des sabacc à la prochaine résolution",		//	CUISINER_LIVRES
	u8"Un joueur défausse et pioche une nouvelle main",				//	EPUISEMENT
	//u8"Echangez votre MAIN avec un autre joueur",					//	TRANSACTION
	u8"Lance 2 dés, le dé choisi devient le meilleur sabacc"		//	SABACC_PRIME
};

//Familles de cartes
enum Famille {
	F_INDEFINI = -1,
	SABLE = 0,
	SANG
};

//cartes
enum Carte {
	C_INDEFINI = -1,
	SYLOP = 0,
	C1,
	C2,
	C3,
	C4,
	C5,
	C6,
	IMPOSTEUR,
	NB_CARTES
};
std::ostream& operator<<(std::ostream& os, const Carte& c);

//absolute position around the table
enum Position {
	P_INDEFINI = -1,
	S = 0,
	O,
	N,
	E,
	NB_POSITIONS
};

std::ostream& operator<<(std::ostream& os, const Position& p);

//relative position around the table
enum PRelative {
	PR_INDEFINI = -1,
	MOI = 0,
	GAUCHE,
	FACE,
	DROITE,
	NB_PRELATIVES
};
std::ostream& operator<<(std::ostream& os, const PRelative& p);

//conversion abs/relative
PRelative abs2rel(const Position p, const Position maPosition);
Position rel2abs(const PRelative p, const Position maPosition);

//phases dans un tour de joueur, utilisé par JeuSabacc
enum PhaseTour {
	PHASE_INDEFINIE,
	PHASE_JETON,		// Joue un Jeton d'Influence ou pas
	PHASE_JETON_SABACC_PRIME_DE, //Phase spéciale tirage et choix du dé pour le JI SABACC PRIME
	PHASE_PIOCHE,		// Pioche Sang Sable ou passe 
	PHASE_DEFAUSSE,		// Defausse ou garde carte piochée
	PHASE_RESOLUTION,	// Tire les dés si nécessaire et choisit pour les Imposteur	
	NB_PHASE_TOUR
};
std::ostream& operator<<(std::ostream& os, const PhaseTour& p);


// actions possibles dans le jeu, utilisé par JeuSabacc
enum Action {
	A_INDEFINIE = 0,
	PJ_PASSE,						// ne joue aucun jeton d'influence
	PJ_PIOCHE_GRATUITE,				// Piochez sans payer de JETON
	PJ_REMBOURSEMENT_2,				// Récupérez jusqu'à 2 JETONS du POT vers votre RESERVE
	PJ_REMBOURSEMENT_3,				// Récupérez jusqu'à 3 JETONS du POT vers votre RESERVE
	PJ_TARIF_GENERAL,				// Tous les adversaires placent 1 JETON dans leur POT
	PJ_TARIF_CIBLE_G,				// Joueur de gauche place 2 JETONS dans son POT
	PJ_TARIF_CIBLE_F,				// Joueur en face place 2 JETONS dans son POT	
	PJ_TARIF_CIBLE_D,				// Joueur à droiteplace 2 JETONS dans son POT		
	PJ_EMBARGO,						// Le joueur suivant doit PASSER
	PJ_DEVALORISATION,				// Valeur SYLOPS = 0 jusqu'à la prochaine RESOLUTION
	PJ_IMMUNITE,					// Protection contre les JETONS D'ACTION jusqu'à la prochaine RESOLUTION
	PJ_AUDIT_GENERAL,				// Joueurs ayant PASSE placent 2 JETONS dans leur POT
	PJ_AUDIT_CIBLE_G,				// Joueur à gauche ayant PASSE place 3 JETONS dans son POT
	PJ_AUDIT_CIBLE_F,				// Joueur en face ayant PASSE place 3 JETONS dans son POT
	PJ_AUDIT_CIBLE_D,				// Joueur à droite ayant PASSE place 3 JETONS dans son POT
	PJ_FRAUDE_MAJEURE,				// Valeur IMPOSTEUR = 6 jusqu'à la prochaine RESOLUTION	
	PJ_DETOURNEMENT,				// Prenez 1 JETON du POT de chaque joueur dans votre POT
	PJ_CUISINER_LIVRES,				// Inverse l'ordre des SABACC à la prochaine RESOLUTION
	PJ_EPUISEMENT_G,				// Joueur à gauche défausse et pioche une nouvelle MAIN
	PJ_EPUISEMENT_F,				// Joueur en face défausse et pioche une nouvelle MAIN
	PJ_EPUISEMENT_D,				// Joueur à droite défausse et pioche une nouvelle MAIN
	//PJ_TRANSACTION_G,				// Echangez votre MAIN avec joueur à gauche
	//PJ_TRANSACTION_F,				// Echangez votre MAIN avec joueur en face
	//PJ_TRANSACTION_D,				// Echangez votre MAIN avec joueur à droite
	PJ_SABACC_PRIME,				// Lance les 2 dés, la valeur choisie devient le meilleur des SABACC
	PJSP_D1,						// Choisit le dé 1 pour SABACC PRIME
	PJSP_D2,						// Choisit le dé 2 pour SABACC PRIME
	PP_PIOCHE_PAQUET_SABLE,			// Pioche dans le paquet SABLE
	PP_PIOCHE_PAQUET_SANG,			// Pioche dans le paquet SANG
	PP_PIOCHE_DEFAUSSE_SABLE,		// Défausse dans le paquet SABLE
	PP_PIOCHE_DEFAUSSE_SANG,		// Défausse dans le paquet SANG
	PP_PASSE,						// Ne pioche pas, passe
	PD_DEFAUSSE_CARTE_PIOCHEE,		// Défausse la carte piochée
	PD_GARDE_CARTE_PIOCHEE,			// Garde la carte piochée
	PR_IMPOSTEUR_SABLE_D1,			// Choisit le Dé 1 pour l'Imposteur Sable
	PR_IMPOSTEUR_SABLE_D2,			// Choisit le Dé 2 pour l'Imposteur Sable
	PR_IMPOSTEUR_SANG_D1,			// Choisit le Dé 1 pour l'Imposteur Sang
	PR_IMPOSTEUR_SANG_D2,			// Choisit le Dé 2 pour l'Imposteur Sang

	NB_ACTIONS
};
std::ostream& operator<<(std::ostream& os, const Action& action);

enum TYPE_JOUEUR {
	TJ_MONTE_CARLO_SIMPLE=0,
	TJ_MONTE_CARLO_SIMPLE2,
	TJ_HUMAIN,
	NB_TYPES_JOUEUR
};

// tour complet d'un joueur, utilisé pour JeuSabacc pour stocker l'historique pour le NN
struct TourJoueur {
	Position m_positionJoueur;
	JetonInfluence m_jeton;
	uint32_t valeurSabaccPrime;
	Carte cartePiochee;
	Famille familleCartePiochee;
	Carte carteDefaussee;
	Famille familleCarteDefaussee;
	uint32_t valeurImposteurSable;
	uint32_t valeurImposteurSang;
};
