/*******************************************************************************
 *    INCLUDED FILES
 ******************************************************************************/
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "gardener.h"

/*******************************************************************************
 *    DATA STRUCTURES
 ******************************************************************************/
plant my_plants_[] = {{.species = "Renanthera monachica",
                   .water_amount = 0.2,
                   .watering_period = 86400,
                   .start_date = 123,
                   .last_watering_date = 246},
                  {.species = "Arachnis annamensis",
                   .water_amount = 0.15,
                   .watering_period = 43200,
                   .start_date = 246,
                   .last_watering_date = 86400}};

/*******************************************************************************
 *    PUBLIC API
 ******************************************************************************/
#ifdef TEST
int main__(void)
#else
int main(void)
#endif
{
  size_t i, plants_amount = sizeof(my_plants_) / sizeof(plant);
  bool was_watered;


  for (i = 0; i < plants_amount; i++) {
    was_watered = water_plant(&my_plants_[i]);

    if (was_watered)
      printf("%s was watered.\n", my_plants_[i].species);
    else
      printf("%s wasn't watered.\n", my_plants_[i].species);
   }

  return 0;
}
