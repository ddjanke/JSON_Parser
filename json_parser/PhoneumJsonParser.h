
#ifndef PhoneumJsonParser_h
#define PhoneumJsonParser_h

#include "phoneum_json_parser.h"

extern std::set<std::string> db_keys;

class PhoneumJsonParser {
    public:
        std::string input_json;
        std::string output_json;
        std::string app;
        PhoneumJsonParser(const std::string &input);
        void parse();
        void set_app(const string AppName);
        const std::string &getOutputJson() const;
};

#endif