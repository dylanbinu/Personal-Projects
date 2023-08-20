#ifndef TRAINER_H
#define TRAINER_H
#include <string> // we use strings for names within the trainer class as well, so we need to include string here.
#include <vector> // we keep our main roster of pokemon in a vector
#include "pokemon.h"//trainers have a roster(array) of pokemon that they fight with, so they need to specifically know what a pokemon is.



/*

TYPING DENOTION FOR POKEMON AND ATTACKS, THIS IS IMPORTANT:
this code denotes the type of attack or type of pokemon that an object is using INTEGERS, not CHARACTERS.

typing order is as such:
Null - 0
Normal - 1
Fire - 2
Water - 3
Grass - 4
Electric - 5
Ground - 6
Rock - 7
Flying - 8
Fighting - 9
Psychic - 10
Dark - 11
Ghost - 12
Ice - 13
Bug - 14
Poison - 15
Dragon - 16
Steel - 17
Fairy - 18

Totaling 18 types, excluding null. NOTE ----->>> When applying this numbering system to the attack side of damage calculation, 1 has to be added, as column zero is being used by the possibility of typeless attacks.
This means attacks of normal type are actually represented by 1, and null attacks by 0.

I don't account for this with any - 1 in any functions or constructors, however; I simply count one higher when inputting the parameters for a newly constructed pokemon. this seemed a lot easier to me,
But obviously isn't as friendly to people who would potentially use my code.
*/


class trainer {
public:
    //Default constructor, not for use
    trainer();
    //the intended constructor for trainer classes. Must have a name and six pokemon objects to add to the roster
    trainer(std::string Name, pokemon& roster1, pokemon& roster2, pokemon& roster3, pokemon& roster4, pokemon& roster5, pokemon& roster6);
    //Predconditions: None
    //Postcondition: returns an integer
    //Purpose: returns the quantity in integers of shields that the calling object has.
    int getShields();
    //Preconditions: None
    //Postconditions: Internal, initializes activePoke integer
    //Purpose: The primary function for setting the variable activePoke, which denotes what pokemon from the Battle Roster the trainer will use in attacks. when chooseActive is run, at startup or whenever a pokemon
    //feints, it will show the user their entire roster and each pokemon's current HP. they are prompted to enter a number to choose an active pokemon. they cannot choose a dead pokemon or a number
    //outside of their roster's range.
    void chooseActive();

    //Preconditions: A parameter of the class trainer
    //Postconditions: Internal, changes parameter's active Pokemon's health, and calling object's Pokemon's energy; may prompt chooseActive() for parameter
    //Purpose: Function that makes the calling object's active Pokemon use its fast attack against the parameter object's active pokemon. Calculates total damage of the attack,
    //factoring in a type matchup of the attack and the defender, and the possibility of the defender having multiple Types. Deals this damage to the enemy Pokemon, adds the attacking pokemon's fast move's energy delta
    //to said Pokemon, and if the enemy pokemon has 0 HP or less, prompts the enemy to choose a new pokemon as their active pokemon from their battle roster.
    void fastAttack(trainer & enemy);

    //Preconditions: A parameter of the class trainer
    //Postconditions: Internal, changes parameter's active Pokemon's health, and calling object's Pokemon's energy; may prompt chooseActive() for parameter
    //Purpose: Function that makes the calling object's active Pokemon use its fast attack against the parameter object's active pokemon. Calculates total damage of the attack,
    //factoring in a type matchup of the attack and the defender, and the possibility of the defender having multiple Types. If the attacking Pokemon does not have sufficient energy, the
    //Attack and its calculations are not executed. If the attack can be executed, then the defending trainer has the option to use one of their shields (if they have any remaining) to block the attack.
    //if the attack is blocked, the defender's health does not change, but the Attacker's energy still goes down by the cost of the attack. If the Defender has no shields, then the calculated damage
    //is done to the defending pokemon,  and the attacking pokemon loses energy equivalent to the cost of the attack.
    //If the enemy pokemon has 0 HP or less, prompts the enemy to choose a new pokemon as their active pokemon from their battle roster.
    void chargedAttack(trainer & enemy);

    //Preconditions: A parameter of the class trainer
    //Postconditions: Internal, makes changes to the calling object either based on fastAttack(), chargedAttack(), or chooseActive()
    //Purpose: Prompts the user to have the calling object "take a turn" against the parameter object. The function displays what calling object is taking the turn, displays the Name, HP and Energy of
    //the calling object's active pokemon, and displays three options for the user to choose. The user can either Fast Attack against the parameter, Charged Attack against the parameter (if the active pokemon has enough energy!)
    //Or change their own active pokemon to another in their battle roster. The function displays the name of the fast attack, and the name of the charged attack if it is available.
    void turnAgainst(trainer& enemy);


    //Preconditions: None
    //Postconditions: Internal; changes the value of the battleRoster integer array, outputs text for user interface
    //Purpose: This function allows the user to changew which three pokemon they have out of all of their pokemon in their battle roster. All pokemon that the user has in their normal roster are
    //displayed next to a number, and the user is prompted to enter three numbers, each which match a pokemon, to be put into their battle roster. The user is then told which thee they have chosen.
    void chooseBattleRoster();

    //Preconditions: None
    //Postconditions: Returns a value of type integer
    //Purpose: Returns the cooldown value of the calling object's active pokemon's fast move for reference.
    float getCooldown() const;

    //Preconditions: None
    //Postconditions: Returns a value of type integer
    //Purpose: Returns the duration value of the calling object's active pokemon's fast move for reference.
    float getDuration() const;

    //Preconditions: None
    //Postconditions: Returns a value of type integer
    //Purpose: Returns the current waitTime value of the calling object for reference.
    int getWaitTime() const;

    //Preconditions: An integer value
    //Postconditions: Internal, changes calling object's value for waitTime variable
    //Purpose: Allows the waitTime variable of the calling object to be changed by the amount input as an integer parameter
    void changeWaitTime(int num);

    //preconditions: None
    //Postconditions: outputs a value of type Boolean
    //Purpose: Function that looks at all three pokemon the calling object has that are indexed in its battle roster, and returns a value of false if all three pokemon are dead, or have zero or less health.
    //Otherwise, this function returns as true.
    bool notDead();
    
    
private:
    //the name of the trainer
    std::string name;
    //An array of numbers that represent the battle roster. Each number denotes a slot within the normal roster where a pokemon that is in the battle roster can be found!
    int battleRoster[3];
    //The vector of pokemon where all pokemon the trainer has are held
    std::vector<pokemon> roster;
    //an integer that represents which pokemon in the battle roster (a.k.a which number in the battle roster that points to the normal roster) is the pokemon active in battle.
    int activePoke;
    //I honestly could've set this as 2 right off the bat, but I felt more comfortable letting my constructors handle it. The shields the trainer has.
    int shields;
    //The amount of time the trainer has to wait before they can take another turn, measured in whole numbers.
    int waitTime;
};


#endif //TRAINER_H
