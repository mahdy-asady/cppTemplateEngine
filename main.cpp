#include <iostream>
#include "cppTemplate.h"


using namespace std;

/*
#include <mapbox/variant.hpp>
using namespace mapbox::util;
typedef variant<int, double, string, bool> any;
*/

int main()
{
    node r = node();
    r["Name"]= string("Mahdy");
    r["Family"] = string("Asady");
    r["Age"] = 34;
    r["Weight"] = 95.5;
    r["children"][0]["Name"] = string("Ali");
    r["children"][1]["Name"] = string("Asal");
    //r["Alive"] = true;

    string tmplate =
"Hi {{Name}} {{Family}}!\n\
Your children list is:\n\
{{#children}}{{Name}}{{/children}}\n\
with enter:\n\
{{#children}}{{Name}}\n{{/children}}\n\
{{=<< >>=}}\
Your age is: <<Age>>\n\
End";


    //cout << tmplate << endl << endl;

    templateEngine te = templateEngine(tmplate, r);
    cout << te.render() << endl;

    cout << "*********************" << endl;
    return 0;
}
