#include "Timer.h"

Timer t;

struct books
{ 
  String name;
  char author[30];
  int ID;
}Mybook;




void setup() 
{
  Serial.begin(9600);
  // put your setup code here, to run once:
  Mybook.name =  "Girl in the train";
  strcpy(Mybook.author, "Gone"); 

  int tickEvent = t.every(2000, printBook, (void*)&Mybook);
}



void loop()
{
  t.update();
}

void printBook(void* context)
{
    struct books * theBook = (struct books *)context;
    Serial.println("Name of the book is: ");
    Serial.println(theBook->name);
    Serial.println("author is: ");
    Serial.println(theBook->author);
    Serial.println("ID is: ");
    Serial.println(theBook->ID);
}
