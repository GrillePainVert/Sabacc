#include "types.h"

std::ostream& operator<<(std::ostream& os, const JetonInfluence& j)
{
    switch (j) {
    case JetonInfluence::AUDIT_CIBLE: os << u8"Audit ciblé"; break;
    case JetonInfluence::AUDIT_GENERAL: os << u8"Audit général"; break;
    case JetonInfluence::CUISINER_LIVRES: os << u8"Cuisiner livres"; break;
    case JetonInfluence::DETOURNEMENT: os << u8"Détournement"; break;
    case JetonInfluence::DEVALORISATION: os << u8"Dévalorisation"; break;
    case JetonInfluence::EMBARGO: os << u8"Embargo"; break;
    case JetonInfluence::EPUISEMENT: os << u8"Epuisement"; break;
    case JetonInfluence::FRAUDE_MAJEURE: os << u8"Fraude Maj."; break;
    case JetonInfluence::IMMUNITE: os << u8"Immunité"; break;
    case JetonInfluence::JI_INDEFINI: os << u8"Indéfini"; break;
    case JetonInfluence::PIOCHE_GRATUITE: os << u8"Pioche Gratuite"; break;
    case JetonInfluence::REMBOURSEMENT_2: os << u8"Remboursement 2"; break;
    case JetonInfluence::REMBOURSEMENT_3: os << u8"Remboursement 3"; break;
    case JetonInfluence::SABACC_PRIME: os << u8"Sabacc Prime"; break;
    case JetonInfluence::TARIF_CIBLE: os << u8"Tarif ciblé"; break;
    case JetonInfluence::TARIF_GENERAL: os << u8"Tarif général"; break;
//    case JetonInfluence::TRANSACTION: os << u8"Transaction"; break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Carte& c)
{
    switch (c) {
    case Carte::C1: os << u8"1 "; break;
    case Carte::C2: os << u8"2 "; break;
    case Carte::C3: os << u8"3 "; break;
    case Carte::C4: os << u8"4 "; break;
    case Carte::C5: os << u8"5 "; break;
    case Carte::C6: os << u8"6 "; break;
    case Carte::C_INDEFINI: os << u8"_ "; break;
    case Carte::IMPOSTEUR: os << u8"I."; break;
    case Carte::SYLOP: os << u8"S."; break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Position& p)
{
    switch (p) {
    case Position::E: os << u8"Est"; break;
    case Position::N: os << u8"Nord"; break;
    case Position::S: os << u8"Sud"; break;
    case Position::O: os << u8"Ouest"; break;
    case Position::P_INDEFINI: os << "Indéfini"; break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const PRelative& p)
{
    switch (p) {
    case PRelative::MOI: os << u8"M"; break;
    case PRelative::GAUCHE: os << u8"G"; break;
    case PRelative::FACE: os << u8"F"; break;
    case PRelative::DROITE: os << u8"D"; break;
    case PRelative::PR_INDEFINI: os << u8"Indéfini"; break;
    default: os << u8"Inconnu"; break;
    }
    return os;
}

PRelative abs2rel(const Position p, const Position maPosition)
{
    return static_cast<PRelative>((p - maPosition + NB_POSITIONS) % NB_POSITIONS);
}

Position rel2abs(const PRelative p, const Position maPosition)
{
    return static_cast<Position>((p + maPosition) % NB_POSITIONS);
}

std::ostream& operator<<(std::ostream& os, const PhaseTour& p)
{
    switch (p) {
    case PHASE_INDEFINIE:           os << u8"N/A";              break;
    case PHASE_JETON:               os << u8"JETON";            break;
    case PHASE_JETON_SABACC_PRIME_DE: os << u8"JETON PRIME DÉ"; break;
    case PHASE_PIOCHE:              os << u8"PIOCHE";           break;
    case PHASE_DEFAUSSE:            os << u8"DEFAUSSE";         break;
    case PHASE_RESOLUTION:          os << u8"RESOLUTION";       break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Action& action) {
    switch (action) {
        // --- Actions de la Phase Jeton (PJ_) ---
    case Action::PJ_PASSE:                  os << u8"---"; break;
    case Action::PJ_PIOCHE_GRATUITE:        os << u8"Pioche Gratuite"; break;
    case Action::PJ_REMBOURSEMENT_2:        os << u8"Remboursement 2"; break;
    case Action::PJ_REMBOURSEMENT_3:        os << u8"Remboursement 3"; break;
    case Action::PJ_TARIF_GENERAL:          os << u8"Tarif Général"; break;
    case Action::PJ_TARIF_CIBLE_G:          os << u8"Tarif Ciblé (G)"; break;
    case Action::PJ_TARIF_CIBLE_F:          os << u8"Tarif Ciblé (F)"; break;
    case Action::PJ_TARIF_CIBLE_D:          os << u8"Tarif Ciblé (D)"; break;
    case Action::PJ_EMBARGO:                os << u8"Embargo"; break;
    case Action::PJ_DEVALORISATION:         os << u8"Dévalorisation"; break;
    case Action::PJ_IMMUNITE:               os << u8"Immunité"; break;
    case Action::PJ_AUDIT_GENERAL:          os << u8"Audit Général"; break;
    case Action::PJ_AUDIT_CIBLE_G:          os << u8"Audit Ciblé (G)"; break;
    case Action::PJ_AUDIT_CIBLE_F:          os << u8"Audit Ciblé (F)"; break;
    case Action::PJ_AUDIT_CIBLE_D:          os << u8"Audit Ciblé (D)"; break;
    case Action::PJ_FRAUDE_MAJEURE:         os << u8"Fraude Majeure"; break;
    case Action::PJ_DETOURNEMENT:           os << u8"Détournement"; break;
    case Action::PJ_CUISINER_LIVRES:        os << u8"Cuisiner Livres"; break;
    case Action::PJ_EPUISEMENT_G:           os << u8"Épuisement (G)"; break;
    case Action::PJ_EPUISEMENT_F:           os << u8"Épuisement (F)"; break;
    case Action::PJ_EPUISEMENT_D:           os << u8"Épuisement (D)"; break;
    //case Action::PJ_TRANSACTION_G:          os << u8"Transaction (G)"; break;
    //case Action::PJ_TRANSACTION_F:          os << u8"Transaction (F)"; break;
    //case Action::PJ_TRANSACTION_D:          os << u8"Transaction (D)"; break;
    case Action::PJ_SABACC_PRIME:           os << u8"Sabacc Prime"; break;
    case Action::PJSP_D1:					os << u8"Sabacc Prime Dé 1"; break;
    case Action::PJSP_D2:					os << u8"Sabacc Prime Dé 2"; break;

        // --- Actions de la Phase Pioche (PP_) ---
    case Action::PP_PIOCHE_PAQUET_SANG:     os << u8"Pioche Paquet Sang"; break;
    case Action::PP_PIOCHE_PAQUET_SABLE:    os << u8"Pioche Paquet Sable"; break;
    case Action::PP_PIOCHE_DEFAUSSE_SANG:   os << u8"Pioche Défausse Sang"; break;
    case Action::PP_PIOCHE_DEFAUSSE_SABLE:  os << u8"Pioche Défausse Sable"; break;
    case Action::PP_PASSE:                  os << u8"Passe"; break;

        // --- Actions de la Phase Défausse (PD_) ---
    case Action::PD_DEFAUSSE_CARTE_PIOCHEE: os << u8"Défausse Carte Piochée"; break;
    case Action::PD_GARDE_CARTE_PIOCHEE:    os << u8"Garde Carte Piochée"; break;

        // --- Actions de la Phase RESOLUTION (PR_) ---
    case Action::PR_IMPOSTEUR_SANG_D1: os << u8"Imposteur Sang Dé 1"; break;
    case Action::PR_IMPOSTEUR_SANG_D2: os << u8"Imposteur Sang Dé 2"; break;
    case Action::PR_IMPOSTEUR_SABLE_D1: os << u8"Imposteur Sable Dé 1"; break;
    case Action::PR_IMPOSTEUR_SABLE_D2: os << u8"Imposteur Sable Dé 2"; break;

        // --- Cas par défaut (ne devrait pas arriu8ver si NB_ACTIONS est bien géré) ---
    default:                                os << u8"Inconnue (" << static_cast<int>(action) << ")"; break;
    }
    return os;
}