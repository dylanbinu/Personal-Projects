#include "pokemon.h" //we need to know what class we're building!
#include "fastMove.h"//pokemon.h already includes these class fiels for us, they're just added here for clarity.
#include "chargedMove.h"//^^
#include <string> //strings are used for names in this class, so we need to include string!
#include <math.h> //gives us the floor function for topping off values like HP generation
#include <time.h>
#include <stdlib.h> //I believe I need this for the rand that I'm using?


using namespace std;

void pokemon::establishHealth() { //This is the function I run in the constructors to quickly establish Max HP. Might use it in some sort of level-up function to refresh the Max and current, as well.
    maxHP = floor((static_cast<float>(baseSTA) + static_cast<float>(ivSTA)) * static_cast<float>(CPM[(level-1)]));
    currentHP = maxHP;
};
pokemon::pokemon() { //This is the default constructor, and its quite clearly never supposed to be used. Default Pokemon start dead, even.

    name = "missingno";
    energy = 0;
    ivATK = 0;
    ivDEF = 0;
    ivSTA = 0;
    baseATK = 0;
    baseDEF = 0;
    baseSTA = 0;
    maxHP = 0;
    currentHP = 0;
    level = 0;

};
pokemon::pokemon(string Name, int type1, int atk, int def, int sta, int Level, fastMove Fast, chargedMove Charged) { //option for a pokemon with just one type
    srand(time(NULL));
    name = Name;
    // I can keep the second portion of the array empty because I have my twoTypes bool value to tell my code later on that only the first slot is useful
    type[0] = type1;
    baseATK = atk;
    baseDEF = def;
    baseSTA = sta;
    level = Level;
    fast = Fast;
    charged = Charged;
    energy = 0;
    //I looked up a way to get random integers in order to randomize IV values for pokemon. Inputting IV values manually felt wrong to me; I felt as though they should be just as
    //random in my code as in the game.
    ivATK = static_cast<int>(rand() % 16);
    ivDEF = static_cast<int>(rand() % 16);
    ivSTA = static_cast<int>(rand() % 16);
    //we call on establishHealth to perform the given function to set out maxHP and currentHP.
    establishHealth();
    twoTypes = false;

};
pokemon::pokemon(string Name, int type1, int type2, int atk, int def, int sta, int Level, fastMove Fast, chargedMove Charged) { // option for a pokemon with two types
    //same as above, but we fill out both parts of the type array and we set twoTypes to true
    srand(time(NULL));
    name = Name;
    type[0] = type1;
    type[1] = type2;
    baseATK = atk;
    baseDEF = def;
    baseSTA = sta;
    level = Level;
    fast = Fast;
    charged = Charged;
    energy = 0;
    ivATK = static_cast<int>(rand() % 16);
    ivDEF = static_cast<int>(rand() % 16);
    ivSTA = static_cast<int>(rand() % 16);
    establishHealth();
    twoTypes = true;
};
//function specifically for removing health from a pokemon, has an integer precondition to tell it just how much health you're removing
void pokemon::subtractHealth(int dmg) {
    currentHP -= dmg;
};
//simple function, no preconditions, returns the name of the pokemon
string pokemon::getName() const{
    return name;
};
//function that gets the type of the pokemon. has a precondition of the position in the type array that you're looking at. the code only calls upon position 0 if its a one type pokemon, but runs this
//for 0 and 1 if it has two types.
int pokemon::getType(int pos) const{
    return type[pos];
};
//simple, preconditionless function that returns the boolean of whether or not the pokemon in question has two types
bool pokemon::checkTwoTypes() const{
    return twoTypes;
};
//a function that has no preconditions, and uses a fastMove member function to get the name of the fast move the pokemon has to return it.
string pokemon::getFastName() {
    return fast.getName();
};
//does the same as the previous function, but calls upon a chargedMove member function to find the charged move's name
string pokemon::getChargedName() {
    return charged.getName();
};
//preconditionless function that returns (as a constant) the pokemon's Base attack.
int pokemon::getBA() const{
    return baseATK;
};
//preconditionless function that returns (as a constant) the pokemon's Base defense.
int pokemon::getBD() const{
    return baseDEF;
};
//preconditionless function that returns (as a constant) the pokemon's Base stamina.
int pokemon::getBS() const{
    return baseSTA;
};
//preconditionless function that returns (as a constant) the pokemon's Independent value attack.
int pokemon::getIA() const{
    return ivATK;
};
//preconditionless function that returns (as a constant) the pokemon's Independent value defense.
int pokemon::getID() const{
    return ivDEF;
};
//preconditionless function that returns (as a constant) the pokemon's Independent value stamina.
int pokemon::getIS() const{
    return ivSTA;
};
//function used for referencing the level multiplier array in each pokemon. precondition of the level you're looking for, outputs the multiplier for said level.
double pokemon::refCPM(int num) {
    return CPM[num - 1];
};
//function used for referencing the type matchup 2D array in each pokemon. finds the damage multiplier of an attack based on the type of the attack and the type of the defending pokemon. has two
//integer preconditions, one for either type value.
double pokemon::refTypeMatchup(int def, int atk) {
    return typeMatchup[def][atk];
}
//preconditionless function that simply returns the level of the pokemon object used to call upon the function.
int pokemon::getLVL() const{
    return level;
};
//Preconditionless function that returns the base power of the fast attack the pokemon in question has. uses a fastMove member function to do so.
int pokemon::getFastPower() {
    return fast.getPower();
};
//preconditionless function that returns the energy delta of the fast attack the pokemon in question has. uses a fastMove member function to do so.
int pokemon::getFastDelta() {
    return fast.getDelta();
};
//preconditionless function that returns the attack type of the fast attack that a pokemon in question has. uses a fastMove member function to do so.
int pokemon::getFastType() {
    return fast.getType();
};
float pokemon::getFastCD() {
    return fast.getFastMoveCoolDown();
}
//preconditionless function that returns the base power of the charged attack the pokemon in question has. uses a chargedMove member function to do so.
int pokemon::getChargedPower() {
    return charged.getPower();
};
//preconditionless function that returns the type of the charged attack the pokemon in question has. uses a chargedMove member fucntion to do so.
int pokemon::getChargedType() {
    return charged.getType();
};
//preconditionless function that simply returns the amount of energy the pokemon in question has.
int pokemon::getEnergy() {
    return energy;
};
//function used for altering the amount of energy a pokemon has. has a precondition which uis simply the value of energy to be added/subtracted. this function ensures that energy can not go above the
//maximum and cannot go below zero.
void pokemon::changeEnergy(int num) {
    energy += num;
    if (energy > maximumEnergy)
        energy = maximumEnergy;
    if (energy < 0)
        energy = 0;
};
//preconditionless function that returns the energy cost of the charged attack of the pokemon in question. uses a chargedMove member function to do so.
int pokemon::getChargedCost() {
    return charged.getCharge();
};
//preconditionless function that quickly checks to see if the type of the fast move a pokemon has matches either of that pokemon's potential types. if it matches one of the hypothetical two,
//it returns the STAB multiplier value, and if it doesn't, it just returns 1.
double pokemon::fastResonanceCheck() {
    if ((type[0] == (getFastType() + 1)) || (type[1] == (getFastType() + 1)))
        return 1.2;
    else
        return 1;
};
//preconditionless function that quickly checks to see if the type of the charged move a pokemon has matches either of that pokemon's potential types. if it matches one of the hypothetical two,
//it returns the STAB multiplier value, and if it doesn't, it just returns 1.
double pokemon::chargedResonanceCheck() {
    if ((type[0] == (getChargedType() + 1)) || (type[1] == (getChargedType() + 1)))
        return 1.2;
    else
        return 1;
};
//simple preconditionless function that returns the max HP of the pokemon in question.
int pokemon::getMaxHP() const{
    return maxHP;
};
//simple preconditionless function that returns the current HP of the pokemon in question
int pokemon::getCurrentHP() const{
    return currentHP;
};
//simple preconditionless function that returns the fastMove object that the pokemon in question has
fastMove pokemon::getFast() const {
    return fast;
};
//simple preconditionless function that returns the chargedMove object that the pokemon in question has
chargedMove pokemon::getCharged() const {
    return charged;
};

//Edited by Dylan Binu. Returns the duration of a pokemon's fast move.
float pokemon::getDuration() const{
    return fast.getFastMoveDuration();
}
