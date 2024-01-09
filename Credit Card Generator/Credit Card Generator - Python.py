# The purpose of this program is to generate a credit card which includes a random 
# credit card number for a user by using the name, expiration month, and expiration year
# that is to be entered into the program by the user.
# The user can choose to generate as many credit cards as he/she wants.

import random
print("""
CSC 122 - 02 - Final Exam Program
By Dylan Binu
26 April 2020
""")

cYr = 20 # Assigns a value of 20 to this variable, which will later be used.
         # when checking for the appropriate expiration year for the credit card.
print("""
            CREDIT CARD GENERATOR
            ---------------------          

This program randomly generates a 16-digit credit card number.
When prompted, please enter the requested information.
""")      # Prints introductory instructions for the user.

def main():      # Defining main function.
    firstname = input("Enter cardholder's first name: ")      # Prompting to enter first name.
    middleinitial = input("Enter cardholder's middle initial: ")      # Prompting to enter middle initial.
    lastname = input("Enter cardholder's last name: ")        # Prompting to enter last name.
    expiremonth = int(input("Enter card's expiration month [as MM]: "))     # Prompting to enter expiration month.
        
    while expiremonth > 13 or expiremonth <= 0:         # Using a while loop to check if expiration month is possible.
        print("       ERROR: Invalid MONTH...MUST be between 1 and 12!")    # If impossible expiration month is entered, this message will pop up.
        expiremonth = int(input("Enter card's expiration month [as MM]: "))     # User will be given a chance to re-enter an appropriate expiration month.
    
    expireyear = int(input("Enter card's expiration year [as YY]: "))     # Prompting to enter expiration year.

    while expireyear <= 0 or expireyear <= cYr or expireyear >= 100:    # Using the cYr variable in a while loop to check if expiration year is possible.
        print("       ERROR: Invalid YEAR... Must be between 21 and 99")     # If impossible expiration year is entered, this message will pop up/
        expireyear = int(input("Enter card's expiration year [as YY]: "))     # User will be given a chance to re-enter an appropriate expiration year.

    correct = input("        Is the information entered correct [Y/N]: ")    # User is then given a chance to validate the information.

    while correct.upper() == "N":         # If user chooses to say that the information is incorrect,user gets a chance to correct that information.
        correction = input("""What information was incorrect? Please enter the appropriate letter:   

Type "F" for First name
Type "I" for Middle initial
Type "L" for Last name
Type "M" for Expiry Month
Type "Y" for Expiry Year

Please enter the appropriate letter [F, I, L, M, Y]: """)    # Instructions that teach the user how to correct specific pieces of information.
    
        if correction.upper() == "F":           # If user chooses to change the first name by selecting the letter "F", they will be given the chance to do so.
            firstname = input("Enter cardholder's first name: ")
            correct = input("Is all the information correct now? [Y/N]: ")
        elif correction.upper() == "I":         # If user chooses to change the middle initial by selecting the letter "I", they will be given the chance to do so.
            middleinitial = input("Enter cardholder's middle initial: ")
            correct = input("Is all the information correct now? [Y/N]: ")
        elif correction.upper() == "L":         # If user chooses to change the last name by selecting the letter "L", they will be given the chance to do so.
            lastname = input("Enter cardholder's last name: ")
            correct = input("Is all the information correct now? [Y/N]: ")
        elif correction.upper() == "M":         # If user chooses to change the expiration month by selecting the letter "M", they will be given the chance to do so.
            expiremonth = int(input("Enter card's expiration month [as MM]: "))
        
            while expiremonth > 13 or expiremonth <= 0:        # Validation of expiration month.
                print("       ERROR: Invalid MONTH...MUST be between 1 and 12!")
                expiremonth = int(input("Enter card's expiration month [as MM]: "))
            correct = input("Is all the information correct now? [Y/N]: ")

        elif correction.upper() == "Y":         # If user chooses to change the expiration year by selecting the letter "Y", they will be given the chance to do so.
            expireyear = int(input("Enter card's expiration year [as YY]: "))

            while expireyear <= 0 or expireyear <= cYr or expireyear >= 100:         # Validation of expiration year.
                print("       ERROR: Invalid YEAR... Must be between 21 and 99")
                expireyear = int(input("Enter card's expiration year [as YY]: "))
            correct = input("Is all the information correct now? [Y/N]: ")
        else:
            correction = input("ERROR. Incorrect letter! Letter has to be 'F', 'I', 'L', 'M', or 'Y'")       # If user enter anything besides the letters F, I, L, M, or Y, this message pops up.


    num1 = random.randint(0,9)          # 16 different variables that generate 16 different numbers by using the "randint" funtion from the "random" module.
    num2 = random.randint(0,9)
    num3 = random.randint(0,9)          # Each number is generated from within the range of 0 and 9.
    num4 = random.randint(0,9)
    num5 = random.randint(0,9)
    num6 = random.randint(0,9)
    num7 = random.randint(0,9)
    num8 = random.randint(0,9)
    num9 = random.randint(0,9)
    num10 = random.randint(0,9)
    num11 = random.randint(0,9)
    num12 = random.randint(0,9)
    num13 = random.randint(0,9)
    num14 = random.randint(0,9)
    num15 = random.randint(0,9)
    num16 = random.randint(0,9)

    creditnum = [num1, num2, num3, num4, num5, num6, num7, num8, num9, num10, num11, num12, num13, num14, num15, num16]    # List that contains 16 random numbers.

    def card():      # Defining "card" function and storing the design of the credit card inside of it.
        print("""
 -------------------------------------------
    VISA CREDIT CARD | CASH REWARDS
    --------
    |######|
    |######|
    |######|
    --------
""")
        print("   ", str(num1) + str(num2) + str(num3) + str(num4), str(num5) + str(num6) + str(num7) + str(num8), str(num9) + str(num10) + str(num11) + str(num12), str(num13) + str(num14) + str(num15) + str(num16))
        print("                               VALID")         # Random numbers are set up to match the 16-digit design on a credit card.
        print("                               THRU", " " + "|>", str(expiremonth) + "/" + str(expireyear))    # Expiration month and year are set up.
        print("    " + str(firstname.upper()), str(middleinitial.upper()), str(lastname.upper()))        # Name (First name, initial, and last name) set up.
        print(" -------------------------------------------")
    card()           # Calling the card function.
main()          # Calling the main function. 
print()
generate = input("Would you like to generate another credit card [Y/N]: ")      # Asking user if they would like to generate another credit card.
print()

if generate.upper() == "Y":      # If user wants to generate another credit card,
    main()                       # The main function is called.
else:                                              # If user does not want to generate another credit card,
    print("Thank you for using this credit card generator!")    # This "Thank you" message is displayed.

# Thank you so much for all your dedication, Mrs. Brown! God bless you.

