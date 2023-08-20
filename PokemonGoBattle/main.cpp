//  Project 3
//  PokemonGO Battle Simulator!
//  04/13/2021
//  Created by Dylan Binu.

#include <iostream> // including iostream for cout and endl.
#include <math.h> // including math library. Enables files to use floor function, srand, etc.
#include <array> //including the array library.
#include <time.h> //including the time library for the srand function.
#include "trainer.h" // including the trainer header class file.
#include "pokemon.h"// including the pokemon header class file
#include "fastMove.h"// including the fastMove header class file
#include "chargedMove.h"// including the chargedMove header class file


/* Most of the work has been done within the class files. I added my own fair share of contributions to the class files as well. */

using namespace std;

int main()
{

    srand(time(NULL)); //the srand function allows the usage of the rand function. It takes a null character as a parameter for the time function in order to return a random value based on the time at that moment, which is always changing. We do this in order to give our Pokemon varying IV Stats.
    
    //We are creating fast moves, charge moves, pokemon, and two trainers.
    //fastMove(std::string name, float ed, int bp,  int Type, float cooldown);
    //chargedMove(std::string Name, int bp, int ce, int Type);
    //pokemon(std::string Name, int type1, int type2, int atk, int def, int sta, int Level, fastMove Fast, chargedMove Charged);
    
    // Creating the fast move Scratch.
    fastMove scratch("Scratch", 2, 4, 1, .5, 1);
    
    // Creating the charge move Flame Burst.
    chargedMove flameBurst("Flame Burst", 70, 55, 2);
    
    // Creating a level 1 charmander with moves Scratch and Flame Burst.
    pokemon charmander("Charmander", 2, 116, 93, 118, 1, scratch, flameBurst);

    // Creating the fast move Fury Cutter.
    fastMove furyCutter("Fury Cutter", 4, 2, 14, .5, 1);
    
    // Creating the charge move Night Slash.
    chargedMove nightSlash("Night Slash", 50, 35, 11);
    
    //Creating a level 10 Scizor with moves Fury Cutter and Night Slash.
    pokemon scizor("Scizor", 14, 17, 236, 181, 172, 10, furyCutter, nightSlash);
    
    // Creating the fast move Quick Attack.
    fastMove quickAttack("Quick Attack", 7, 5, 1, 1.0, 1);
    
    // Creating the charge move Thunderbolt.
    chargedMove thunderbolt("Thunderbolt", 90, 55, 5);
    
    // Creating a level 4 Pikachu with moves Quick Attack and Thunderbolt.
    pokemon pikachu("Pikachu", 5, 112, 96, 111, 4, quickAttack, thunderbolt);
    
    // Creating the fast move Bullet Punch.
    fastMove bulletPunch("Bullet Punch", 7, 6, 17, 1.0, 1);
    
    // Creating the charge move Flash Cannon.
    chargedMove flashCannon("Flash Cannon", 110, 70, 17);
    
    //Creating a level 6 Metagross with moves Bullet Punch and Flash Cannon
    pokemon metagross("Metagross", 10, 17, 257, 228, 190, 6, bulletPunch, flashCannon);

    // Creating the fast move Tackle.
    fastMove tackle("Tackle", 2, 3, 1, .5, 1);
    
    // Creating the charge move Aqua Jet.
    chargedMove aquaJet("Aqua Jet", 45, 45, 3);
    
    // Creating a level 5 Squirtle with moves Tackle and Aqua Jet
    pokemon squirtle("Squirtle", 3, 94, 121, 127, 5, tackle, aquaJet);

    // Creating the fast move Thunderbolt.
    fastMove zenHeadbutt("Zen Headbutt", 8, 8, 10, 1.5, 1);
    // Deoxys's charge move is Thunderbolt as well, however since it has already been created, we don't need to create it again.
    
    // Creating a level 15 Deoxys with moves Zen Headbutt and Thunderbolt.
    pokemon deoxys("Deoxys (Normal Forme)", 10, 345, 115, 137, 15, zenHeadbutt, thunderbolt);
    
     // Creating a trainer named "Red" and adding the Pokemon Charmander, Pikachu, Squirtle, Scizor, Metagross, and Deoxys to his team.
    trainer red("Red", charmander, pikachu, squirtle, scizor, metagross, deoxys);
    
    // Creating a trainer named "Jellybean" and adding the Pokemon Deoxys, Scizor, Metagross, Pikachu, Squirtle, and Charmander to his team.
    trainer jelly("Jellybean", deoxys, scizor, metagross, pikachu, squirtle, charmander);

    // Since we have now created all our fast moves, charge moves, pokemon, and trainers, we can begin battle!
    
    // Calling the chooseBattleRoster function for both trainers to select 3 pokemon for their battle team from their  roster.
    red.chooseBattleRoster();
    jelly.chooseBattleRoster();

    // Print a message to suggest that the battle has begun.
    cout << endl;
    cout << "Red wants to battle Jellybean!" << endl;
    cout << endl;
    
    // Calls the chooseActive() function for both Red and Jellybean so that they can choose their active Pokemon from their battle roster of three Pokemon.
    red.chooseActive();
    jelly.chooseActive();
    cout << endl;
    
    // A while loop which reiterates itself as long as the notDead function for both trainers returns true.
    while (red.notDead() && jelly.notDead()) {
        
        // If wait time of either trainer falls below 0, this function sets it at 0.
        red.changeWaitTime(-1);
        jelly.changeWaitTime(-1);
        
        cout << endl;
        
        // If trainer Red has at least one pokemon conscious, call the turnAgainst function for Red to make a move against Jellybean.
        if (red.notDead()) red.turnAgainst(jelly);
        
        // If trainer Jellybean has at least one pokemon conscious, call the turnAgainst function for Jellybean to make a move against Red.
        if (jelly.notDead()) jelly.turnAgainst(red);
        }
    
    // If either trainer has lost all three battle pokemon, you will be brought out of the while loop.
        
    cout << endl;
           
    // If Red has pokemon remaining, output a message stating the Red has won.
    // Otherwise, output a messaging stating the Jellybean has won.
    if(red.notDead()) cout << "Jellybean is out of Pokemon! Red has won the battle! " << endl;
    else cout << "Red is out of Pokemon! Jellybean has won the battle! " << endl;

    return 0;
}
