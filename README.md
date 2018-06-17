# RubiksProject
pour installer opengl : 
sudo apt-get freeglut3-dev
important que se soit la version dev, aucune idée pourquoi (-dev est censé etre la bêta d'après se que j'ai compris)

Pour compiler : 
    make
tout les -lxxx sont des linkers files qui permettent d'importer les dependances des librairies importées

main in cube.c

ATTENTION, la resolution marche presque. 
./cube pour lancer le programme

U,F,D,B,R,L sur le clavier pour faire bouger les faces du cube
6 pour scramble le cube
5 pour le resoudre
1 pour le reset

A ENCORE BESOIN D'AJUSTEMENTS
Je met tout les cubes à la bonne position mais pas dans la bonne orientation. Je n'avais pas prévu ca au début et je n'ai rien dans mes structs pour connaitre l'orientation du cube. 
J'ai donc beaucoup de chose à changer et je n'ai pas encore l'occasion de changer tout ca
