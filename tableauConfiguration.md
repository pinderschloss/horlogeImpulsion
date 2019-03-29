# Configuration matérielle et logicielle

Cette partie détaille les différences entre les configurations matérielles pour la platine. La partie commune à toutes les platines est décrite dans BOM.txt. Ce fichier comprend aussi les différents éléments spécifiques listés ci-dessous, mais sans explications.

## Tensions:
Alimentation du module: **Vc** (doit être comprise entre 3V et 5V)
Tension de fonctionnement de l'horloge: **VP** (doit être comprise entre Vc et 36V)

- Si **VP** = **Vc** : pas de module élévateur de tension
- Si **Vc** < **VP** :  prévoir module élévateur de tension *x-tension*

## Connection de l'alimentation:
Si on utilise pour une alimentation qui se termine par deux fils (par exemple un boitier à piles), il faut installer un bornier d'alimentation:
- En général utiliser un bornier double (**GND** / **Vc**),
- Dans le cas d'une horloge à impulsions alternées avec **VP** > **Vc**, prévoir un bornier triple.

Si on utilise une alimentation avec un connecteur microUSB mâle et le module élévateur de tension *x-tension*, il n'y a rien à prévoir.

Pour tous les autres cas, prévoir un raccordement spécifique de l'alimentation à **GND** / **VC**.

## Commande de puissance
Pour les horloges à impulsions monopolaires, on peut au choix utiliser l'une des 3 solutions suivantes:
- Un transistor *T1*, une résistance de courant de base *R1*, et une diode de roue libre *DRL1*,
- Un pont en H :
   - Si **VP** < 15V : *pontH* soudé à la platine
   - Si **VP** < 36V : un pont en H externe (convient aussi si **VP** < 15V)
   
Pour les horloges à impulsions alternées, on doit utiliser un pont en H:
   - Si **VP** < 15V : *pontH* soudé à la platine
   - Si **VP** < 36V : un pont en H externe (convient aussi si **VP** < 15V)

## Connection à l'horloge
Si on utilise transistor *T1*: prévoir un bornier *monopolaire* double (pas 2.54mm ou bien pas 5.08 mm) ou triple (pas 2.54mm)
Si on utilise un pont en H : utiliser les connecteurs borniers du pont.
Pour les horloges Brillié "10ohms" prévoir aussi une résistance de 10 ohms.

## Economie d'énergie
Si:
- on souhaite une alimentation autonome (pile, batterie) et non pas alimenté sur le secteur,
- et on a besoin de l'élévateur de tension, 
On peut avoir plus d'autonomie en économisant l'énergie dissipée par l'élévateur de tension au repos.
Pour cela, prévoir:
- un shunt (j'utilise une résistance d'une centaine d'ohm, un fil conviendra)
- pour les horloges à impulsions alternées: deux diodes *DS1* et *DS2*.
- pour les horloges à impulsions monopolaires: un pont de soudure sur le cavalier dans l'empreinte DS1 ou un shunt à la place de la diode *DS1*.

## Interrupteur de commande
On peut installer l'*interrupteur* pour la partie commande (**Vc**). C'est un interrupteur facultatif. S'il n'est pas installé, faire un pont de soudure sur *JP2*.

## Bouton poussoir de réglage
Si on n'utilise pas le bouton poussoir *Reglage* prévu (soudé sur la platine par 4 points):
- ne pas l'installer
- utilise un bouton poussoir déporté et le raccorder à la platine sur un des deux points qui sont le long du bord (**SQW**), et un des deux points qui sont près de *DRL1* (**GND**).
