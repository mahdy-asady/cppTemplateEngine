#ifndef CPPTEMPLATE_H_INCLUDED
#define CPPTEMPLATE_H_INCLUDED


#include <string>
#include <unordered_map>
#include <iostream>
#include <vector>
#include <mapbox/variant.hpp>

using namespace std;
using namespace mapbox::util;

//class node;
//define a variant type that will store everything we need.
typedef variant<int, double, float, string, bool/*,node* */> any;
//it is a helper for print out variant data
struct ResponseVisitor {
    void operator()(int r) const    {cout << r << endl;}
    void operator()(string r) const {cout << r << endl;}
    void operator()(float r) const  {cout << r << endl;}
    void operator()(double r) const  {cout << r << endl;}
    void operator()(bool r) const  {cout << r << endl;}
};

/*struct ResponseVisitor {
    int operator()(int r) const    {return r;}
    string operator()(string r) const {return r;}
    float operator()(float r) const  {return r;}
    double operator()(double r) const  {return r;}
    bool operator()(bool r) const  {return r;}
};*/

/*base data class. all data will be stored in this class as a tree
just create a root node, then add any leaf by using braces for child name.

node r = node(strig("Test"));
    r["Name"]= string("Mahdy");
    r["Name"]["Family"] = string("Asady");
    r["Age"] = 34;
    r["Weight"] = 95.5;
    r["Married"] = true;

*/
typedef variant<string, int> key;
class node {
private:
    unordered_map<key, node*> children;
    any data;
    ResponseVisitor visitor;
public:
    node(){data = string("");}
    node(any d):data(d){}
    ~node() {}

    node &operator[](key name){
        auto &rv = children[name];
        if (!rv) rv = new node;
        return (*rv);
    }
    void operator=(any d) {data = d;};
    //todo
    void operator=(node d) {};

    bool exists(key name) {
        try{
            auto &rv = children.at(name);
            return true;
        }
        catch(const std::out_of_range& e) {
            return false;
        }
    }
    any get() {
        return data;
    }
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
    struct sections {
        string content;
        bool isTag;
        int level;
    };

    string templateSource;
    node parameters;
    vector<sections> contents;


    void splitTemplate() {

        contents.clear();

        string sTag = "{{"; //start tag mark
        string eTag = "}}"; //end tag mark
        size_t tagStartPos = 0, tagEndPos = 0;

        int level = 0;


        while(tagStartPos != string::npos) {
            size_t contentStartPos = tagEndPos;
            tagStartPos = templateSource.find(sTag, tagEndPos);//finding next start tag mark position
            if(tagStartPos != string::npos) {//if there is an open tag ...
                tagEndPos=templateSource.find(eTag, tagStartPos+sTag.length());//finding closing tag mark
                if(tagEndPos != string::npos) {
                    //now we have start and end of a tag
                    //if there is no content before this tag just add empty string to content tag
                    string contentSlice = templateSource.substr(contentStartPos, tagStartPos-contentStartPos);
                    contents.push_back({contentSlice, false, level});

                    //get tag label
                    tagStartPos += sTag.length();
                    string tagSlice = templateSource.substr(tagStartPos, tagEndPos-tagStartPos);

                    if(tagSlice[0] == '=' && tagSlice[tagSlice.length()-1] == '=') {/*change delimeter*/
                        //TODO
                        sTag = tagSlice.substr(1, 2);
                        eTag = tagSlice.substr(tagSlice.length()-3, 2);
                    }
                    else {
                        //if our tag started with a / we should go back one level up before this tag insertion
                        if(tagSlice[0] == '/') level--;

                        contents.push_back({tagSlice,true, level});

                        //if our tag started with a # we should go one level down after this tag insertion
                        if(tagSlice[0] == '#') level++;
                    }

                    tagEndPos += eTag.length();
                }
            }
        }
        contents.push_back({templateSource.substr(tagEndPos, templateSource.length()-tagEndPos), false, level});

        cout << "Content size: " << contents.size() << "." << endl;
        cout << templateSource << endl;

        vector<sections>::iterator it = contents.begin();
        cout << "Contents:" << endl;
        while(it != contents.end()) {
            cout << "'" << (*it++).content  << "'" << endl;
        }

    }


    string renderSection(int startIndex, node params){
        string result = "";
        int currentLevel = contents[startIndex].level;
        int i = startIndex;
        while(contents[i].level >= currentLevel && i<contents.size()) {
            if(!contents[i].isTag) {
                result += contents[i].content;
            }
            else {
                if(contents[i].content[0] == '#') { //we got a section. just loop through children items
                    string p = contents[i].content.substr(1);
                    int childrenCounter = 0;
                    while(params[p].exists(childrenCounter)){
                        result += renderSection(i+1, params[p][childrenCounter]);
                        childrenCounter++;
                    }
                    while(contents[++i].level>currentLevel);
                }
                else {
                    //result += contents[i].content;
                    result += params[contents[i].content].get().match(
                                                                      [](string r){return r;},
                                                                      [](int r){return to_string(r);},
                                                                      [](float r){return to_string(r);},
                                                                      [](double r){return to_string(r);},
                                                                      [](bool r){return to_string(r);}
                                                                    );
                    //result += params[contents[i].content].get().get();
                }
            }
            i++;
        }

        return result;
    }
public:
    templateEngine() {}
    templateEngine(string tmplt, node params):templateSource(tmplt), parameters(params) {
        splitTemplate();
    }

    string render() {
        return renderSection(0, parameters);
    }
};

#endif // CPPTEMPLATE_H_INCLUDED
