# TowerDefense

A 2D sidescrolling tower defense game built with SDL and Tiled Map Editor. Builds off of [Limeoats's Cavestory Remake](https://github.com/Limeoats/cavestory-development).

bugs & changes:  
	- Clicking towerMenuItem multiple times gives you multiple copies of a tower  
	- Towers fire if enemy is to right of tower (makes sense for rockets...)
	- A lot of code can be deleted (BasicEnemy::draw and ToughEnemy::draw do the same thing)
  
If you are unable to place a tower after using WASD to move the screen, it's probably due to keyboard ghosting.
