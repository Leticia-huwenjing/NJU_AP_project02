# Ants vs. Bees

**@author Wenjing Hu**

## Introduction

This is the project02 in the ***Advanced Programming***, Spring 2022, NJU. It is a tower defense game like Plants vs. Zombies. The project is written based on Qt6.2.2 and most of the gui assets are from proj3 in SICP.

## Structure

1. **map**

   In this game, paths are divided into **flying** and **ground**, lattices are divided into **normal**, **missile** and **barriar**. Further, there are two kinds of normal lattices, **land** and **water**. You can construct your own map by referring to ` ./MapConfig/ConfigFormat`.Your exported map will be stored in` ./MapConfig/MapExport.txt`.

2. **main logic**

   All interaction is done in ` ./state.app`, the main **Update()** logic as below :

   ```c++
   addTower();
   generateEnemy();
   generateSun();
   
   for (auto& tower : Towers) {
     tower.action();
   }
   for (auto& enemy : Enemies) {
     enemy.action();
   }
   
   isGameOver();
   update();
   ```

   

3. **tower**

   - deploy on normal lattice : **Scuba**(waterResistant, can attack all), **Wall**(high blood volume, cannot attack), **Ninja**(can only attack enemy on ground), **Fire**(booooomb!).
   - deploy on missile lattice : **Harvester**(cannot attack but generate sun), **Thrower**(attack all), **Slow**(can only attack enemy on ground but can slow them).

   Besides, there is a special kind ant called BodyguardAnt. It can be deploy on any tower and protect them. If a lattice has a bodyguard, enemies will prefer to attack bodyguard. However, a lattice can only has one bodyguard.

4. **enemy**

   - flying path : **AggressiveFly**(stop if can attack), **ScaryFly**(cannot be stopped).
   - ground path : **Bee**(attack from a distance), **TankAnt**(only attack tower on ground).

5. **sun**

   The game generates a sun in an average of 8 seconds, and each sun is worth money 50. Where the sun falls is random. Harvester can also generate sun.

6. **QueenAnt**

   Similar to the lawnmower in Plants vs. Zombies.

## Demo

<video src="/Users/huwenjing/project02/CarrotFantasy/Demo.mp4"></video>