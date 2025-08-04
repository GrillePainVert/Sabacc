#pragma once
#include <algorithm>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "SabaccModel.h" 
#include "logger.h" 
#include <map>
#include <thread>
#include <queue>

// UI constants
//fonts sizes
const float TEXT_PROPORTION_MINI = 0.04f;
const float TEXT_PROPORTION_PETIT = 0.05f;
const float TEXT_PROPORTION_NORMAL = 0.06f;
const float TEXT_PROPORTION_MOYEN = 0.07f;
const float TEXT_PROPORTION_GRAND = 0.09f;
const float TEXT_PROPORTION_TITRE = 0.05f;
const float TEXT_PROPORTION_FIN_JEU = 0.1f;

//couleurs
const sf::Color COULEURS_JOUEURS[NB_POSITIONS] = {
    sf::Color(255, 100, 100), // N (Reddish)
    sf::Color(100, 255, 100), // E (Greenish)
    sf::Color(100, 100, 255), // S (Bluish) joueur humain
    sf::Color(255, 255, 100)  // O (Yellowish)
};

//boite de messages à gauche
const float BOITE_MESSAGES_HAUTEUR = 150.f;
const float BOITE_MESSAGES_REMPLISSAGE = 10.f;
const unsigned int MAX_MESSAGES = 18;

//durée des animations
const uint32_t CARTE_ANIM_DUREE_LENTE = 60;
const uint32_t CARTE_ANIM_DUREE_RAPIDE = 20;
const uint32_t MESSAGE_DUREE_LENTE = 70;
const uint32_t MESSAGE_DUREE_RAPIDE = 40;

//action d'un bouton
enum ActionBouton {
    VA_INDEFINI=0,
    VA_NOUVELLE_PARTIE,
    VA_AFFICHER_CACHER_CARTES,
    VA_CHANGE_VITESSE,
    VA_FERME
};

//Bouton de la vue
struct BoutonCliquable {
    Action gameAction = Action::PJ_PASSE;
    ActionBouton ViewAction = VA_INDEFINI;
    sf::Text textShape;
    sf::RectangleShape backgroundShape; 
    sf::FloatRect bounds;

    BoutonCliquable(sf::Font& f) : textShape(f) {}
};

//cible possible d'une animation
enum SVAnimationCible {
    AC_INDEFINI = 0,
	AC_PAQUET_SABLE = 1,
	AC_PAQUET_SANG = 2,
	AC_DEFAUSSE_SABLE = 4,
	AC_DEFAUSSE_SANG = 8,
	AC_CARTE_SANG_JOUEUR = 16,
	AC_CARTE_SABLE_JOUEUR = 32,
	AC_CARTE_PIOCHEE_JOUEUR = 64,
    AC_J1 = 128,
    AC_J2 = 256,
    AC_J3 = 512
};

//types d'animations possibles
enum SVAnimationType {
    AT_ZOOM,    
    AT_DEZOOM,
    AT_DISPARAIT,
    AT_APPARAIT,
    AT_SOUND,
    AT_GRAND_MESSAGE
};

//sons
enum Son {
    SON_INDEFINI=-1,
    SON_JETON,
    SON_CARD,
    SON_WOOSH,
    SON_CLOCHE
};

//animation
struct SVAnimation {
    Position pJoueur;
    uint32_t cible;
    SVAnimationType type;
    uint32_t dureeInitiale;
    uint32_t dureeRestante;
    Son son;
    std::string message;
};

//listener, methodes qu'une classe doit implémenter pour pouvoir écouter la vue
class SabaccVueListener {
public:	
    virtual void surActionJeuChoisie(Action a) = 0; //lorsque l'utilisateur a choisi une action de jeu par bouton
	virtual void surRedemarreJeu() = 0; //redémarrage demandé de la partie
	virtual void surFermeFenetre() = 0; //fermeture de fenêtre demandée
    virtual void surScoreReset() = 0;  //réset du score suite à affichage des cartes cachées
};

//vue du jeu
class SabaccVue {
public:
    SabaccVue(JeuSabacc& game);
    virtual ~SabaccVue();
    void setListener(SabaccVueListener* pListener) { m_pListener = pListener; };

    void demarre();
    void stoppeEtFerme();

    //gestion des actions de jeu
    void definitActionsJeuAChoisir(std::vector<Action>& va) { m_actionsAChoisir = va; };
    void videActionsJeuAChoisir() { m_actionsAChoisir.clear(); };
    
    //boîte de messages à gauche de l'écran
    void ajouteLogMessage(const std::string& message);
    void ajouteLogMessage(const std::ostringstream& sstr);
    void ajouteLogMessage(const Action& a);

    //gestion des animations
    void ajouteAnimationSon(Son s);
    void ajouteAnimationAvantAction(Action a);
    void ajouteAnimationApresAction(Action a);
    void effacerAnimations() { m_animations = {}; };
    bool animationEnCours() { return (m_animations.size() != 0); };
    void ajouteGrandMessage(const std::string& s) { ajouteAnimation(Position::P_INDEFINI, SVAnimationCible::AC_INDEFINI, SVAnimationType::AT_GRAND_MESSAGE , m_messageDuree, Son::SON_INDEFINI, s); };
    void ajouteGrandMessagePermanent(const std::string& s) { ajouteAnimation(Position::P_INDEFINI, SVAnimationCible::AC_INDEFINI, SVAnimationType::AT_GRAND_MESSAGE, -1, Son::SON_INDEFINI, s); };
    void effacerGrandMessages() { m_grandMessage = ""; while (m_animations.size()!=0 && m_animations.front().type == SVAnimationType::AT_GRAND_MESSAGE) { m_animations.pop(); }; };
                
    const sf::RenderWindow& getFenetre() { return m_fenetre; };    
    void setScoresPrisEnCompte(std::deque<int>& scores) { m_scoresPrisEnCompte.clear(); if (!m_afficheCartes && m_suitLesScores) { for (auto& s : scores) m_scoresPrisEnCompte.push_back(s); } };

private:    
    void boucleThread();
    
    JeuSabacc& m_jeuSabacc;
    SabaccVueListener* m_pListener = NULL;
    bool carteDefausseeSable;
    bool m_keepRunning = true;
    bool m_afficheCartes = false;
	bool m_suitLesScores = true;
    std::vector<int> m_scoresPrisEnCompte;
    bool m_rapide = false;
    std::thread* m_pTache;

    std::vector<Action> m_actionsAChoisir;

   //fenêtre, fonts, textures, sprites
    sf::RenderWindow m_fenetre;
    sf::Texture m_textureElements;
    sf::Texture m_textureArrierePlan;
    sf::Font m_policeTexte;

    std::map<Carte, sf::IntRect> m_carteSableRects;
    std::map<Carte, sf::IntRect> m_carteSangRects;    
    sf::IntRect m_carteSableVersoRect;
    sf::IntRect m_carteSangVersoRect;
    std::map<JetonInfluence, sf::IntRect> m_jetonInfluenceRects;
    sf::IntRect m_jetonInfluenceVersoRect;
    sf::IntRect m_jetonReserveRect;
    std::vector<BoutonCliquable> m_actionsCliquables;   
    std::string m_messagesJoueurs[Position::NB_POSITIONS] = { u8"",u8"",u8"",u8"" }; //petits messages en bas des joueurs...pas utilisé pour l'instant
       
    //variables d'animation
    Position m_positionCibleAnimation;  //position cible de l'animation
    float m_jAnimMultipliers[3] = { 0.0f,0.0f,0.0f }; //multiplicateurs d'animation de jetons d'influence    
    bool m_jVisible[Position::NB_POSITIONS][3] = { { true,true,true},{ true,true,true}, { true,true,true }, { true,true,true } }; //visibilité des jetons d'influence
    float m_paquetSableAnimMultiplier = 0.0f; 
    float m_paquetSangAnimMultiplier = 0.0f;
    float m_defausseSableAnimMultiplier = 0.0f;
    bool m_defausseSableVisible = true;
    float m_defausseSangAnimMultiplier = 0.0f;
    bool m_defausseSangVisible = true;
    float m_carteSableJoueurAnimMultiplier = 0.0f;
    bool m_carteSableVisible[Position::NB_POSITIONS]= { true,true,true,true };
    float m_carteSangJoueurAnimMultiplier = 0.0f;
    bool m_carteSangVisible[Position::NB_POSITIONS] = { true,true,true,true };
    float m_cartePiocheeJoueurAnimMultiplier = 0.0f;    
    uint32_t m_carteAnimDuree = CARTE_ANIM_DUREE_LENTE;
    uint32_t m_messageDuree = MESSAGE_DUREE_LENTE;
    const float CARTE_ROTAION_MAX = 5.0f;
    const float CARTE_ZOOM_MAX = 0.2f;
    const float JETON_ROTAION_MAX = 5.0f;
    const float JETON_ZOOM_MAX = 1.0f;

    std::queue < SVAnimation> m_animations;

    //buffers de son
    const std::vector<sf::SoundBuffer> m_buffersSonsCarte = { sf::SoundBuffer("flip1.mp3"), sf::SoundBuffer("flip2.mp3"), sf::SoundBuffer("flip3.mp3"), sf::SoundBuffer("flip4.mp3"), sf::SoundBuffer("flip5.mp3") };
    std::vector <sf::Sound> m_sonsCarte;
    const sf::SoundBuffer m_bufferSonBouton = sf::SoundBuffer("bouton.mp3");
    sf::Sound* m_pSonBouton;
    const std::vector<sf::SoundBuffer> m_buffersSonsWoosh = { sf::SoundBuffer("woosh1.mp3"), sf::SoundBuffer("woosh2.mp3") };
    std::vector <sf::Sound> m_sonsWoosh;
    const sf::SoundBuffer m_bufferSonCloche = sf::SoundBuffer("cloche.mp3");
    sf::Sound* m_pSonCloche;
    const sf::SoundBuffer m_bufferSonJeton = sf::SoundBuffer("jeton.mp3");
    sf::Sound* m_pSonJeton;

    void chargeElements();  //charge les elements graphiques
    void traiteEvenements();    //traite les événements 
    void traiteClicJoueur(sf::Vector2i mousePos);   //traite clic souris
    void affiche(); //construit et affiche la vue
    void afficheRecapJetons(const sf::FloatRect& area);
    void dessineRectangleJoueur(sf::RenderWindow& w, sf::FloatRect& fr, bool currentPlayer);
    void afficheJoueur(Position p, const sf::FloatRect& area);
    void afficheCentreTable(const sf::FloatRect& area);
    void afficheActionsCliquables(const sf::FloatRect& area);
	void afficheScore(const sf::FloatRect& area);    
    
    void dessineTexte(const std::string& str, float x, float y, uint32_t taillePolice,
        sf::Color couleur = sf::Color::White, sf::Text::Style style = sf::Text::Regular,
        float originXFactor = 0.f, float originYFactor = 0.f);    
        
    // Message log
    void afficheMessages(const sf::FloatRect& area);
    std::vector<sf::Text> m_texteMessages;

    //grand message
    void afficheGrandMessage(const sf::FloatRect& area);
    std::string m_grandMessage;
    
    //animations
    void ajouteAnimation(Position pJ, uint32_t cible, SVAnimationType type, uint32_t duree, Son s=Son::SON_INDEFINI, std::string message="");
    SVAnimationCible animationCibleJeton(JetonInfluence jeton);
    void executeAnimations();

    //fonctions de son
    void joueSonCarte();
    void joueSonBouton();
    void joueSonJeton();
    void joueSonWoosh();
    void joueSonCloche();

};