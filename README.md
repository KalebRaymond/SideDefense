# TowerDefense

A 2D sidescrolling tower defense game built with SDL and Tiled Map Editor. Builds off of [Limeoats's Cavestory Remake](https://github.com/Limeoats/cavestory-development).

bugs & changes:
	-Clicking towerMenuItem multiple times gives you multiple copies of a tower  
	-Towers fire if enemy is to right of tower (makes sense for rockets...)  
	-Fix memory leak. As of now, all instances of "new" that are never deleted point to objects that exist in _enemies, _towers, or _projectiles. Need to free these between each level in game::runGame()  
  
If you are unable to place a tower after using WASD to move the screen, it's probably due to keyboard ghosting.
