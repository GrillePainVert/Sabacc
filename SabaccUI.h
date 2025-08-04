#pragma once
#include "SabaccUI.h"
#include "SabaccModel.h"
#include "SabaccView.h"
#include "windows.h"

class SabaccUI : JeuSabaccListener, SabaccVueListener{
public:
    SabaccUI();

    //boucle du thread de l'UI
    void boucle();

private:
    //flags enregistrant les demandes faite via la vue
    bool m_redemarrageDemande = false;
    bool m_fermeFenetreDemande = false;
    
    //générateur de nombres aléatoires rapide
    PCGAlea m_rng;

    //logique du jeu
    JeuSabacc m_jeu;

    //vue
    SabaccVue m_vue;

    //action choisie dans la vue
    Action m_actionJeuChoisie=Action::A_INDEFINIE;
    
    //stocke le score des 3 dernières parties
    std::deque<int> m_score3Parties;

    // Héritées de JeuSabaccListener
    void surFinInitJeu() override;
    void surFinInitManche() override;
    void surJeuTermine() override;
    void surMAJReserveJoueur(Position p, uint32_t reserveActuelle, int delta) override;
    void surPhaseResolution() override;
    void surTirageDesAnnonce() override;
    void surFinTourJoueur() override;
    void surTirageDesResultat() override;
    
    // Héritées de SabaccViewListener
    void surActionJeuChoisie(Action a) override;
    void surRedemarreJeu() override;                   
    void surFermeFenetre() override;
    void surScoreReset() override;
};
