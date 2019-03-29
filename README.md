https://bit.ly/2SEGcSN
# horlogeImpulsion
Contrôleur d'horloge fille à impulsion, en subsitution à une horloge mère. Permet, selon les configurations, le pilotage par une boitier autonome alimenté par pile, batterie, ou port USB.
Il est conçu autour:
- d'un programme dont les options de compilation doivent être adaptées au besoin,
- et d'un circuit imprimé unique sur lequel l'implémentation des composant varie en fonction des besoins.

Convient (testé) pour:
- Brillié 'rosace 10 ohms' à impulsions électriques alternées (2 impulsions par minute) sur 2 piles AA, soit en pas simples toutes les 30secondes, soit en double pas toutes les minutes.
- Bodet '24V' à impulsions électriques alternées (1 impulsion par minute) sur alimentation USB.
- IBM '24V' à impulsions électriques non-alternées sur alimentation USB. 

Devrait convenir pour:
- Tout type d'horloge fille à impulsions alternée ou non, nécessitant une tension entre 2 et 36V, sur tout type d'alimentation entre 2 et 5V.

## Description :
Le controleur se base sur:
- une base de temps électronique (RTC : real time clock).
- une alimentation - n’importe quelle source délivrant entre 3V et 5V continu.
- un microcontroleur ATTinyx5 (25, 45 ou 85; testé uniquement sur Attiny85)

## Composants :
1.	Platine
2.	Microcontrôleur ATTINYx5-20PU (testé sur ATTINY85-20PU)
3.	Module RTC (la pile est facultative et ne sert pas) - testé sur un DS3231
4.	Boitier de pile ou alimentation microUSB
5.	Bouton poussoir de réglage
Selon les versions/besoin:
6.	Interrupteur ON/OFF 
7.	Pont en H et bornier pour relier la platine à l’horloge (avec sa résistance de 10 ohms)
8. transistor et bornier de sortie directe
9. bornier de sortie pour contrôle d'un pont H extérieur
10. Module convertisseur (élévateur) de tension vers la tension de fonctionnement de l'horloge (généralement 18V ou 24V)

## Brancher la platine à l’horloge :
### Brillié (10Ohms) 
-  Eteindre le contrôleur (interrupteur ON/OFF) ou bien débrancher l'alimentation
-  Relier bornes de sortie de la platine  avec les bornes d’entrée de l’horloge (2 fils). Laisser la résistance de 10ohm qui relie les deux bornes à vis .
-  Fixer l’ensemble des éléments au dos de l’horloge en assurant que l’on puisse manipuler : 
   -  les 2 interrupteurs (ON/OFF et le bouton poussoir AVANCE),
   -  le bouton,
   -  le boitier de piles.
-  Rallumer le contrôleur (interrupteur ON/OFF) ou bien brancher l'alimentation
-  Appuyer plusieurs secondes sur le bouton AVANCE, pour faire avancer l’aiguille des minutes de quelques minutes.
-  Quelques après avoir relâché le bouton AVANCE, regarder la position de l’aiguille des minutes :
   -  Si elle est alignée sur un point du cadran indiquant les minute, passer à l’étape de mise à l’heure.
   -  Sinon, elle se trouve au milieu entre deux points de minutes. Recommencer cette étape de branchement en inversant les deux fils qui relient la platine à l’horloge.

### Horloges à impulsions alternées
- Si la tension requise est supérieure à la tension d'alimentation, relier le pont en H externe (4 fils: VP, GND, S1 et S2) et brancher l'horloge comme une Brillié (10ohms) depuis le pont en H.
- Sinon (pas d'élévation de tension), procéder comme pour les Brillié (10ohms)

### Horloges à impulsions monopôlaires (pas d'inversion de polarité)
Ces horloges peuvent avoir 2 ou 3 fils. La platine peut elle-même comporter un bornier double ou triple. Toutes les combinaisons sont possibles. Attention: ne pas considérer ni connecter le fil de masse de l'horloge.
- S'il y a deux fils: en connecter un sur le connecteur marqué "rouge" (vers le coin de la platine) et l'autre fil dans l'autre bornier (n'importe lequel s'il y en a deux)
- S'il a 3 fils: le fil rouge doit être connecté à la platine sur le connecteur marqué "rouge" (vers le coin de la platine), et les deux autres soit ensemble soit séparemment dans le ou les les autres connecteurs du borniers.
- Lorsqu'on appuie sur le bouton poussoir AVANCE, vérifier que l'aiguille avance bien d'une minute.

## Mettre à l’heure l’horloge
-	Eteindre le contrôleur (interrupteur ON/OFF)
-	Approcher de l’heure réelle à la main, sans la dépasser (environ cinq à dix minutes avant l’heure réelle), en agissant directement sur le mécanisme. Pour cela agir directement sur le mécanisme d'entrainement (dépend du mécanisme - en général on peut l'avance et le reculer avec les deux mains ou tournevis), ou bien allumer le controleur et appuyer sur le bouton AVANCE sans le relacher (la vitesse d'avancement augmente lorsqu'on reste appuyé, mais cette opération peut être un peu fastidieuse).
-  Allumer le contrôleur (interrupteur ON/OFF)
-	En appuyant sur le bouton AVANCE, faire avancer jusqu’à afficher l’heure actuelle. A ce stade, l’horloge a de 0 à 59 secondes de retard.
-	Surveiller le nombre de secondes de l’heure réelle. Entre 55s et 59s, appuyer sur le bouton AVANCE. L’aiguille des minutes passe à la minute suivante: relacher immédiatement.

Note : 
-	 Ce réglage s’effectue de la même façon quel que soit le mode choisi (mode 60 : minute entière, ou mode 30 : demi minute), et se fait sur les minutes entières uniquement.
-	Compte tenu du temps variable de prise en compte de l’appui sur le bouton AVANCE, il est difficile d’obtenir une synchronisation meilleure que 2 secondes.

## Choix du mode de fonctionnement :
Identifier les caractéristiques de l'horloge:
- voltage
- nombre de fils
- type de 'moteur': 
  - électro-aimant et crabot sur roue crantée = impulsions non-alternées (les impulsion alternées fonctionneront aussi)
  - moteur type pendule et mécanisme d'échapement = impulsions alternées
  - moteur type Lavet (pas-à-pas 2 pas) = impulsions alternées
- période : temps minimal entre deux états stables de l'entrainement: 30 secondes ou 60 secondes. A tester à la main.

Attention: pour les horloges de période différentes de 30s ou 60s, il faudra modifier le programme (et pas seulement les options de compilation).

Pour les horloges dont la période est 30s:
- choisir si on souhaite un mouvement toutes les 30 secondes, ou un double mouvement toutes les 60s
- Ce choix n’a pas d’influence sur la précision de l’horloge. Le bruit produit par l’avancée de l’aiguille est différent selon le mode (un ‘clac’, ou bien un double ‘clac’ mais deux fois moins fréquent).

## Préparation de la platine
Voir le BOM.txt

## Programmation du microcontroleur :
- Le code est disponible à l’adresse suivante https://github.com/pinderschloss/horlogeImpulsion/
- Platine et code sont prévus pour fonctionner avec les microcontrôleurs ATTINYx5-20PU (x5 : 25 ou 45 ou 85) d’ATMEL. La consommation d'énergie est réduite autant que possible par une utilisation du mode sommeil autant de possible.
- La programmation du microcontrôleur peut se faire par une carte arduino par exemple (chercher Arduino ISP).
- Attention le microcontroleur ne peut pas être programmé en place sur le circuit imprimé.
- Le code est adaptable pour fonctionner avec des horloges nécessitant des signaux différents, en commentant ou décommentant les options de compilation.
