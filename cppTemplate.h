#ifndef CPPTEMPLATE_H_INCLUDED
#define CPPTEMPLATE_H_INCLUDED


#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <mapbox/variant.hpp>

using namespace std;
using namespace mapbox::util;


//define a variant type that will store everything we need.
typedef variant<int, double, float, string, bool> any;
//it is a helper for print out variant data
struct ResponseVisitor {
    void operator()(int r) const    {cout << r << endl;}
    void operator()(string r) const {cout << r << endl;}
    void operator()(float r) const  {cout << r << endl;}
    void operator()(double r) const  {cout << r << endl;}
    void operator()(bool r) const  {cout << r << endl;}
};


/*base data class. all data will be stored in this class as a tree
just create a root node, then add any leaf by using braces for child name.

node r = node(strig("Test"));
    r["Name"]= string("Mahdy");
    r["Name"]["Family"] = string("Asady");
    r["Age"] = 34;
    r["Weight"] = 95.5;
    r["Married"] = true;

*/
class node {
private:
    unordered_map<string, node*> children;
    any data;
    ResponseVisitor visitor;
public:
    node(){data = string("");}
    node(any d):data(d){}
    ~node() {}

    node &operator[](string name){
        auto &rv = children[name];
        if (!rv) rv = new node;
        return (*rv);
    }
    void operator=(any d) {data = d;};
    //todo
    void operator=(node d) {};

    void print() {
        cout << "Print: ";
        apply_visitor(visitor, data);
    }
};























/*
    Template engine implementation
*/

class templateEngine{
private:
    string templateSource;
    node parameters;
    vector<string> contents, tags;


    void splitTemplate() {

        contents.clear();
        tags.clear();

        string sTag = "{{";
        string eTag = "}}";
        size_t tagStartPos = 0, tagEndPos = 0;

        while(tagStartPos != string::npos) {
            size_t contentStartPos = tagEndPos;
            tagStartPos = templateSource.find(sTag, tagStartPos);
            if(tagStartPos != string::npos) {
                tagEndPos=templateSource.find(eTag, tagStartPos+sTag.length());
                if(tagEndPos != string::npos) {
                    //now we have start and end of a tag
                    //if there is no content before this tag just add empty string to content tag
                    contents.push_back(templateSource.substr(contentStartPos, tagStartPos-contentStartPos));

                    tagStartPos += sTag.length();
                    tags.push_back(templateSource.substr(tagStartPos, tagEndPos-tagStartPos));

                    tagEndPos += eTag.length();
                }
            }
        }
        contents.push_back(templateSource.substr(tagEndPos, templateSource.length()-tagEndPos));

        cout << "Content size: " << contents.size() << ". Tags size: " << tags.size() << "." << endl;
        /*cout << templateSource << endl;

        vector<string>::iterator it = contents.begin();
        cout << "Contents:" << endl;
        while(it != contents.end()) {
            cout << "'" << *it++  << "'" << endl;
        }

        it = tags.begin();
        cout << "Tags:" << endl;
        while(it != tags.end()) {
            cout << *it++ << endl;
        }
*/
    }
public:
    templateEngine() {}
    templateEngine(string tmplt, node params):templateSource(tmplt), parameters(params) {
        splitTemplate();
    }

    string render() {
        string data = "";
        for(int i = 0; i < contents.size(); i++) {
            data += contents[i];
        }
        return data;
    }
};

#endif // CPPTEMPLATE_H_INCLUDED
