#include <iostream>
#include "cppTemplate.h"


using namespace std;

int main()
{
    node r = node("Test");
    r= "Test2";
    //static node<string> a("Mahdy");
    r["Name"]= "Mahdy";
    //r["Name"]= "Ali";
    //r["Age"] = node<int>(34);
    //r["Sex"] = node<string>("Male");
    //r["children"]["1"] = new node<string>("Ali");
    //r["Name"]->add("Family", new node<string>("Asady"));
    r["Name"]["Family"] = "Asady";


    r.print();
    r["Name"].print();
    r["Name"]["Family"].print();

    //(*r["Name"])["Family"].print();
    //r["Name"]->operator[]("Family")->print();
    //nodeTemplate *b = r["Name"];
    //nodeTemplate *c = b["Family"];
    //b["Family"]->print();
    //r["Age"]->print();
    //r["Sex"]->print();

    //r["children"]->print();
    //r["children"]["1"]->print();
    //r["blabla"]->print();
    cout << "*********************" << endl;
    return 0;
}
