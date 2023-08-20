#ifndef CHARGEDMOVE_H
#define CHARGEDMOVE_H
#include <string> // this calss uses strings for names, and so we need to include strings up here

class chargedMove {
public:
    //The default constructor for a charged move. Not intended to be used.
    chargedMove();
    //The preferred constructor for the fast move. has its parameters to be input in this order:
    //The name of the charged move, its base power, its energy cost, and Type that the charged move is.
    chargedMove(std::string Name, int bp, int ce, int Type);
    //A precautionary equivalency operator made to ensure that pokemon class objects could successfully obtain their own chargedMove object.
    chargedMove& operator =(const chargedMove& entry);
    //Preconditions: None
    //Postconditions: Returns a value of type string
    //purpose: Returns the name of the charged move for reference.
    std::string getName() const;
    //Preconditions: None
    //Postconditions: Returns a value of type integer
    //purpose: Returns the base power of the charged move, for reference.
    int getPower() const;
    //Preconditions: None
    //Postconditions: Returns a value of type integer
    //purpose:Returns the energy cost required to use the move for reference.
    int getCharge() const;
    //Preconditions: None
    //Postconditions: Returns a value of type integer
    //purpose: Returns the integer denoting the type of the move, for reference.
    int getType() const;
private:
    //the name of the charged move
    std::string name;
    //the base power of the charged move
    int basePower;
    //the charged energy, or the cost required to use this particular charged move
    int chargeEnergy;
    //the integer denoting the type that this charged move is.
    int type;
};


#endif
