#pragma once
#include "SabaccUI.h"
#include "SabaccModel.h"
#include "SabaccView.h"
#include "windows.h"

class SabaccUI : JeuSabaccListener, SabaccVueListener{
public:
    SabaccUI();    
    void boucle();

private:
    bool m_redemarrageDemande = false;
    bool m_fermeFenetreDemande = false;

    PCGAlea m_rng;
    JeuSabacc m_jeu;
    SabaccVue m_vue;

    Action m_actionJeuChoisie=Action::A_INDEFINIE;
    std::deque<int> m_score3Parties;

    // Inherited via JeuSabaccListener
    void surFinInitJeu() override;
    void surFinInitManche() override;
    void surJeuTermine() override;
    void surMAJReserveJoueur(Position p, uint32_t reserveActuelle, int delta) override;
    void surPhaseResolution() override;
    void surTirageDesAnnonce() override;
    void surFinTourJoueur() override;
    void surTirageDesResultat() override;
    
    // Inherited via SabaccViewListener
    void surActionJeuChoisie(Action a) override;
    void surRedemarreJeu() override;                   
    void surFermeFenetre() override;
    void surScoreReset() override;
};
