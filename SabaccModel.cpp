#include <algorithm> 
#include "SabaccModel.h"
#include "logger.h"
#include "types.h"

void PaquetSabacc::appliqueDistribution(const uint32_t dist[NB_CARTES]) {  
   
   // Copopie la distribution de cartes
   m_nbCartesRestantes = 0;

   for (uint32_t i = 0; i < NB_CARTES; ++i) {  
       m_distribution[i] = dist[i];  
       m_nbCartesRestantes += dist[i];
   }  
   
}

void PaquetSabacc::reset()
{         
    appliqueDistribution(DISTRIBUTION_PAQUET_INITIAL);
}


uint32_t PaquetSabacc::getNbCartes() const
{
    return m_nbCartesRestantes;
}

Carte PaquetSabacc::pioche(PCGAlea& rng)
{
    if (m_nbCartesRestantes == 0) {
        // Paquet vide, retourner une carte indéfinie
        return Carte::C_INDEFINI;
    }

    // Sélection d'une carte aléatoire
    uint32_t index = rng.aleaInferieurA(m_nbCartesRestantes);
    uint32_t type = 0;

    while (true) {
        if (m_distribution[type] > index) {
            break;  // Nous avons trouvé le type
        }
        index -= m_distribution[type];
        type++;
        // Note: Pas besoin de vérifier si type < 8 car le total est correct
    }

    // Décrémenter le compteur pour ce type de carte
    m_distribution[type]--;
    m_nbCartesRestantes--;

    return static_cast<Carte>(type);
}

void DefausseSabacc::vide()
{
    nbCartes = 0;
}

void DefausseSabacc::defausse(Carte c)
{
    cartesDefausse[nbCartes] = c;
    nbCartes++;
}

uint32_t DefausseSabacc::getNbCartes() const
{
    return nbCartes;
}

Carte DefausseSabacc::pioche()
{
    if (nbCartes == 0) {
        //pioche vide
        LOGWARNING << u8"ATTENTION pioche vide!" << std::endl;
        return C_INDEFINI;
    }

    nbCartes--;
    return cartesDefausse[nbCartes];
}

Carte DefausseSabacc::regarde() const
{
    if (nbCartes == 0) {
        //pioche vide
        //LOGWARNING << u8"ATTENTION pioche vide!" << std::endl;
        return C_INDEFINI;
    }
    return cartesDefausse[nbCartes-1];    
}

void ConnaissanceJoueur::reset()
{
    for (uint32_t i = 0; i < NB_CARTES; ++i) {
        m_distributionSable[i] = DISTRIBUTION_PAQUET_INITIAL[i];
        m_distributionSang[i] = DISTRIBUTION_PAQUET_INITIAL[i];
    }
    m_carteSableConnueDesAutres = false;
    m_carteSangConnueDesAutres = false;
    m_aPiocheDefausse = false;
}


void JeuSabacc::defausseCartePaquet(Famille f, PCGAlea& rng)
{
    
    switch (f) {
    case Famille::SABLE: {
        Carte cartePiochee = m_paquetSable.pioche(rng);
        m_defausseSable.defausse(cartePiochee);
        LOGDEBUG << u8" Carte Sable défaussée :" << cartePiochee << std::endl;
        //Chaque joueur sort carte de sa distribution de paquet Sable
        if (m_suitConnaissanceJoueurs) {
            for (uint32_t p = 0; p < NB_POSITIONS; p++) {
                if (m_connaissanceJoueurs[p].m_distributionSable[cartePiochee] == 0)
                    LOGWARNING << "ERREUR: aucune carte Sable restante de valeur: " << cartePiochee << std::endl;
                else
                    m_connaissanceJoueurs[p].m_distributionSable[cartePiochee]--;
            }
        }
        break;
    }
    case Famille::SANG: {
        Carte cartePiochee = m_paquetSang.pioche(rng);
        m_defausseSang.defausse(cartePiochee);
        LOGDEBUG << u8" Carte Sang défaussée :" << cartePiochee << std::endl;
        //Chaque joueur sort carte de sa distribution de paquet Sang
        if (m_suitConnaissanceJoueurs) {
            for (uint32_t p = 0; p < NB_POSITIONS; p++) {
                if (m_connaissanceJoueurs[p].m_distributionSang[cartePiochee] == 0)
                    LOGWARNING << "ERREUR: aucune carte Sang restante de valeur: " << cartePiochee << std::endl;
                else
                    m_connaissanceJoueurs[p].m_distributionSang[cartePiochee]--;
            }
        }
        break;
    }
    }
}

void JeuSabacc::defausseCarteMain(Position pJoueur, Famille f)
{
    switch (f) {
    case Famille::SABLE: {        
        Carte c = m_joueurs[pJoueur].m_carteSable;
        //Si pas connue, AUTRES JOUEURS sortent carte de leur distribution
        if (m_suitConnaissanceJoueurs) {
            if (!m_connaissanceJoueurs[pJoueur].m_carteSableConnueDesAutres) {
                for (uint32_t p = 0; p < NB_POSITIONS; p++) {
                    if (p != pJoueur) {
                        if (m_connaissanceJoueurs[p].m_distributionSable[c] == 0)
                            LOGWARNING << "ERREUR: aucune carte Sable restante de valeur: " << c << std::endl;
                        else
                            m_connaissanceJoueurs[p].m_distributionSable[c]--;
                    }
                }
            }
            else { //Si connue remet flag à pas connue.
                m_connaissanceJoueurs[pJoueur].m_carteSableConnueDesAutres = false;
            }
            //si vient de piocher carte défausse 
            if (m_connaissanceJoueurs[pJoueur].m_aPiocheDefausse) {
                m_connaissanceJoueurs[pJoueur].m_carteSableConnueDesAutres = true;
                m_connaissanceJoueurs[pJoueur].m_aPiocheDefausse = false;
            }
        }
        m_defausseSable.defausse(c);
        m_joueurs[pJoueur].m_carteSable = m_joueurs[pJoueur].m_carteSablePiochee;
        m_joueurs[pJoueur].m_carteSablePiochee = Carte::C_INDEFINI;        
        break;
    }
    case Famille::SANG: {
        Carte c = m_joueurs[pJoueur].m_carteSang;        
        //Si pas connue, AUTRES JOUEURS sortent carte de leur distribution.
        if (m_suitConnaissanceJoueurs) {
            if (!m_connaissanceJoueurs[pJoueur].m_carteSangConnueDesAutres) {
                for (uint32_t p = 0; p < NB_POSITIONS; p++) {
                    if (p != pJoueur) {
                        if (m_connaissanceJoueurs[p].m_distributionSang[c] == 0)
                            LOGWARNING << "ERREUR: aucune carte Sang restante de valeur: " << c << std::endl;
                        else
                            m_connaissanceJoueurs[p].m_distributionSang[c]--;
                    }
                }
            }
            else { //Si connue remet flag à pas connue.
                m_connaissanceJoueurs[pJoueur].m_carteSangConnueDesAutres = false;
            }
            //si vient de piocher carte défausse 
            if (m_connaissanceJoueurs[pJoueur].m_aPiocheDefausse) {
                m_connaissanceJoueurs[pJoueur].m_carteSangConnueDesAutres = true;
                m_connaissanceJoueurs[pJoueur].m_aPiocheDefausse = false;
            }
        }
        m_defausseSang.defausse(c);
        m_joueurs[pJoueur].m_carteSang = m_joueurs[pJoueur].m_carteSangPiochee;
        m_joueurs[pJoueur].m_carteSangPiochee = Carte::C_INDEFINI;
        break;
    }
    }
}

void JeuSabacc::defausseCartePiochee(Position pJoueur, Famille f)
{
    switch (f) {
    case Famille::SABLE: {
        Carte cp = m_joueurs[pJoueur].m_carteSablePiochee;
        LOGDEBUG << u8"  Joueur " << pJoueur << u8" défausse la carte Sable piochée (" << cp << ")." << std::endl;        
        m_defausseSable.defausse(cp);
        m_joueurs[pJoueur].m_carteSablePiochee = Carte::C_INDEFINI;
        // si la carte a été piochée du paquet, AUTRES JOUEURS sortent carte de leur distribution
        if (m_suitConnaissanceJoueurs) {
            if (!m_connaissanceJoueurs[pJoueur].m_aPiocheDefausse) {
                for (uint32_t p = 0; p < NB_POSITIONS; p++) {
                    if (p != pJoueur) {
                        if (m_connaissanceJoueurs[p].m_distributionSable[cp] == 0)
                            LOGWARNING << "ERREUR: aucune carte Sable restante de valeur: " << cp << std::endl;
                        else
                            m_connaissanceJoueurs[p].m_distributionSable[cp]--;
                    }
                }
            }
            else {
                m_connaissanceJoueurs[pJoueur].m_aPiocheDefausse = false;
            }
        }
        break;
    }
    case Famille::SANG: {
        Carte cp = m_joueurs[pJoueur].m_carteSangPiochee;
        LOGDEBUG << u8"  Joueur " << pJoueur << u8" défausse la carte Sang piochée (" << cp << ")." << std::endl;        
        m_defausseSang.defausse(m_joueurs[pJoueur].m_carteSangPiochee);
        m_joueurs[pJoueur].m_carteSangPiochee = Carte::C_INDEFINI;
        // si la carte a été piochée de le paquet, AUTRES JOUEURS sortent carte de leur distribution
        if (m_suitConnaissanceJoueurs) {
            if (!m_connaissanceJoueurs[pJoueur].m_aPiocheDefausse) {
                for (uint32_t p = 0; p < NB_POSITIONS; p++) {
                    if (p != pJoueur) {
                        if (m_connaissanceJoueurs[p].m_distributionSang[cp] == 0)
                            LOGWARNING << "ERREUR: aucune carte Sang restante de valeur: " << cp << std::endl;
                        else
                            m_connaissanceJoueurs[p].m_distributionSang[cp]--;
                    }
                }
            }
            else {
                m_connaissanceJoueurs[pJoueur].m_aPiocheDefausse = false;
            }
        }
        break;
    }
    }
}

void JeuSabacc::piocheCartePaquet(Position pJoueur, Famille f, bool directEnMain, PCGAlea& rng)
{
	switch (f) {
	case Famille::SABLE: {
		Carte cp = m_paquetSable.pioche(rng);
//		LOGDEBUG << u8"  Joueur " << pJoueur << u8" pioche " << cp << u8" de Paquet Sable" << std::endl;
		directEnMain ? m_joueurs[pJoueur].m_carteSable = cp : m_joueurs[pJoueur].m_carteSablePiochee = cp;
        //Le joueur sort carte de sa distribution de paquet Sable
        if (m_suitConnaissanceJoueurs) {
            if (m_connaissanceJoueurs[pJoueur].m_distributionSable[cp] == 0)
                LOGWARNING << "ERREUR: aucune carte Sable restante de valeur: " << cp << std::endl;
            else
                m_connaissanceJoueurs[pJoueur].m_distributionSable[cp]--;
        }
        break;
	} case Famille::SANG: {
		Carte cp = m_paquetSang.pioche(rng);
//		LOGDEBUG << u8"  Joueur " << pJoueur << u8" pioche " << cp << u8" de Paquet Sang" << std::endl;
		directEnMain ? m_joueurs[pJoueur].m_carteSang = cp : m_joueurs[pJoueur].m_carteSangPiochee = cp;
        //Le joueur sort carte de sa distribution de paquet Sang
        if (m_suitConnaissanceJoueurs) {
            if (m_connaissanceJoueurs[pJoueur].m_distributionSang[cp] == 0)
                LOGWARNING << "ERREUR: aucune carte Sang restante de valeur: " << cp << std::endl;
            else
                m_connaissanceJoueurs[pJoueur].m_distributionSang[cp]--;
        }
        break;
	}
	}
}

void JeuSabacc::piocheCarteDefausse(Position pJoueur, Famille f)
{
    switch (f) {
    case Famille::SABLE: {
        Carte cp = m_defausseSable.pioche();
        LOGDEBUG << u8"  Joueur " << pJoueur << u8" pioche " << cp << u8" de Défausse Sable" << std::endl;
        m_joueurs[pJoueur].m_carteSablePiochee = cp;
        break;
    }
    case Famille::SANG: {
        Carte cp = m_defausseSang.pioche();
        LOGDEBUG << u8"  Joueur " << pJoueur << u8" pioche " << cp << u8" de Défausse Sang" << std::endl;
        m_joueurs[pJoueur].m_carteSangPiochee = cp;
        break;
    }
    }
    //met le flag
    m_connaissanceJoueurs[pJoueur].m_aPiocheDefausse = true;
}

void JeuSabacc::initJeu(PCGAlea& rng)
{
    m_manche = 0;
    m_jeuTermine = false;
    m_vainqueur = P_INDEFINI;
    
    LOGDEBUG<< u8"début jeu, tirage influences, ";
    
    // Tirage au sort des 3 jetons d'influence disponibles
    JetonInfluence tousLesJetons[NB_JETONS_INFLUENCE];
    for (uint32_t i = 0; i < NB_JETONS_INFLUENCE; ++i) {
        tousLesJetons[i] = static_cast<JetonInfluence>(i);
    }
    // Mélange des jetons (Fisher-Yates)
    for (uint32_t i = NB_JETONS_INFLUENCE - 1; i > 0; --i) {
        uint32_t j = rng.aleaInferieurA(i + 1);
        std::swap(tousLesJetons[i], tousLesJetons[j]);
    }
    // Sélection des 3 premiers jetons
    for (uint32_t i = 0; i < 3; ++i) {
        m_jetonsTiresAuSort[i] = tousLesJetons[i];
    }

    //TODO remove test
    //m_jetonsTiresAuSort[0] = JetonInfluence::FRAUDE_MAJEURE;
    //m_jetonsTiresAuSort[1] = JetonInfluence::FRAUDE_MAJEURE;
    //m_jetonsTiresAuSort[2] = JetonInfluence::FRAUDE_MAJEURE;
    
    LOGDEBUG << u8"init joueurs" <<std::endl;
    for (uint32_t i = 0; i < NB_POSITIONS; ++i) {
        // Réinitialiser le joueur        
        m_joueurs[i].m_reserve = RESERVE_TAILLE; // Chaque joueur commence avec 8 jetons
        m_joueurs[i].m_enJeu = true;
        m_joueurs[i].m_aPasseCeTour = false;

        // Attribuer les jetons d'influence
        for (int j = 0; j < 3; ++j) {
            m_joueurs[i].m_jetonsInfluence[j] = m_jetonsTiresAuSort[j];
        }
    }

    if (m_pListener)
        m_pListener->surFinInitJeu();

    initManche(rng);    
}

void JeuSabacc::initManche(PCGAlea& rng)
{
    //augmente manche, mets tour à 1    
    m_manche++;
    m_tour = 0;
    LOGDEBUG << u8"début manche: " << m_manche << ", ";

    // Vider les défausses
    LOGDEBUG << u8"défausses vides, ";
    m_defausseSable.vide();
    m_defausseSang.vide();

    //paquets, défausses
    LOGDEBUG << u8"reset paquets, ";
    m_paquetSable.reset();
    m_paquetSang.reset();

    //reset de l'historique
    m_nbToursDansHistorique = 0;

    //positions premier joueur et j actuel
    if (m_manche == 1) {
        m_positionJ1Manche = static_cast<Position>(rng.aleaInferieurA(NB_POSITIONS));
    }
    else {
        // Passer au joueur suivant (dans le sens horaire)
        m_positionJ1Manche = static_cast<Position>((static_cast<uint32_t>(m_positionJ1Manche) + 1) % NB_POSITIONS);

        // S'assurer que le premier joueur n'est pas éliminé
        uint32_t compteur = 0;
        while (!m_joueurs[m_positionJ1Manche].m_enJeu && compteur < NB_POSITIONS) {
            m_positionJ1Manche = static_cast<Position>((static_cast<uint32_t>(m_positionJ1Manche) + 1) % NB_POSITIONS);
            compteur++;
        }
        if (compteur == NB_POSITIONS) {
            LOGDEBUG << u8"Erreur: Impossible de démarrer la manche, plus de joueurs en jeu!"<<std::endl;
        }
    }
    
    m_positionJActuel = m_positionJ1Manche;
    m_phaseActuelle = PHASE_JETON;
    LOGDEBUG << u8"1er joueur: " << m_positionJ1Manche<< ", ";

    //reinit les effets des jetons d'influence
    //dans le jeu
    LOGDEBUG << u8"reinit influence, ";
    m_sylopsAZero = false;
    m_imposteurASix = false;
    m_sabaccInverse = false;
    m_sabaccPrime = Carte::C_INDEFINI;
    //chez les joueurs et les connaissances du jeu
    for (uint32_t i = 0; i < NB_POSITIONS; ++i) {        
        m_joueurs[i].m_pot = 0;
        m_joueurs[i].m_doitPasser = false;
        m_joueurs[i].m_piocheGratuite = false;
        m_joueurs[i].m_immunite = false;
        m_joueurs[i].m_de1 = 0;
        m_joueurs[i].m_de2 = 0;        
        m_joueurs[i].m_aPasseCeTour = false;
        m_connaissanceJoueurs[i].reset();
    }
    //suivra la connaissance des joueurs
    m_suitConnaissanceJoueurs = true;
    m_positionConnaissanceAppliquee = Position::P_INDEFINI;

    // Distribution des cartes initiales
    LOGDEBUG << u8"distrib cartes"<<std::endl;
    for (uint32_t i = 0; i < NB_POSITIONS; ++i) {
        if (m_joueurs[i].m_enJeu) {
            piocheCartePaquet(static_cast<Position>(i), Famille::SABLE, true, rng);
            piocheCartePaquet(static_cast<Position>(i), Famille::SANG, true, rng);
        }
    }

    // Placer une carte face visible sur chaque défausse           
    defausseCartePaquet(Famille::SABLE, rng);
    defausseCartePaquet(Famille::SANG, rng);

    //listener
    if (m_pListener)
        m_pListener->surFinInitManche();

    initTour();
}


void JeuSabacc::initTour() {
        
    m_tour++;
    LOGDEBUG << u8"début tour: " << m_tour << std::endl;
}


void JeuSabacc::appliqueConnaissanceJoueur(Position pj, PCGAlea& rng)
{
    //si connaissance d'un joueur déjà appliquée, ne le fait plus
    //sauf si c'est la même connaissance qui est réappliquée
    if ((m_positionConnaissanceAppliquee== Position::P_INDEFINI || m_positionConnaissanceAppliquee ==pj) && m_phaseActuelle!=PHASE_RESOLUTION) {
        //désactive le suivi de la connaissance
        m_suitConnaissanceJoueurs = false;
        m_positionConnaissanceAppliquee = pj;

        //reset paquets 
        m_paquetSable.appliqueDistribution(m_connaissanceJoueurs[pj].m_distributionSable);
        m_paquetSang.appliqueDistribution(m_connaissanceJoueurs[pj].m_distributionSang);

        //retire au sort les cartes inconnues des autres joueurs
        for (uint32_t pos = 0; pos < NB_POSITIONS; pos++) {
            if (pos != pj) {
                if (!m_connaissanceJoueurs[pos].m_carteSableConnueDesAutres) {
                    piocheCartePaquet(static_cast<Position>(pos), SABLE, true, rng);
                }
                if (!m_connaissanceJoueurs[pos].m_carteSangConnueDesAutres) {
                    piocheCartePaquet(static_cast<Position>(pos), SANG, true, rng);
                }
            }
        }
    }
    else {
        //LOGWARNING << "Connaissance des joueurs non suivie, inapplicable!" << std::endl;
    }
}


void JeuSabacc::listerActionsLegales(std::vector<Action>& actions) const
{
    actions.clear();
    const JoueurSabacc& joueurActuel = m_joueurs[m_positionJActuel];

    // Si le joueur est éliminé, aucune action
    if (!joueurActuel.m_enJeu) {
        return;
    }

    switch (m_phaseActuelle) {
    case PHASE_JETON: {
        // Action de base : ne pas jouer de jeton
        actions.push_back(Action::PJ_PASSE);

        // Actions possibles avec les Jetons d'Influence possédés
        for (JetonInfluence jeton : joueurActuel.m_jetonsInfluence) {
            switch (jeton) {
            case JetonInfluence::PIOCHE_GRATUITE:   actions.push_back(Action::PJ_PIOCHE_GRATUITE); break;
            case JetonInfluence::REMBOURSEMENT_2:   actions.push_back(Action::PJ_REMBOURSEMENT_2); break; // suggérer l'action seulement si pot >0?
            case JetonInfluence::REMBOURSEMENT_3:   actions.push_back(Action::PJ_REMBOURSEMENT_3); break; // suggérer l'action seulement si pot >0?
            case JetonInfluence::TARIF_GENERAL:     actions.push_back(Action::PJ_TARIF_GENERAL); break;
            case JetonInfluence::DEVALORISATION:    actions.push_back(Action::PJ_DEVALORISATION); break;
            case JetonInfluence::IMMUNITE:          actions.push_back(Action::PJ_IMMUNITE); break;
            case JetonInfluence::AUDIT_GENERAL:     actions.push_back(Action::PJ_AUDIT_GENERAL); break;
            case JetonInfluence::FRAUDE_MAJEURE:    actions.push_back(Action::PJ_FRAUDE_MAJEURE); break;
            case JetonInfluence::DETOURNEMENT:      actions.push_back(Action::PJ_DETOURNEMENT); break;
            case JetonInfluence::CUISINER_LIVRES:   actions.push_back(Action::PJ_CUISINER_LIVRES); break;
            case JetonInfluence::SABACC_PRIME:      actions.push_back(Action::PJ_SABACC_PRIME); break; // Logique de lancer de dés à gérer ailleurs
            //seulement si joueur suivant pas immunité
            case JetonInfluence::EMBARGO: {
                Position posJoueurSuivantEnJeu = m_positionJActuel;
                while (posJoueurSuivantEnJeu == m_positionJActuel || !m_joueurs[posJoueurSuivantEnJeu].m_enJeu) {
                    posJoueurSuivantEnJeu = static_cast<Position>((posJoueurSuivantEnJeu + 1) % NB_POSITIONS);
                }
                if (!m_joueurs[posJoueurSuivantEnJeu].m_immunite) {
                    actions.push_back(Action::PJ_EMBARGO);
                }
                break;
            }                
            // Jetons nécessitant une cible
            case JetonInfluence::TARIF_CIBLE:
            case JetonInfluence::AUDIT_CIBLE:
            case JetonInfluence::EPUISEMENT:
//            case JetonInfluence::TRANSACTION:
                for (uint32_t i = 1; i < NB_PRELATIVES; ++i) {
                    PRelative relativeTargetPos = static_cast<PRelative>(i);
                    Position absTargetPos = rel2abs(relativeTargetPos, m_positionJActuel);
                    
                    // La cible doit être en jeu et parfois avoir passé
                    if (m_joueurs[absTargetPos].m_enJeu && !m_joueurs[absTargetPos].m_immunite) {
                        bool targetAPasse = m_joueurs[absTargetPos].m_aPasseCeTour;
                        Action actionCible = Action::NB_ACTIONS; // Valeur invalide par défaut
                        switch (jeton) {
                        case JetonInfluence::TARIF_CIBLE:
                            if (relativeTargetPos == PRelative::GAUCHE) actionCible = Action::PJ_TARIF_CIBLE_G;
                            else if (relativeTargetPos == PRelative::FACE) actionCible = Action::PJ_TARIF_CIBLE_F;
                            else if (relativeTargetPos == PRelative::DROITE) actionCible = Action::PJ_TARIF_CIBLE_D;
                            break;
                        case JetonInfluence::AUDIT_CIBLE:
                            if(targetAPasse){
                                if (relativeTargetPos == PRelative::GAUCHE) actionCible = Action::PJ_AUDIT_CIBLE_G;
                                else if (relativeTargetPos == PRelative::FACE) actionCible = Action::PJ_AUDIT_CIBLE_F;
                                else if (relativeTargetPos == PRelative::DROITE) actionCible = Action::PJ_AUDIT_CIBLE_D;
                            }
                            break;
                        case JetonInfluence::EPUISEMENT:
                            if (relativeTargetPos == PRelative::GAUCHE) actionCible = Action::PJ_EPUISEMENT_G;
                            else if (relativeTargetPos == PRelative::FACE) actionCible = Action::PJ_EPUISEMENT_F;
                            else if (relativeTargetPos == PRelative::DROITE) actionCible = Action::PJ_EPUISEMENT_D;
                            break;
                        //case JetonInfluence::TRANSACTION:
                        //    if (relativeTargetPos == PRelative::GAUCHE) actionCible = Action::PJ_TRANSACTION_G;
                        //    else if (relativeTargetPos == PRelative::FACE) actionCible = Action::PJ_TRANSACTION_F;
                        //    else if (relativeTargetPos == PRelative::DROITE) actionCible = Action::PJ_TRANSACTION_D;
                        //    break;
                        default: break; // Ne devrait pas arriver
                        }
                        if (actionCible != Action::NB_ACTIONS) {
                            actions.push_back(actionCible);
                        }
                    }
                }
                break;
            default: break; // JI_INDEFINI ou NB_JETONS_INFLUENCE
            }
        }
        break;
    } // Fin PHASE_JETON

    case PHASE_JETON_SABACC_PRIME_DE: {
        // Cette phase est active après avoir joué PJ_SABACC_PRIME et lancé les dés du joueur        
        actions.push_back(Action::PJSP_D1); // Choisir le résultat du dé 1
        actions.push_back(Action::PJSP_D2); // Choisir le résultat du dé 2
        break;
    } // Fin PHASE_JETON_SABACC_PRIME_DE

    case PHASE_PIOCHE: {
        // Action 1 passer
        actions.push_back(Action::PP_PASSE);

        // Action 2 piocher si argent dispo et pas doitpasser
        bool peutPayerPioche = joueurActuel.m_reserve >= 1 || joueurActuel.m_piocheGratuite;
        if (peutPayerPioche && !joueurActuel.m_doitPasser) {
            actions.push_back(Action::PP_PIOCHE_DEFAUSSE_SABLE);
            actions.push_back(Action::PP_PIOCHE_PAQUET_SABLE);
            actions.push_back(Action::PP_PIOCHE_PAQUET_SANG);            
            actions.push_back(Action::PP_PIOCHE_DEFAUSSE_SANG);
        }

        break;
    } // Fin PHASE_PIOCHE

    case PHASE_DEFAUSSE: {
        // Cette phase n'est active que si le joueur a effectivement pioché une carte
        // On vérifie si une carte est en attente dans m_carteSablePiochee ou m_carteSangPiochee
        if (joueurActuel.m_carteSablePiochee != Carte::C_INDEFINI || joueurActuel.m_carteSangPiochee != Carte::C_INDEFINI) {
            // Le joueur a le choix entre garder la nouvelle carte (et défausser l'ancienne) ou défausser la nouvelle
            actions.push_back(Action::PD_GARDE_CARTE_PIOCHEE);
            actions.push_back(Action::PD_DEFAUSSE_CARTE_PIOCHEE);
        }
        else {
            LOGWARNING<<"ERREUR: Phase DEFAUSSE sans carte piochée!" << std::endl;
        }
        break;
    }  // Fin PHASE_DEFAUSSE

    case PHASE_RESOLUTION: {
        // Actions possibles pendant la résolution : choisir la valeur des Imposteurs, les dés ayant été lancés.
        if (joueurActuel.m_carteSable == Carte::IMPOSTEUR) {
            actions.push_back(Action::PR_IMPOSTEUR_SABLE_D1);
            actions.push_back(Action::PR_IMPOSTEUR_SABLE_D2);
            
        } else if (joueurActuel.m_carteSang == Carte::IMPOSTEUR) {
            actions.push_back(Action::PR_IMPOSTEUR_SANG_D1);
            actions.push_back(Action::PR_IMPOSTEUR_SANG_D2);            
        }
        else {
            LOGWARNING << "ERREUR: Phase RESOLUTION sans imposteur!" << std::endl;
        }

        break;
    } // Fin PHASE_RESOLUTION

    default:
        break;
    } 

    return;
}

bool findAndRemoveJeton(JoueurSabacc& joueur, JetonInfluence jetonToRemove) {
    for (int i = 0; i < 3; ++i) {
        if (joueur.m_jetonsInfluence[i] == jetonToRemove) {
            joueur.m_jetonsInfluence[i] = JetonInfluence::JI_INDEFINI;
            return true;
        }
    }
    LOGWARNING << u8"ERREUR JI non possédé: " << jetonToRemove << std::endl;
    return false;
}

Position getActionAbsTargetPos(Action a, Position currentPos) {
    PRelative rel = PRelative::PR_INDEFINI;
    switch (a) {
    case Action::PJ_TARIF_CIBLE_G:
    case Action::PJ_AUDIT_CIBLE_G:
    case Action::PJ_EPUISEMENT_G:
//    case Action::PJ_TRANSACTION_G:
        rel = PRelative::GAUCHE; break;
    case Action::PJ_TARIF_CIBLE_F:
    case Action::PJ_AUDIT_CIBLE_F:
    case Action::PJ_EPUISEMENT_F:
//    case Action::PJ_TRANSACTION_F:
        rel = PRelative::FACE; break;
    case Action::PJ_TARIF_CIBLE_D:
    case Action::PJ_AUDIT_CIBLE_D:
    case Action::PJ_EPUISEMENT_D:
//    case Action::PJ_TRANSACTION_D:
        rel = PRelative::DROITE; break;
    default:
        break; // Action sans cible relative
    }

    if (rel != PRelative::PR_INDEFINI) {
        return rel2abs(rel, currentPos);
    }
    return Position::P_INDEFINI; // Ne devrait pas arriver pour les actions ciblées
}

bool JeuSabacc::executeAction(Action &a, PCGAlea& rng) {
    //LOGDEBUG << u8"Execute action: " << a << u8" par Joueur " << m_positionJActuel << u8" en phase " << m_phaseActuelle << std::endl;
    JoueurSabacc& joueurActuel = m_joueurs[m_positionJActuel];

    // Vérification 1: Joueur doit être en jeu
    if (!joueurActuel.m_enJeu) {
        LOGWARNING << u8"ERREUR: Joueur " << m_positionJActuel << u8" hors jeu!" << std::endl;        
        return false;
    }    
    
    // Vérification 2: Le joueur ne doit pas être forcé de passer (Embargo) sauf si l'action est de passer
    if (joueurActuel.m_doitPasser && m_phaseActuelle == PHASE_PIOCHE && a != Action::PP_PASSE) {
        LOGWARNING << u8"ERREUR executeAction: Joueur " << m_positionJActuel << u8" doit passer (Embargo) mais a tenté l'action " << a << std::endl;
        return false; 
    }

    // Vérification 3: Si la phase est DEFAUSSE, le joueur doit avoir une carte en attente
    if (m_phaseActuelle == PHASE_DEFAUSSE && joueurActuel.m_carteSablePiochee == Carte::C_INDEFINI && joueurActuel.m_carteSangPiochee == Carte::C_INDEFINI) {
        LOGWARNING << u8"ERREUR executeAction: Joueur " << m_positionJActuel << u8" est en PHASE_DEFAUSSE sans carte piochée en attente." << std::endl;
        return false; // Cas d'erreur -> false
    }

    // Vérification 4: Si la phase est RESOLUTION, le joueur doit avoir l'Imposteur correspondant et les dés lancés
    if (m_phaseActuelle == PHASE_RESOLUTION) {
        if ((a == PR_IMPOSTEUR_SABLE_D1 || a == PR_IMPOSTEUR_SABLE_D2) && (joueurActuel.m_carteSable != Carte::IMPOSTEUR || joueurActuel.m_de1 == 0)) {
            LOGWARNING << u8"ERREUR executeAction: Action " << a << u8" invalide (pas d'Imposteur Sable ou dés non lancés)." << std::endl;
            return false; // Cas d'erreur -> false
        }
        if ((a == PR_IMPOSTEUR_SANG_D1 || a == PR_IMPOSTEUR_SANG_D2) && (joueurActuel.m_carteSang != Carte::IMPOSTEUR || joueurActuel.m_de1 == 0)) {
            LOGWARNING << u8"ERREUR executeAction: Action " << a << u8" invalide (pas d'Imposteur Sang ou dés non lancés)." << std::endl;
            return false; // Cas d'erreur -> false
        }
    }

    // Vérification 5: Si la phase est PHASE_JETON_SABACC_PRIME_DE, les dés doivent être lancés
    if (m_phaseActuelle == PHASE_JETON_SABACC_PRIME_DE && joueurActuel.m_de1 == 0) {
        LOGWARNING << u8"ERREUR executeAction: Action " << a << u8" invalide (dés pour Sabacc Prime non lancés ou déjà utilisés)." << std::endl;
        return false; // Cas d'erreur -> false
    }

    bool result = true; 

    switch (m_phaseActuelle) {

    case PHASE_JETON: {
        // Phase PHASE_JETON
        switch (a) {
        case Action::PJ_PASSE:
            LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" passe la phase Jeton." << std::endl;
            m_phaseActuelle = PHASE_PIOCHE;
            break;

        case Action::PJ_PIOCHE_GRATUITE:
            if (findAndRemoveJeton(joueurActuel, JetonInfluence::PIOCHE_GRATUITE)) {
                joueurActuel.m_piocheGratuite = true;
                LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" active Pioche Gratuite." << std::endl;
                m_phaseActuelle = PHASE_PIOCHE;
            }
            else result = false;
            break;

        case Action::PJ_REMBOURSEMENT_2:
            if (findAndRemoveJeton(joueurActuel, JetonInfluence::REMBOURSEMENT_2)) {
                uint32_t montantRecup = std::min(2u, joueurActuel.m_pot);
                joueurActuel.m_pot -= montantRecup;
                joueurActuel.m_reserve += montantRecup;
                LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" récupère " << montantRecup << u8" jetons (Remboursement 2)." << std::endl;
                m_phaseActuelle = PHASE_PIOCHE;
            }
            else result = false;
            break;

        case Action::PJ_REMBOURSEMENT_3:
            if (findAndRemoveJeton(joueurActuel, JetonInfluence::REMBOURSEMENT_3)) {
                uint32_t montantRecup = std::min(3u, joueurActuel.m_pot);
                joueurActuel.m_pot -= montantRecup;
                joueurActuel.m_reserve += montantRecup;
                LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" récupère " << montantRecup << u8" jetons (Remboursement 3)." << std::endl;
                m_phaseActuelle = PHASE_PIOCHE;
			} 
            else result = false;
            break;

        case Action::PJ_TARIF_GENERAL:
            if (findAndRemoveJeton(joueurActuel, JetonInfluence::TARIF_GENERAL)) {
                LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" applique Tarif Général." << std::endl;
                for (uint32_t i = 0; i < NB_POSITIONS; ++i) {
                    Position pos = static_cast<Position>(i);
                    if (pos != m_positionJActuel && m_joueurs[pos].m_enJeu && !m_joueurs[pos].m_immunite) {
                        uint32_t montantPaye = std::min(1u, m_joueurs[pos].m_reserve);
                        m_joueurs[pos].m_reserve -= montantPaye;
                        m_joueurs[pos].m_pot += montantPaye;
                        LOGDEBUG << u8"    Joueur " << pos << u8" paie 1 jeton." << std::endl;
                    }
                }
                m_phaseActuelle = PHASE_PIOCHE;
            } 
            else result = false;
            break;

        case Action::PJ_TARIF_CIBLE_G:
        case Action::PJ_TARIF_CIBLE_F:
        case Action::PJ_TARIF_CIBLE_D:
            if (findAndRemoveJeton(joueurActuel, JetonInfluence::TARIF_CIBLE)) {
                Position targetPos = getActionAbsTargetPos(a, m_positionJActuel);
                if (m_joueurs[targetPos].m_enJeu && !m_joueurs[targetPos].m_immunite) {
                    uint32_t montantPaye = std::min(2u, m_joueurs[targetPos].m_reserve);
                    m_joueurs[targetPos].m_reserve -= montantPaye;
                    m_joueurs[targetPos].m_pot += montantPaye;
                    LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" applique Tarif Ciblé sur " << targetPos << u8". Il paie " << montantPaye << u8" jetons." << std::endl;
                }
                else {
                    LOGWARNING << u8"  ERREUR: Joueur "<<targetPos<<" ciblé hors jeu ou immunisée pour Tarif Ciblé." << std::endl;
                    result = false;
                }
                m_phaseActuelle = PHASE_PIOCHE;
            }
            else result = false;
            break;

        case Action::PJ_EMBARGO:
            if (findAndRemoveJeton(joueurActuel, JetonInfluence::EMBARGO)) {
                Position nextPos = m_positionJActuel;
                while (nextPos == m_positionJActuel || !m_joueurs[nextPos].m_enJeu) {
                    nextPos = static_cast<Position>((static_cast<uint32_t>(nextPos) + 1) % NB_POSITIONS);
                }
                if (!m_joueurs[nextPos].m_immunite) {
                    m_joueurs[nextPos].m_doitPasser = true;
                    LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" applique Embargo sur " << nextPos << "." << std::endl;
                }
                else {
                    LOGWARNING << u8"  ERREUR: Joueur " << nextPos << " ciblé immunisée ." << std::endl;
                    result = false;
                }
                m_phaseActuelle = PHASE_PIOCHE;
            }
            else result = false;
            break;

        case Action::PJ_DEVALORISATION:
            if (findAndRemoveJeton(joueurActuel, JetonInfluence::DEVALORISATION)) {
                m_sylopsAZero = true;
                LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" active Dévalorisation (Sylops=0)." << std::endl;
                m_phaseActuelle = PHASE_PIOCHE;
            }
            else result = false;
            break;

        case Action::PJ_IMMUNITE:
            if (findAndRemoveJeton(joueurActuel, JetonInfluence::IMMUNITE)) {
                joueurActuel.m_immunite = true;
                LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" active Immunité." << std::endl;
                m_phaseActuelle = PHASE_PIOCHE;
            }
            else result = false;
            break;

        case Action::PJ_AUDIT_GENERAL:
            if (findAndRemoveJeton(joueurActuel, JetonInfluence::AUDIT_GENERAL)) {
                LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" applique Audit Général." << std::endl;
                for (uint32_t i = 0; i < NB_POSITIONS; ++i) {
                    Position pos = static_cast<Position>(i);
                    if (pos != m_positionJActuel && m_joueurs[pos].m_enJeu && m_joueurs[pos].m_aPasseCeTour && !m_joueurs[pos].m_immunite) {
                        uint32_t montantPaye = std::min(2u, m_joueurs[pos].m_reserve);
                        m_joueurs[pos].m_reserve -= montantPaye;
                        m_joueurs[pos].m_pot += montantPaye;
                        LOGDEBUG << u8"    Joueur " << pos << u8" (a passé) paie " << montantPaye << u8" jetons." << std::endl;
                    }
                }
                m_phaseActuelle = PHASE_PIOCHE;
            }
            else result = false;
            break;

        case Action::PJ_AUDIT_CIBLE_G:
        case Action::PJ_AUDIT_CIBLE_F:
        case Action::PJ_AUDIT_CIBLE_D:
            if (findAndRemoveJeton(joueurActuel, JetonInfluence::AUDIT_CIBLE)) {
                Position targetPos = getActionAbsTargetPos(a, m_positionJActuel);
                if (targetPos != Position::P_INDEFINI && m_joueurs[targetPos].m_enJeu && m_joueurs[targetPos].m_aPasseCeTour && !m_joueurs[targetPos].m_immunite) {
                    uint32_t montantPaye = std::min(3u, m_joueurs[targetPos].m_reserve);
                    m_joueurs[targetPos].m_reserve -= montantPaye;
                    m_joueurs[targetPos].m_pot += montantPaye;
                    LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" applique Audit Ciblé sur " << targetPos << u8" (a passé). Il paie " << montantPaye << u8" jetons." << std::endl;
                }
                else {
                    LOGWARNING << u8"  ERREUR: Cible hors jeu ou immunisée pour Audit Ciblé." << std::endl;
                    result = false;
                }
                m_phaseActuelle = PHASE_PIOCHE;
            }
            else result = false;
            break;

        case Action::PJ_FRAUDE_MAJEURE:
            if (findAndRemoveJeton(joueurActuel, JetonInfluence::FRAUDE_MAJEURE)) {
                m_imposteurASix = true;
                LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" active Fraude Majeure (Imposteur=6)." << std::endl;
                m_phaseActuelle = PHASE_PIOCHE;                
            }
            else result = false;
            break;

        case Action::PJ_DETOURNEMENT:
            if (findAndRemoveJeton(joueurActuel, JetonInfluence::DETOURNEMENT)) {
                LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" applique Détournement." << std::endl;
                uint32_t totalDetourne = 0;
                for (uint32_t i = 0; i < NB_POSITIONS; ++i) {
                    Position pos = static_cast<Position>(i);
                    if (pos != m_positionJActuel && m_joueurs[pos].m_enJeu) {
                        uint32_t montantPris = std::min(1u, m_joueurs[pos].m_pot);
                        m_joueurs[pos].m_pot -= montantPris;
                        totalDetourne += montantPris;
                        LOGDEBUG << u8"    Prend " << montantPris << u8" jeton du pot de Joueur " << pos << "." << std::endl;
                    }
                }
                joueurActuel.m_pot += totalDetourne;
                LOGDEBUG << u8"    Total détourné dans le pot de Joueur " << m_positionJActuel << ": " << totalDetourne << "." << std::endl;
                m_phaseActuelle = PHASE_PIOCHE;
            }
            else result = false;
            break;

        case Action::PJ_CUISINER_LIVRES:
            if (findAndRemoveJeton(joueurActuel, JetonInfluence::CUISINER_LIVRES)) {
                m_sabaccInverse = true;
                LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" active Cuisiner Livres (Sabacc Inverse)." << std::endl;
                m_phaseActuelle = PHASE_PIOCHE;
            }
            else result = false;
            break;

        case Action::PJ_EPUISEMENT_G:
        case Action::PJ_EPUISEMENT_F:
        case Action::PJ_EPUISEMENT_D:
            if (findAndRemoveJeton(joueurActuel, JetonInfluence::EPUISEMENT)) {
                Position targetPos = getActionAbsTargetPos(a, m_positionJActuel);
                if (targetPos != Position::P_INDEFINI && m_joueurs[targetPos].m_enJeu && !m_joueurs[targetPos].m_immunite) {
                    LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" applique Épuisement sur " << targetPos << "." << std::endl;
                    JoueurSabacc& targetJoueur = m_joueurs[targetPos];
                    defausseCarteMain(targetPos, Famille::SABLE);
                    piocheCartePaquet(targetPos, Famille::SABLE,true,rng);
                    defausseCarteMain(targetPos, Famille::SANG);
                    piocheCartePaquet(targetPos, Famille::SANG, true, rng);
                    LOGDEBUG << u8"    Nouvelle main pour " << targetPos << ": [" << targetJoueur.m_carteSable << "|" << targetJoueur.m_carteSang << "]" << std::endl;
                }
                else {
                    LOGWARNING << u8"  Cible invalide ou immunisée pour Épuisement." << std::endl;
                    result = false;
                }
                m_phaseActuelle = PHASE_PIOCHE;
            }
            else result = false;
            break;

        //case Action::PJ_TRANSACTION_G:
        //case Action::PJ_TRANSACTION_F:
        //case Action::PJ_TRANSACTION_D:
        //    if (findAndRemoveJeton(joueurActuel, JetonInfluence::TRANSACTION)) {
        //        Position targetPos = getActionAbsTargetPos(a, m_positionJActuel);
        //        if (targetPos != Position::P_INDEFINI && m_joueurs[targetPos].m_enJeu && !m_joueurs[targetPos].m_immunite) {
        //            LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" applique Transaction avec " << targetPos << "." << std::endl;
        //            std::swap(joueurActuel.m_carteSable, m_joueurs[targetPos].m_carteSable);
        //            std::swap(joueurActuel.m_carteSang, m_joueurs[targetPos].m_carteSang);
        //            LOGDEBUG << u8"    Mains échangées." << std::endl;
        //        }
        //        else {
        //            LOGDEBUG << u8"  Cible invalide ou immunisée pour Transaction." << std::endl;
        //            result = false;
        //        }
        //        m_phaseActuelle = PHASE_PIOCHE;
        //    }
        //    else result = false;
        //    break;
        case Action::PJ_SABACC_PRIME:
            if (findAndRemoveJeton(joueurActuel, JetonInfluence::SABACC_PRIME)) {
                if (m_pListener)
                    m_pListener->surTirageDesAnnonce();
                joueurActuel.tireDes(rng); // tirage dés
                if (m_pListener)
                    m_pListener->surTirageDesResultat();
                LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" active Sabacc Prime et lance les dés: [" << joueurActuel.m_de1 << ", " << joueurActuel.m_de2 << "]" << std::endl;
                m_phaseActuelle = PHASE_JETON_SABACC_PRIME_DE; // Phase spéciale pour choisir le dé
                
            }
            else result = false;
            break;

        default:
            // Action fournie n'est pas une action de la PHASE_JETON
            LOGWARNING << u8"  ERREUR: Action illégale (" << a << u8") pour la phase actuelle (" << m_phaseActuelle << u8")." << std::endl;
            result = false;
            break;
        } 
        break; 
    } // Fin case PHASE_JETON

    case PHASE_JETON_SABACC_PRIME_DE: {
        // Switch interne basé sur l'Action pour la PHASE_JETON_SABACC_PRIME_DE
        switch (a) {
        case Action::PJSP_D1:
            if (joueurActuel.m_de1 != 0) {
                m_sabaccPrime = static_cast<Carte>(joueurActuel.m_de1);
                LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" choisit le dé 1 (" << joueurActuel.m_de1 << u8") pour Sabacc Prime. Nouvelle carte prime: " << m_sabaccPrime << std::endl;
                joueurActuel.m_de1 = 0; joueurActuel.m_de2 = 0;
                m_phaseActuelle = PHASE_PIOCHE;
            }
            else {
                LOGWARNING << u8"  ERREUR: Action PJSP_D1 invalide (dés non lancés ou déjà utilisés)." << std::endl;
                result = false;
            }
            break;
        case Action::PJSP_D2:
            if (joueurActuel.m_de2 != 0) {
                m_sabaccPrime = static_cast<Carte>(joueurActuel.m_de2);
                LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" choisit le dé 2 (" << joueurActuel.m_de2 << u8") pour Sabacc Prime. Nouvelle carte prime: " << m_sabaccPrime << std::endl;
                joueurActuel.m_de1 = 0; joueurActuel.m_de2 = 0;
                m_phaseActuelle = PHASE_PIOCHE;
            }
            else {
                LOGWARNING << u8"  ERREUR Action PJSP_D2 invalide (dés non lancés ou déjà utilisés)." << std::endl;
                result = false;
            }
            break;
        default:
            // Action fournie n'est pas une action de la PHASE_JETON_SABACC_PRIME_DE
            LOGWARNING << u8"  ERREUR: Action illégale (" << a << u8") pour la phase actuelle (" << m_phaseActuelle << u8")." << std::endl;
            result = false;
            break;
        } 
        break; 
    } // Fin case PHASE_JETON_SABACC_PRIME_DE

    case PHASE_PIOCHE: {
        switch (a) {
        case Action::PP_PASSE:
            LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" passe la phase Pioche." << std::endl;
            joueurActuel.m_aPasseCeTour = true;
            result &= finTourJoueurActuel(rng);
            break;

        case Action::PP_PIOCHE_PAQUET_SABLE:
        case Action::PP_PIOCHE_PAQUET_SANG:
        case Action::PP_PIOCHE_DEFAUSSE_SABLE:
        case Action::PP_PIOCHE_DEFAUSSE_SANG:
        { 
            joueurActuel.m_aPasseCeTour = false;
            bool isSable = (a == Action::PP_PIOCHE_PAQUET_SABLE || a == Action::PP_PIOCHE_DEFAUSSE_SABLE);
            bool isPaquet = (a == Action::PP_PIOCHE_PAQUET_SABLE || a == Action::PP_PIOCHE_PAQUET_SANG);

            bool mustPay = !joueurActuel.m_piocheGratuite;
            if (mustPay && joueurActuel.m_reserve < 1) {
                LOGWARNING << u8"  ERREUR: Joueur " << m_positionJActuel << u8" ne peut pas payer pour piocher." << std::endl;
                result = false;
                break; 
            }

            if (mustPay) {
                joueurActuel.m_reserve--;
                joueurActuel.m_pot++;
                LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" paie 1 jeton pour piocher." << std::endl;
            }
            else {
                LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" pioche gratuitement." << std::endl;
                joueurActuel.m_piocheGratuite = false;
            }

            if (isPaquet)
                piocheCartePaquet(m_positionJActuel, isSable ? Famille::SABLE : Famille::SANG, false,rng);
            else
                piocheCarteDefausse(m_positionJActuel, isSable ? Famille::SABLE : Famille::SANG);
            
            m_phaseActuelle = PHASE_DEFAUSSE; // Passe à la phase de défausse
        }
        break; // Fin case PP_PIOCHE_*

        default:            
            LOGWARNING << u8"  ERREUR: Action illégale (" << a << u8") pour la phase actuelle (" << m_phaseActuelle << u8")." << std::endl;
            result = false;
            break;
        } 
        break; 
    } // Fin case PHASE_PIOCHE

    case PHASE_DEFAUSSE: {
        
        switch (a) {
        case Action::PD_DEFAUSSE_CARTE_PIOCHEE:
        {
			if (joueurActuel.m_carteSablePiochee != Carte::C_INDEFINI) {
				defausseCartePiochee(m_positionJActuel, Famille::SABLE);
			}
			else if (joueurActuel.m_carteSangPiochee != Carte::C_INDEFINI) {
				defausseCartePiochee(m_positionJActuel, Famille::SANG);
			}
			else {
				LOGWARNING << u8" ERREUR: Action PD_DEFAUSSE_CARTE_PIOCHEE invalide (aucune carte en attente)." << std::endl;
				result = false;
			}
            result &= finTourJoueurActuel(rng);
        }
        break;

        case Action::PD_GARDE_CARTE_PIOCHEE:
        {            
            if (joueurActuel.m_carteSablePiochee != Carte::C_INDEFINI) {       
                LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" garde la carte Sable piochée et défausse l'ancienne (" << joueurActuel.m_carteSable << ")." << std::endl;
                defausseCarteMain(m_positionJActuel, Famille::SABLE);
            }
            else if (joueurActuel.m_carteSangPiochee != Carte::C_INDEFINI) {
                LOGDEBUG << u8"  Joueur " << m_positionJActuel << u8" garde la carte Sang piochée et défausse l'ancienne (" << joueurActuel.m_carteSang << ")." << std::endl;
                defausseCarteMain(m_positionJActuel, Famille::SANG);
            }
            else {
                LOGWARNING << u8"  ERREUR: Action PD_GARDE_CARTE_PIOCHEE invalide (aucune carte piochée en attente)." << std::endl;
                result = false;
            }
            result &= finTourJoueurActuel(rng);
        }
        break;

        default:
            // Action fournie n'est pas une action de la PHASE_DEFAUSSE
            LOGWARNING << u8"  ERREUR: Action illégale (" << a << u8") pour la phase actuelle (" << m_phaseActuelle << u8")." << std::endl;
            result = false;
            break;
        } 
        break; 
    } // Fin case PHASE_DEFAUSSE

    case PHASE_RESOLUTION: {
        switch (a) {
        case Action::PR_IMPOSTEUR_SABLE_D1:
            if (joueurActuel.m_carteSable == Carte::IMPOSTEUR && joueurActuel.m_de1 != 0) {
                LOGDEBUG << u8"  Résolution: Joueur " << m_positionJActuel << u8" choisit Dé 1 (" << joueurActuel.m_de1 << ") pour Imposteur Sable." << std::endl;
                joueurActuel.m_carteSable = static_cast<Carte>(joueurActuel.m_de1);
                joueurActuel.m_de1 = 0;
                joueurActuel.m_de2 = 0;
                if (joueurActuel.m_carteSang == Carte::IMPOSTEUR) {
                    if (m_pListener)
                        m_pListener->surTirageDesAnnonce();
                    joueurActuel.tireDes(rng); // Lance les dés
                    if (m_pListener)
                        m_pListener->surTirageDesResultat();
                    LOGDEBUG << u8"      Joueur " << static_cast<Position>(m_positionJActuel) << u8" lance les dés pour Imposteur: [" << joueurActuel.m_de1 << ", " << joueurActuel.m_de2 << "]" << std::endl;
                }
                else {
                    result &= finTourJoueurActuel(rng);
                }                    
            }
            else {
                LOGWARNING << u8"  ERREUR: Action PR_IMPOSTEUR_SABLE_D1 invalide (pas imposteur sable ou dés non lancés)." << std::endl;
                result = false;
            }
            break;
        case Action::PR_IMPOSTEUR_SABLE_D2:
            if (joueurActuel.m_carteSable == Carte::IMPOSTEUR && joueurActuel.m_de2 != 0) {
                LOGDEBUG << u8"  Résolution: Joueur " << m_positionJActuel << u8" choisit Dé 2 (" << joueurActuel.m_de2 << ") pour Imposteur Sable." << std::endl;
                joueurActuel.m_carteSable = static_cast<Carte>(joueurActuel.m_de2);
                joueurActuel.m_de1 = 0;
                joueurActuel.m_de2 = 0;
                if (joueurActuel.m_carteSang == Carte::IMPOSTEUR) {
                    if (m_pListener)
                        m_pListener->surTirageDesAnnonce();
                    joueurActuel.tireDes(rng); // Lance les dés
                    if (m_pListener)
                        m_pListener->surTirageDesResultat();
                    LOGDEBUG << u8"      Joueur " << static_cast<Position>(m_positionJActuel) << u8" lance les dés pour Imposteur: [" << joueurActuel.m_de1 << ", " << joueurActuel.m_de2 << "]" << std::endl;
                }
                else {
                    result &= finTourJoueurActuel(rng);
                }
            }
            else {
                LOGWARNING << u8" ERREUR: Action PR_IMPOSTEUR_SABLE_D2 invalide (pas imposteur sable ou dés non lancés)." << std::endl;
                result = false;
            }
            break;
        case Action::PR_IMPOSTEUR_SANG_D1:
            if (joueurActuel.m_carteSang == Carte::IMPOSTEUR && joueurActuel.m_de1 != 0) {
                LOGDEBUG << u8"  Résolution: Joueur " << m_positionJActuel << u8" choisit Dé 1 (" << joueurActuel.m_de1 << ") pour Imposteur Sang." << std::endl;
                joueurActuel.m_carteSang = static_cast<Carte>(joueurActuel.m_de1);
                joueurActuel.m_de1 = 0;
                joueurActuel.m_de2 = 0;
                if (!(joueurActuel.m_carteSable == Carte::IMPOSTEUR))
                    result &= finTourJoueurActuel(rng);
            }
            else {
                LOGWARNING << u8"  ERREUR: Action PR_IMPOSTEUR_SANG_D1 invalide (pas imposteur sang ou dés non lancés)." << std::endl;
                result = false;
            }
            break;
        case Action::PR_IMPOSTEUR_SANG_D2:
            if (joueurActuel.m_carteSang == Carte::IMPOSTEUR && joueurActuel.m_de2 != 0) {
                LOGDEBUG << u8"  Résolution: Joueur " << m_positionJActuel << u8" choisit Dé 2 (" << joueurActuel.m_de2 << ") pour Imposteur Sang." << std::endl;
                joueurActuel.m_carteSang = static_cast<Carte>(joueurActuel.m_de2);
                joueurActuel.m_de1 = 0;
                joueurActuel.m_de2 = 0;
                if (!(joueurActuel.m_carteSable == Carte::IMPOSTEUR))
                    result &= finTourJoueurActuel(rng);
            }
            else {
                LOGWARNING << u8"  ERREUR: Action PR_IMPOSTEUR_SANG_D2 invalide (pas imposteur sang ou dés non lancés)." << std::endl;
                result = false;
            }
            break;
        default:
            // Action fournie n'est pas une action de la PHASE_RESOLUTION
            LOGWARNING << u8"  ERREUR: Action illégale (" << a << u8") pour la phase actuelle (" << m_phaseActuelle << u8")." << std::endl;
            result = false;
            break;
        } 
        break; 
    } // Fin PHASE_RESOLUTION

    default:
        // Cas où m_phaseActuelle a une valeur inattendue
        LOGWARNING << u8" Phase de jeu inconnue ou non gérée: " << static_cast<int>(m_phaseActuelle) << std::endl;
        result = false;
        break;
    } 

    // Si une action a échoué (result == false), log supplémentaire
    if (!result) {
        LOGWARNING << u8"  --> executeAction a échoué pour l'action " << a << u8" par Joueur " << m_positionJActuel << u8" en phase " << m_phaseActuelle << std::endl;
    }

    return result;
}

bool JeuSabacc::finTourJoueurActuel(PCGAlea& rng)
{
    //joueur qui finit 
    Position posJoueurQuiFinit = m_positionJActuel;
    JoueurSabacc& joueurQuiFinit = m_joueurs[posJoueurQuiFinit]; 
    //LOGDEBUG << u8"  finTourJoueurActuel pour Joueur " << posJoueurQuiFinit << u8" (Phase actuelle avant fin: " << m_phaseActuelle << ")" << std::endl;    
    
    //joueur suivant, tour de table complet?
    bool tourDeTableComplet = false;
    Position posJoueurSuivantEnJeu = posJoueurQuiFinit;
    while (posJoueurSuivantEnJeu == posJoueurQuiFinit || !m_joueurs[posJoueurSuivantEnJeu].m_enJeu) {
        posJoueurSuivantEnJeu = static_cast<Position>((posJoueurSuivantEnJeu + 1) % NB_POSITIONS);
    }
    if (posJoueurSuivantEnJeu == m_positionJ1Manche)
        tourDeTableComplet = true;

    switch (m_phaseActuelle) {
    case PHASE_PIOCHE: //le joueur vient de faire PP_PASSE         
    case PHASE_DEFAUSSE: //le joueur vient de faire PD_DEFAUSSE_CARTE_PIOCHEE ou PD_GARDE_CARTE_PIOCHEE        
        //passe au joueur suivant en jeu
        m_positionJActuel = posJoueurSuivantEnJeu;

        //verifie si on a fini un tour de table
        if (tourDeTableComplet)
        {
            LOGDEBUG << u8"    Tour de table complet." << std::endl;
            //regarde si tout le monde a passé son tour
            bool toutLeMondeAPasse = true;
            for (auto& j : m_joueurs) {
                if (j.m_enJeu) {
                    toutLeMondeAPasse &= j.m_aPasseCeTour;
                }
            }
            //si fin du 3eme tour ou tout le monde a passé
            //--> RESOLUTION
            
            if (m_tour == 3 || toutLeMondeAPasse) {
                LOGDEBUG << u8"    Condition de résolution atteinte (Tour=" << m_tour << "/3 ou ToutLeMondeAPasse=" << toutLeMondeAPasse << ")." << std::endl;
                LOGDEBUG << u8"    Passage à la PHASE_RESOLUTION. Lancement des dés pour les Imposteurs..." << std::endl;

                m_phaseActuelle = PHASE_RESOLUTION;
                if (m_pListener)
                    m_pListener->surPhaseResolution();
                
                //met tous les imposteurs à 6 si nécessaire
                if (m_imposteurASix) {
                    for (uint32_t i = 0; i < NB_POSITIONS; ++i) {
                        Position jPos = static_cast<Position>((i + m_positionJActuel) % NB_POSITIONS);
                        JoueurSabacc& j = m_joueurs[jPos];
                        if (j.m_enJeu) {
                            if (j.m_carteSable == Carte::IMPOSTEUR)
                                j.m_carteSable = Carte::C6;
                            if (j.m_carteSang == Carte::IMPOSTEUR)
                                j.m_carteSang = Carte::C6;
                        }
                    }
                }
               
                //regarde si un imposteur à résoudre est présent et passe au joueur concerné si c'est le cas
                bool imposteurPresent = false;
                for (uint32_t i = 0; i < NB_POSITIONS; ++i) {
                    Position jPos = static_cast<Position>((i + m_positionJActuel) % NB_POSITIONS);
                    JoueurSabacc& j = m_joueurs[jPos];
                    if (j.m_enJeu && (j.m_carteSable == Carte::IMPOSTEUR || j.m_carteSang == Carte::IMPOSTEUR)) {
                        //premier joueur avec imposteurs devient joueur actuel
                        m_positionJActuel = jPos;
                        imposteurPresent = true;
                        if (m_pListener)
                            m_pListener->surTirageDesAnnonce();
                        j.tireDes(rng); // Lance les dés
                        if (m_pListener)
                            m_pListener->surTirageDesResultat();

                        LOGDEBUG << u8"      Joueur " << static_cast<Position>(jPos) << u8" lance les dés pour Imposteur: [" << j.m_de1 << ", " << j.m_de2 << "]" << std::endl;
                        LOGDEBUG << u8"      Joueur " << static_cast<Position>(jPos) << u8"devient joueur actuel" << std::endl;
                        break;
                    }
                }
                
                // Si aucun imposteur n'est présent, on peut aller directement à la logique de fin de manche.
                if (!imposteurPresent) {
                    LOGDEBUG << u8"    Aucun Imposteur à résoudre. Application directe de la résolution de manche." << std::endl;
                    finManche(); // Applique gains/pertes/éliminations
                    if (!m_jeuTermine) {
                        LOGDEBUG << u8"    Préparation de la manche suivante..." << std::endl;
                        initManche(rng); // Prépare la manche suivante (MISE A JOUR: m_manche, m_tour, m_positionJActuel, m_phaseActuelle etc.)
                    }
                }
                else {
                    LOGDEBUG << u8"    Le joueur " << m_positionJActuel << " commence la résolution des Imposteurs." << std::endl;
                }
            }
            else {
                //sinon c un nouveau tour normal
                LOGDEBUG << u8"    Début du tour suivant." << std::endl;
                initTour(); // MISE A JOUR: m_tour, reset m_aPasseCeTour
                // MISE A JOUR: m_phaseActuelle (remis à JETON par initTour implicitement ou explicitement)
                m_phaseActuelle = PHASE_JETON; // Assure qu'on recommence par la phase jeton
            }
        }
        else {
            // Tour de table non complet, on passe juste au joueur suivant                 
            m_phaseActuelle = PHASE_JETON;
            // Réinitialise doitPasser pour le joueur qui vient de jouer (au cas où il était sous embargo)
            joueurQuiFinit.m_doitPasser = false;
        }
        break; //Fin de PHASE_PIOCHE et PHASE_DEFAUSSE
    
    case PHASE_RESOLUTION: // Un joueur vient de finir de choisir la valeur de son/ses Imposteur(s)
    
        //passe au joueur suivant en jeu
        m_positionJActuel = posJoueurSuivantEnJeu;
        LOGDEBUG << u8"    --> Résolution Imposteur: Joueur suivant: " << m_positionJActuel << std::endl;

        //verifie si on a fini un tour de table
        if (tourDeTableComplet)
        {
            LOGDEBUG << u8"    Tour de table de résolution des Imposteurs terminé." << std::endl;
            // Tous les imposteurs ont été résolus (ou il n'y en avait pas).
            // Appliquer la logique de fin de manche (calcul scores, pénalités, éliminations).
            finManche();

            if (!m_jeuTermine) {
                // Si le jeu n'est pas terminé, on démarre une nouvelle manche.
                LOGDEBUG << u8"    Préparation de la manche suivante..." << std::endl;
                initManche(rng);
            }
        }
        else {
            // Le tour de table de résolution n'est pas fini.
            // cherche le prochain joueur avec un imposteur non résolu et tire les dés
            bool imposteurPresent = false;
            for (uint32_t i = 0; i < NB_POSITIONS; ++i) {
                Position jPos = static_cast<Position>((i + m_positionJActuel) % NB_POSITIONS);
                JoueurSabacc& j = m_joueurs[jPos];
                if (j.m_enJeu && (j.m_carteSable == Carte::IMPOSTEUR || j.m_carteSang == Carte::IMPOSTEUR)) {
                    //premier joueur avec imposteurs devient joueur actuel
                    m_positionJActuel = jPos;
                    imposteurPresent = true;
                    if (m_pListener)
                        m_pListener->surTirageDesAnnonce();
                    j.tireDes(rng); // Lance les dés
                    if (m_pListener)
                        m_pListener->surTirageDesResultat();
                    LOGDEBUG << u8"      Joueur " << static_cast<Position>(jPos) << u8" lance les dés pour Imposteur: [" << j.m_de1 << ", " << j.m_de2 << "]" << std::endl;
                    LOGDEBUG << u8"      Joueur " << static_cast<Position>(jPos) << u8"devient joueur actuel" << std::endl;
                    break;
                }
            }

            // Si aucun imposteur n'est présent, on peut aller directement à la logique de fin de manche.
            if (!imposteurPresent) {
                LOGDEBUG << u8"    Aucun Imposteur à résoudre. Application directe de la résolution de manche." << std::endl;
                finManche(); // Applique gains/pertes/éliminations
                if (!m_jeuTermine) {
                    LOGDEBUG << u8"    Préparation de la manche suivante..." << std::endl;
                    initManche(rng); // Prépare la manche suivante (MISE A JOUR: m_manche, m_tour, m_positionJActuel, m_phaseActuelle etc.)
                }
            }
            else {
                LOGDEBUG << u8"    Le joueur " << m_positionJActuel << " commence la résolution des Imposteurs." << std::endl;
            }
        }
        break;// Fin de PHASE_RESOLUTION
    
    default:
        LOGWARNING << u8"ERREUR finTourJoueurActuel: Appel depuis une phase inattendue (" << m_phaseActuelle << ")!" << std::endl;
        return false; // Erreur        
    }
    if(!m_jeuTermine)
        LOGDEBUG << u8"  Nouvelle phase: " << m_phaseActuelle << ", Joueur actuel: " << m_positionJActuel << std::endl;

    if (m_pListener)
        m_pListener->surFinTourJoueur();

    return true; // Succès
}

struct PlayerScore {
    Position pos;
    uint32_t score = 0;         // Score combiné pour le tri
    uint32_t diff = 0;          // Différence absolue des cartes
    uint32_t sum = 0;           // Somme des cartes

};

bool JeuSabacc::finManche()
{

    LOGDEBUG << "Manche terminée, calcul des scores";

    PlayerScore scores[NB_POSITIONS];

    // 1. & 2. Résolution des Sylops et Calcul des Scores
    // 3. cherche le plus petit score

    uint32_t petitScore = 10000;
    LOGDEBUG << u8"  1. Résolution des Sylops et Calcul des Scores..." << std::endl;
    for (uint32_t i = 0; i < NB_POSITIONS; ++i) {
        Position currentPos = static_cast<Position>(i);
        JoueurSabacc& joueur = m_joueurs[currentPos];

        if (!joueur.m_enJeu) continue; // Ignorer les joueurs éliminés

        // chaque sylop copie la valeur de l'autre carte sauf si sylopAZero
        if(!m_sylopsAZero){
            if (joueur.m_carteSable == Carte::SYLOP)
                joueur.m_carteSable = joueur.m_carteSang;
            if (joueur.m_carteSang == Carte::SYLOP)
                joueur.m_carteSang = joueur.m_carteSable;
        }

        // --- Calcul score ---
        PlayerScore& currentScore= scores[currentPos];
        currentScore.pos = currentPos;
        currentScore.diff = std::abs(joueur.m_carteSable - joueur.m_carteSang);
        currentScore.sum = joueur.m_carteSable + joueur.m_carteSang;

        //gère le sabaccInverse, n'affecte pas le sabacc sylop
        if (m_sabaccInverse && currentScore.diff == 0 && currentScore.sum > 0) {
            currentScore.sum = 14 - currentScore.sum;
        }

        // gère le sabacc prime
        if (m_sabaccPrime != Carte::C_INDEFINI && joueur.m_carteSable == m_sabaccPrime && joueur.m_carteSang == m_sabaccPrime) {
            currentScore.sum = 1;
        }

        currentScore.score = currentScore.diff * 100 + currentScore.sum;        
        petitScore = (currentScore.score < petitScore) ? currentScore.score : petitScore;
        LOGDEBUG << u8"    Joueur " << currentPos << u8": Cartes[" << joueur.m_carteSable << "|" << joueur.m_carteSang << u8"], Diff=" << currentScore.diff << u8", Sum=" << currentScore.sum << u8", Score=" << currentScore.score << (m_sabaccInverse ? " (Inv)" : "") << std::endl;
    }

    // 4. Attribuer gains et pertes
    LOGDEBUG << u8"  3. Attribution Gains/Pertes..." << std::endl;

    uint32_t nbJEnJeu = 0; //nombre de joueurs encore en jeu
    Position posGagnant = P_INDEFINI;
    for (uint32_t i = 0; i < NB_POSITIONS; ++i) {
        Position currentPos = static_cast<Position>(i);
        JoueurSabacc& joueur = m_joueurs[currentPos];
        PlayerScore& currentScore = scores[currentPos];

        if (!joueur.m_enJeu) continue; // Ignorer les joueurs éliminés

        if (currentScore.score == petitScore) {
            // --- Gagnant(s) ---
            LOGDEBUG << u8"    Gagnant: Joueur " << currentPos << u8" récupère son pot de " << joueur.m_pot << ". Réserve -> " << joueur.m_reserve << std::endl;
            if (m_pListener)
                m_pListener->surMAJReserveJoueur(currentPos, joueur.m_reserve, joueur.m_pot);
            joueur.m_reserve += joueur.m_pot; // Récupère son pot            
            joueur.m_pot = 0;
            
            nbJEnJeu++;
        }
        else {
            // --- Perdant(s) ---
            uint32_t penalite = std::max(1u, currentScore.diff);
            LOGDEBUG << u8"    Perdant: Joueur " << currentPos << u8" perd son pot de " << joueur.m_pot << u8" + pénalité de " << penalite << " jetons" << std::endl;
            if (m_pListener)
                m_pListener->surMAJReserveJoueur(currentPos, joueur.m_reserve, -(static_cast<int>(std::min(penalite, joueur.m_reserve))));

            joueur.m_pot = 0;
            joueur.m_reserve -= std::min(penalite, joueur.m_reserve);
            
            
            if (joueur.m_reserve != 0) {
                nbJEnJeu++;
            }
            else {
                LOGDEBUG << u8"    Joueur " << currentPos << u8" est éliminé" << std::endl;
                joueur.m_enJeu = false;
            }
        }
    }

    
    if (nbJEnJeu == 1) {
        m_jeuTermine = true;
        for (uint32_t i = 0; i < NB_POSITIONS; ++i) {
            Position currentPos = static_cast<Position>(i);
            JoueurSabacc& joueur = m_joueurs[currentPos];
            PlayerScore& currentScore = scores[currentPos];
            if (joueur.m_enJeu) {
                m_vainqueur = currentPos;
                break;
            }
        }
        LOGDEBUG << u8"#############################################" << std::endl;
        LOGDEBUG << u8"### Jeu Terminé! Vainqueur: Joueur " << m_vainqueur << " ###" << std::endl;
        LOGDEBUG << u8"#############################################" << std::endl;
        if (m_pListener)
            m_pListener->surJeuTermine();            
    }
    else if (nbJEnJeu == 0) {
        LOGWARNING << u8"  ERREUR: zero joueur en jeu!" << std::endl;
    }
    else {
        LOGDEBUG << u8"  Manche terminée. " << nbJEnJeu << u8" joueurs restants." << std::endl;
    }

    return true;
}


std::ostream& operator<<(std::ostream& os, const JeuSabacc& jeu)
{
    // Entête avec infos générales du jeu, bien tabulé
    os << u8"SABACC Manche: " << jeu.m_manche << u8" Tour: " << jeu.m_tour << u8"/3 Phase: ";

    os << jeu.m_phaseActuelle;
    os << std::endl;

    // Info paquets avec tabulations pour alignement
    os << u8"Paquets: \033[33m Sable: " << jeu.m_paquetSable.getNbCartes()
        << u8" \033[31m Sang: " << jeu.m_paquetSang.getNbCartes()
        << u8"\033[0m \t| Défausses: \033[33m Sable: " << jeu.m_defausseSable.regarde() << " (" << jeu.m_defausseSable.getNbCartes()<<") "
        << u8"\033[31m Sang: " << jeu.m_defausseSang.regarde()<<" ("<<jeu.m_defausseSang.getNbCartes()<<") " << "\033[0m" << std::endl;

    // Effets globaux actifs, alignés
    os << u8"Effets globaux:\t";
    bool hasGlobalEffects = false;
    if (jeu.m_sylopsAZero) {
        os << u8"[Sylops=0]\t";
        hasGlobalEffects = true;
    }
    if (jeu.m_imposteurASix) {
        os << u8"[Imposteur=6]\t";
        hasGlobalEffects = true;
    }
    if (jeu.m_sabaccInverse) {
        os << u8"[Sabacc Inverse]\t";
        hasGlobalEffects = true;
    }
    if (jeu.m_sabaccPrime != Carte::C_INDEFINI) {
        os << u8"[Sabacc Prime=" << jeu.m_sabaccPrime << u8"]\t";
        hasGlobalEffects = true;
    }
    if (!hasGlobalEffects) {
        os << u8"Aucun";
    }
    os << std::endl;

    // Affichage des joueurs
    os << u8"--- Joueurs ---" << std::endl;
    for (uint32_t i = 0; i < NB_POSITIONS; ++i) {
        Position pos = static_cast<Position>(i);
        os << pos << (pos == jeu.m_positionJActuel ? u8"(*)" : u8"   ") << u8"\t";
        //if (pos != Position::S && jeu.m_phaseActuelle!=PHASE_RESOLUTION) {
        //    JoueurSabacc js(jeu.m_joueurs[i]);
        //    if (!jeu.m_connaissanceJoueurs[i].m_carteSableConnueDesAutres)
        //        js.m_carteSable = Carte::C_INDEFINI;
        //    if (!jeu.m_connaissanceJoueurs[i].m_carteSangConnueDesAutres)
        //        js.m_carteSang = Carte::C_INDEFINI;
        //    if (!jeu.m_connaissanceJoueurs[i].m_aPiocheDefausse) {
        //        js.m_carteSablePiochee = Carte::C_INDEFINI;
        //        js.m_carteSangPiochee = Carte::C_INDEFINI;
        //    }
        //    os << js<< std::endl;
        //} else
        os << jeu.m_joueurs[i] << std::endl;
    }

    return os;
}

std::ostream& operator<<(std::ostream& os, const JoueurSabacc& joueur)
{
    // Ligne 1: État, cartes, réserve et pot avec tabulations pour alignement
    os << (joueur.m_enJeu ? u8"\033[32m[En jeu]\033[0m\t" : u8"\033[31m[Éliminé]\t\033[0m");
    os << u8"Main:";

    if (joueur.m_carteSablePiochee != Carte::C_INDEFINI)
        os << "\033[33m" << joueur.m_carteSablePiochee<<"\033[0m+";
    os << u8"[ \033[33m" << joueur.m_carteSable << "\033[0m| \033[31m" << joueur.m_carteSang << "\033[0m]";    
    if (joueur.m_carteSangPiochee != Carte::C_INDEFINI)
        os << "+ \033[31m" << joueur.m_carteSangPiochee;
    os << "\033[0m\t";
    os << u8"R:" << joueur.m_reserve << u8"\tP:" << joueur.m_pot << "\t";
    if (joueur.m_de1 != 0) {
        os << u8"Dés:" << joueur.m_de1 << u8"/" << joueur.m_de2<< "\t";
    }   

    // Affichage des effets actifs
    if (joueur.m_doitPasser || joueur.m_immunite || joueur.m_piocheGratuite) {
        os << u8"Effets:";
        if (joueur.m_doitPasser)     os << u8"[Passer]";
        if (joueur.m_immunite)       os << u8"[Immunité]";
        if (joueur.m_piocheGratuite) os << u8"[Pioche Gratuite]";
        os << u8"\t";
    }
    else {
        os << u8"\t\t";
    }

    // Affichage des jetons d'influence sur la même ligne
    os << u8"JI:";
    for (int i = 0; i < 3; ++i) {
        if (joueur.m_jetonsInfluence[i] != JetonInfluence::JI_INDEFINI) {
            os << u8" [" << joueur.m_jetonsInfluence[i] << u8"]";
        }
    }

    return os;
}

void JoueurSabacc::tireDes(PCGAlea& rng)
{
     m_de1 = rng.aleaInferieurA(6) + 1; 
     m_de2 = rng.aleaInferieurA(6) + 1;     
}
