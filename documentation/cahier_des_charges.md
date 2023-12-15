# Robot aspirateur
## Description du projet
L’idée est de développer un robot aspirateur ménager autonome capable d’aspirer la poussière et autres petits déchets. Le robot sera capable de modéliser la pièce dans laquelle il fait le ménage, et devra pouvoir couvrir toute la surface du sol de celle-ci. Le robot pourra s’arrêter s’il rencontre un obstacle, et de changer de direction pour l’éviter. Enfin, le robot devra avoir une contenance nécessaire pour stocker les déchets sur la durée d’un ménage (voir plusieurs). 
## Cahier des Charges Fonctionnel
**Objectifs principaux à remplir**|**détails des tâches à remplir **|**contraintes techniques associées / solutions envisages**
|:-----:|:-----:|:-----:|
|1. Le concept du robot|Créer un robot aspirateur ménager autonome| 
| |Capable d’aspirer la poussière et autres petits déchets. | 
| | Le robot doit passer sur toute la surface de la pièce | 
|2. Contenance | Capacité de stocker des déchets pour au moins la durée d’un ménage (voir plus) |prévoir un bac d’une capacité suffisante ( volume envisagé 1L) 
| | | prévoir un bac démontable facilement démontable par l’utilisateur 
| | |Le conteneur devra s’insérer dans le robot de la manière la plus optimisée possible 
|3. Capture des déchets|Le robot doit être muni d’un système pour capturer la poussière et les déchets |Système technique envisagé: 
| | | double brosse rotative repoussant les déchets dans le bac de rétention.
| | |- ajouter des brosses dirigeant les déchets vers les 2 brosses principales
|4. Direction du robot |Le robot doit être capable d'aller tout droit, et de tourner sur lui-même|2 roues motrices + roue folle
| |S’il rencontre un obstacle, le roomba doit s’arrêter avant |Prévoir l’ajout de capteur(s) de distance pour les détecter les obstacles 
| |Changer de direction en cas de rencontre d’un obstacle imprévu  |
|5. Modélisation de la pièce| Le roomba doit enregistrer la disposition de la pièce. |Prévoir comment stocker la pièce de façon permanente (carte sd ?)
|6. Base | le robot possède une base fixe sur laquelle il peut se recharger| Le robot sera muni d’une batterie rechargeable 
| | | Prévoir un orifice pour pouvoir connecter fil de rechargement à la batterie. 
| |point de départ de la routine de nettoyage|retour à la base en cas de batterie faible ou fin de routine
