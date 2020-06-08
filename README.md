# TowerDefense

A 2D sidescrolling tower defense game built with SDL and Tiled Map Editor. Builds off of [Limeoats's Cavestory Remake](https://github.com/Limeoats/cavestory-development).

Minor bugs/changes:  
- Bullets are destroyed when they collide with enemy hitbox, instead of the center of enemy hitbox  
- SDL initializes a second blank window behind the main one  
- Can't place tower after moving screen without moving mouse first
- globals::SCREEN_WIDTH used in places where globals::GAME_VIEWPORT_W is more appropriate  
- Phase out Vector2  
- Level::update is very inefficient - bad runtime, unecessary vector accesses, bad memory management (RAII)
