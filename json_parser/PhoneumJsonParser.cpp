
#include "PhoneumJsonParser.h"

using std::string;

std::set<string> db_keys = {"\"t\":", "\"i\":", "\"p\":", "\"dr\":",
                                    "\"c\":", "\"w\":", "\"s\":", "\"q\":",
                                    "\"d\":", "\"l\":", "\"r\":", "\"st\":"};

PhoneumJsonParser::PhoneumJsonParser(const string &input) {
    input_json = input;
    app = "";
}

void PhoneumJsonParser::parse() {
    if (app != "CT") db_keys.erase("\"i\":");

    json_ptr = input_json.begin();
    output_json = user_parser();
    if (app != "CT") db_keys.insert("\"i\":");
    
}

void PhoneumJsonParser::set_app(const string AppName) {
    app = AppName;
    
}

const string &PhoneumJsonParser::getOutputJson() const { return output_json; }