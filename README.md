# Snake Multi-Joueur avec QT
Ce projet detaille une implémentation en C++ avec Qt du jeu Snake classique, incluant un mode deux joueurs et des fruits spéciaux.

## Structure du projet
- ```main```: initialiser l'application, lance l'exécution (```app.exec()```)
- ```snakeWindow```: Fenêtre principale, gère le rendu (QPainter) et les boutons.
- ```jeu```: Moteur du jeu, gère la logique, les collisions et les scores
- ```fruit```: Système de fruits ; définit les effets spéciaux (Pomme, Banane, Fraise).
- ```position```: Classe utilitaire pour les coordonnées $(x, y)$ et les directions.

## Installation et Lancement

**1. Prérequis:** Qt Small 5.15.2 et MinGW installés et configurés dans votre variable d'environnement PATH.

**2. Compilation:** Ouvrez un terminal dans le dossier du projet et lancez :
```mingw32-make.exe```

**3. Exécution:** Lancez le fichier .exe généré. ```.\snake.exe```

**4. Contrôles:** Utilisez les touches fléchées pour diriger le serpent 1, et les touches zqsd pour le serpent 2. Interagissez avec les boutons à l'écran pour modifier la difficulté du terrain. Les bananes inversent la direction du serpent et les fraises donnent le pouvoir de manger les murs.