#include "SabaccView.h"
#include "eval.h"
#define _USE_MATH_DEFINES
#include <algorithm>
#include "math.h"


// Helper function to get scaled font size based on an area's height
uint32_t getScaledFontSizeForArea(float areaHeight, float proportion) {
    uint32_t size = static_cast<uint32_t>(areaHeight * proportion);
    return std::max(10u, std::min(size, 50u)); // Min size 10, Max 50 (to prevent huge text on very large areas)
};

void SabaccVue::chargeElements() {
	    
    if (!m_textureElements.loadFromFile("Sprites.png")) {
		LOGWARNING << "ERROR: Failed to load sprite sheet!" << std::endl;
	}
    m_textureElements.setSmooth(true);

    if (!m_textureArrierePlan.loadFromFile("background.jpg")) {
        LOGWARNING << "ERROR: Failed to load background!" << std::endl;
    }
    m_textureArrierePlan.setSmooth(true);    

    // Constants for sprite sheet layout
    static const int CARD_WIDTH = 300; //
    static const int CARD_HEIGHT = 435; //
    static const int TOKEN_WIDTH = 240; //
    static const int TOKEN_HEIGHT = 425; //

    static const int SHEET_MARGIN_X_CARDS = 30; //
    static const int SHEET_MARGIN_Y_CARDS = 65; //
    
    static const int SHEET_MARGIN_X_TOKEN = 175; // 

    static const int COL_SPACING_CARDS = 30; // 
    static const int ROW_SPACING_CARDS = 45; //
    
    static const int COL_SPACING_TOKENS = 25; // Horizontal gap between token columns
    static const int ROW_SPACING_TOKENS = 35; // Vertical gap between token rows

    static const int SHEET_MARGIN_Y_TOKENS_ROW3 = 1010; //
    static const int SHEET_MARGIN_Y_TOKENS_ROW4 = 1470; //

	// SABLE CARDS (Top row in your image)
	m_carteSableRects[Carte::C1] = sf::IntRect({ SHEET_MARGIN_X_CARDS + 0 * (CARD_WIDTH + COL_SPACING_CARDS), SHEET_MARGIN_Y_CARDS + 0 * (CARD_HEIGHT + ROW_SPACING_CARDS) }, { CARD_WIDTH, CARD_HEIGHT });
	m_carteSableRects[Carte::C2] = sf::IntRect({ SHEET_MARGIN_X_CARDS + 1 * (CARD_WIDTH + COL_SPACING_CARDS), SHEET_MARGIN_Y_CARDS + 0 * (CARD_HEIGHT + ROW_SPACING_CARDS) }, { CARD_WIDTH, CARD_HEIGHT });
	m_carteSableRects[Carte::C3] = sf::IntRect({ SHEET_MARGIN_X_CARDS + 2 * (CARD_WIDTH + COL_SPACING_CARDS), SHEET_MARGIN_Y_CARDS + 0 * (CARD_HEIGHT + ROW_SPACING_CARDS) }, { CARD_WIDTH, CARD_HEIGHT });
	m_carteSableRects[Carte::C4] = sf::IntRect({ SHEET_MARGIN_X_CARDS + 3 * (CARD_WIDTH + COL_SPACING_CARDS), SHEET_MARGIN_Y_CARDS + 0 * (CARD_HEIGHT + ROW_SPACING_CARDS) }, { CARD_WIDTH, CARD_HEIGHT });
	m_carteSableRects[Carte::C5] = sf::IntRect({ SHEET_MARGIN_X_CARDS + 4 * (CARD_WIDTH + COL_SPACING_CARDS), SHEET_MARGIN_Y_CARDS + 0 * (CARD_HEIGHT + ROW_SPACING_CARDS) }, { CARD_WIDTH, CARD_HEIGHT });
	m_carteSableRects[Carte::C6] = sf::IntRect({ SHEET_MARGIN_X_CARDS + 5 * (CARD_WIDTH + COL_SPACING_CARDS), SHEET_MARGIN_Y_CARDS + 0 * (CARD_HEIGHT + ROW_SPACING_CARDS) }, { CARD_WIDTH, CARD_HEIGHT });
	m_carteSableRects[Carte::IMPOSTEUR] = sf::IntRect({ SHEET_MARGIN_X_CARDS + 6 * (CARD_WIDTH + COL_SPACING_CARDS), SHEET_MARGIN_Y_CARDS + 0 * (CARD_HEIGHT + ROW_SPACING_CARDS) }, { CARD_WIDTH, CARD_HEIGHT });
	m_carteSableRects[Carte::SYLOP] = sf::IntRect({ SHEET_MARGIN_X_CARDS + 7 * (CARD_WIDTH + COL_SPACING_CARDS), SHEET_MARGIN_Y_CARDS + 0 * (CARD_HEIGHT + ROW_SPACING_CARDS) }, { CARD_WIDTH, CARD_HEIGHT });
	m_carteSableVersoRect = sf::IntRect({ SHEET_MARGIN_X_CARDS + 8 * (CARD_WIDTH + COL_SPACING_CARDS), SHEET_MARGIN_Y_CARDS + 0 * (CARD_HEIGHT + ROW_SPACING_CARDS) }, { CARD_WIDTH, CARD_HEIGHT });

	// SANG CARDS (Second row)
    m_carteSangRects[Carte::C1] = sf::IntRect({ SHEET_MARGIN_X_CARDS + 0 * (CARD_WIDTH + COL_SPACING_CARDS), SHEET_MARGIN_Y_CARDS + 1 * (CARD_HEIGHT + ROW_SPACING_CARDS) }, { CARD_WIDTH, CARD_HEIGHT });
    m_carteSangRects[Carte::C2] = sf::IntRect({ SHEET_MARGIN_X_CARDS + 1 * (CARD_WIDTH + COL_SPACING_CARDS), SHEET_MARGIN_Y_CARDS + 1 * (CARD_HEIGHT + ROW_SPACING_CARDS) }, { CARD_WIDTH, CARD_HEIGHT });
    m_carteSangRects[Carte::C3] = sf::IntRect({ SHEET_MARGIN_X_CARDS + 2 * (CARD_WIDTH + COL_SPACING_CARDS), SHEET_MARGIN_Y_CARDS + 1 * (CARD_HEIGHT + ROW_SPACING_CARDS) }, { CARD_WIDTH, CARD_HEIGHT });
    m_carteSangRects[Carte::C4] = sf::IntRect({ SHEET_MARGIN_X_CARDS + 3 * (CARD_WIDTH + COL_SPACING_CARDS), SHEET_MARGIN_Y_CARDS + 1 * (CARD_HEIGHT + ROW_SPACING_CARDS) }, { CARD_WIDTH, CARD_HEIGHT });
    m_carteSangRects[Carte::C5] = sf::IntRect({ SHEET_MARGIN_X_CARDS + 4 * (CARD_WIDTH + COL_SPACING_CARDS), SHEET_MARGIN_Y_CARDS + 1 * (CARD_HEIGHT + ROW_SPACING_CARDS) }, { CARD_WIDTH, CARD_HEIGHT });
    m_carteSangRects[Carte::C6] = sf::IntRect({ SHEET_MARGIN_X_CARDS + 5 * (CARD_WIDTH + COL_SPACING_CARDS), SHEET_MARGIN_Y_CARDS + 1 * (CARD_HEIGHT + ROW_SPACING_CARDS) }, { CARD_WIDTH, CARD_HEIGHT });
    m_carteSangRects[Carte::IMPOSTEUR] = sf::IntRect({ SHEET_MARGIN_X_CARDS + 6 * (CARD_WIDTH + COL_SPACING_CARDS), SHEET_MARGIN_Y_CARDS + 1 * (CARD_HEIGHT + ROW_SPACING_CARDS) }, { CARD_WIDTH, CARD_HEIGHT });
    m_carteSangRects[Carte::SYLOP] = sf::IntRect({ SHEET_MARGIN_X_CARDS + 7 * (CARD_WIDTH + COL_SPACING_CARDS), SHEET_MARGIN_Y_CARDS + 1 * (CARD_HEIGHT + ROW_SPACING_CARDS) }, { CARD_WIDTH, CARD_HEIGHT });
    m_carteSangVersoRect = sf::IntRect({ SHEET_MARGIN_X_CARDS + 8 * (CARD_WIDTH + COL_SPACING_CARDS), SHEET_MARGIN_Y_CARDS + 1 * (CARD_HEIGHT + ROW_SPACING_CARDS) }, { CARD_WIDTH, CARD_HEIGHT });

	// INFLUENCE TOKENS (Third and Fourth rows - map these to JetonInfluence enum)
	// Row 3
	m_jetonInfluenceRects[JetonInfluence::PIOCHE_GRATUITE] = sf::IntRect({ SHEET_MARGIN_X_TOKEN + 0 * (TOKEN_WIDTH + COL_SPACING_TOKENS), SHEET_MARGIN_Y_TOKENS_ROW3 + 0 * (TOKEN_HEIGHT + ROW_SPACING_TOKENS) }, { TOKEN_WIDTH, TOKEN_HEIGHT });
	m_jetonInfluenceRects[JetonInfluence::REMBOURSEMENT_2] = sf::IntRect({ SHEET_MARGIN_X_TOKEN + 1 * (TOKEN_WIDTH + COL_SPACING_TOKENS), SHEET_MARGIN_Y_TOKENS_ROW3 + 0 * (TOKEN_HEIGHT + ROW_SPACING_TOKENS) }, { TOKEN_WIDTH, TOKEN_HEIGHT });
	m_jetonInfluenceRects[JetonInfluence::REMBOURSEMENT_3] = sf::IntRect({ SHEET_MARGIN_X_TOKEN + 2 * (TOKEN_WIDTH + COL_SPACING_TOKENS), SHEET_MARGIN_Y_TOKENS_ROW3 + 0 * (TOKEN_HEIGHT + ROW_SPACING_TOKENS) }, { TOKEN_WIDTH, TOKEN_HEIGHT });
	m_jetonInfluenceRects[JetonInfluence::DETOURNEMENT] = sf::IntRect({ SHEET_MARGIN_X_TOKEN + 3 * (TOKEN_WIDTH + COL_SPACING_TOKENS), SHEET_MARGIN_Y_TOKENS_ROW3 + 0 * (TOKEN_HEIGHT + ROW_SPACING_TOKENS) }, { TOKEN_WIDTH, TOKEN_HEIGHT });
	m_jetonInfluenceRects[JetonInfluence::FRAUDE_MAJEURE] = sf::IntRect({ SHEET_MARGIN_X_TOKEN + 4 * (TOKEN_WIDTH + COL_SPACING_TOKENS), SHEET_MARGIN_Y_TOKENS_ROW3 + 0 * (TOKEN_HEIGHT + ROW_SPACING_TOKENS) }, { TOKEN_WIDTH, TOKEN_HEIGHT });
	m_jetonInfluenceRects[JetonInfluence::CUISINER_LIVRES] = sf::IntRect({ SHEET_MARGIN_X_TOKEN + 5 * (TOKEN_WIDTH + COL_SPACING_TOKENS), SHEET_MARGIN_Y_TOKENS_ROW3 + 0 * (TOKEN_HEIGHT + ROW_SPACING_TOKENS) }, { TOKEN_WIDTH, TOKEN_HEIGHT });
	m_jetonInfluenceRects[JetonInfluence::DEVALORISATION] = sf::IntRect({ SHEET_MARGIN_X_TOKEN + 6 * (TOKEN_WIDTH + COL_SPACING_TOKENS), SHEET_MARGIN_Y_TOKENS_ROW3 + 0 * (TOKEN_HEIGHT + ROW_SPACING_TOKENS) }, { TOKEN_WIDTH, TOKEN_HEIGHT });
	m_jetonInfluenceRects[JetonInfluence::SABACC_PRIME] = sf::IntRect({ SHEET_MARGIN_X_TOKEN + 7 * (TOKEN_WIDTH + COL_SPACING_TOKENS), SHEET_MARGIN_Y_TOKENS_ROW3 + 0 * (TOKEN_HEIGHT + ROW_SPACING_TOKENS) }, { TOKEN_WIDTH, TOKEN_HEIGHT });
    m_jetonInfluenceVersoRect= sf::IntRect({ SHEET_MARGIN_X_TOKEN + 8 * (TOKEN_WIDTH + COL_SPACING_TOKENS), SHEET_MARGIN_Y_TOKENS_ROW3 + 0 * (TOKEN_HEIGHT + ROW_SPACING_TOKENS) }, { TOKEN_WIDTH, TOKEN_HEIGHT });
	
    // Row 4
    m_jetonInfluenceRects[JetonInfluence::TARIF_GENERAL] = sf::IntRect({ SHEET_MARGIN_X_TOKEN + 0 * (TOKEN_WIDTH + COL_SPACING_TOKENS), SHEET_MARGIN_Y_TOKENS_ROW3 + 1 * (TOKEN_HEIGHT + ROW_SPACING_TOKENS) }, { TOKEN_WIDTH, TOKEN_HEIGHT });
    m_jetonInfluenceRects[JetonInfluence::TARIF_CIBLE] = sf::IntRect({ SHEET_MARGIN_X_TOKEN + 1 * (TOKEN_WIDTH + COL_SPACING_TOKENS), SHEET_MARGIN_Y_TOKENS_ROW3 + 1 * (TOKEN_HEIGHT + ROW_SPACING_TOKENS) }, { TOKEN_WIDTH, TOKEN_HEIGHT });
    m_jetonInfluenceRects[JetonInfluence::AUDIT_GENERAL] = sf::IntRect({ SHEET_MARGIN_X_TOKEN + 2 * (TOKEN_WIDTH + COL_SPACING_TOKENS), SHEET_MARGIN_Y_TOKENS_ROW3 + 1 * (TOKEN_HEIGHT + ROW_SPACING_TOKENS) }, { TOKEN_WIDTH, TOKEN_HEIGHT });
    m_jetonInfluenceRects[JetonInfluence::AUDIT_CIBLE] = sf::IntRect({ SHEET_MARGIN_X_TOKEN + 3 * (TOKEN_WIDTH + COL_SPACING_TOKENS), SHEET_MARGIN_Y_TOKENS_ROW3 + 1 * (TOKEN_HEIGHT + ROW_SPACING_TOKENS) }, { TOKEN_WIDTH, TOKEN_HEIGHT });
    m_jetonInfluenceRects[JetonInfluence::EMBARGO] = sf::IntRect({ SHEET_MARGIN_X_TOKEN + 4 * (TOKEN_WIDTH + COL_SPACING_TOKENS), SHEET_MARGIN_Y_TOKENS_ROW3 + 1 * (TOKEN_HEIGHT + ROW_SPACING_TOKENS) }, { TOKEN_WIDTH, TOKEN_HEIGHT });
    m_jetonInfluenceRects[JetonInfluence::IMMUNITE] = sf::IntRect({ SHEET_MARGIN_X_TOKEN + 5 * (TOKEN_WIDTH + COL_SPACING_TOKENS), SHEET_MARGIN_Y_TOKENS_ROW3 + 1 * (TOKEN_HEIGHT + ROW_SPACING_TOKENS) }, { TOKEN_WIDTH, TOKEN_HEIGHT });
    m_jetonInfluenceRects[JetonInfluence::EPUISEMENT] = sf::IntRect({ SHEET_MARGIN_X_TOKEN + 6 * (TOKEN_WIDTH + COL_SPACING_TOKENS), SHEET_MARGIN_Y_TOKENS_ROW3 + 1 * (TOKEN_HEIGHT + ROW_SPACING_TOKENS) }, { TOKEN_WIDTH, TOKEN_HEIGHT });
    // m_influenceTokenRects[JetonInfluence::TRANSACTION] = sf::IntRect({648, 435}, {75, 115}); // two connected shapes 
	// One token icon is unassigned, JetonInfluence::JI_INDEFINI won't have a rect.

	// Jeton Coin (bottom right)
	m_jetonReserveRect = sf::IntRect({ 2605, 1350 }, { 220, 220 });

    //chargeons les sons
    for (auto& sb : m_buffersSonsCarte) {
        sf::Sound s(sb);
        m_sonsCarte.push_back(s);
    }    
    for (auto& sb : m_buffersSonsWoosh) {
        sf::Sound s(sb);
        m_sonsWoosh.push_back(s);
    }

    m_pSonBouton = new sf::Sound(m_bufferSonBouton);
    m_pSonCloche = new sf::Sound(m_bufferSonCloche);
    m_pSonJeton = new sf::Sound(m_bufferSonJeton);

}

void SabaccVue::joueSonBouton() {
    m_pSonBouton->setVolume(80.0f);
    m_pSonBouton->play();
}

void SabaccVue::joueSonJeton()
{
    m_pSonJeton->setVolume(80.0f);
    m_pSonJeton->play();
}

void SabaccVue::joueSonCarte()
{
    uint32_t index = rand() % m_sonsCarte.size();
    m_sonsCarte[index].setVolume(80.0f);
    m_sonsCarte[index].play();
};

void SabaccVue::joueSonWoosh()
{
    uint32_t index = rand() % m_sonsWoosh.size();
    m_sonsWoosh[index].setVolume(80.0f);
    m_sonsWoosh[index].play();
}
void SabaccVue::joueSonCloche()
{    
    m_pSonCloche->play();
}
;

SabaccVue::SabaccVue(JeuSabacc& game) : m_jeuSabacc(game), m_policeTexte("arial.ttf") {
}

SabaccVue::~SabaccVue()
{
    stoppeEtFerme();
}

void SabaccVue::demarre()
{
    if(!m_pTache)
        m_pTache = new std::thread(&SabaccVue::boucleThread, this);
}

void SabaccVue::boucleThread() {

    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;
    sf::VideoMode vm = sf::VideoMode::getDesktopMode();

    //vm.size.x = static_cast<uint32_t>(1920);
    //vm.size.y = static_cast<uint32_t>(1080);

    //vm.size.x = static_cast<uint32_t>(vm.size.x * 0.95f);
    //vm.size.y = static_cast<uint32_t>(vm.size.y * 0.95f);
    //vm.size.x = static_cast<uint32_t>(vm.size.x);
    //vm.size.y = static_cast<uint32_t>(vm.size.y);

    m_fenetre.create(vm, "Sabacc de Kessel par GrillePainVert", sf::Style::Close | sf::Style::Titlebar, sf::State::Fullscreen, settings);
    m_fenetre.setVerticalSyncEnabled(true);
    m_fenetre.setMinimumSize(sf::Vector2u(1400, 800));
    chargeElements();

    while (m_fenetre.isOpen() && m_keepRunning) {
        traiteEvenements();
        // update(); // If needed for animations or GUI state not tied to game state
        affiche();
        Sleep(15);
    }
    
    //unload assets

    //close window
    m_fenetre.close();
}

void SabaccVue::stoppeEtFerme()
{
    m_keepRunning = false;
    if (m_pTache) {
        m_pTache->join();
        delete m_pTache;
    }
    m_pTache = NULL;
}

void SabaccVue::traiteEvenements() {
    
    while (const std::optional event = m_fenetre.pollEvent()) {
        if (event->is<sf::Event::Closed>()) {
            m_fenetre.close();
        }
        // Mouse Button Pressed
        if (const auto* mouseButtonReleased = event->getIf<sf::Event::MouseButtonReleased>()) {
            if (mouseButtonReleased->button == sf::Mouse::Button::Left) {
                //todo test
                //if (m_jeuSabacc.m_jeuTermine || m_jeuSabacc.m_joueurs[m_jeuSabacc.m_positionJActuel].m_type == TYPE_JOUEUR::TJ_HUMAIN || !m_jeuSabacc.m_joueurs[Position::S].m_enJeu) {
                //if (m_jeuSabacc.m_joueurs[m_jeuSabacc.m_positionJActuel].m_type == TYPE_JOUEUR::TJ_HUMAIN) {
                    traiteClicJoueur(mouseButtonReleased->position);
                //}
            }
        }
        // Press Enter Event
        //if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
        //    if (keyPressed->scancode == sf::Keyboard::Scan::Enter) {
        //        // This logic needs to be more nuanced.
        //        // Only advance if waiting for this specific input.
        //        // For now, let's assume AI turns are fast.
        //    }
        //}
        // catch the resize events
        if (const auto* resizedEvent = event->getIf<sf::Event::Resized>())
        {            
            // update the view to the new size of the window
            sf::FloatRect visibleArea({ 0.f, 0.f }, { static_cast<float> (resizedEvent->size.x), static_cast<float> (resizedEvent->size.y)});
            m_fenetre.setView(sf::View(visibleArea));
        }
    }    
}

void SabaccVue::dessineTexte(const std::string& str, float x, float y, uint32_t characterSize, sf::Color color, sf::Text::Style style, float originXFactor, float originYFactor) {
    sf::Text text(m_policeTexte);    
    text.setString(sf::String::fromUtf8(str.begin(), str.end()));
    text.setCharacterSize(characterSize);
    text.setFillColor(color);
    text.setStyle(style);

    sf::FloatRect bounds = text.getLocalBounds();
    text.setOrigin({ bounds.position.x + bounds.size.x * originXFactor, bounds.position.y + bounds.size.y * originYFactor });
    text.setPosition({ x, y });
    m_fenetre.draw(text);
}

void SabaccVue::afficheJoueur(Position playerPos, const sf::FloatRect& area) {
    const JoueurSabacc& joueur = m_jeuSabacc.m_joueurs[playerPos];
    //bool joueurActuel = m_jeuSabacc.m_positionJActuel == playerPos;
    float padding = area.size.x* 0.01f; // Padding relative to area width
    float currentX = area.position.x+ padding;
    float currentY = area.position.y + 2.0f*padding;
    float availableWidth = area.size.x - 2 * padding;
    // float availableHeight = area.height - 2 * padding; // If needed

    // Line spacing and text sizes relative to area height
    float lineSpacing = area.size.y * 0.08f;
    unsigned int nameSize = getScaledFontSizeForArea(area.size.y, TEXT_PROPORTION_GRAND);
    unsigned int statusSize = getScaledFontSizeForArea(area.size.y, TEXT_PROPORTION_NORMAL);
    unsigned int cardLabelSize = getScaledFontSizeForArea(area.size.y, TEXT_PROPORTION_PETIT);
    unsigned int infoSize = getScaledFontSizeForArea(area.size.y, TEXT_PROPORTION_GRAND);
    unsigned int effectSize = getScaledFontSizeForArea(area.size.y, TEXT_PROPORTION_GRAND);
    unsigned int tokenLabelSize = getScaledFontSizeForArea(area.size.y, TEXT_PROPORTION_MINI);

    // Cards
    float targetCardHeight = area.size.y * 0.65f; // Cards take up 35% of player area height
    float cardScaleY = targetCardHeight / static_cast<float>(m_carteSableVersoRect.size.y);
    float cardScaleX = cardScaleY; // Maintain aspect ratio
    float scaledCardWidth = static_cast<float>(m_carteSableVersoRect.size.x) * cardScaleX;
    float cardSpacing = availableWidth * 0.05f;
    float cardDisplayWidthTotal = 2 * scaledCardWidth + cardSpacing;
    float cardStartX = currentX;

    // Player Name and Status
    std::ostringstream playerNameOss;
    if (playerPos == m_jeuSabacc.m_positionJ1Manche) playerNameOss << "(1e) ";
    playerNameOss << "Joueur " << playerPos;    
    if (joueur.m_aPasseCeTour) playerNameOss << u8" - A PASSÉ";
    sf::Color playerColor = COULEURS_JOUEURS[playerPos];
    dessineTexte(playerNameOss.str(), currentX + 0.5f*scaledCardWidth, currentY, nameSize, playerColor, sf::Text::Bold);
    currentY += lineSpacing * 2.f;

    if (!joueur.m_enJeu) {
        dessineTexte(u8"Hors Jeu", currentX+ 0.5f*scaledCardWidth, currentY, nameSize, sf::Color::Red, sf::Text::Bold);
        return;
    }

    //message
    dessineTexte(m_messagesJoueurs[playerPos], area.position.x + padding, area.position.y + area.size.y - padding - lineSpacing, infoSize, sf::Color::White, sf::Text::Style::Regular);

    //carte sable
    sf::Sprite sableSprite(m_textureElements);
    if (joueur.m_carteSable != Carte::C_INDEFINI && (m_afficheCartes || playerPos == Position::S || m_jeuSabacc.m_phaseActuelle == PHASE_RESOLUTION)) {
        sableSprite.setTextureRect(m_carteSableRects.at(joueur.m_carteSable));
    }
    else { sableSprite.setTextureRect(m_carteSableVersoRect); }
    if (playerPos == m_positionCibleAnimation) {
        sableSprite.setScale({ (m_carteSableVisible[playerPos] ? 1.0f : 0.0f) * (1.0f + CARTE_ZOOM_MAX * m_carteSableJoueurAnimMultiplier) * cardScaleX, (m_carteSableVisible[playerPos] ? 1.0f : 0.0f) * (1.0f + CARTE_ZOOM_MAX * m_carteSableJoueurAnimMultiplier) * cardScaleY});
        sableSprite.setRotation(sf::degrees(CARTE_ROTAION_MAX * m_carteSableJoueurAnimMultiplier));
    }
    else {
        sableSprite.setScale({ (m_carteSangVisible[playerPos] ? 1.0f : 0.0f) * cardScaleX, (m_carteSangVisible[playerPos] ? 1.0f : 0.0f) * cardScaleY });
        sableSprite.setRotation(sf::degrees(0));
    }
    sableSprite.setPosition({ cardStartX, currentY });
    m_fenetre.draw(sableSprite);

    //carte sang
    sf::Sprite sangSprite(m_textureElements);
	if (joueur.m_carteSang != Carte::C_INDEFINI && (m_afficheCartes|| playerPos ==Position::S || m_jeuSabacc.m_phaseActuelle == PHASE_RESOLUTION)) {
		sangSprite.setTextureRect(m_carteSangRects.at(joueur.m_carteSang));
	}
	else { sangSprite.setTextureRect(m_carteSangVersoRect); }
    if (playerPos == m_positionCibleAnimation) {
        sangSprite.setScale({ (m_carteSangVisible[playerPos] ? 1.0f : 0.0f)*(1.0f + CARTE_ZOOM_MAX * m_carteSangJoueurAnimMultiplier) * cardScaleX, (m_carteSangVisible[playerPos] ? 1.0f : 0.0f) * (1.0f + CARTE_ZOOM_MAX * m_carteSangJoueurAnimMultiplier) * cardScaleY });
        sangSprite.setRotation(sf::degrees(CARTE_ROTAION_MAX * m_carteSangJoueurAnimMultiplier));
    }    
    else {
        sangSprite.setScale({ (m_carteSangVisible[playerPos] ? 1.0f : 0.0f)* cardScaleX, (m_carteSangVisible[playerPos] ? 1.0f : 0.0f) * cardScaleY });
        sangSprite.setRotation(sf::degrees(0));
    }
    sangSprite.setPosition({ cardStartX + scaledCardWidth + 0.25f* cardSpacing, currentY });
    m_fenetre.draw(sangSprite);
    
    // Carte Sable Piochée
    if (joueur.m_carteSablePiochee != Carte::C_INDEFINI ) {        
        if(m_afficheCartes || playerPos == Position::S || m_jeuSabacc.m_phaseActuelle == PHASE_RESOLUTION)
            sableSprite.setTextureRect(m_carteSableRects.at(joueur.m_carteSablePiochee));
        else 
            sableSprite.setTextureRect(m_carteSableVersoRect);

        if (playerPos == m_positionCibleAnimation) {
            sableSprite.setScale({ (1.0f + CARTE_ZOOM_MAX * m_cartePiocheeJoueurAnimMultiplier) * cardScaleX, (1.0f + CARTE_ZOOM_MAX * m_cartePiocheeJoueurAnimMultiplier) * cardScaleY });
            sableSprite.setRotation(sf::degrees(CARTE_ROTAION_MAX * m_cartePiocheeJoueurAnimMultiplier));
        }
        else {
            sableSprite.setScale({ cardScaleX, cardScaleY });
            sableSprite.setRotation(sf::degrees(0));
        }

        sableSprite.setPosition({ cardStartX + 2.0f*scaledCardWidth + 1.25f * cardSpacing, currentY });      
        m_fenetre.draw(sableSprite);
    }

    // Carte Sang Piochée
    if (joueur.m_carteSangPiochee != Carte::C_INDEFINI) {
        if (m_afficheCartes || playerPos == Position::S || m_jeuSabacc.m_phaseActuelle == PHASE_RESOLUTION)
            sangSprite.setTextureRect(m_carteSangRects.at(joueur.m_carteSangPiochee));
        else
            sangSprite.setTextureRect(m_carteSangVersoRect);

        if (playerPos == m_positionCibleAnimation) {
            sangSprite.setScale({ (1.0f + CARTE_ZOOM_MAX * m_cartePiocheeJoueurAnimMultiplier) * cardScaleX, (1.0f + CARTE_ZOOM_MAX * m_cartePiocheeJoueurAnimMultiplier) * cardScaleY });
            sangSprite.setRotation(sf::degrees(CARTE_ROTAION_MAX * m_cartePiocheeJoueurAnimMultiplier));
        }
        else {
            sangSprite.setScale({ cardScaleX, cardScaleY });
            sangSprite.setRotation(sf::degrees(0));
        }

        sangSprite.setPosition({ cardStartX + 2.0f * scaledCardWidth + 1.25f * cardSpacing, currentY });
        m_fenetre.draw(sangSprite);
    }

    //currentY += targetCardHeight + lineSpacing * 0.3f;
    currentX = cardStartX + 3.0f * scaledCardWidth + 2.00f * cardSpacing;
    currentY = area.position.y + padding + lineSpacing;
    // Reserve et pot
    std::ostringstream RPOss;
    RPOss << "R: " << joueur.m_reserve;
    dessineTexte(RPOss.str(), currentX , currentY, infoSize, sf::Color::White, sf::Text::Style::Bold);

    RPOss.str(""); RPOss.clear(); 
    RPOss << "Pot: " << joueur.m_pot;
    float potTextX = currentX + availableWidth * 0.12f;
    dessineTexte(RPOss.str(), potTextX, currentY, infoSize, sf::Color::White, sf::Text::Style::Bold);
    currentY += lineSpacing * 1.2f;

    // Jetons d'influence
    float targetTokenHeight = area.size.y * 0.20f;
    float tokenScale = targetTokenHeight / static_cast<float>(m_jetonInfluenceVersoRect.size.y);
    float scaledTokenWidth = static_cast<float>(m_jetonInfluenceVersoRect.size.x) * tokenScale;
    float tokenSpacing = availableWidth * 0.03f;
    float totalTokenWidth = 3 * scaledTokenWidth + 2 * tokenSpacing;
    float tokenStartX = currentX;// + (availableWidth - totalTokenWidth) / 2.f; // Center tokens

    for (int i = 2; i >=0; --i) {
        JetonInfluence jeton = joueur.m_jetonsInfluence[i];
        sf::Sprite tokenSprite(m_textureElements);
        if (jeton != JetonInfluence::JI_INDEFINI ) {
            tokenSprite.setTextureRect(m_jetonInfluenceRects.at(jeton));
            if (playerPos == m_positionCibleAnimation) {
                tokenSprite.setScale({ (m_jVisible[playerPos][i] ? 1.0f : 0.0f) * (m_jAnimMultipliers[i] * JETON_ZOOM_MAX + 1.0f) * tokenScale, (m_jVisible[playerPos][i] ? 1.0f : 0.0f) * (m_jAnimMultipliers[i] * JETON_ZOOM_MAX + 1.0f) * tokenScale});
                tokenSprite.setRotation(sf::degrees(m_jAnimMultipliers[i] * JETON_ROTAION_MAX));
            }
            else {
                tokenSprite.setScale({ (m_jVisible[playerPos][i] ? 1.0f : 0.0f) * tokenScale, (m_jVisible[playerPos][i] ? 1.0f : 0.0f) * tokenScale });
                tokenSprite.setRotation(sf::degrees(0.0f));
            }
            
            tokenSprite.setPosition({ tokenStartX + 1.0f * padding + i * (scaledTokenWidth + tokenSpacing), currentY });
            m_fenetre.draw(tokenSprite);
        }
    }
    currentY += targetTokenHeight +0.5f*lineSpacing;

    // Active Effects
    std::ostringstream effectsOss;
    uint32_t nbEffects = 0;
    if (joueur.m_doitPasser) effectsOss << u8"[Doit Passer!]\n", nbEffects++;
    if (joueur.m_immunite) effectsOss << u8"[Immunité]\n", nbEffects++;
    if (joueur.m_piocheGratuite) effectsOss << u8"[P.Gratuite]\n", nbEffects++;
    std::string str(effectsOss.str());
    if (!effectsOss.str().empty()) {
        dessineTexte(effectsOss.str(), currentX, currentY, effectSize,sf::Color::White, sf::Text::Style::Regular);
        currentY += nbEffects*lineSpacing * 1.2f;
    }
    currentY += lineSpacing;
    // Dice
    if (joueur.m_de1 != 0 || joueur.m_de2 != 0) {
        std::ostringstream diceOss;
        diceOss << u8"Dés: [" << joueur.m_de1 << "," << joueur.m_de2 << "]";
        dessineTexte(diceOss.str(), currentX, currentY, infoSize, sf::Color::White, sf::Text::Style::Regular);
        currentY += lineSpacing;
    }
    
}

void SabaccVue::afficheCentreTable(const sf::FloatRect& area) {
    float padding = area.size.x * 0.02f;
    float titleY = area.position.y + padding;
    float contentY = titleY + area.size.y * 0.1f; // Space for title
    float availableContentHeight = area.size.y- (contentY - area.position.y) - padding;

    unsigned int titleSize = getScaledFontSizeForArea(area.size.y, TEXT_PROPORTION_GRAND);
    unsigned int labelSize = getScaledFontSizeForArea(area.size.y, TEXT_PROPORTION_NORMAL);
    unsigned int countSize = getScaledFontSizeForArea(area.size.y, TEXT_PROPORTION_NORMAL);
    unsigned int effectLabelSize = getScaledFontSizeForArea(area.size.y, TEXT_PROPORTION_GRAND);

    // Game Info: Manche, Tour, Phase (Centered Horizontally at the top of board area)
    std::ostringstream gameInfoOss;
    gameInfoOss << "Manche: " << m_jeuSabacc.m_manche << "  Tour: " << m_jeuSabacc.m_tour << "/3  Phase: " << m_jeuSabacc.m_phaseActuelle;
    dessineTexte(gameInfoOss.str(), area.position.x + area.size.x / 2.f, titleY, titleSize, sf::Color::White, sf::Text::Bold, 0.5f, 0.f);

    // Decks and Discards Area (takes about 60% of the available content height)
    float deckAreaHeight = availableContentHeight;
    float targetCardHeight = deckAreaHeight * 0.9f; // Cards in deck/discard are 70% of this sub-area's height
    float cardScale = targetCardHeight / static_cast<float>(m_carteSableVersoRect.size.y);
    float scaledCardWidth = static_cast<float>(m_carteSableVersoRect.size.x) * cardScale;
    float elementSpacing = area.size.x * 0.05f; // Spacing between deck/discard elements

    // Total width needed for 4 card elements (2 decks, 2 discards)
    float totalElementsWidth = 4 * scaledCardWidth + 3 * elementSpacing;
    float deckStartX = area.position.x + (area.size.x - totalElementsWidth) / 2.f;
    float deckYPos = contentY + (deckAreaHeight - targetCardHeight) / 2.f; // Center vertically in their sub-area
    float labelYOffset = labelSize * 1.5f; // For labels above cards
    float countYOffset = targetCardHeight + countSize * 0.5f; // For counts below cards

    // Global Effects (Below decks/discards)
    float effectsY = contentY + deckAreaHeight + padding * 1.5f;
    std::ostringstream globalEffectsOss;
    if (m_jeuSabacc.m_sylopsAZero) globalEffectsOss << "[Sylops=0] ";
    if (m_jeuSabacc.m_imposteurASix) globalEffectsOss << "[Imposteur=6] ";
    if (m_jeuSabacc.m_sabaccInverse) globalEffectsOss << "[Sabacc Inverse] ";
    if (m_jeuSabacc.m_sabaccPrime != Carte::C_INDEFINI) globalEffectsOss << "[Sabacc Prime=" << m_jeuSabacc.m_sabaccPrime << "] ";

    std::string effectsString = globalEffectsOss.str();
    if (!effectsString.empty()) {
        dessineTexte(effectsString, area.position.x + area.size.x / 2.f, effectsY, effectLabelSize, sf::Color::Cyan, sf::Text::Bold, 0.5f, 0.f);
    }
    else {
        dessineTexte("Effets: Aucun", area.position.x + area.size.x / 2.f, effectsY, effectLabelSize, sf::Color(255,255,255,128), sf::Text::Regular, 0.5f, 0.f);
    }

    // Sable Discard
    dessineTexte("(" + std::to_string(m_jeuSabacc.m_defausseSable.getNbCartes()) + ")", deckStartX + scaledCardWidth / 2.f, deckYPos + countYOffset, countSize, sf::Color(255, 215, 0), sf::Text::Regular, 0.5f, 0.f);
    Carte topSableDiscard = m_jeuSabacc.m_defausseSable.regarde();
    sf::Sprite sableDiscardSprite(m_textureElements);
    if (topSableDiscard != Carte::C_INDEFINI  && m_defausseSableVisible) {
        sableDiscardSprite.setTextureRect(m_carteSableRects.at(topSableDiscard));
    }
    else { // Draw back or nothing if empty
        sableDiscardSprite.setTextureRect(m_carteSableVersoRect); // Could make it transparent or smaller
        sableDiscardSprite.setColor(sf::Color(255, 255, 255, 100)); // Dim if empty placeholder
    }
    sableDiscardSprite.setScale({ (1.0f + CARTE_ZOOM_MAX * m_defausseSableAnimMultiplier) * cardScale, (1.0f + CARTE_ZOOM_MAX * m_defausseSableAnimMultiplier) * cardScale });
    sableDiscardSprite.setRotation(sf::degrees(m_defausseSableAnimMultiplier * CARTE_ROTAION_MAX));
    sableDiscardSprite.setPosition({ deckStartX, deckYPos });
    m_fenetre.draw(sableDiscardSprite);
    deckStartX += scaledCardWidth + elementSpacing;

    // Sable Deck
    dessineTexte(std::to_string(m_jeuSabacc.m_paquetSable.getNbCartes()), deckStartX + scaledCardWidth / 2.f, deckYPos + countYOffset, countSize, sf::Color(255, 215, 0), sf::Text::Regular, 0.5f, 0.f);
    sf::Sprite sableDeckSprite(m_textureElements);
    sableDeckSprite.setTextureRect(m_carteSableVersoRect);
    sableDeckSprite.setScale({ (1.0f + CARTE_ZOOM_MAX * m_paquetSableAnimMultiplier) * cardScale, (1.0f + CARTE_ZOOM_MAX * m_paquetSableAnimMultiplier) * cardScale });
    sableDeckSprite.setRotation(sf::degrees(m_paquetSableAnimMultiplier * CARTE_ROTAION_MAX));
    sableDeckSprite.setPosition({ deckStartX, deckYPos });
    m_fenetre.draw(sableDeckSprite);

    deckStartX += scaledCardWidth + elementSpacing;

    // Sang Deck
    dessineTexte(std::to_string(m_jeuSabacc.m_paquetSang.getNbCartes()), deckStartX + scaledCardWidth / 2.f, deckYPos + countYOffset, countSize, sf::Color::Red, sf::Text::Regular, 0.5f, 0.f);
    sf::Sprite sangDeckSprite(m_textureElements);
    sangDeckSprite.setTextureRect(m_carteSangVersoRect);
	sangDeckSprite.setScale({ (1.0f + CARTE_ZOOM_MAX * m_paquetSangAnimMultiplier) * cardScale, (1.0f + CARTE_ZOOM_MAX * m_paquetSangAnimMultiplier) * cardScale });
    sangDeckSprite.setRotation(sf::degrees(m_paquetSangAnimMultiplier * CARTE_ROTAION_MAX));
    sangDeckSprite.setPosition({ deckStartX, deckYPos });
    m_fenetre.draw(sangDeckSprite);
    
    deckStartX += scaledCardWidth + elementSpacing;

    // Sang Discard
    dessineTexte("(" + std::to_string(m_jeuSabacc.m_defausseSang.getNbCartes()) + ")", deckStartX + scaledCardWidth / 2.f, deckYPos + countYOffset, countSize, sf::Color::Red, sf::Text::Regular, 0.5f, 0.f);
    Carte topSangDiscard = m_jeuSabacc.m_defausseSang.regarde();
    sf::Sprite sangDiscardSprite(m_textureElements);
    if (topSangDiscard != Carte::C_INDEFINI && m_defausseSangVisible) {
        sangDiscardSprite.setTextureRect(m_carteSangRects.at(topSangDiscard));
    }
    else {
        sangDiscardSprite.setTextureRect(m_carteSangVersoRect);
        sangDiscardSprite.setColor(sf::Color(255, 255, 255, 100)); // Dim if empty placeholder
    }
    sangDiscardSprite.setScale({ (1.0f + CARTE_ZOOM_MAX * m_defausseSangAnimMultiplier) * cardScale, (1.0f + CARTE_ZOOM_MAX * m_defausseSangAnimMultiplier) * cardScale });
    sangDiscardSprite.setRotation(sf::degrees(m_defausseSangAnimMultiplier * CARTE_ROTAION_MAX));
    sangDiscardSprite.setPosition({ deckStartX, deckYPos });
    m_fenetre.draw(sangDiscardSprite);

}

void SabaccVue::afficheActionsCliquables(const sf::FloatRect& area) {
    m_actionsCliquables.clear();
    
    float padding = area.size.x * 0.015f;
    float buttonHeight = area.size.y/14.5f; // Each button row takes up height
    float buttonWidth = area.size.x -2.0f*padding; 
    float crossHeight = area.size.y /15.0f; 
    float crossWidth = area.size.x / 15.0f;
    unsigned int buttonTextSize = getScaledFontSizeForArea(buttonHeight, 0.6f);
    float currentX = area.position.x + padding;
    float currentY = area.position.y + padding;
    float initialY = currentY;
    
    //affiche les actions optionnelles
    if (!animationEnCours() &&(!m_actionsAChoisir.empty() && m_jeuSabacc.m_joueurs[m_jeuSabacc.m_positionJActuel].m_enJeu && m_jeuSabacc.m_joueurs[m_jeuSabacc.m_positionJActuel].m_type == TYPE_JOUEUR::TJ_HUMAIN)) {

        for (const auto& action : m_actionsAChoisir) {

            ActionCliquable cad(m_policeTexte); // Assuming constructor takes font, or setFont is called
            cad.gameAction = action;

            std::ostringstream actionOss;
            
            if (action == Action::PJSP_D1) {
                actionOss << u8"Sabacc Prime: " << m_jeuSabacc.m_joueurs[m_jeuSabacc.m_positionJActuel].m_de1;
            }
            else if (action == Action::PR_IMPOSTEUR_SABLE_D1) {
                actionOss << u8"Imposteur Sable: " << m_jeuSabacc.m_joueurs[m_jeuSabacc.m_positionJActuel].m_de1;
            }
            else if (action == Action::PR_IMPOSTEUR_SANG_D1) {
                actionOss << u8"Imposteur Sang: " << m_jeuSabacc.m_joueurs[m_jeuSabacc.m_positionJActuel].m_de1;
            }
            else if (action == Action::PJSP_D2) {
                actionOss << u8"Sabacc Prime: " << m_jeuSabacc.m_joueurs[m_jeuSabacc.m_positionJActuel].m_de2;
            }
            else if (action == Action::PR_IMPOSTEUR_SABLE_D2) {
                actionOss << u8"Imposteur Sable: " << m_jeuSabacc.m_joueurs[m_jeuSabacc.m_positionJActuel].m_de2;
            }
            else if (action == Action::PR_IMPOSTEUR_SANG_D2) {
                actionOss << u8"Imposteur Sang: " << m_jeuSabacc.m_joueurs[m_jeuSabacc.m_positionJActuel].m_de2;
            }
            else if (action == Action::PR_IMPOSTEUR_SANG_D2) {
                actionOss << u8"Imposteur Sang: " << m_jeuSabacc.m_joueurs[m_jeuSabacc.m_positionJActuel].m_de2;
            }
            else if (action == Action::PJ_AUDIT_CIBLE_D) {
                actionOss << u8"Audit ciblé : Est";
            }
            else if (action == Action::PJ_AUDIT_CIBLE_F) {
                actionOss << u8"Audit ciblé : Nord";
            }
            else if (action == Action::PJ_AUDIT_CIBLE_G) {
                actionOss << u8"Audit ciblé : Ouest";
            }
            else if (action == Action::PJ_EPUISEMENT_D) {
                actionOss << u8"Epuisement : Est";
            }
            else if (action == Action::PJ_EPUISEMENT_F) {
                actionOss << u8"Epuisement : Nord";
            }
            else if (action == Action::PJ_EPUISEMENT_G) {
                actionOss << u8"Epuisement : Ouest";
            }
            else if (action == Action::PJ_TARIF_CIBLE_D) {
                actionOss << u8"Tarif ciblé: Est";
            }
            else if (action == Action::PJ_TARIF_CIBLE_F) {
                actionOss << u8"Tarif ciblé: Nord";
            }
            else if (action == Action::PJ_TARIF_CIBLE_G) {
                actionOss << u8"Tarif ciblé: Ouest";
            }
            else
                actionOss << action;

            std::string actionStr(actionOss.str());

            cad.textShape.setFont(m_policeTexte); // Explicitly set font
            cad.textShape.setString(sf::String::fromUtf8(actionStr.begin(), actionStr.end()));
            cad.textShape.setCharacterSize(buttonTextSize);
            cad.textShape.setFillColor(sf::Color::White);

            cad.backgroundShape.setSize({ buttonWidth, buttonHeight });
            cad.backgroundShape.setPosition({ currentX, currentY });
            cad.backgroundShape.setFillColor(sf::Color(50, 100, 255, 255));
            cad.backgroundShape.setOutlineThickness(1.f);
            cad.backgroundShape.setOutlineColor(sf::Color::Black);

            sf::FloatRect textBounds = cad.textShape.getLocalBounds();
            cad.textShape.setOrigin({ textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f });
            cad.textShape.setPosition({ currentX + buttonWidth / 2.f, currentY + buttonHeight / 2.f });

            cad.bounds = cad.backgroundShape.getGlobalBounds();
            m_fenetre.draw(cad.backgroundShape);
            m_fenetre.draw(cad.textShape);
            m_actionsCliquables.push_back(cad);
            currentY += buttonHeight + 1.0f * padding;
        }
    }

    //affiche les actions permanentes
    //Nouveau Jeu
    currentY = area.position.y + area.size.y - buttonHeight;

    ActionCliquable actionNouveauJeu(m_policeTexte);
    actionNouveauJeu.ViewAction = VA_NOUVELLE_PARTIE;
    std::string actionStr("Nouvelle Partie");

    actionNouveauJeu.textShape.setFont(m_policeTexte);
    actionNouveauJeu.textShape.setString(sf::String::fromUtf8(actionStr.begin(), actionStr.end()));
    actionNouveauJeu.textShape.setCharacterSize(buttonTextSize);
    actionNouveauJeu.textShape.setFillColor(sf::Color::White);
    
    actionNouveauJeu.backgroundShape.setSize({ buttonWidth, buttonHeight });
    actionNouveauJeu.backgroundShape.setPosition({ currentX, currentY });
    actionNouveauJeu.backgroundShape.setFillColor(sf::Color(80, 20, 20, 255));
    actionNouveauJeu.backgroundShape.setOutlineThickness(1.f);
    actionNouveauJeu.backgroundShape.setOutlineColor(sf::Color::Black);

    sf::FloatRect textBounds = actionNouveauJeu.textShape.getLocalBounds();
    actionNouveauJeu.textShape.setOrigin({ textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f });
    actionNouveauJeu.textShape.setPosition({ currentX + buttonWidth / 2.f, currentY + buttonHeight / 2.f });

    actionNouveauJeu.bounds = actionNouveauJeu.backgroundShape.getGlobalBounds();
    m_fenetre.draw(actionNouveauJeu.backgroundShape);
    m_fenetre.draw(actionNouveauJeu.textShape);
    m_actionsCliquables.push_back(actionNouveauJeu);
    currentY -= 1.0f * buttonHeight + 1.0f * padding;
    
    //afficher-cacher cartes
    ActionCliquable actionAfficherCacher(m_policeTexte);
    actionAfficherCacher.ViewAction = VA_AFFICHER_CACHER_CARTES;
    
    actionStr;
    if(m_afficheCartes) 
        actionStr="Cacher Cartes";
    else 
        actionStr = "Afficher Cartes";

    actionAfficherCacher.textShape.setFont(m_policeTexte);
    actionAfficherCacher.textShape.setString(sf::String::fromUtf8(actionStr.begin(), actionStr.end()));
    actionAfficherCacher.textShape.setCharacterSize(buttonTextSize);
    actionAfficherCacher.textShape.setFillColor(sf::Color::White);
    
    actionAfficherCacher.backgroundShape.setSize({ buttonWidth/2.0f - padding, buttonHeight });
    actionAfficherCacher.backgroundShape.setPosition({ currentX, currentY });
    actionAfficherCacher.backgroundShape.setFillColor(sf::Color(80, 20, 20, 255));
    actionAfficherCacher.backgroundShape.setOutlineThickness(1.f);
    actionAfficherCacher.backgroundShape.setOutlineColor(sf::Color::Black);

    textBounds = actionAfficherCacher.textShape.getLocalBounds();
    actionAfficherCacher.textShape.setOrigin({ textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f });
    actionAfficherCacher.textShape.setPosition({ currentX + buttonWidth / 4.f, currentY + buttonHeight / 2.f });

    actionAfficherCacher.bounds = actionAfficherCacher.backgroundShape.getGlobalBounds();
    m_fenetre.draw(actionAfficherCacher.backgroundShape);
    m_fenetre.draw(actionAfficherCacher.textShape);
    m_actionsCliquables.push_back(actionAfficherCacher);

    //changer la vitesse
    ActionCliquable actionVitesse(m_policeTexte);
    actionVitesse.ViewAction = VA_CHANGE_VITESSE;

    actionStr;
    if (m_rapide)
        actionStr = "Rapide";
    else
        actionStr = "Lent";
        
    actionVitesse.textShape.setFont(m_policeTexte);
    actionVitesse.textShape.setString(sf::String::fromUtf8(actionStr.begin(), actionStr.end()));
    actionVitesse.textShape.setCharacterSize(buttonTextSize);
    actionVitesse.textShape.setFillColor(sf::Color::White);

    actionVitesse.backgroundShape.setSize({ buttonWidth / 2.0f, buttonHeight });
    actionVitesse.backgroundShape.setPosition({ currentX + buttonWidth / 2.0f, currentY });
    actionVitesse.backgroundShape.setFillColor(sf::Color(80, 20, 20, 255));
    actionVitesse.backgroundShape.setOutlineThickness(1.f);
    actionVitesse.backgroundShape.setOutlineColor(sf::Color::Black);

    textBounds = actionVitesse.textShape.getLocalBounds();
    actionVitesse.textShape.setOrigin({ textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f });
    actionVitesse.textShape.setPosition({ currentX + buttonWidth* 3.f/ 4.f, currentY + buttonHeight / 2.f });

    actionVitesse.bounds = actionVitesse.backgroundShape.getGlobalBounds();
    m_fenetre.draw(actionVitesse.backgroundShape);
    m_fenetre.draw(actionVitesse.textShape);
    m_actionsCliquables.push_back(actionVitesse);

    //Ferme le jeu
    ActionCliquable actionFerme(m_policeTexte);
    actionFerme.ViewAction = VA_FERME;

    actionStr="X";

    actionFerme.textShape.setFont(m_policeTexte);
    actionFerme.textShape.setString(sf::String::fromUtf8(actionStr.begin(), actionStr.end()));
    actionFerme.textShape.setCharacterSize(crossHeight*0.7f);
    actionFerme.textShape.setFillColor(sf::Color::White);

    actionFerme.backgroundShape.setSize({ crossWidth, crossHeight});
    actionFerme.backgroundShape.setPosition({ m_fenetre.getSize().x - crossWidth,0 });
    actionFerme.backgroundShape.setFillColor(sf::Color(80, 20, 20, 255));
    actionFerme.backgroundShape.setOutlineThickness(1.f);
    actionFerme.backgroundShape.setOutlineColor(sf::Color::Black);

    textBounds = actionFerme.textShape.getLocalBounds();
    actionFerme.textShape.setOrigin({ textBounds.position.x + textBounds.size.x / 2.f, textBounds.position.y + textBounds.size.y / 2.f });
    actionFerme.textShape.setPosition({ m_fenetre.getSize().x - 0.5f*crossWidth, 0.5f * crossHeight });

    actionFerme.bounds = actionFerme.backgroundShape.getGlobalBounds();
    m_fenetre.draw(actionFerme.backgroundShape);
    m_fenetre.draw(actionFerme.textShape);
    m_actionsCliquables.push_back(actionFerme);

}

void SabaccVue::afficheScore(const sf::FloatRect& area)
{    
    unsigned int scoreSize= getScaledFontSizeForArea(area.size.y, TEXT_PROPORTION_MOYEN);
    float espace = area.size.y * 0.03f;
    float currentX = area.position.x + espace;
    float currentY = area.position.y + espace + scoreSize * 0.5f;
    int partieNb = m_scoresPrisEnCompte.size();
    int total=0;
    for (int score: m_scoresPrisEnCompte) {
        std::ostringstream oss;
        oss << "Partie "<<partieNb<< " : " << score;
        total += score;
        std::string s = oss.str();
        dessineTexte(s, currentX, currentY, scoreSize, sf::Color(255, 255, 255, 200), sf::Text::Style::Regular);
        currentY += espace + scoreSize;
        partieNb--;
    }
    std::ostringstream oss;
    oss << "Score total : " << total;
    std::string s = oss.str();
    dessineTexte(s, currentX, currentY, 1.5f* scoreSize, sf::Color(128, 255, 128, 200), sf::Text::Style::Bold);
}

void SabaccVue::dessineRectangleJoueur(sf::RenderWindow& w, sf::FloatRect& fr, bool currentPlayer) {
    sf::RectangleShape shape(fr.size);
    if (currentPlayer)
        shape.setFillColor(sf::Color(60, 20, 20, 200));
    else
        shape.setFillColor(sf::Color(20, 20, 20, 100));
    if (currentPlayer)
        shape.setOutlineColor(sf::Color(200, 100, 100));
    else
        shape.setOutlineColor(sf::Color(100, 100, 100));
    shape.setOutlineThickness(1.f);
    shape.setPosition(fr.position);
    w.draw(shape);
}

void SabaccVue::ajouteAnimation(Position pJ, uint32_t cible, SVAnimationType type, uint32_t duree, Son s, std::string message) {
    SVAnimation anim;
    anim.pJoueur = pJ;
    anim.cible = cible;
    anim.type = type;
    anim.dureeInitiale = duree;
    anim.dureeRestante = duree;
    anim.son = s;
    anim.message = message;
    m_animations.push(anim);
}

SVAnimationCible SabaccVue::animationCibleJeton(JetonInfluence jeton) {
    if (m_jeuSabacc.m_jetonsTiresAuSort[0] == jeton)
        return AC_J1;
    else if (m_jeuSabacc.m_jetonsTiresAuSort[1] == jeton)
        return AC_J2;    
    return AC_J3;
}

void SabaccVue::ajouteAnimationAvantAction(Action a)
{    
    Position pCible = Position::P_INDEFINI;
    SVAnimationCible cibleJeton = SVAnimationCible::AC_INDEFINI;
    switch (a) {
    case Action::PJ_AUDIT_CIBLE_D:
    case Action::PJ_AUDIT_CIBLE_G:
    case Action::PJ_AUDIT_CIBLE_F:
        cibleJeton = animationCibleJeton(AUDIT_CIBLE);        
        ajouteAnimationSon(Son::SON_JETON);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_ZOOM, m_carteAnimDuree);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_DISPARAIT, 0);        
        break;
    case Action::PJ_AUDIT_GENERAL:
        cibleJeton = animationCibleJeton(AUDIT_GENERAL);        
        ajouteAnimationSon(Son::SON_JETON);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_ZOOM, m_carteAnimDuree);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_DISPARAIT, 0);        
        break;
    case Action::PJ_CUISINER_LIVRES:
        cibleJeton = animationCibleJeton(CUISINER_LIVRES);        
        ajouteAnimationSon(Son::SON_JETON);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_ZOOM, m_carteAnimDuree);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_DISPARAIT, 0);        
        break;
    case Action::PJ_DETOURNEMENT:
        cibleJeton = animationCibleJeton(DETOURNEMENT);        
        ajouteAnimationSon(Son::SON_JETON);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_ZOOM, m_carteAnimDuree);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_DISPARAIT, 0);        
        break;
    case Action::PJ_DEVALORISATION:
        cibleJeton = animationCibleJeton(DEVALORISATION);        
        ajouteAnimationSon(Son::SON_JETON);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_ZOOM, m_carteAnimDuree);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_DISPARAIT, 0);        
        break;
    case Action::PJ_EMBARGO:
        cibleJeton = animationCibleJeton(EMBARGO);        
        ajouteAnimationSon(Son::SON_JETON);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_ZOOM, m_carteAnimDuree);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_DISPARAIT, 0);        
        break;
    case Action::PJ_EPUISEMENT_D:
        if (pCible == Position::P_INDEFINI)
            pCible = static_cast<Position>((m_jeuSabacc.m_positionJActuel + 3) % Position::NB_POSITIONS);
    case Action::PJ_EPUISEMENT_G:
        if (pCible == Position::P_INDEFINI)
            pCible = static_cast<Position>((m_jeuSabacc.m_positionJActuel + 1) % Position::NB_POSITIONS);
    case Action::PJ_EPUISEMENT_F:
        if (pCible == Position::P_INDEFINI)
            pCible = static_cast<Position>((m_jeuSabacc.m_positionJActuel + 2) % Position::NB_POSITIONS);
        cibleJeton = animationCibleJeton(EPUISEMENT);

        //jeton zoom et disparait
        ajouteAnimationSon(Son::SON_JETON);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_ZOOM, m_carteAnimDuree);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_DISPARAIT, 0);

        //cartes joueur zoom et disparaissent
        ajouteAnimation(pCible, AC_CARTE_SABLE_JOUEUR | AC_CARTE_SANG_JOUEUR, AT_ZOOM, m_carteAnimDuree);
        ajouteAnimation(pCible, AC_CARTE_SABLE_JOUEUR | AC_CARTE_SANG_JOUEUR, AT_DISPARAIT, 0);
        break;
    case Action::PJ_FRAUDE_MAJEURE:
        cibleJeton = animationCibleJeton(FRAUDE_MAJEURE);        
        ajouteAnimationSon(Son::SON_JETON);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_ZOOM, m_carteAnimDuree);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_DISPARAIT, 0);        
        break;
    case Action::PJ_IMMUNITE:
        cibleJeton = animationCibleJeton(IMMUNITE);        
        ajouteAnimationSon(Son::SON_JETON);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_ZOOM, m_carteAnimDuree);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_DISPARAIT, 0);        
        break;
    case Action::PJ_PIOCHE_GRATUITE:
        cibleJeton = animationCibleJeton(PIOCHE_GRATUITE);        
        ajouteAnimationSon(Son::SON_JETON);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_ZOOM, m_carteAnimDuree);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_DISPARAIT, 0);        
        break;
    case Action::PJ_REMBOURSEMENT_2:
        cibleJeton = animationCibleJeton(REMBOURSEMENT_2);        
        ajouteAnimationSon(Son::SON_JETON);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_ZOOM, m_carteAnimDuree);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_DISPARAIT, 0);        
        break;
    case Action::PJ_REMBOURSEMENT_3:
        cibleJeton = animationCibleJeton(REMBOURSEMENT_3);        
        ajouteAnimationSon(Son::SON_JETON);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_ZOOM, m_carteAnimDuree);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_DISPARAIT, 0);        
        break;
    case Action::PJ_SABACC_PRIME:
        cibleJeton = animationCibleJeton(SABACC_PRIME);        
        ajouteAnimationSon(Son::SON_JETON);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_ZOOM, m_carteAnimDuree);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_DISPARAIT, 0);        
        break;
    case Action::PJ_TARIF_CIBLE_D:
    case Action::PJ_TARIF_CIBLE_G:
    case Action::PJ_TARIF_CIBLE_F:
        cibleJeton = animationCibleJeton(TARIF_CIBLE);        
        ajouteAnimationSon(Son::SON_JETON);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_ZOOM, m_carteAnimDuree);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_DISPARAIT, 0);        
        break;
    case Action::PJ_TARIF_GENERAL:
        cibleJeton = animationCibleJeton(TARIF_GENERAL);
        ajouteAnimationSon(Son::SON_JETON);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_ZOOM, m_carteAnimDuree);
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_DISPARAIT, 0);
        break;
    case Action::PP_PIOCHE_DEFAUSSE_SABLE: {
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, AC_DEFAUSSE_SABLE, AT_ZOOM, m_carteAnimDuree);
        break;
    }
    case Action::PP_PIOCHE_DEFAUSSE_SANG:
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, AC_DEFAUSSE_SANG, AT_ZOOM, m_carteAnimDuree);
        break;
    case Action::PP_PIOCHE_PAQUET_SABLE:
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, AC_PAQUET_SABLE, AT_ZOOM, m_carteAnimDuree);
        break;
    case Action::PP_PIOCHE_PAQUET_SANG:
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, AC_PAQUET_SANG, AT_ZOOM, m_carteAnimDuree);
        break;
    case Action::PD_DEFAUSSE_CARTE_PIOCHEE:
        if (m_jeuSabacc.m_joueurs[m_jeuSabacc.m_positionJActuel].m_carteSablePiochee != Carte::C_INDEFINI)
            carteDefausseeSable = true;
        else
            carteDefausseeSable = false;      
        ajouteAnimation(m_jeuSabacc.m_positionJActuel, SVAnimationCible::AC_CARTE_PIOCHEE_JOUEUR, AT_ZOOM, m_carteAnimDuree);
        break;
    case Action::PD_GARDE_CARTE_PIOCHEE:
        if (m_jeuSabacc.m_joueurs[m_jeuSabacc.m_positionJActuel].m_carteSablePiochee != Carte::C_INDEFINI) {
            ajouteAnimation(m_jeuSabacc.m_positionJActuel, SVAnimationCible::AC_CARTE_SABLE_JOUEUR, AT_ZOOM, m_carteAnimDuree);
            carteDefausseeSable = true;
        }
        else {
            ajouteAnimation(m_jeuSabacc.m_positionJActuel, SVAnimationCible::AC_CARTE_SANG_JOUEUR, AT_ZOOM, m_carteAnimDuree);
            carteDefausseeSable = false;
        }        
        break;        
    }
}

void SabaccVue::ajouteAnimationApresAction(Action a)
{
    SVAnimationCible cibleJeton = SVAnimationCible::AC_INDEFINI;
    Position pCible= Position::P_INDEFINI;
    switch (a) {
        case Action::PJ_AUDIT_CIBLE_D:
        case Action::PJ_AUDIT_CIBLE_G:
        case Action::PJ_AUDIT_CIBLE_F:
            cibleJeton = animationCibleJeton(AUDIT_CIBLE);
            ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_APPARAIT, 0);
            break;
        case Action::PJ_AUDIT_GENERAL:
            cibleJeton = animationCibleJeton(AUDIT_GENERAL);
            ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_APPARAIT, 0);
            break;
        case Action::PJ_CUISINER_LIVRES:
            cibleJeton = animationCibleJeton(CUISINER_LIVRES);
            ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_APPARAIT, 0);
            break;
        case Action::PJ_DETOURNEMENT:
            cibleJeton = animationCibleJeton(DETOURNEMENT);
            ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_APPARAIT, 0);
            break;
        case Action::PJ_DEVALORISATION:
            cibleJeton = animationCibleJeton(DEVALORISATION);
            ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_APPARAIT, 0);
            break;
        case Action::PJ_EMBARGO:
            cibleJeton = animationCibleJeton(EMBARGO);
            ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_APPARAIT, 0);
            break;
        case Action::PJ_FRAUDE_MAJEURE:
            cibleJeton = animationCibleJeton(FRAUDE_MAJEURE);
            ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_APPARAIT, 0);
            break;
        case Action::PJ_IMMUNITE:
            cibleJeton = animationCibleJeton(IMMUNITE);
            ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_APPARAIT, 0);
            break;
        case Action::PJ_PIOCHE_GRATUITE:
            cibleJeton = animationCibleJeton(PIOCHE_GRATUITE);
            ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_APPARAIT, 0);
            break;
        case Action::PJ_REMBOURSEMENT_2:
            cibleJeton = animationCibleJeton(REMBOURSEMENT_2);
            ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_APPARAIT, 0);
            break;
        case Action::PJ_REMBOURSEMENT_3:
            cibleJeton = animationCibleJeton(REMBOURSEMENT_3);
            ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_APPARAIT, 0);
            break;
        case Action::PJ_SABACC_PRIME:
            cibleJeton = animationCibleJeton(SABACC_PRIME);
            ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_APPARAIT, 0);
            break;
        case Action::PJ_TARIF_CIBLE_D:
        case Action::PJ_TARIF_CIBLE_G:
        case Action::PJ_TARIF_CIBLE_F:
            cibleJeton = animationCibleJeton(TARIF_CIBLE);            
            ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_APPARAIT, 0);
            break;
        case Action::PJ_EPUISEMENT_D:
            if (pCible == Position::P_INDEFINI)
                pCible = static_cast<Position>((m_jeuSabacc.m_positionJActuel + 3) % Position::NB_POSITIONS);
        case Action::PJ_EPUISEMENT_G:
            if(pCible== Position::P_INDEFINI)
                pCible = static_cast<Position>((m_jeuSabacc.m_positionJActuel + 1) % Position::NB_POSITIONS);
        case Action::PJ_EPUISEMENT_F:
            if (pCible == Position::P_INDEFINI)
                pCible = static_cast<Position>((m_jeuSabacc.m_positionJActuel + 2) % Position::NB_POSITIONS);                                    
            //defausse sable et sang dezoom
            ajouteAnimationSon(Son::SON_CARD);
            ajouteAnimation(pCible, AC_DEFAUSSE_SABLE | AC_DEFAUSSE_SANG, AT_DEZOOM, m_carteAnimDuree);
            ajouteAnimation(pCible, AC_PAQUET_SABLE | AC_PAQUET_SANG, AT_ZOOM, m_carteAnimDuree);
            ajouteAnimation(pCible, AC_CARTE_SABLE_JOUEUR | AC_CARTE_SANG_JOUEUR, AT_APPARAIT, 0);
            ajouteAnimationSon(Son::SON_CARD);
            ajouteAnimation(pCible, AC_CARTE_SABLE_JOUEUR | AC_CARTE_SANG_JOUEUR, AT_DEZOOM, m_carteAnimDuree);          
            
            cibleJeton = animationCibleJeton(EPUISEMENT);
            ajouteAnimation(m_jeuSabacc.m_positionJActuel, cibleJeton, AT_APPARAIT, 0);
            break;
        case Action::PP_PIOCHE_DEFAUSSE_SABLE:
        case Action::PP_PIOCHE_DEFAUSSE_SANG:
        case Action::PP_PIOCHE_PAQUET_SABLE:
        case Action::PP_PIOCHE_PAQUET_SANG:
            ajouteAnimationSon(Son::SON_CARD);
            ajouteAnimation(m_jeuSabacc.m_positionJActuel, SVAnimationCible::AC_CARTE_PIOCHEE_JOUEUR, AT_DEZOOM, m_carteAnimDuree);
            break;
        case Action::PD_GARDE_CARTE_PIOCHEE:
            ajouteAnimationSon(Son::SON_CARD);
            if (carteDefausseeSable) {
                ajouteAnimation(m_jeuSabacc.m_positionJActuel, SVAnimationCible::AC_DEFAUSSE_SABLE, AT_DEZOOM, m_carteAnimDuree);
            }
            else {
                ajouteAnimation(m_jeuSabacc.m_positionJActuel, SVAnimationCible::AC_DEFAUSSE_SANG, AT_DEZOOM, m_carteAnimDuree);
            }
            break;
        case Action::PD_DEFAUSSE_CARTE_PIOCHEE:
            ajouteAnimationSon(Son::SON_CARD);
            if (carteDefausseeSable)
                ajouteAnimation(m_jeuSabacc.m_positionJActuel, SVAnimationCible::AC_DEFAUSSE_SABLE, AT_DEZOOM, m_carteAnimDuree);
            else
                ajouteAnimation(m_jeuSabacc.m_positionJActuel, SVAnimationCible::AC_DEFAUSSE_SANG, AT_DEZOOM, m_carteAnimDuree);
            break;
    }

}

void SabaccVue::ajouteAnimationSon(Son s)
{
    ajouteAnimation(Position::P_INDEFINI, SVAnimationCible::AC_INDEFINI, SVAnimationType::AT_SOUND, 1, s);
}

void SabaccVue::traiteClicJoueur(sf::Vector2i mousePos) {
    // Convert mousePos from window coordinates to view coordinates if necessary (not needed for default view)
    sf::Vector2f worldPos = m_fenetre.mapPixelToCoords(mousePos);

    for (const auto& cad : m_actionsCliquables) {
        if (cad.bounds.contains(worldPos)) {
            joueSonBouton();
            Action actionJeu = cad.gameAction;
            VueAction actionVue = cad.ViewAction;
            if (actionVue == VueAction::VA_AFFICHER_CACHER_CARTES) {                
                if (!m_afficheCartes && MessageBox(m_fenetre.getNativeHandle(), L"Attention: Afficher les cartes efface les scores. Voulez-vous continuer?", L"Afficher toutes les cartes", MB_YESNO) == IDYES)
                {
                    m_afficheCartes = true;
                    m_suitLesScores = false;
                    if(m_pListener)
						m_pListener->surScoreReset();
                } else if (m_afficheCartes) {
                    m_afficheCartes = false;
				}
                return;
            }
            else if (actionVue == VueAction::VA_NOUVELLE_PARTIE) {
                //appelle onGameRestart des listeners
                
                if (m_jeuSabacc.m_jeuTermine || MessageBox(m_fenetre.getNativeHandle(), L"Voulez-vous vraiment recommencer la partie?", L"Nouvelle Partie", MB_YESNO) == IDYES) {
                        m_pListener->surRedemarreJeu();
                        if(!m_afficheCartes)
                            m_suitLesScores = true;
                        for (int i = 0; i < 4; ++i) {
                            for (int j = 0; j < 3; ++j) {
                                m_jVisible[i][j] = true;
                            }
                        }                        
                }
                return;
            }
            else if (actionVue == VueAction::VA_CHANGE_VITESSE) {
                m_rapide = !m_rapide;
                if (m_rapide) {
                    m_carteAnimDuree = CARTE_ANIM_DUREE_RAPIDE;
                    m_messageDuree = MESSAGE_DUREE_RAPIDE;
                }
                else {
                    m_carteAnimDuree = CARTE_ANIM_DUREE_LENTE;
                    m_messageDuree = MESSAGE_DUREE_LENTE;
                }                    
                return;
            }
            else if(actionVue ==VueAction::VA_FERME) {                
                if (MessageBox(m_fenetre.getNativeHandle(), L"Voulez-vous vraiment quitter?", L"Quitter le Jeu", MB_YESNO) == IDYES) 
                    m_pListener->surFermeFenetre();
                return;
            }
            else {                                
                //execute action
                m_pListener->surActionJeuChoisie(actionJeu);
            }
        }
    }
}

void SabaccVue::ajouteLogMessage(const std::string& message) {
    if (m_texteMessages.size() >= MAX_MESSAGES) {
        m_texteMessages.erase(m_texteMessages.begin());
    }
    sf::Text text(m_policeTexte);
    text.setString(sf::String::fromUtf8(message.begin(), message.end()));
    text.setFillColor(sf::Color::White);
    m_texteMessages.push_back(text);
}

void SabaccVue::ajouteLogMessage(const std::ostringstream& sstr) {
    std::string message(sstr.str());
    ajouteLogMessage(message);
}

void SabaccVue::ajouteLogMessage(const Action& a)
{
    std::ostringstream drawnOss;
    if (m_jeuSabacc.m_positionJActuel != Position::S)
        drawnOss << u8"IA " << m_jeuSabacc.m_positionJActuel;
    else
        drawnOss << u8"Humain ";
    drawnOss << u8" a joué: " << a;
    
    Position posJoueurCible=Position::P_INDEFINI;
    if (a == Action::PJ_AUDIT_CIBLE_D || a == Action::PJ_EPUISEMENT_D || a == Action::PJ_TARIF_CIBLE_D) 
        posJoueurCible = static_cast<Position>((m_jeuSabacc.m_positionJActuel+3)%Position::NB_POSITIONS);
    else if (a == Action::PJ_AUDIT_CIBLE_F || a == Action::PJ_EPUISEMENT_F || a == Action::PJ_TARIF_CIBLE_F) 
        posJoueurCible = static_cast<Position>((m_jeuSabacc.m_positionJActuel + 2) % Position::NB_POSITIONS);
    else if (a == Action::PJ_AUDIT_CIBLE_G || a == Action::PJ_EPUISEMENT_G || a == Action::PJ_TARIF_CIBLE_G)
        posJoueurCible = static_cast<Position>((m_jeuSabacc.m_positionJActuel + 1) % Position::NB_POSITIONS);        

    if (posJoueurCible != Position::P_INDEFINI)
        drawnOss << u8" sur joueur: " << posJoueurCible;

    ajouteLogMessage(drawnOss);
}

void SabaccVue::executeAnimations()
{
    bool traiteAction = true;
    
    while (traiteAction) {
        if (m_animations.size() == 0)
            return;
        SVAnimation& anim = m_animations.front();
        m_positionCibleAnimation = anim.pJoueur;
        float mult = 1.0f - sinf(M_PI_2 + M_PI_2 * ((float)(anim.dureeRestante) / (float)(anim.dureeInitiale)));
        //float mult = ((float)(anim.dureeRestante) / (float)(anim.dureeInitiale));
        if (anim.type == AT_ZOOM) {
            mult = cosf(M_PI_2 * ((float)(anim.dureeRestante) / (float)(anim.dureeInitiale)));
            if (anim.dureeRestante == 0)
                mult = 0.0f;
        }
        if (anim.cible & SVAnimationCible::AC_CARTE_PIOCHEE_JOUEUR) {
            if (anim.type == SVAnimationType::AT_ZOOM || anim.type == SVAnimationType::AT_DEZOOM)
                m_cartePiocheeJoueurAnimMultiplier = mult;
        }
        if (anim.cible & SVAnimationCible::AC_CARTE_SABLE_JOUEUR) {
            if (anim.type == SVAnimationType::AT_ZOOM || anim.type == SVAnimationType::AT_DEZOOM)
                m_carteSableJoueurAnimMultiplier = mult;
            else if (anim.type == SVAnimationType::AT_APPARAIT)
                m_carteSableVisible[anim.pJoueur] = true;
            else if (anim.type == SVAnimationType::AT_DISPARAIT)
                m_carteSableVisible[anim.pJoueur] = false;
        }
        if (anim.cible & SVAnimationCible::AC_CARTE_SANG_JOUEUR) {
            if (anim.type == SVAnimationType::AT_ZOOM || anim.type == SVAnimationType::AT_DEZOOM)
                m_carteSangJoueurAnimMultiplier = mult;
            else if (anim.type == SVAnimationType::AT_APPARAIT)
                m_carteSangVisible[anim.pJoueur] = true;
            else if (anim.type == SVAnimationType::AT_DISPARAIT)
                m_carteSangVisible[anim.pJoueur] = false;
        }
        if (anim.cible & SVAnimationCible::AC_PAQUET_SABLE)
            m_paquetSableAnimMultiplier = mult;
        if (anim.cible & SVAnimationCible::AC_PAQUET_SANG)
            m_paquetSangAnimMultiplier = mult;
        if (anim.cible & SVAnimationCible::AC_DEFAUSSE_SABLE) {
            if (anim.type == SVAnimationType::AT_ZOOM || anim.type == SVAnimationType::AT_DEZOOM)
                m_defausseSableAnimMultiplier = mult;
            else if(anim.type == SVAnimationType::AT_APPARAIT)
                m_defausseSableVisible = true;
            else if (anim.type == SVAnimationType::AT_DISPARAIT)
                m_defausseSableVisible = false;
        }
            
        if (anim.cible & SVAnimationCible::AC_DEFAUSSE_SANG) {
            if (anim.type == SVAnimationType::AT_ZOOM || anim.type == SVAnimationType::AT_DEZOOM)
                m_defausseSangAnimMultiplier = mult;
            else if (anim.type == SVAnimationType::AT_APPARAIT)
                m_defausseSangVisible = true;
            else if (anim.type == SVAnimationType::AT_DISPARAIT)
                m_defausseSangVisible = false;
        }
        
        if (anim.cible & SVAnimationCible::AC_J1) {
            if (anim.type == SVAnimationType::AT_ZOOM || anim.type == SVAnimationType::AT_DEZOOM)
                m_jAnimMultipliers[0] = mult;
            else if (anim.type == SVAnimationType::AT_APPARAIT)
                m_jVisible[anim.pJoueur][0] = true;
            else if (anim.type == SVAnimationType::AT_DISPARAIT)
                m_jVisible[anim.pJoueur][0] = false;
        }

        if (anim.cible & SVAnimationCible::AC_J2) {
            if (anim.type == SVAnimationType::AT_ZOOM || anim.type == SVAnimationType::AT_DEZOOM)
                m_jAnimMultipliers[1] = mult;
            else if (anim.type == SVAnimationType::AT_APPARAIT)
                m_jVisible[anim.pJoueur][1] = true;
            else if (anim.type == SVAnimationType::AT_DISPARAIT)
                m_jVisible[anim.pJoueur][1] = false;
        }

        if (anim.cible & SVAnimationCible::AC_J3) {
            if (anim.type == SVAnimationType::AT_ZOOM || anim.type == SVAnimationType::AT_DEZOOM)
                m_jAnimMultipliers[2] = mult;
            else if (anim.type == SVAnimationType::AT_APPARAIT)
                m_jVisible[anim.pJoueur][2] = true;
            else if (anim.type == SVAnimationType::AT_DISPARAIT)
                m_jVisible[anim.pJoueur][2] = false;
        }

        if (anim.type == AT_SOUND)
        {
            switch (anim.son) {
            case Son::SON_CARD:
                joueSonCarte();
                break;
            case Son::SON_JETON:
                joueSonJeton();
                break;
            case Son::SON_WOOSH:
                joueSonWoosh();
                break;
            case Son::SON_CLOCHE:
                joueSonCloche();
                break;
            }
        }

		if (anim.type == AT_GRAND_MESSAGE)
		{
            if (anim.dureeRestante == 0)
                m_grandMessage = "";
            else
                m_grandMessage = anim.message;
		}
        
        if (anim.dureeRestante == 0)
            m_animations.pop();
        else
            anim.dureeRestante--;

        //si on est sur une anim terminée on démarre direct la suivante
        traiteAction = anim.dureeRestante == 0;
    }
}

void SabaccVue::afficheMessages(const sf::FloatRect& area) {
    float padding = area.size.y * 0.05f; // Padding relative to message area height

    sf::RectangleShape arrierePlanMsg(sf::Vector2f(area.size.x, area.size.y));
    arrierePlanMsg.setPosition({ area.position.x, area.position.y });
    arrierePlanMsg.setFillColor(sf::Color(30, 30, 30, 100));
    arrierePlanMsg.setOutlineColor(sf::Color(100, 100, 100));
    arrierePlanMsg.setOutlineThickness(1.f);
    m_fenetre.draw(arrierePlanMsg);

    float textY = area.position.y + padding;
    unsigned int msgTextSize = getScaledFontSizeForArea(area.size.y, TEXT_PROPORTION_MINI ); // Scale based on num messages
    msgTextSize = std::max(12u, std::min(msgTextSize, 24u)); // Clamp message text size
    float lineSpacing = msgTextSize * 1.5f;// area.size.y / static_cast<float>(MAX_MESSAGES + 1); // Distribute lines in available height

    for (const auto& msgTextBase : m_texteMessages) {
        if (textY + lineSpacing > area.position.y + area.size.y - padding) break; // Don't draw outside bounds

        sf::Text tempText = msgTextBase;
        tempText.setFont(m_policeTexte); // Ensure font is set
        tempText.setCharacterSize(msgTextSize);
        tempText.setFillColor(sf::Color(255, 255, 255, 200));
        tempText.setPosition({ area.position.x + padding, textY });
        m_fenetre.draw(tempText);
        textY += lineSpacing;
    }
}

void SabaccVue::afficheGrandMessage(const sf::FloatRect& area)
{
    float padding = area.size.y * 0.05f; // Padding relative to message area height

    sf::RectangleShape arrierePlanMsg(sf::Vector2f(area.size.x, area.size.y));
    arrierePlanMsg.setPosition({ area.position.x, area.position.y });
    arrierePlanMsg.setFillColor(sf::Color(80, 20, 20, 200));
    arrierePlanMsg.setOutlineColor(sf::Color(100, 100, 100));
    arrierePlanMsg.setOutlineThickness(1.f);
    m_fenetre.draw(arrierePlanMsg);

    unsigned int msgTextSize = 0.3f*area.size.y; // Scale based on num messages

    std::string message = sf::String::fromUtf8(m_grandMessage.begin(), m_grandMessage.end());
    
    sf::Text tempText(m_policeTexte, message);
    tempText.setCharacterSize(msgTextSize);
    
    sf::FloatRect bounds = tempText.getLocalBounds();
    tempText.setOrigin({ bounds.position.x + bounds.size.x * 0.5f, bounds.position.y + bounds.size.y * 0.5f});

    tempText.setPosition({ area.position.x + area.size.x / 2, area.position.y + area.size.y / 2 });
    m_fenetre.draw(tempText);
    
}

void SabaccVue::affiche() {
    m_fenetre.clear(sf::Color(30, 30, 30)); 

    sf::Vector2f tailleFenetre = m_fenetre.getView().getSize();
    float fenetreL = static_cast<float>(tailleFenetre.x);
    float fenetreH = static_cast<float>(tailleFenetre.y);

    //arriere plan
    sf::Sprite spriteAP(m_textureArrierePlan);
    spriteAP.setPosition({ 0, 0 });
    float scaleV = fenetreH / m_textureArrierePlan.getSize().y;
    float scaleH = fenetreL / m_textureArrierePlan.getSize().x;	
    spriteAP.setScale({ std::max(scaleV,scaleH),std::max(scaleV,scaleH) });
	m_fenetre.draw(spriteAP);

    // Define areas using percentages
    float vRemplissage = fenetreH * 0.01f; // remplissage vertical
    float hRemplissage = fenetreL * 0.01f; // remplissage horizontal 

    // Taille affichage joueurs
    float joueurL = fenetreL * 0.32f;
    float joueurH = fenetreH * 0.24f;

    sf::FloatRect zoneRecapJetons({ hRemplissage,vRemplissage },{ fenetreL * 0.5f - joueurL * 0.5f,fenetreH * 0.33f - joueurH * 0.5f });
    
    sf::FloatRect zoneScore({ fenetreL - 0.5f*joueurL - hRemplissage,vRemplissage }, { fenetreL * 0.5f - joueurL * 0.5f,fenetreH * 0.33f - joueurH * 0.5f });
    if(m_suitLesScores)
        afficheScore(zoneScore);

    sf::FloatRect zoneJoueurNord({ fenetreL * 0.5f - joueurL * 0.5f, vRemplissage }, { joueurL, joueurH });
    dessineRectangleJoueur(m_fenetre, zoneJoueurNord, m_jeuSabacc.m_positionJActuel== Position::N);
    sf::FloatRect zoneJoueurEst({ fenetreL - joueurL - hRemplissage, fenetreH * 0.33f - joueurH * 0.5f }, { joueurL, joueurH });
    dessineRectangleJoueur(m_fenetre, zoneJoueurEst, m_jeuSabacc.m_positionJActuel == Position::E);
    sf::FloatRect zoneJoueurOuest({ hRemplissage, fenetreH * 0.33f - joueurH * 0.5f }, { joueurL, joueurH });
    dessineRectangleJoueur(m_fenetre, zoneJoueurOuest, m_jeuSabacc.m_positionJActuel == Position::O);
    sf::FloatRect zoneJoueurSud({ fenetreL * 0.5f - joueurL * 0.5f , fenetreH - joueurH - vRemplissage }, { joueurL, joueurH });
    dessineRectangleJoueur(m_fenetre, zoneJoueurSud, m_jeuSabacc.m_positionJActuel == Position::S);

    // Boite de messages
    float boiteMessageH = fenetreH * 0.54f - 2.0f * vRemplissage;
    sf::FloatRect zoneBoiteMessages({ hRemplissage, fenetreH - boiteMessageH - vRemplissage }, { joueurL - 2 * hRemplissage, boiteMessageH });    

    // Zone des actions cliquables
    //float actionAreaH = fenetreH * 0.13f;
    sf::FloatRect actionArea({ fenetreL - joueurL + hRemplissage, fenetreH - boiteMessageH - vRemplissage }, { joueurL - 2.0f * hRemplissage, boiteMessageH });   

    // Board State Area (Central)
    // Calculate remaining space for board area
    float boardTop = zoneJoueurNord.position.y + zoneJoueurNord.size.y + vRemplissage;
    float boardBottom = zoneJoueurSud.position.y - vRemplissage; // Or actionArea.top if that's higher and pSouthArea is not fixed
    float boardLeft = zoneJoueurOuest.position.x + zoneJoueurOuest.size.x + hRemplissage;
    float boardRight = zoneJoueurEst.position.x - hRemplissage;
    sf::FloatRect boardArea({ fenetreL * 0.5f - joueurL * 0.5f, fenetreH * 0.5f - joueurH * 0.5f }, { joueurL, joueurH });

    //affiche la recap des jetons d'influence
    if(m_jeuSabacc.m_jetonsTiresAuSort[0]!= JetonInfluence::JI_INDEFINI)
        afficheRecapJetons(zoneRecapJetons);

    // Render components into their areas
    afficheJoueur(Position::N, zoneJoueurNord);
    afficheJoueur(Position::E, zoneJoueurEst);
    afficheJoueur(Position::S, zoneJoueurSud);
    afficheJoueur(Position::O, zoneJoueurOuest);

    afficheCentreTable(boardArea);

    //todo test
    //if (m_jeuSabacc.m_jeuTermine || m_jeuSabacc.m_joueurs[m_jeuSabacc.m_positionJActuel].m_type == TYPE_JOUEUR::TJ_HUMAIN || !m_jeuSabacc.m_joueurs[Position::S].m_enJeu) {
        afficheActionsCliquables(actionArea);
    //}
  
    afficheMessages(zoneBoiteMessages);

    //grand message
    sf::FloatRect grandMessageArea({ 0, 0.5f* fenetreH -0.035f * fenetreH }, { fenetreL, fenetreH * 0.25f } );
    if (m_grandMessage!="")
        afficheGrandMessage(grandMessageArea);

    //execute les animations
    if (m_animations.size() != 0)
        executeAnimations();

    m_fenetre.display();
}

void SabaccVue::afficheRecapJetons(const sf::FloatRect& area)
{
    // Draw each token in the recap area
    unsigned int jetonRecapSize= getScaledFontSizeForArea(area.size.y, TEXT_PROPORTION_MOYEN);
    float targetTokenHeight = area.size.y * 0.20f;
    float tokenScale = targetTokenHeight / static_cast<float>(m_jetonInfluenceVersoRect.size.y);
    float scaledTokenWidth = static_cast<float>(m_jetonInfluenceVersoRect.size.x) * tokenScale;
    float tokenSpacing = area.size.y* 0.03f;
    float currentX = area.position.x + tokenSpacing;
    float currentY = area.position.y + tokenSpacing;// +targetTokenHeight * 0.5f;

    for (const auto& jeton : m_jeuSabacc.m_jetonsTiresAuSort) {
        sf::Sprite sprite(m_textureElements);
        sprite.setTextureRect(m_jetonInfluenceRects.at(jeton));
		sprite.setColor(sf::Color(255, 255, 255, 128));
        sprite.setScale({ tokenScale, tokenScale });
        sprite.setPosition({ currentX, currentY });
        sprite.setOrigin({ scaledTokenWidth * 0.5f,targetTokenHeight * 0.5f });
        m_fenetre.draw(sprite);
		currentX += scaledTokenWidth + tokenSpacing;
		currentY += targetTokenHeight * 0.5f- jetonRecapSize*0.5f;
        std::ostringstream oss;
        oss<< jeton<<u8" : "<< DESCRIPTION_JI[jeton];
        std::string s = oss.str();
        dessineTexte(s, currentX, currentY, jetonRecapSize, sf::Color(255,255,255,200), sf::Text::Style::Regular);

        currentX = area.position.x + tokenSpacing;
		currentY += targetTokenHeight*0.5f + tokenSpacing+ jetonRecapSize*0.5f;
	}
}

