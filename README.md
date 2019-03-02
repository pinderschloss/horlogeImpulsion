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
- une alimentation peut être assurée par n’importe quelle source délivrant entre 3.3V et 5V.
- un microcontroleur ATTinyx5 (25, 45 ou 85: test uniquement sur Attiny85)

## Composants :
1.	Platine
2.	Microcontrôleur ATTINYx5-20PU
3.	Module RTC (la pile est facultative et ne sert pas)
4.	Boitier de pile ou alimentation microUSB
5.	Bouton poussoir de réglage
Selon les versions/besoin:
6.	Interrupteur ON/OFF 
7.	Pont en H et bornier pour relier la platine à l’horloge (avec sa résistance de 10 ohms)
8. transistor et bornier de sortie directe
9. bornier de sortie pour contrôle d'un pont H extérieur

## Brancher la platine à l’horloge :
-  Eteindre le contrôleur (interrupteur ON/OFF) ou bien débrancher l'alimentation
-  Relier par deux fils les bornes de sortie de la platine (bornier à vis) avec les bornes d’entrée de l’horloge. Conserver la résistance de 10ohm qui relie les deux bornes à vis.
-  Fixer l’ensemble des éléments au dos de l’horloge en assurant que l’on puisse manipuler : 
   -  les 2 interrupteurs,
   -  le bouton,
   -  le boitier de piles.
-  Rallumer le contrôleur (interrupteur ON/OFF) ou bien brancher l'alimentation
-  Appuyer plusieurs secondes sur le bouton AVANCE, faire avancer l’aiguille des minutes de quelques minutes.
-  Entre 3 et 30 secondes après avoir relâché le bouton AVANCE, regarder la position de l’aiguille des minutes :
   -  Si elle est alignée sur un point de minute, passer à l’étape de mise à l’heure.
   -  Sinon, elle se trouve au milieu entre deux point de minutes. Recommencer cette étape de branchement en inversant les deux fils qui relient la platine à l’horloge.

## Mettre à l’heure l’horloge
-	Eteindre le contrôleur (interrupteur ON/OFF)
-	Approcher de l’heure réelle à la main, sans la dépasser (environ cinq minutes avant l’heure réelle), en agissant directement sur le mécanisme. Retourner l’horloge, puis:
   -  Avancer directement l’entrainement (pour faire tourner l’aiguille des minutes dans le sens des aiguilles d’une montre)
   -  Reculer l’entrainement. Pour cela, il faut lever le taquet anti retour avant de manipuler la roue
   -  Rallumer le contrôleur (interrupteur ON/OFF)
-	En appuyant sur le bouton AVANCE, faire avancer jusqu’à afficher l’heure actuelle. A ce stade, l’horloge a de 0 à 59 secondes de retard.
-	Surveiller le nombre de secondes de l’heure réelle. Entre 55s et 59s, appuyer sur le bouton AVANCE jusqu’à ce que l’aiguille des minutes passe à la minute suivante, puis relacher immédiatement.

Note : 
-	ce réglage s’effectue de la même façon quel que soit le mode choisi (mode 60 : minute entière, ou mode 30 : demi minute), et se fait sur les minutes entières uniquement.
-	Compte tenu du temps variable de prise en compte de l’appui sur le bouton AVANCE, il est difficile d’obtenir une synchronisation meilleure que 2 secondes.

## Choix du mode de fonctionnement :
Identifier les besoins de l'horloge:
- voltage
- nombre de fils
- type de 'moteur': 
  - électro-aimant sur roue crantée = impulsions non-alternées (les impulsion alternées fonctionneront aussi)
  - moteur type pendule et mécanisme d'échapement = impulsions alternées
  - moteur type Lavet (pas-à-pas 2 pas) = impulsions alternées
- période : temps minimal entre deux états stables de l'entrainement: 30 secondes ou 60 secondes.

Attention: pour les horloges de période différentes de 30s ou 60s, il faudra modifier le programme (et pas seulement les options de compilation).

Pour les horloges dont la période est 30s:
- choisir si on souhaite un mouvement toutes les 30 secondes, ou un double mouvement toutes les 60s
- Ce choix n’a pas d’influence sur la précision de l’horloge. Le bruit produit par l’avancée de l’aiguille est différent selon le mode (un ‘clac’, ou bien un double ‘clac’ mais deux fois moins fréquent).

## Programmation du microcontroleur :
- Le code est disponible à l’adresse suivante https://github.com/pinderschloss/horlogeImpulsion/
- Platine et code sont prévus pour fonctionner avec les microcontrôleurs ATTINYx5-20PU (x5 : 25 ou 45 ou 85) d’ATMEL. La consommation d'énergie est réduite autant que possible par une utilisation du mode sommeil autant de possible.
- La programmation du microcontrôleur peut se faire par une carte arduino par exemple (chercher Arduino ISP).
- Attention le microcontroleur ne peut pas être programmé en place sur le circuit imprimé.
- Le code est adaptable pour fonctionner avec des horloges nécessitant des signaux différents, en commentant ou décommentant les options de compilation.
