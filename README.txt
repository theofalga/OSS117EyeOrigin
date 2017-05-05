Ceci est un rapide guide pour l'utilisation de EyeOrigin1.0

Une fois le projet décompressé, vous êtes libre de le placer où bon vous semble


Prérequis :
openCV3.2 (voir tuto : www.mon-club-elec.fr/mes_docs/docs_opencv/ma_doc_opencv_install_opencv_ubuntu.pdf).
Suivre le tutoriel mais installer la version 3.2 de openCV.

imageJ ((https://imagej.nih.gov/ij/download.html) / ij150.zip) extraire le fichier et ouvrir le fichier ImageJ/run et remplacer 512 par 2048 (pour augmenter l'allocation de mémoire d'imageJ).


Protocol :
Ce dossier contient un programme permettant de créer une collection d'images interpretable par la suite (voir plus d'informations dans protocole-oss.pdf).
Au préalable veuillez ouvrir l'image points_reperes_num.jpg en plein écran

Depuis un terminal placez vous dans le dossier Protocol
Exécuter la commande suivante : make
Enfin suivez les instructions une fois le programme lancé avec : $./protocol

La collection d'image sera enregistrée dans un nouveau sous-dossier du dossier images/ nommé par l'utilisateur en début de programme


Oss117Plugin :
Ce dossier contient un plugin java pour imageJ permettant d'enregistrer les coordonnées du centre des iris par évènement click souris (voir plus d'informations dans le ReadmePlugin.txt)

Placez-vous dans le dossier Oss117Plugin
Copiez le plugin Oss117plugin.java vers ~/.imagej/plugins : $sudo cp Oss117plugin.java ~/.imagej/plugins
Ouvrir imagej en se placant dans le dossier imagej et faire la commande $./run
Creer le stack dans imagej :
File -> import -> image sequence -> sélectioner n'importe quel image du dossier -> OK
Cliquer sur le centre de l'oeil gauche
Cliquer sur le centre de l'oeil droit
(Le changement d'image se fait automatiquement après le deuxième clic)
En cas d'erreur, finissez l'image ratée pour passer à la suivante, puis cliquez sur la roulette pour revenir à l'image ratée.

Le fichier positionReference.txt est enregistré dans le même dossier sélectioner lors de la création du stack.

EyeLike :
Ce dossier contient le programme open-source de Tristan Humme basé sur un algorithme de détection de l'iris conceptualisé par Fabian Timm. Il permet d'enregistrer dans un fichier les coordonnées des centres des iris de l'oeil gauche et de l'oeil droit

Placez-vous dans le dossier EyeLike
Exécuter la commande suivante : $make
Enfin suivez les instructions une fois le programme lancé avec : $./eyelike

Le fichier eyelike.txt est enregistré dans le dossier renseigné par l'utilisateur en début de programme


Interpretation :
Ce dossier contient un programme de comparaison des coordonnées des iris collectés précédemment

Placez-vous dans le dossier Interpretation
Exécuter la commande suivante : $make
Enfin suivez les instructions une fois le programme lancé avec : $./interpretation
