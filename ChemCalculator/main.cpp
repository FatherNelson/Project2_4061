#include "stdio.h"

int main(void) {{
    /*Variables. They are initialized with 0's and will be checked in the "if" statements, so we may calculate the other things given the others*/
    float M = 0;
    float m = 0;
    float molar_mass_solute = 0;
    float mass_of_solute = 0;
    float moles_solute = 0;
    float density = 0;
    float volume_solvent = 0;
    float mass_solvent = 0;
    /*Information collection stage, need a way to get some of this data from a corpus, and statements to check if variables are fulfilled*/
    printf("What is the molarity of your solution (mol/L) ? ");
    scanf("%f", &M);
    printf("What is the molar mass of the solute in g/mol? ");
    scanf("%f", &molar_mass_solute);
    printf("What is the molar mass of the solute in g/mol? ");
    scanf("%f", &molar_mass_solute);
    printf("What is the molar mass of the solute in g/mol? ");
    scanf("%f", &molar_mass_solute);
    printf("What is the mass of the solute in grams? ");
    scanf("%f", &mass_of_solute);
    printf("What is the density of the solvent in mg/ml? ");
    scanf("%f", &density);
    printf("What is the mass of the solvent in kg?");
    scanf("%f", &mass_solvent);
    printf("What is the volume of the solvent in ml?");
    scanf("%f", &volume_solvent);
    /*Calculations Section. For now we will only calculate the molality*/
    moles_solute = mass_of_solute / molar_mass_solute;
    mass_solvent = (density * volume_solvent) / 1000;
    M = moles_solute / (volume_solvent/1000);
    m = moles_solute / mass_solvent;
    /*Results printout*/
    printf("The molality of your solution is %f mol/kg \n", m);
    printf("The molarity of your solution is %f M \n", M);
}
return 0;
}

/* Test case: Given 19g of methanol, 150mL solvent with density 0.87 g/ml. The molarity is 4M and the molality is 4.5 mol/kg*/