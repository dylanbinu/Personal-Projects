#include <string> // string needs to be included as we use it for names in the fastMove class (I have written this like nine times now)
#include "fastMove.h" // we need to know what class we're building

using namespace std; //used for convenience

fastMove::fastMove() { //default constructor, not intended to be used. makes what's effectively a dead fast move.

    name = "";
    energyDelta = 0;
    basePower = 0;

};
//the intended constructor, simply assigns a name, base power, and energy delta based on precondition inputs.
fastMove::fastMove(string Name, int ed, int bp, int Type, float cooldown, float duration) {

    name = Name;
    energyDelta = ed;
    basePower = bp;
    type = Type;
    //Adding cool down vaiable GF

    fastMoveCoolDown = cooldown;
   // currCoolDown = fastMoveCoolDown;
    fastMoveDuration = duration;

};


//G.F. Get the cooldown value
float fastMove::getFastMoveCoolDown() const
{
    return fastMoveCoolDown;
}

float fastMove::getFastMoveDuration() const
{
    return fastMoveDuration;
}
//simple preconditionless function that returns the name of the fast move.
string fastMove::getName() const{
    return name;
};
//simple preconditionless function that returns the energy delta of the fast move.
int fastMove::getDelta() const{
    return energyDelta;
};
//simple preconditionless function that returns the base power of the fast move.
int fastMove::getPower() const{
    return basePower;
};
//simple preconditionless function that returns the type of the fast move.
int fastMove::getType() const{
    return type;
};


//an equivalence operator overload I built as a precaution for the insertion of pokemon objects into the trainer's array of pokemon objects. I wanted to be sure that putting fast moves of the pokemon
//into the array would go smoothly as well.
fastMove& fastMove:: operator=(const fastMove& entry) {
    name = entry.name;
    energyDelta = entry.energyDelta;
    basePower = entry.basePower;
    type = entry.type;
    //Adding cool down vaiable GF

    fastMoveCoolDown = entry.fastMoveCoolDown;
   // currCoolDown = fastMoveCoolDown;
    fastMoveDuration = entry.fastMoveDuration;


    return *this;
};

