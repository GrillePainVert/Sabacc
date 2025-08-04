#include "SabaccUI.h"
#include "SabaccModel.h"
#include "eval.h"
#include "logger.h"
#include <algorithm>


SabaccUI::SabaccUI() : m_jeu(), m_vue(m_jeu), m_rng(rand())
{
    LOGDEBUG.setActif(true);
    m_vue.setListener(this);
    m_jeu.setListener(this);
    m_jeu.m_joueurs[Position::S].m_type = TYPE_JOUEUR::TJ_HUMAIN;
	m_jeu.m_joueurs[Position::O].m_type = TYPE_JOUEUR::TJ_MONTE_CARLO_SIMPLE;
	m_jeu.m_joueurs[Position::N].m_type = TYPE_JOUEUR::TJ_MONTE_CARLO_SIMPLE;
	m_jeu.m_joueurs[Position::E].m_type = TYPE_JOUEUR::TJ_MONTE_CARLO_SIMPLE;
    
    //lance le thread de la vue qui va la créer
    m_vue.demarre();

    LOGDEBUG.setActif(false);
}

//boucle du thread de l'UI
void SabaccUI::boucle()
{    
    while (!m_fermeFenetreDemande) {
        //au tour d'une IA de jouer.
        if (m_jeu.m_joueurs[m_jeu.m_positionJActuel].m_type != TYPE_JOUEUR::TJ_HUMAIN &&
            !m_jeu.m_jeuTermine && !m_vue.animationEnCours() && !m_fermeFenetreDemande) {
            
            //choisis action
            Action acChoisie;
            LOGDEBUG.setActif(false); // Keep console clean during AI eval
            if (m_jeu.m_joueurs[m_jeu.m_positionJActuel].m_type == TYPE_JOUEUR::TJ_MONTE_CARLO_SIMPLE) {
                acChoisie = choisisActionMonteCarloSimple(m_jeu, 150000, m_rng);
            }
            LOGDEBUG.setActif(true);
            
            //message
            m_vue.ajouteLogMessage(acChoisie);
            m_vue.ajouteAnimationAvantAction(acChoisie);
            while (m_vue.animationEnCours()) Sleep(10);
            m_vue.ajouteAnimationApresAction(acChoisie);
            Sleep(10);
            m_jeu.executeAction(acChoisie, m_rng);
            while (m_vue.animationEnCours() && !m_fermeFenetreDemande) Sleep(10);
        }
        
        //Tour d'un humain
        else if (m_jeu.m_joueurs[m_jeu.m_positionJActuel].m_type == TYPE_JOUEUR::TJ_HUMAIN && !m_jeu.m_jeuTermine && !m_vue.animationEnCours() && !m_fermeFenetreDemande) {
            std::vector<Action> legalActions;            
            m_actionJeuChoisie = Action::A_INDEFINIE;
            m_jeu.listerActionsLegales(legalActions);
            m_vue.definitActionsJeuAChoisir(legalActions);

            //attend d'avoir reçu une action choisie et que toutes les animations soient finies
            while (!m_redemarrageDemande && !m_fermeFenetreDemande &&( m_vue.animationEnCours()|| (m_actionJeuChoisie == Action::A_INDEFINIE && m_jeu.m_joueurs[m_jeu.m_positionJActuel].m_type == TYPE_JOUEUR::TJ_HUMAIN && !m_jeu.m_jeuTermine)))
                Sleep(10);
            Action aChoisie = m_actionJeuChoisie;
            m_actionJeuChoisie = Action::A_INDEFINIE;
            
            if (aChoisie != Action::A_INDEFINIE) {
                m_vue.videActionsJeuAChoisir();

                m_vue.ajouteLogMessage(aChoisie);
                m_vue.ajouteAnimationAvantAction(aChoisie);
                while (m_vue.animationEnCours()) Sleep(10);
                m_vue.ajouteAnimationApresAction(aChoisie);
                Sleep(10);
                bool success = m_jeu.executeAction(aChoisie, m_rng);
                while (m_vue.animationEnCours()) Sleep(10);
            }
        }

        if (m_redemarrageDemande) {
            m_redemarrageDemande = false;
            //attend la fin des animations
            m_vue.effacerGrandMessages();
            
            while (m_vue.animationEnCours()) {
                Sleep(15);
            }
            m_jeu.initJeu(m_rng);
        }

		Sleep(100);
	}
    //ferme la vue
    m_vue.stoppeEtFerme();
}

void SabaccUI::surPhaseResolution()
{
    std::stringstream ss;
    ss << u8"Résolution de la manche";
    m_vue.ajouteGrandMessage(ss.str());
	m_vue.ajouteLogMessage(ss.str());
}

void SabaccUI::surJeuTermine()
{
    std::stringstream ss;
    if(m_jeu.m_vainqueur==Position::S)
        ss << u8"Vous avez gagné!!";
    else
        ss << u8"Jeu terminé, vainqueur: " << m_jeu.m_vainqueur;
    m_vue.ajouteGrandMessagePermanent(ss.str());

    //calcul du score si on est vainqueur
    if (m_jeu.m_vainqueur == Position::S) {
        int score = 1000 * m_jeu.m_joueurs[Position::S].m_reserve;
        m_score3Parties.push_front(score);
        if(m_score3Parties.size()>3) {
            m_score3Parties.pop_back();
		}
    }
    m_vue.setScoresPrisEnCompte(m_score3Parties);
}

void SabaccUI::surActionJeuChoisie(Action a)
{
    m_actionJeuChoisie = a;
}

void SabaccUI::surRedemarreJeu()
{    
    m_vue.effacerAnimations();
    m_redemarrageDemande = true;    

}

void SabaccUI::surFinTourJoueur()
{
    m_vue.ajouteAnimationSon(Son::SON_WOOSH);
}

void SabaccUI::surTirageDesAnnonce()
{
    std::stringstream ss;
    if (m_jeu.m_positionJActuel == Position::S) {
        MessageBox(m_vue.getFenetre().getNativeHandle(), L"Appuyez pour lancer les dés", L"Lancer de dés", MB_OK);
    }
}

void SabaccUI::surTirageDesResultat()
{
    std::stringstream ss;
    if (m_jeu.m_positionJActuel == Position::S) {
        ss << u8"Vous obtenez ";
    }
    else {
        ss << u8"Joueur " << m_jeu.m_positionJActuel << u8" obtient ";
    }

    ss << m_jeu.m_joueurs[m_jeu.m_positionJActuel].m_de1 << u8" et " << m_jeu.m_joueurs[m_jeu.m_positionJActuel].m_de2 << u8" aux dés";
    m_vue.ajouteLogMessage(ss.str());
    m_vue.ajouteGrandMessage(ss.str());
}

void SabaccUI::surScoreReset()
{
    m_score3Parties.clear();
}

void SabaccUI::surFermeFenetre()
{
    m_fermeFenetreDemande = true;
}

void SabaccUI::surFinInitJeu()
{    
    m_vue.ajouteLogMessage("__________Nouvelle Partie__________"); 
    m_vue.ajouteAnimationSon(Son::SON_CLOCHE);
    m_vue.ajouteGrandMessage("Nouvelle Partie");

    while (m_vue.animationEnCours())
        Sleep(20);
}

void SabaccUI::surFinInitManche()
{
    std::stringstream ss;
    ss << "____________Manche " << m_jeu.m_manche<< "____________" ;
    m_vue.ajouteLogMessage(ss.str());
    
    ss = {};
    ss << "Manche " << m_jeu.m_manche;
    m_vue.ajouteAnimationSon(Son::SON_WOOSH);
    m_vue.ajouteGrandMessage(ss.str());

    ss = {};
    ss << "Joueur " << m_jeu.m_positionJActuel<<" commence";
    m_vue.ajouteAnimationSon(Son::SON_WOOSH);
    m_vue.ajouteGrandMessage(ss.str());

    while (m_vue.animationEnCours() && !m_fermeFenetreDemande)
        Sleep(20);
}

void SabaccUI::surMAJReserveJoueur(Position p, uint32_t reserveActuelle, int delta)
{
    std::stringstream ss;
            
    if(reserveActuelle + delta <=0) { 
        //éliminé cette manche-ci
        if (p == Position::S)
            ss << u8"Vous êtes éliminé";
        else
            ss << u8"Joueur " << p << u8" éliminé";
            
        m_vue.ajouteLogMessage(ss.str());
        m_vue.ajouteGrandMessage(ss.str());
    }
    else if(reserveActuelle !=0){
        //joueur survivant            
        if (p == Position::S) {
            std::string s;
            if (delta < 0)
                s = u8" perdez ";
            else 
                s = u8" récupérez ";

            ss << u8"Vous " << s << abs(delta) << u8" jeton(s)";
        }
        else {
            std::string s;
            if (delta < 0)
                s = u8" Perd ";
            else
                s = u8" Récupère ";

            ss << u8"Joueur " << p << s << abs(delta) << u8" jeton(s)";
        }
            
        m_vue.ajouteLogMessage(ss.str());
        m_vue.ajouteGrandMessage(ss.str());
    }
    
    //LorexZen joue à Ghost Recon Breakpoint comme à CallOf.
    
    //calcul du score si on est perdant
    if (reserveActuelle + delta <= 0 && p == Position::S) {        
        //je cherche la reserve la plus grande chez les autres joueurs
        uint32_t maxReserveJoueurs = std::max(m_jeu.m_joueurs[Position::E].m_reserve, m_jeu.m_joueurs[Position::N].m_reserve);
        maxReserveJoueurs = std::max(maxReserveJoueurs, m_jeu.m_joueurs[Position::O].m_reserve);
        int score = -100 * (int)maxReserveJoueurs;
        m_score3Parties.push_front(score);
        if (m_score3Parties.size() > 3) {
            m_score3Parties.pop_back();
        }
        m_vue.setScoresPrisEnCompte(m_score3Parties);
    }

	//attend la fin des animations
    while (m_vue.animationEnCours()) {
        Sleep(15);
    }
}
