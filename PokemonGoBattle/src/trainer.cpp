/* Modifications made by Dylan Binu:
 1) chooseActive function: Prints 0 next to the HP of pokemon during the pokemon selection phase if the pokemon has feinted.
 2) chooseActive function: Also modified the activePoke section of the code to reference the battle roster instead of the roster containing all six pokemon.
 3) chooseBattleRoster function: Modifications to ensure that the user cannot choose the same pokemon twice and that they don't choose a number lesser than 0 or greater than 5 for the battle roster index.
 4) fastAttack and chargeAttack functions: Prints the damage taken and a "feinted" message if a pokemon feints. Prints the damage taken and HP remaining if pokemon doesn't feint.
 
 */

#include <string> // we use strings for names in the trainer class, and in other classes the trainer references, so we need to include strings here.
#include <iostream> // we use cout and cin statement here, so we need this
#include <math.h> // math.h is used for the floor function
#include <vector>
#include "trainer.h" // we need to know what class we're building! duh!
#include "fastMove.h"

using namespace std; //used for convenience
//chooseActive  is the primary function for setting the variable activePoke, which denotes what pokemon the trainer will use in attacks. when chooseActive is run, at startup or whenever a pokemon
//feints, it will show the user their entire roster and each pokemon's current HP. they are prompted to enter a number to choose an active pokemon. they cannot choose a dead pokemon or a number
//outside of their roster's range. This function requires no arguments, just a calling object.
void trainer::chooseActive() {
    bool right = true;
    while(right == true) {
       //Edited by Dylan Binu. Runs this if statement if Pokemon 1 has feinted.
        if(roster[battleRoster[0]].getCurrentHP() <= 0 && roster[battleRoster[1]].getCurrentHP() > 0 && roster[battleRoster[2]].getCurrentHP() > 0) {
            cout << name << ", choose active Pokemon:" << endl << roster[battleRoster[0]].getName() << " : " << "0" << " HP" <<
                endl << roster[battleRoster[1]].getName() << " : " << roster[battleRoster[1]].getCurrentHP() << " HP" << endl << roster[battleRoster[2]].getName() << " : " << roster[battleRoster[2]].getCurrentHP() << " HP" << endl << "[1,2,3]: ";
        }
        
        //Edited by Dylan Binu. Runs this if statement if Pokemon 2 has feinted.
        if(roster[battleRoster[0]].getCurrentHP() > 0 && roster[battleRoster[1]].getCurrentHP() <= 0 && roster[battleRoster[2]].getCurrentHP() > 0) {
            cout << name << ", choose active Pokemon:" << endl << roster[battleRoster[0]].getName() << " : " << roster[battleRoster[0]].getCurrentHP() << " HP" <<
            endl << roster[battleRoster[1]].getName() << " : " << "0" << " HP" << endl << roster[battleRoster[2]].getName() << " : " << roster[battleRoster[2]].getCurrentHP() << " HP" << endl << "[1,2,3]: ";
        }
        
        // Edit by Dylan Binu. Runs this if statement if Pokemon 3 has feinted.
        if(roster[battleRoster[0]].getCurrentHP() > 0 && roster[battleRoster[1]].getCurrentHP() > 0 && roster[battleRoster[2]].getCurrentHP() <= 0) {
            cout << name << ", choose active Pokemon:" << endl << roster[battleRoster[0]].getName() << " : " << roster[battleRoster[0]].getCurrentHP() << " HP" <<
                endl << roster[battleRoster[1]].getName() << " : " << roster[battleRoster[1]].getCurrentHP() << " HP" << endl << roster[battleRoster[2]].getName() << " : " << "0" << " HP" << endl << "[1,2,3]: ";
        }
        
        // Edit by Dylan Binu. Runs this if statement if Pokemon 1 and 2 have feinted.
        if(roster[battleRoster[0]].getCurrentHP() <= 0 && roster[battleRoster[1]].getCurrentHP() <= 0 && roster[battleRoster[2]].getCurrentHP() > 0) {
            cout << name << ", choose active Pokemon:" << endl << roster[battleRoster[0]].getName() << " : " << "0" << " HP" <<
                endl << roster[battleRoster[1]].getName() << " : " << "0" << " HP" << endl << roster[battleRoster[2]].getName() << " : " << roster[battleRoster[2]].getCurrentHP() << " HP" << endl << "[1,2,3]: ";
        }
        
        // Edit by Dylan Binu. Runs this if statement if Pokemon 1 and 3 have feinted.
        if(roster[battleRoster[0]].getCurrentHP() <= 0 && roster[battleRoster[1]].getCurrentHP() > 0 && roster[battleRoster[2]].getCurrentHP() <= 0) {
            cout << name << ", choose active Pokemon:" << endl << roster[battleRoster[0]].getName() << " : " << "0" << " HP" <<
                endl << roster[battleRoster[1]].getName() << " : " << roster[battleRoster[1]].getCurrentHP() << " HP" << endl << roster[battleRoster[2]].getName() << " : " << "0" << " HP" << endl << "[1,2,3]: ";
        }
        
        // Edit by Dylan Binu. Runs this if statement if Pokemon 2 and 3 have feinted.
        if(roster[battleRoster[0]].getCurrentHP() > 0 && roster[battleRoster[1]].getCurrentHP() <= 0 && roster[battleRoster[2]].getCurrentHP() <= 0) {
            cout << name << ", choose active Pokemon:" << endl << roster[battleRoster[0]].getName() << " : " << roster[battleRoster[0]].getCurrentHP() << " HP" <<
                endl << roster[battleRoster[1]].getName() << " : " << "0" << " HP" << endl << roster[battleRoster[2]].getName() << " : " << "0" << " HP" << endl << "[1,2,3]: ";
        }
        
        // Edited by Dylan Binu. Runs his if statement if no pokemon has 0 HP.
        if(roster[battleRoster[0]].getCurrentHP() > 0 && roster[battleRoster[1]].getCurrentHP() > 0 && roster[battleRoster[2]].getCurrentHP() > 0) {
            cout << name << ", choose active Pokemon:" << endl << roster[battleRoster[0]].getName() << " : " << roster[battleRoster[0]].getCurrentHP() << " HP" <<
                endl << roster[battleRoster[1]].getName() << " : " << roster[battleRoster[1]].getCurrentHP() << " HP" << endl << roster[battleRoster[2]].getName() << " : " << roster[battleRoster[2]].getCurrentHP() << " HP" << endl << "[1,2,3]: ";
        }
       
        cin >> activePoke;
        if (activePoke <= 3 && activePoke >= 1 && (roster[battleRoster[(activePoke - 1)]].getCurrentHP() > 0)) {
            activePoke = activePoke - 1;
            right = false;
        }
        else {
            cout << "Something went wrong, please try again." << endl;
        }
    }
}
void trainer::chooseBattleRoster() {
    cout << name <<", choose your three Pokemon to battle! \nEnter three numbers in a row to represent each Pokemon." << endl;
    for (int i = 0; i < roster.size(); i++) {
        cout << i << " - " << roster[i].getName()<< endl;
    }
    cout << "Enter Pokemon number one: ";
    cin >> battleRoster[0];

    // Edited by Dylan Binu. While loop to ensure that the user only selects a number from 0 to 5.
    while (battleRoster[0] < 0 or battleRoster[0] > 5) {
        cout << "PokeMistake! You can only choose a number from 0 - 5!" << endl;
        cout << "Enter Pokemon number one: ";
        cin >> battleRoster[0];
    }
    cout << "Enter Pokemon number two: ";
    cin >> battleRoster[1];

    // Edited by Dylan Binu. While loop to ensure that the user only selects a number from 0 to 5 and also doesn't pick the same pokemon twice.
    while (battleRoster[1] < 0 or battleRoster[1] > 5) {
        cout << "PokeMistake! You can only choose a number from 0 - 5!" << endl;
        cout << "Enter Pokemon number two: ";
        cin >> battleRoster[1];
        while(battleRoster[1] == battleRoster[0]) {
            cout << "Sorry! You can't choose the same Pokemon! Choose another Pokemon!" << endl;
            cout << "Enter Pokemon number two: ";
            cin >> battleRoster[1];
        }
    }

    // Edited by Dylan Binu. While loop to ensure that the same Pokemon doesn't get picked twice and also to ensure the trainer only picks a number from 0 - 5.
    while(battleRoster[1] == battleRoster[0]) {
        cout << "Sorry! You can't choose the same Pokemon! Choose another Pokemon!" << endl;
        cout << "Enter Pokemon number two: ";
        cin >> battleRoster[1];
        while (battleRoster[1] < 0 or battleRoster[1] > 5) {
            cout << "PokeMistake! You can only choose a number from 0 - 5!" << endl;
            cout << "Enter Pokemon number two: ";
            cin >> battleRoster[1];
        }
    }
    

    cout << "Enter Pokemon number three: ";
    cin >> battleRoster[2];

        // Edited by Dylan Binu. While loop to ensure that the user only selects a number from 0 to 5 and also doesn't pick the same pokemon twice.
    while (battleRoster[2] < 0 or battleRoster[2] > 5) {
        cout << "PokeMistake! You can only choose a number from 0 - 5!" << endl;
        cout << "Enter Pokemon number three: ";
        cin >> battleRoster[2];
        while(battleRoster[2] == battleRoster[0] or battleRoster[2] == battleRoster[1]) {
            cout << "Sorry! You can't choose the same Pokemon! Choose another Pokemon!" << endl;
            cout << "Enter Pokemon number three: ";
            cin >> battleRoster[2];
        }
    }

        // Edited by Dylan Binu. While loop to ensure that the same Pokemon doesn't get picked twice and also to ensure the trainer only picks a number from 0 - 5.
    while(battleRoster[2] == battleRoster[0] or battleRoster[2] == battleRoster[1]) {
        cout << "Sorry! You can't choose the same Pokemon! Choose another Pokemon!" << endl;
        cout << "Enter Pokemon number three: ";
        cin >> battleRoster[2];
        while (battleRoster[2] < 0 or battleRoster[2] > 5) {
            cout << "PokeMistake! You can only choose a number from 0 - 5!" << endl;
            cout << "Enter Pokemon number three: ";
            cin >> battleRoster[2];
        }
    }

    cout << "The pokemon on your roster are " << roster[battleRoster[0]].getName() << ", " << roster[battleRoster[1]].getName() << ", and " << roster[battleRoster[2]].getName() << "!" << endl << endl;
}
trainer::trainer() { //default constructor that gives no pokemon, no shields and a null name. the Punished.
    name = "null";
    shields = 0;
}
//the intended constructor for trainer classes. Must have a name and three pokemon objects to add to the roster, because having constructors and other code to accomodate for a partially filled roster
//was going to be painful.
trainer::trainer(std::string Name, pokemon& roster1, pokemon& roster2, pokemon& roster3, pokemon& roster4, pokemon& roster5, pokemon& roster6) {
    name = Name;
    roster.push_back(roster1);
    roster.push_back(roster2);
    roster.push_back(roster3);
    roster.push_back(roster4);
    roster.push_back(roster5);
    roster.push_back(roster6);
    shields = 2;
}
//simple function for referring to how many shields a trainer has. has no preconditions, and simply returns the calling object's value of shields.
int trainer::getShields() {
    return shields;
}
float trainer::getCooldown(/*fastMove& fastMove*/) const{
    return roster[battleRoster[activePoke]].getFast().getFastMoveCoolDown();//fastMove.getFastMoveCoolDown();
};
float trainer::getDuration(/*fastMove& fastMove*/) const {
    return roster[battleRoster[activePoke]].getFast().getFastMoveDuration();//fastMove.getFastMoveCoolDown();
};
void trainer::changeWaitTime(int num) {
    waitTime += num;
    if (waitTime < 0)
        waitTime = 0;
};
int trainer::getWaitTime() const{
    return waitTime;
};
//oh god. here we go. the battle functions.

//below us, we have the two functions used for attacks , one for fast atacks (fastAttack) and one for charged attacks (chargedAttack). they have one precondition, which is the trainer whose active
//pokemon you are attempting to attack. they both function fairly similarly, with some minor differences. they both tell us, first and foremost, whose pokemon is attacking, what their name is, and
//what attack they're using first. Then they move on to making the calculations given to us by the professor for the assignment. They make the temporary values cATK_attacker/defender,  and totalDamage,
//and then they define them based on given parameters. they are sure to cross reference for STAB multipliers (which I refer to as attack resonance) and the type matchup chart, which is the
//previously/later discussed 2D array in the pokemon class. if the pokemon has two types (a fact denoted by that pokemon's bool value twoTypes!), then this type matchup is run twice, each time with a
//different one of that pokemon's types. this total damage we just calculated is subtracted from the attacked pokemon's health, the attacking pokemon's energy changes accordingly, and if the attacked
//trainer's pokemon has just fainted, they are prompted to pick a new one from their roster.


void trainer::fastAttack(trainer & enemy) {
    //there are no stipulations as to whether or not a fast attack can occur; they just do.
    cout << name << "'s " << roster[battleRoster[activePoke]].getName() << " used " << roster[battleRoster[activePoke]].getFastName() << "!" << endl;
    
    int cATK_attacker = (roster[battleRoster[activePoke]].getBA() + roster[battleRoster[activePoke]].getIA()) * roster[battleRoster[activePoke]].refCPM(roster[battleRoster[activePoke]].getLVL());
    int cDEF_defender = (enemy.roster[enemy.battleRoster[enemy.activePoke]].getBD() + enemy.roster[enemy.battleRoster[enemy.activePoke]].getID()) * enemy.roster[enemy.battleRoster[enemy.activePoke]].refCPM(enemy.roster[enemy.battleRoster[enemy.activePoke]].getLVL());
    int totalDamage;
    if (enemy.roster[enemy.battleRoster[enemy.activePoke]].checkTwoTypes()) //if the nemy pokemon has two types,
    {
        totalDamage = floor(0.5 * roster[battleRoster[activePoke]].getFastPower() * cATK_attacker / cDEF_defender *
            roster[battleRoster[activePoke]].refTypeMatchup(roster[battleRoster[activePoke]].getFastType(),
                enemy.roster[enemy.battleRoster[enemy.activePoke]].getType(0)) * roster[battleRoster[activePoke]].refTypeMatchup(roster[battleRoster[activePoke]].getFastType(),
                    enemy.roster[enemy.battleRoster[enemy.activePoke]].getType(1)) * roster[battleRoster[activePoke]].fastResonanceCheck());
        enemy.roster[enemy.battleRoster[enemy.activePoke]].subtractHealth(totalDamage);
        //Edited by Dylan Binu. Move effectiveness messages.
    if  ((roster[battleRoster[activePoke]].refTypeMatchup(roster[battleRoster[activePoke]].getFastType(), enemy.roster[enemy.battleRoster[enemy.activePoke]].getType(0)) * roster[battleRoster[activePoke]].refTypeMatchup(roster[battleRoster[activePoke]].getFastType(), enemy.roster[enemy.battleRoster[enemy.activePoke]].getType(1))) > 1)
    {
        cout << "It was super effective!" << endl;
    }
    if ((roster[battleRoster[activePoke]].refTypeMatchup(roster[battleRoster[activePoke]].getFastType(), enemy.roster[enemy.battleRoster[enemy.activePoke]].getType(0)) * roster[battleRoster[activePoke]].refTypeMatchup(roster[battleRoster[activePoke]].getFastType(), enemy.roster[enemy.battleRoster[enemy.activePoke]].getType(1))) < 1)
    {
        cout << "It was not very effective!" << endl;
    }
    }
    else {
        totalDamage = floor(0.5 * roster[battleRoster[activePoke]].getFastPower() * cATK_attacker/cDEF_defender * roster[battleRoster[activePoke]].refTypeMatchup(roster[battleRoster[activePoke]].getFastType(), enemy.roster[enemy.battleRoster[enemy.activePoke]].getType(0)) * roster[battleRoster[activePoke]].fastResonanceCheck());
    enemy.roster[enemy.battleRoster[enemy.activePoke]].subtractHealth(totalDamage); //here we take the damage we just calculated and subtract it from the enemy's health
        //Edited by Dylan Binu. Move effectiveness messages.
    if(roster[battleRoster[activePoke]].refTypeMatchup(roster[battleRoster[activePoke]].getFastType(), (enemy.roster[enemy.battleRoster[enemy.activePoke]].getType(0))) > 1) {
        cout << "It was super effective!" << endl;
    }
    if((roster[battleRoster[activePoke]].refTypeMatchup(roster[battleRoster[activePoke]].getFastType(), enemy.roster[enemy.battleRoster[enemy.activePoke]].getType(0))) < 1) {
        cout << "It was not very effective!" << endl;
    }
    }
    //Edited by Dylan. Prints out the damage taken by the target pokemon and also prints out the remaining HP. If the pokemon feints, it prints out a feinted message.
    
    if (enemy.roster[enemy.battleRoster[enemy.activePoke]].getCurrentHP() > 0) {
    cout << enemy.roster[enemy.battleRoster[enemy.activePoke]].getName() << " has taken " << totalDamage << " damage, and is now at " << enemy.roster[enemy.battleRoster[enemy.activePoke]].getCurrentHP() << " HP!" << endl << endl;
    }
    else {
        cout << enemy.roster[enemy.battleRoster[enemy.activePoke]].getName() << " has taken " << totalDamage << " damage. " << endl;
        cout << enemy.roster[enemy.battleRoster[enemy.activePoke]].getName() << " has feinted!" << endl << endl;
    }
    //attacking pokemon's energy is increased by their fast attack's delta
    roster[battleRoster[activePoke]].changeEnergy(roster[battleRoster[activePoke]].getFastDelta());
   // if (!(enemy.notDead()))
     //   break;
    if (enemy.roster[enemy.battleRoster[enemy.activePoke]].getCurrentHP() <= 0 && enemy.notDead())
        enemy.chooseActive();
    changeWaitTime((getCooldown() + getDuration()) * 2);
}
void trainer::chargedAttack(trainer & enemy) {
    //for charged attacks, we need to make sure that the attacking/active pokemon of the attacker has accrued enough energy to successfully do their charged attack. if they have not, the code just
    //treats it like a fluke and keeps going.
    if (roster[battleRoster[activePoke]].getEnergy() >= roster[battleRoster[activePoke]].getChargedCost()) {
        //but if they have, the code makes some temporary values in order to easily interface with the player.
        bool chargeBlocked = false;
        char input;
        //it needs to interface with the player because it needs to know whether or not the defending trainer wants to use a shield to block the incoming attack. first it needs to know if the defender
        //even has any shields.
        if (enemy.getShields() > 0) {
            //if they do, they're given the option.
            cout << enemy.name << ", you have " << enemy.getShields() << " shields remaining! Do you wish to use a shield? [Y/N]" << endl << ": ";
            cin >> input;
            if ((input == 'Y') || (input == 'y'))
                chargeBlocked = true;
        }
        //if they block the attack, they lose a shield and the attacker loses their energy.
        if (chargeBlocked) {
            cout << enemy.name << " blocked " << roster[battleRoster[activePoke]].getName() << "'s " << roster[battleRoster[activePoke]].getChargedName() << "!";
            roster[battleRoster[activePoke]].changeEnergy(-1 * (roster[battleRoster[activePoke]].getChargedCost()));
            enemy.shields -= 1;
        }
        //if they don't, huzzah! attack!
        else {
            cout << roster[battleRoster[activePoke]].getName() << " used " << roster[battleRoster[activePoke]].getChargedName() << "!" << endl;
            int cATK_attacker = (roster[battleRoster[activePoke]].getBA() + roster[battleRoster[activePoke]].getIA()) * roster[battleRoster[activePoke]].refCPM(roster[battleRoster[activePoke]].getLVL());
            int cDEF_defender = (enemy.roster[enemy.battleRoster[enemy.activePoke]].getBD() + enemy.roster[enemy.battleRoster[enemy.activePoke]].getID()) * enemy.roster[enemy.battleRoster[enemy.activePoke]].refCPM(enemy.roster[enemy.battleRoster[enemy.activePoke]].getLVL());
            int totalDamage;
            if (enemy.roster[enemy.battleRoster[enemy.activePoke]].checkTwoTypes()) //if the nemy pokemon has two types,
            {
                totalDamage = floor(0.5 * roster[battleRoster[activePoke]].getChargedPower() * cATK_attacker / cDEF_defender * roster[battleRoster[activePoke]].refTypeMatchup(roster[battleRoster[activePoke]].getChargedType(), enemy.roster[enemy.battleRoster[enemy.activePoke]].getType(0)) * roster[battleRoster[activePoke]].refTypeMatchup(roster[battleRoster[activePoke]].getChargedType(), enemy.roster[enemy.battleRoster[enemy.activePoke]].getType(1)) * roster[battleRoster[activePoke]].chargedResonanceCheck()) + 1;
                enemy.roster[enemy.battleRoster[enemy.activePoke]].subtractHealth(totalDamage);
                //Edited by Dylan Binu. Move effectiveness messages.
                if ((roster[battleRoster[activePoke]].refTypeMatchup(roster[battleRoster[activePoke]].getChargedType(), enemy.roster[enemy.battleRoster[enemy.activePoke]].getType(0)) * roster[battleRoster[activePoke]].refTypeMatchup(roster[battleRoster[activePoke]].getChargedType(), enemy.roster[enemy.battleRoster[enemy.activePoke]].getType(1)) /* * roster[battleRoster[activePoke]].chargedResonanceCheck()*/) > 1)
                {
                    cout << "It was super effective!" << endl;
                }
                if ((roster[battleRoster[activePoke]].refTypeMatchup(roster[battleRoster[activePoke]].getChargedType(), enemy.roster[enemy.battleRoster[enemy.activePoke]].getType(0)) * roster[battleRoster[activePoke]].refTypeMatchup(roster[battleRoster[activePoke]].getChargedType(), enemy.roster[enemy.battleRoster[enemy.activePoke]].getType(1)) /* * roster[battleRoster[activePoke]].chargedResonanceCheck()*/) < 1)
                {
                    cout << "It was not very effective!" << endl;
                }
            }
            else {
                totalDamage = floor(0.5 * roster[battleRoster[activePoke]].getChargedPower() * cATK_attacker / cDEF_defender * roster[battleRoster[activePoke]].refTypeMatchup(roster[battleRoster[activePoke]].getChargedType(), enemy.roster[enemy.battleRoster[enemy.activePoke]].getType(0)) * roster[battleRoster[activePoke]].chargedResonanceCheck()) + 1;
            enemy.roster[enemy.battleRoster[enemy.activePoke]].subtractHealth(totalDamage); //here we take the damage we just calculated and subtract it from the enemy's health

            //Edited by Dylan Binu. Move effectiveness messages.
            if((roster[battleRoster[activePoke]].refTypeMatchup(roster[battleRoster[activePoke]].getChargedType(), enemy.roster[enemy.battleRoster[enemy.activePoke]].getType(0)) /* * roster[battleRoster[activePoke]].chargedResonanceCheck()*/) > 1) {
                cout << "It was super effective!" << endl;
            }
            if((roster[battleRoster[activePoke]].refTypeMatchup(roster[battleRoster[activePoke]].getChargedType(), enemy.roster[enemy.battleRoster[enemy.activePoke]].getType(0)) /* * roster[battleRoster[activePoke]].chargedResonanceCheck()*/) < 1) {
                cout << "It was not very effective!" << endl;
            }
            }
            
            //Edited by Dylan. Prints out the damage taken by the target pokemon and also prints out the remaining HP. If the pokemon feints, it prints out a feinted message.
            
            if (enemy.roster[enemy.battleRoster[enemy.activePoke]].getCurrentHP() > 0) {
            cout << enemy.roster[enemy.battleRoster[enemy.activePoke]].getName() << " has taken " << totalDamage << " damage, and is now at " << enemy.roster[enemy.battleRoster[enemy.activePoke]].getCurrentHP() << " HP!" << endl << endl;
            }
            else {
                cout << enemy.roster[enemy.battleRoster[enemy.activePoke]].getName() << " has taken " << totalDamage << " damage. " << endl;
                cout << enemy.roster[enemy.battleRoster[enemy.activePoke]].getName() << " has feinted!" << endl << endl;
            }
            roster[battleRoster[activePoke]].changeEnergy(-1 * (roster[battleRoster[activePoke]].getChargedCost()));
            if (enemy.roster[enemy.battleRoster[enemy.activePoke]].getCurrentHP() <= 0 && enemy.notDead())
                enemy.chooseActive();
        }
    }
    //this is the fluke mentioned above, when a pokemon can't charge attack but tries to.
    else {
        cout << name << "'s " << roster[battleRoster[activePoke]].getName() << " tried to use " << roster[battleRoster[activePoke]].getChargedName() << ", but didn't have enough energy!" << endl;
    }
}

void trainer::turnAgainst(trainer& enemy) {
    bool wrong = true;
    while (wrong) {
        wrong = false;
        cout << "It is " << name << "'s turn! Choose your action:" << endl;
        cout << " | " << roster[battleRoster[activePoke]].getName() << " | " << roster[battleRoster[activePoke]].getCurrentHP()
            << " HP | " << roster[battleRoster[activePoke]].getEnergy() << " Energy |" << endl;
        cout << "1 - Fast Attack - " << roster[battleRoster[activePoke]].getFastName() << endl;
        if (roster[battleRoster[activePoke]].getEnergy() >= roster[battleRoster[activePoke]].getChargedCost())
            cout << "2 - Charged Attack - " << roster[battleRoster[activePoke]].getChargedName() << endl;
        else
            cout << "2 - Charged Attack - NOT ENOUGH ENERGY" << endl;
        cout << "3 - Change active Pokemon" << endl;
        int choice;
        cin >> choice;
        switch (choice) {
        case 1:
            fastAttack(enemy);
            break;
        case 2:
            chargedAttack(enemy);
            break;
        case 3:
            chooseActive();
            break;
        default:
            cout << "Something went wrong!";
            wrong = true;
        }
    }
}

//Edited by Dylan Binu. Returns false if all pokemon's HP is below 0. Returns true otherwise.
bool trainer::notDead() {
    if (roster[battleRoster[0]].getCurrentHP() <= 0 && roster[battleRoster[1]].getCurrentHP() <= 0 && roster[battleRoster[2]].getCurrentHP() <= 0) return false;
    else return true;
}
