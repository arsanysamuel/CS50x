#include <cs50.h>
#include <stdio.h>
#include <string.h>

typedef struct  // typedef: define a new datatype, struct: the type of this datatype (it's not int or char)
{
    // Content of this datatype
    string name;
    string number;
}
person;  // the new name of this datatype

int main(void)
{
    //string names[] = {"Carter", "David"};
    //string numbers[] = {"+1-234-567-8910", "+0-198-765-4321"};
    // introducing structs (data structure)
    person people[2];
    people[0].name = "Carter";
    people[0].number = "+1-234-567-8910";
    people[1].name = "David";
    people[1].number = "+0-198-765-4321";

    for (int i = 0; i < 2; i++)
    {
        if(strcmp(people[i].name, "David") == 0)
        {
            printf("Phone number: %s\n", people[i].number);
        }
        else
        {
            printf("Not found\n");
            return 1;
        }
    }
}