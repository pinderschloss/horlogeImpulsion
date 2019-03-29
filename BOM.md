Le projet a été développé avec le matériel suivant (commum à toutes les versions) :
-  1x ATTINY85-20PU (version DIP8, ATTINY25 et ATTINY45 conviennent aussi)
-  *RTC*: 1x module assemblé DS3231 (Real Time Clock de Maxim Integrated) :
   - Un autre module RTC avec alarmes (**SQW**) devrait convenir.
   - La diode DEL a été supprimée pour économiser l'énergie.
   - Le support de pile a été supprimé pour économiser la place.
- 1x circuit imprimé dédié:
   - Il est recommandé d'utiliser la dernière version.
   - Ne pas utiliser les versions antérieures à la V5.

Suivant les versions (se référer à pour les explications):
-  *C* : 1x condensateur radial, diamètre 5mm (facultatif, valeur peu importante). Peut-être remplacé par un autre format.
-  *monopolaire* : 1x bornier pas 2.54mm avec 2 ou 3 bornes, ou bien un bornier pas 5.08mm à 2 bornes
-  *VPGNDVc* : 1x bornier pas 2.54mm avec 2 ou 3 bornes
-  *pontH* 1x pont en H sur une breakout board. Ce pont est basé sur un L9110S, limité à 15V.
   - Tout autre pont en H doit convenir, notamment externe.
   - La diode DEL a été supprimée pour économiser les piles.
   - Le bornier inutile a été supprimé pour éviter des confusions au moment du montage
-  *Reglage*: 1x interrupteur momentané (à poussoir)
-  *interrupteur* : 1x interrupteur à glissière (bistable)
-  *x-tension* : 1x module élévateur de tension
-  *R1* : 1x résistance d'environ 1200 ohms
-  *T1* : 1x transistor NPN en package TO-90, par exemple BD259. Tout autre NPN devrait convenir s'il supporte la tension **VP**, et que ses pattes peuvent être soudées à la bonne place.
-  *DS1* et *DS2* : 2x diodes standard type 1N4001
-  *DRL1* : 1x diodes standard type 1N4001
-  1x support de pile 2xAA
-  1x alimentation microUSB
