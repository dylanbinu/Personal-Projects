#include <string> //we use string type variables for names in this class
#include "chargedMove.h" //it needs to know what class we're building!

using namespace std; //for convenience

chargedMove::chargedMove() { //default constructor, not meant to be used; makes what's basically a dead charged move

    name = "";
    basePower = 0;
    chargeEnergy = 0;
}
chargedMove::chargedMove(string Name, int bp, int ce, int Type) { //the intended constructor

    name = Name;
    basePower = bp;
    chargeEnergy = ce;
    type = Type;

}
string chargedMove::getName() const{ //used for referencing names
    return name;
}
int chargedMove::getPower() const{ //used for referencing the base power of the move
    return basePower;
}
int chargedMove::getCharge() const{ //used to reference how much energy the move needs
    return chargeEnergy;
}
int chargedMove::getType() const{ //used to reference the type of move this is
    return type;
}

//an equivalency operator overload. I made this because I wanted to ensure that moves were translated correctly into pokemon in their construction.
chargedMove& chargedMove::operator=(const chargedMove& entry) {

    name = entry.getName();
    basePower = entry.getPower();
    chargeEnergy = entry.getCharge();
    type = entry.getType();

    return *this;
}
