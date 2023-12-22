*******************
# Compte rendu 
*******************
## Présentation du projet
Ce projet réalisé dans le cadre de notre cours d'arduino en PEIP2 à Polytech Nice-Sophia (prépa intégré) consiste à concevoir un robot aspirateur permettant de cartographier une pièce et de nettoyer ladite pièce de facon autonome. Nous allons pour cela utiliser une carte arduino Uno.

## Séance du 14 Décembre 2023
### Détails du matériel nécessaire (~1h15)
Pendant la première partie de cette séance, j’ai travaillé conjointement avec Loïc pour
faire la liste de tout le matériel dont nous aurons besoin pour le projet.\
→Dans un premier temps, nous avons discuté du matériel que nous avions envisagé
provisoirement à la première séance.
Nous avons par exemple revu notre système de capteur: nous avons abandonné l’idée d’un
sonar unique, au profit de 3 capteurs de position sur les côtés du rumba. Nous sommes
revenus sur les besoins du robots dans sa manière de fonctionner ( système d’aspiration,
fonctionnement des brosses, le nombre de roues motrices...).\
→Après, s’être mis d’accord sur les composants nécessaires au projet, nous avons
commencé à sélectionner les pièces en fonction de leur caractéristiques et de leur
disponibilité:
-Les roues, le ventilateur d’aspiration, les moteurs, les capteurs de distance
proviennent des projets des années précédentes
- Choix d’une carte SD en plus de la carte arduino pour pouvoir réaliser du mapping
- Sélection des filtres, des brosses à acheter
- Choix de la batterie rechargeable pour le robot\
→extrait du document excel qui répertorie les composants sélectionnés:
![](/images/20231215_211909.png)

### Maquette du robot (~45min)
→Après, le choix du matériel, nous avons commencé à envisager la manière
d'agencer les différentes pièces.
En effet, nous avons dû nous adapter à certaines caractéristiques des pièces que
nous avons choisi: le système de ventilation est particulièrement volumineux et ne peut être
orienté que d’une certaine manière, le ventilateur ne doit pas s’encrasser ce qui nécessite la
création de 2 conduits qui ne communiquent pas...\
→Face à ces nombreuses problématiques, nous avons décidé de réaliser une maquette en
carton du roomba, afin de déterminer les dimensions minimales du roomba.

Photo de la maquette:
![](/images/20231215_211900.png)

Nous sommes partis sur la réalisation d’un roomba de forme circulaire: de rayon 15cm et
de hauteur 8 cm. A voir si nous modifierons le gabarit du roomba.

### Modélisation du robot sur OnShape (~1h)
→A partir de cette étape, nous avons commencé à travailler séparément, Loic a
commencé à coder le code de fonctionnement des moteurs des roues, à faire les calculs
pour déterminer la puissance de la batterie et du couple du moteur des brosses.\
→De mon côté, j’ai commencé à créer la maquette virtuelle du roomba sur le logiciel
OnShape

Avancement de la modélisation:
![](/images/20231215_211850.png)

Objectif pour la séance suivante: En l’état la modélisation du robot n’est pas assez
avancée, il faut donc au moins finir le corps du roomba (socle et corps) , afin de pouvoir
commencer l’impression 3D à la prochaine séance. A voir si on ne réalisera pas le robot à
la découpe laser.