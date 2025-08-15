# Jeu de Sabacc Kessel par Grille-Pain Vert 
<img width="100" height="100" alt="Profil2_sym_100" src="https://github.com/user-attachments/assets/054adb38-98f2-4e5e-8cc3-88546b73944d" /> 

J'ai développé ce jeu au cours de sessions de streaming sur ma chaîne:
https://www.twitch.tv/grillepainvert
Aussi, ce code ne suit pas les bonnes pratiques (encapsulation manquante, texte de l'UI en dur dans le code) mais je pense qu'il reste lisible et peut être utile à des personnes qui souhaiteraient voir comment on code un jeu de ce genre.

Le Sabacc Kessel est une variante du jeu de cartes Sabacc introduite dans le jeu Star Wars Outlaws de Ubisoft.
J'ai choisi ce jeu pour développer une petite IA car il est simple, mais présente quelques choix stratégiques.
Le joueur humain tient la position Sud, tandis que les trois autres joueurs sont des PNJ.
Les PNJ ne trichent pas et n'ont aucun avantage.
Le score total est la somme des scores des 3 dernières parties.

<img width="1120" height="700" alt="Release Screenshot 2025 08 15 - 10 17 04 69" src="https://github.com/user-attachments/assets/c58d17b5-9186-4497-b59f-26e61a928733" />

## Etapes de développement
J'ai procédé en 3 étapes: d'abord le modèle que j'ai fait tourner en texte dans la console, puis j'ai rajouté une IA simple, puis l'interface graphique.

### Modèle
SabaccModel et Types contiennent toute la logique du jeu, ce qui permet de gérer l'état du jeu, lister et exécuter les actions possibles.
PNGAlea est un RNG très rapide.
Le modèle est conçu pour pouvoir à terme développer une IA à base de réseaux de neurones de type actor-critic: toutes les actions possibles sont dans une enum.
Le modèle utilise le moins possible de structures dynamiques (Vector...) pour rester rapide. 
Avec un seul thread sur mon processeur i5 12400 il peut simuler 600 000 parties aléatoires complètes par seconde.

### IA 
Eval contient une fonction d'évaluation simple de Monte-Carlo qui permet aux PNJ de choisir une action parmi les actions possibles.
La fonction actuelle est très naive mais fonctionne curieusement bien, donnant une IA par trop bête mais pas trop forte non plus.
Voici comment elle évalue une action a:
1) Prend l'état actuel du jeu et tire au hasard toute l'information cachée que le PNJ ne peut pas connaître 
2) Exécute l'action a
3) Déroule la partie au hasard jusqu'à ce que la fin de partie soit atteinte.
4) répète 1-3 un grand nombre de fois (150 000 parties simulées). Le score de l'action est le % de victoires obtenues par le PNJ lors de ces parties.

Cette évaluation très naive fonctionne assez bien sur le Sabacc Kessel car le facteur de branchement est faible, mais elle est incapable de détecter l'avantage stratégique de conserver ses jetons d'influence pour la fin de partie. 
Une IA actor-critic utilisant deux réseaux de neurones simples et potentiellement un auto-encodeur serait bien plus puissante.

### Interface Utilisateur
L'IU utilise le framework SFML.
SabaccUI est le controleur
SabaccView est la vue avec son propre thread pour gérer les événements

### Crédits
L'image de fond et le design des cartes proviennent du jeu Star Wars Outlaws (propriété Ubisoft)

Sons utilisés:

https://pixabay.com/sound-effects/axe-on-wedge-1-40390/

https://pixabay.com/sound-effects/boxing-bell-signals-6115/

https://pixabay.com/sound-effects/fast-woosh-230497/

https://pixabay.com/sound-effects/woosh-260275/

https://pixabay.com/sound-effects/mouse-click-346047/

https://pixabay.com/sound-effects/flipcard-91468/

https://pixabay.com/sound-effects/shuffleandcardflip1-40942/


# Sabacc de Kessel - Règles du Jeu
[Regles Sabacc.txt](https://github.com/user-attachments/files/21791406/Regles.Sabacc.txt)

## Matériel et Terminologie

### Cartes
- **FAMILLE DE CARTES** : Deux couleurs - SANG (rouge) et SABLE (jaune)
- Chaque FAMILLE contient 22 cartes (44 au total) :
  - 3 cartes de chaque valeur (1 à 6)
  - 3 cartes IMPOSTEUR (valeur déterminée par lancer de dés)
  - 1 carte SYLOPS (prend la valeur de l'autre carte en main)
- **MAIN DU JOUEUR** : 2 cartes (1 SANG + 1 SABLE)
- **VALEUR DE LA MAIN** : Différence absolue entre les valeurs des deux cartes
- **SABACC** : Main où les deux cartes ont la même valeur
- **SABACC PUR** : Une paire de SYLOPS

### Système d'Économie
- **JETON** : Monnaie de jeu
- **RÉSERVE** : Jetons que possède le joueur
- **POT** : Jetons dépensés pendant la MANCHE en cours

### Autres Éléments
- **PILE DE PIOCHE** : Cartes face cachée disponibles (une pile par FAMILLE)
- **PILE DE DÉFAUSSE** : Cartes face visible (une pile par FAMILLE)
- **JETON D'INFLUENCE** : Capacités spéciales à usage unique (3 par joueur au début)

## Structure du Jeu
- Le jeu se compose d'une succession de **MANCHES**
- Chaque **MANCHE** comprend exactement **3 TOURS** suivis d'une **RÉSOLUTION**
- Le jeu continue jusqu'à ce qu'il ne reste qu'un seul joueur avec une RÉSERVE > 0

## Déroulement du Jeu

### Préparation
1. **MISE EN PLACE DE LA PARTIE**
   - Chaque joueur reçoit 8 JETONS dans sa RÉSERVE
   - Premier joueur désigné au hasard
   - Chaque joueur reçoit les mêmes 3 JETONS D'INFLUENCE qui ont été tirés au sort en début de partie
   
2. **MISE EN PLACE DE LA MANCHE**
   - Mélangez séparément chaque FAMILLE de cartes
   - Distribuez 1 carte de chaque FAMILLE à chaque joueur
   - Placez 1 carte face visible de chaque FAMILLE sur les PILES DE DÉFAUSSE
   - Premier joueur : aléatoire (1ère manche), puis suivant dans le sens horaire

### Tour de Jeu
À son tour, chaque joueur (dans le sens horaire) :
1. Peut jouer un JETON D'INFLUENCE (optionnel)
2. Doit choisir entre :
   - **PASSER** : ne rien faire
   - **PIOCHER** (si au moins 1 JETON en RÉSERVE) :
     - Placer 1 JETON de sa RÉSERVE dans son POT
     - Piocher une carte d'une des PILES
     - Accepter (remplace la carte de même FAMILLE) ou refuser (défausser)

### Phase de RÉSOLUTION (après 3 TOURS)
1. Révélation des MAINS
2. Résolution des cartes spéciales (dans cet ordre) :
   - IMPOSTEUR (SANG puis SABLE) → lancer 2D6, choisir une valeur
   - Paire de SYLOPS (SANG+SABLE) → valeur 0 pour les deux
   - SYLOPS (SANG puis SABLE) → prend valeur de l'autre carte
3. Classement des joueurs :
   - Par différence croissante entre les 2 cartes de la MAIN.
   - Si égalité : par SOMME croissante des valeurs des cartes de la MAIN
4. Conséquences :
   - 1er rang : récupère tous les JETONS de son POT
   - Autres rangs avec DIFFERENCE = 0 : perd 1 JETON (RÉSERVE → POT)
   - Autres rangs avec DIFFERENCE > 0 : perd PÉNALITÉ JETONS (RÉSERVE → POT)
   - Tous les POT sont ensuite vidés
5. Élimination :
   - Joueurs avec RÉSERVE ≤ 0 sont éliminés
   - Une nouvelle MANCHE commence avec les joueurs restants

## Condition de Victoire
- Le dernier joueur avec une RÉSERVE > 0 est déclaré vainqueur
- Le jeu se poursuit en MANCHES successives jusqu'à ce qu'il n'y ait plus qu'un seul joueur

## JETONS D'INFLUENCE (capacités spéciales)

| Nom | Effet |
|-----|-------|
| **Pioche Gratuite** | Piochez sans payer de JETON |
| **Remboursement** | Récupérez jusqu'à 2 JETONS du POT vers votre RÉSERVE |
| **Remboursement Supplémentaire** | Récupérez jusqu'à 3 JETONS du POT vers votre RÉSERVE |
| **Tarif Général** | Tous les adversaires placent 1 JETON dans leur POT |
| **Tarif Ciblé** | Un joueur ciblé place 2 JETONS dans son POT |
| **Embargo** | Le joueur suivant doit PASSER |
| **Dévalorisation** | Valeur SYLOPS = 0 jusqu'à la prochaine RÉSOLUTION |
| **Immunité** | Protection contre les JETONS DE CHANGEMENT jusqu'à la prochaine RÉSOLUTION |
| **Audit Général** | Joueurs ayant PASSE placent 2 JETONS dans leur POT |
| **Audit Ciblé** | Un joueur ayant PASSE place 3 JETONS dans son POT |
| **Fraude Majeure** | Valeur IMPOSTEUR = 6 jusqu'à la prochaine RÉSOLUTION |
| **Détournement** | Prenez 1 JETON du POT de chaque joueur dans votre POT|
| **Cuisiner les Livres** | Inverse l'ordre des SABACC à la prochaine RÉSOLUTION |
| **Épuisement** | Un joueur défausse et pioche une nouvelle MAIN |
| **Transaction Directe** | Échangez votre MAIN avec un autre joueur |
| **Sabacc Prime** | Lance les 2 dés, la valeur choisie devient le meilleur des SABACC.
