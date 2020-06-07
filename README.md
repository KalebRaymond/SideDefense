# TowerDefense

A 2D sidescrolling tower defense game built with SDL and Tiled Map Editor. Builds off of [Limeoats's Cavestory Remake](https://github.com/Limeoats/cavestory-development).

Minor bugs/changes:  
- Bullets are destroyed when they collide with enemy hitbox, instead of the center of enemy hitbox  
- SDL initializes a second blank window behind the main one  
- Bullet tower sometimes starts off following cursor when game starts
- globals::SCREEN_WIDTH used in places where globals::GAME_VIEWPORT_W is more appropriate  
- Phase out Vector2  
