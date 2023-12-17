*******************
# Compte rendu 
*******************
## Présentation du projet
Ce projet réalisé dans le cadre de notre cours d'arduino en PEIP2 à Polytech Nice-Sophia (prépa intégré) consiste à concevoir un robot aspirateur permettant de cartographier une pièce et de nettoyer ladite pièce de facon autonome. Nous allons pour cela utiliser une carte arduino Uno.

## Séance du 14 Décembre 2023 (et tout travail en amont )
- Création du projet sur Github
- Mise en place du projet
- Négociation avec le professeur pour obtenir le materiel (2 moteurs, 3 sonars, 1 blower fan de pc (moteur brushless), adaptateur carte sd arduino).
- Création d'une maquette du projet avec le binôme ([photo ici](/images/20231214_115155.heic)). Volume interieur: 30cm de diametre par 10cm de hauteur.
- Disposition des composants déjà en notre possession dans la maquête: tout rentre, à voir quand il faudra mettre les brosses au milieu, et la carte arduino, et les batteries, et certainement plus...
- Commande au professeur d'un [kit de maintenance iRobot](https://amzn.eu/d/cpejmey) pour obtenir un filtre, les brosses à rouleau et une brosse latérale.
- Premier branchement à blanc de tout les composants sur une breadboard (voir photo ci-dessous).\
Je voulais deja voir si une UNO avait assez de pin pour notre cas de figure, et on est déjà serré: coté digital i/o il me reste 4 pin PWM, où je comptais brancher les moteurs, sauf que j'ai besoin encore d'un pin PWM pour regler la vitesse du ventilo (ou a minima qu'il tourne). On est donc déjà en négatif, ce sera un probleme pour la semaine prochaine.\
*Ne pas regarder les numéro des pin sur le brouillon, ils ont déjà été changés. Une version maintenue à jour est dans le fichier [pin](/code/pin.hpp).*

![](/images/20231214_124527.heic "uno+bread=♥")

## Séance du 21 Décembre 2023
- Correction sur ce que j'ai dis sur les pin la semaine derniere: pas besoin de 4pin PWM, seulement 2 (pour les enable des moteurs, les 2 autres sont pour le sens de rotation et donc pas besoin de pwm (double inverseur nécessaire pour economiser 2pin supplémentaire, nouveau montage electrique [ici](/images/montage_electrique_v1.png))).
- Le pin n°9 sert a controller la vitesse du ventilateur de pc, mais necessite une frequence PWM de 25kHz. La carte delivre actuellement 500Hz...
- Merci [Edgar Bonet sur StackExchange](https://arduino.stackexchange.com/a/25623) pour le code servant a débiter 25kHz sur le pin 9. Le ventilo va certainement toujours tourner a fond mais ça peut toujours servir pour augmenter l'autonomie ou diminuer les décibels.
