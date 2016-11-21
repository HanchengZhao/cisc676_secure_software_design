#include <stdio.h>
#include <stdlib.h>

struct Person{
    int age;
    char name[];
}Person;

bool isOlderThan30(const struct Person input){
    if(input.age > 30){
        printf("%c is older than 30", input.name);
        return true;
    }
}

int main(){
    Person person1;
    Person person2;
    
    strcpy(person1.name, "Andy");
    strcpy(person2.name, "Betty");
    person1.age = 33;
    person2.age = 28;
    isOlderThan30(person1);
    isOlderThan30(person2);
    return 0;
}

