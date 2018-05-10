# RubiksProject
pour installer opengl : 
sudo apt-get freeglut3-dev
important que se soit la version dev, aucune idée pourquoi (-dev est censé etre la bêta d'après se que j'ai compris


Pour compiler : 
gcc cube.c -o cube -lglut -lGLU -lGL
tout les -lxxx sont des linkers files qui permettent d'importer les dependances des librairies importées
