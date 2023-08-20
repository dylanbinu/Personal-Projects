#ifndef FASTMOVE_H
#define FASTMOVE_H
#include <string> //We use strings as names for fast moves, so we need to include string


class fastMove {
public:
    //default constructor, not intended to be used
    fastMove();
    //intended constructor, builds a fast move object with the following arameters, in this order
    //a name, an energy delta, a base power, the attack's type, the cooldown value, and the duration value
    fastMove(std::string name, int ed, int bp, int Type, float cooldown ,float duration);
    //A copy constructor, which is likely redundant, made to ensure that the pokemon constructor can successfully give pokemon objects their own fastMove objects.
    fastMove& operator=(const fastMove& entry);
    //Preconditions: None
    //Postconditions: Returns a value of type string
    //Purpose: returns the name of the fast move, for reference.
    std::string getName() const;
    //Preconditions: None
    //Postconditions: Returns a value of type float
    //Purpose: Returns the energy delta of the fast move, for reference.
    int getDelta() const;
    //Preconditions: None
    //Postconditions: Returns a value of type float
    //Purpose: Returns the base power of the fast move, for reference.
    int getPower() const;
    //Preconditions: None
    //Postconditions: Returns a value of type integer
    //Purpose: Returns the type of the fast move, for reference.
    int getType() const;
    //Preconditions: None
    //Postconditions: returns a value of type integer
    //Purpose: Returns the cooldown value of the fast move, for reference.
    float getFastMoveCoolDown() const;
    //Preconditions: None
    //Postconditions: returns a value of type integer
    //Purpose: Returns the duration value of the fast move, for reference.
    float getFastMoveDuration() const;

private:
    //The name of the fast move
    std::string name;
    //The energy delta, or the amount of energy using the move gives you.
    int energyDelta;
    //The base power of the fast move.
    int basePower;
    //The type that the fast move is.
    int type;

    //Edited By Grant Finn. declare the cooldown.
    //The cooldown value of the fast move
    float fastMoveCoolDown;
    //The duration of the fast move
    float fastMoveDuration;
};


#endif
