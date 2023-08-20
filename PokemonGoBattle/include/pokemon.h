#ifndef POKEMON_H
#define POKEMON_H
#include <string> // we use strings for names in this class, so we need to include string.
#include "fastMove.h" //the pokemon class needs to know what a fast move is because it has one allocated to it.
#include "chargedMove.h" //The pokemon objects also have a charged move allocated to them.
#include <math.h>
#include <time.h>
#include <stdlib.h>



class pokemon {
public:
    //Default constructor for pokemon, not intended to be used. Names default pokemon "Missingno" as a red flag.
    pokemon();
    //Intended pokemon object constructors. Both intake virtually the same parameters, but one takes in an additional integer value representing a second type a pokemon may have.
    //These constructors also choose a random value within the appropriate range for the IV values of the pokemon object.
    //Inputs, in order: Name, Type, bask attack, base defense, base stamina, Level, a fast move object, a charged move object
    pokemon(std::string Name, int type1, int atk, int def, int sta, int Level, fastMove Fast, chargedMove Charged);
    //Inputs, in order: Name, First type, second type, bask attack, base defense, base stamina, Level, a fast move object, a charged move object
    pokemon(std::string Name, int type1, int type2, int atk, int def, int sta, int Level, fastMove Fast, chargedMove Charged);
    //Preconditions: None
    //Postconditions: Returns a value of type string
    //Purpose: Returns ths name of the calling object for reference.
    std::string getName() const;
    //Preconditions: None
    //Postconditions: Returns a value of type string
    //Purpose: Uses a fastMove member function to return the string name of the fast attack the pokemon has.
    std::string getFastName();
    //Preconditions: None
    //Postconditions: Returns an a value of type integer
    //Purpose: Returns the base power value of the calling object's fastAttack object for reference.
    int getFastPower();
    //Preconditions: None
    //Postconditions: Returns an a value of type integer
    //Purpose: Returns the energy delta value of the calling object's fastAttack object for reference.
    int getFastDelta();
    //Preconditions: None
    //Postconditions: Returns an a value of type integer
    //Purpose: Returns the type value of the calling object's fastAttack object for reference.
    int getFastType();
    //Preconditions: None
    //Postconditions: Returns an a value of type float
    //Purpose: Returns the cooldown value of the calling object's fastAttack object for reference.
    float getFastCD();
    //Preconditions: None
    //Postconditions: Returns a value of type string
    //Purpose: Uses a chargedMove member function to return the string name of the charged attack the pokemon has.
    std::string getChargedName();
    //Preconditions: None
    //Postconditions: Returns an a value of type integer
    //Purpose:  Returns the base power value of the calling object's chargedAttack object for reference.
    int getChargedPower();
    //Preconditions: None
    //Postconditions: Returns an a value of type integer
    //Purpose:Returns the type value of the calling object's chargedAttack object for reference.
    int getChargedType();
    //Preconditions: None
    //Postconditions: Returns an a value of type integer
    //Purpose: Returns the energy cost value of the calling object's chargedAttack object for reference.
    int getChargedCost();
    //Preconditions: An integer
    //Postconditions: Internal, Chnages currentHP value
    //Purpose: USed to subtract an amount of health from the calling pokemon equal to the parameter.
    void subtractHealth(int dmg);
    //Preconditions: none
    //Postconditions: Internal, initializes maxHP and currentHP
    //Purpose: uses other private variables like level, base and IV stats to compute what the calling object's maximum HP should be. Sets the pokemon's current HP equal to this value of maximum HP.
    void establishHealth();
    //Preconditions: None
    //Postconditions: Returns an a value of type integer
    //Purpose: Returns the Maximum HP value of the calling object for reference.
    int getMaxHP() const;
    //Preconditions: None
    //Postconditions: Returns an a value of type integer
    //Purpose: Returns the current HP of the calling object for reference.
    int getCurrentHP() const;
    //Preconditions: None
    //Postconditions: Returns an a value of type integer
    //Purpose: Returns the Base Attack value of the calling object for reference.
    int getBA() const;
    //Preconditions: None
    //Postconditions: Returns an a value of type integer
    //Purpose:Returns the Base Defense value of the calling object for reference.
    int getBD() const;
    //Preconditions: None
    //Postconditions: Returns an a value of type integer
    //Purpose: Returns the Base Stamina value of the calling object for reference.
    int getBS() const;
    //Preconditions: None
    //Postconditions: Returns an a value of type integer
    //Purpose:Returns the individual Value (IV) Attack value of the calling object.
    int getIA() const;
    //Preconditions: None
    //Postconditions: Returns an a value of type integer
    //Purpose: Returns the individual Value (IV) Defense value of the calling object.
    int getID() const;
    //Preconditions: None
    //Postconditions: Returns an a value of type integer
    //Purpose:Returns the individual Value (IV) Stamina value of the calling object.
    int getIS() const;
    //Preconditions: an integer
    //Postconditions: Returns a value of type double
    //Purpose: Returns the CPM value for the pokemon level inputted as a parameter, for reference.
    double refCPM(int num);
    //Preconditions: None
    //Postconditions: Returns an a value of type integer
    //Purpose: Returns the level value of the calling object for reference.
    int getLVL() const;
    //Preconditions: None
    //Postconditions: Returns a value of type boolean
    //Purpose: Returns a boolean value which states whether or not the calling pokemon has two types, for reference.
    bool checkTwoTypes() const;
    //Preconditions: Two integer values
    //Postconditions: Returns a value of type double
    //Purpose: Treating the two input integers as types, this function returns the damage/type matchup multiplier of an attack of the type of the second integer hitting a pokemon of the type of the first.
    double refTypeMatchup(int def, int atk);
    //Preconditions: an integer
    //Postconditions: Returns an a value of type integer
    //Purpose: Returns the type value of the calling object. Types for pokemon are held in a size 2 array, as some pokemon can have more than one type; as such, the first slot
    //(position zero) of the array is always to be used for the one type that every pokemon must have, and the second slot is used if the pokemon has two types. boolean variable twoTypes indicates
    //if the calling object in question has two types if it is equal to true.
    int getType(int pos) const;
    //Preconditions: None
    //Postconditions: Returns an a value of type integer
    //Purpose: Returns the energy that the calling object has accrued through attacks.
    int getEnergy();
    //Preconditions: an integer value
    //Postconditions: internal, changes the energy value of the calling object
    //Purpose: changes how much energy the pokemon has by the amount put in as a parameter.
    void changeEnergy(int num);
    //Preconditions: None
    //Postconditions: returns a value of type double
    //Purpose: a quick function to see if the calling object's fast attack has the same type as the calling object. compares the type value of the fast attack to the type value(s) of the calling pokemon,
    //and if they are the same in at least one place, returns a value of 1.2; if not, simply returns a value of 1.
    double fastResonanceCheck();
    //Preconditions: None
    //Postconditions: returns a value of type double
    //Purpose: a quick function to see if the calling object's charged attack has the same type as the calling object. compares the type value of the charged attack to the type value(s) of the calling pokemon,
    //and if they are the same in at least one place, returns a value of 1.2; if not, simply returns a value of 1.
    double chargedResonanceCheck();
    //Preconditions: None
    //Postconditions: Returns the reference of an object of the fastMove Class
    //Purpose: Returns the fast move that the pokemon has, for reference.
    fastMove getFast() const;
    //Preconditions: None
    //Postconditions: Returns the reference of an object of the chargedMove Class
    //Purpose: Returns the charged move that the pokemon has, for reference.
    chargedMove getCharged() const;
    //Preconditions: none
    //Postconditions: Returns a value of type integer
    //Purpose: Returns the duration value of the fast attack the pokemon has.
    float getDuration() const;
private:
    //This array contains, by index, the multiplier for some values that a pokemon has based on their level. basically, every decimal in this array represents a multiplier factor
    //A pokemon would have at a given level. It is included as private data within the pokemon class to ensure that all pokemon have access to it.
    //It could go outside the class, but eh.
     const double CPM[40] = { .094, 0.16639787, 0.21573247, 0.25572005, 0.29024988, 0.3210876, 0.34921268, 0.3752356, 0.39956728, 0.4225, 0.44310755, 0.4627984, 0.48168495, 0.49985844, 0.51739395, 0.5343543,
0.5507927 , 0.5667545 , 0.5822789 , 0.5974, 0.6121573 , 0.6265671 , 0.64065295 , 0.65443563 , 0.667934 , 0.6811649 , 0.69414365 , 0.7068842 , 0.7193991 , 0.7317 , 0.7377695 , 0.74378943 , 0.74976104,
0.7556855 , 0.76156384 , 0.76739717 ,     0.7731865 , 0.77893275 , 0.784637 , 0.7903 };
     //A two dimensional array that describes the damage multiplier for when x type of move attacks y type of pokemon. Also included within the pokemon class for ease of access.
     //vertical columns represent the type of attack, I believe, and horizontal rows represent the type being attacked?
     const double typeMatchup[18][19] = {
   {1, 1, 1, 1, 1, 1, 1, .625, 1, 1, 1, 1, .391, 1, 1, 1, 1, .625, 1}, //Normal row
   {1, 1, .625, .625, 1.6, 1, 1, .625, 1, 1, 1, 1, 1, 1.6, 1.6, 1, .625, 1.6, 1}, //Fire row
   {1, 1, 1.6, .625, .625, 1, 1.6, 1.6, 1, 1, 1, 1, 1, 1, 1, 1, .625, 1, 1}, // Water row
   {1, 1, .625, 1.6, .625, 1, 1.6, 1.6, .625, 1, 1, 1, 1, 1, .625, .625, .625, .625, 1}, //Grass row
   {1, 1, 1, 1.6, .625, .625, .391, 1, 1.6, 1, 1, 1, 1, 1, 1, 1, .625, 1, 1}, //Electric
   {1, 1, 1.6, 1, .625, 1.6, 1, 1.6, .391, 1, 1, 1, 1, 1, .625, 1.6, 1, 1.6, 1}, //Ground
   {1, 1, 1.6, 1, 1, 1, .625, 1, 1.6, .625, 1, 1, 1, 1.6, 1.6, 1, 1, .625, 1}, //Rock
   {1, 1, 1, 1, 1.6, .625, 1, .625, 1, 1.6, 1, 1, 1, 1, 1.6, 1, 1, .625, 1}, //Flying
   {1, 1.6, 1, 1, 1, 1, 1, 1.6, .625, 1, .625, 1.6, .391, 1.6, .625, 1, 1, 1.6, .625}, //Fighting
   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1.6, .625, .391, 1, 1, 1, 1.6, 1, .625, 1}, //Psychic
   {1, 1, 1, 1, 1, 1, 1, 1, 1, .625, 1.6, .625, 1.6, 1, 1, 1, 1, 1, .625}, //Dark
   {1, .391, 1, 1, 1, 1, 1, 1, 1, 1, 1.6, .625, 1.6, 1, 1, 1, 1, 1, 1}, //Ghost
   {1, 1, .625, .625, 1.6, 1, 1.6, 1, 1.6, 1, 1, 1, 1, .625, 1, 1, 1.6, .625, 1}, //Ice
   {1, 1, .625, 1, 1.6, 1, 1, 1, .625, .625, 1.6, 1.6, .625, 1, 1, .625, 1, .625, .625}, //Bug
   {1, 1, 1, 1, 1.6, 1, .625, .625, 1, 1, 1, 1, .625, 1, 1, .625, 1, .391, 1.6}, //Poison
   {1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1.6, .625, .391}, //Dragon
   {1, 1, .625, .625, 1, .625, 1, 1.6, 1, 1, 1, 1, 1, 1.6, 1, 1, 1, .625, 1.6}, //Steel
   {1, 1, .625, 1, 1, 1, 1, 1, 1, 1.6, 1, 1.6, 1, 1, 1, .625, 1.6, .625, 1} //Fairy
     };
     //I use an array of two integers and a boolean value to hold type(s) for the pokemon. the (s) is important, because it can be one or two. so, the array always holds at least one type, and
     //the boolean value tells my code if the second type exists/is relevant. if I didn't have this, then my code might look at the second slot of the array and find a garbage value and think it's a type
     //the pokemon has.
    int type[2];
    //This is the boolean value that tells other functions whether or not this pokemon has two types. if it is true, then the type array uses both slots; if not, only the first slot in the type array
    //is relevant.
    bool twoTypes;
    //The name of the pokemon
    std::string name;
    //The fast move the pokemon has
    fastMove fast;
    //The charged move the pokemon has
    chargedMove charged;
    //It's quite explicitly stated in the assignment parameters that this value is/should be hardlocked at 500, so I took a liberty and just inserted this flat variable. The maximum energy a pokemon can have.
    const int maximumEnergy = 500;
    //The amount of energy the pokemon currently has, which was accrued from using fast attacks.
    int energy;
    //The Individual Value (IV) Attack value this pokemon has
    int ivATK;
    //The Individual Value (IV) Defense value this pokemon has
    int ivDEF;
    //The Individual Value (IV) Stamina value this pokemon has
    int ivSTA;
    //The Base Attack value this pokemon has
    int baseATK;
    //The Base Defense value this pokemon has
    int baseDEF;
    //The Base Stamina value this pokemon has
    int baseSTA;
    //The Maximum HP this pokemon can have, established with the function establishHealth() above.
    int maxHP;
    //The current HP this pokemon has
    int currentHP;
    //The level that this pokemon is, set at construction
    int level;

};


#endif /*POKEMON_HPP*/
