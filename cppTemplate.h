#ifndef CPPTEMPLATE_H_INCLUDED
#define CPPTEMPLATE_H_INCLUDED

#include <string>
#include <unordered_map>
#include <iostream>

using namespace std;
typedef string any;

class node {
private:
    unordered_map<string, node*> children;
    any data;
public:
    node(){};
    node(any d):data(d){};
    ~node() {};

    node &operator[](string name){
        auto &rv = children[name];
        if (!rv) rv = new node;
        return (*rv);
    }
    void operator=(any d) {data = d;};

    void print() {
        cout << "Print:" << data << endl;
        return "";
    };
};























/*
    Template engine implementation
*/

/*
char *render(char *template, char **paramNames,...) {
}
*/

#endif // CPPTEMPLATE_H_INCLUDED
