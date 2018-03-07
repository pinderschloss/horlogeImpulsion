# horlogeImpulsionATTINY
Contrôler une horloge Brillié à impulsions électriques alternées (2 impulsions par minute).

## Description :
Cette platine génère les impulsions électriques nécessaires au fonctionnement de certaines horloges ‘filles’ de la marque Brillié, lorsqu’on ne dispose pas d’une horloge mère. Elle se base sur une base de temps électronique (RTC : real time clock). L’alimentation peut être assurée par n’importe quelle source délivrant entre 3.3V et 5V.

La platine génère au choix (voir mode de fonctionnement):
-	Une impulsion toutes les 30 secondes (alternativement ‘positive’ puis ‘négative’),
-	Ou bien une paire d’impulsion (‘positive’ puis ‘négative’) toutes les minutes.

## Composants :
1.	Platine
2.	Microcontrôleur ATTINYx5-20PU
3.	Module RTC (la pile est facultative et ne sert pas)
4.	Boitier de pile
5.	Interrupteur ON/OFF
6.	Bouton AVANCE
7.	Interrupteur MODE3060
8.	Point alimentation (+)
9.	Point alimentation (-)
10.	Pont en H et bornier pour relier la platine à l’horloge (avec sa résistance de 10 ohms)

## Brancher la platine à l’horloge :
-  Eteindre le contrôleur (interrupteur ON/OFF)
-  Relier par deux fils les bornes de sortie de la platine (bornier à vis) avec les bornes d’entrée de l’horloge. Conserver la résistance de 10ohm qui relie les deux bornes à vis.
-  Fixer l’ensemble des éléments au dos de l’horloge en assurant que l’on puisse manipuler : 
   -  les 2 interrupteurs,
   -  le bouton,
   -  le boitier de piles.
-  Rallumer le contrôleur (interrupteur ON/OFF)
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
-	Pour faire avancer l’horloge toutes les 30s (une impulsion toutes les 30s), mettre l’interrupteur MODE3060 en position 30. 
-	Pour faire avancer l’horloge toutes les 60s (une double impulsion toutes les 60s) mettre l’interrupteur MODE3060 en position 60. L’aiguille des minutes reste ainsi toujours alignée avec les points des minutes sur le cadran.

Note :
-	On peut permuter d’un mode à l’autre sans limite, à n’importe quel moment (pas d’impact sur le réglage).
-	Le choix n’a pas d’influence sur la précision de l’horloge. Le bruit produit par l’avancée de l’aiguille est différent selon le mode (un ‘clac’, ou bien un double ‘clac’ mais deux fois moins fréquent).

## Programmation du microcontroleur :
-	Le code est disponible à l’adresse suivante https://github.com/pinderschloss/horlogeImpulsion/
-	Platine et code sont prévus pour fonctionner avec les microcontrôleurs ATTINYx5-20PU (x5 : 25 ou 45 ou 85) d’ATMEL. La consommation d'énergie est réduite autant que possible.
-	La programmation du microcontrôleur peut se faire par une carte arduino par exemple (chercher Arduino ISP).
-	Le code est adaptable pour fonctionner avec des horloges nécessitant des signaux différents (par exemple une impulsion toutes les minutes, polarité inversée entre deux impusions).
