#ifndef json_navigator_h
#define json_navigator_h

#include <string>
#include <utility>
#include <iostream>
#include <set>
#include <unordered_map>
#include <stack>

using std::string;

// variables and constants
extern string::iterator json_ptr;

const std::set<char> brackets = {'{', '[', '}', ']'};
const std::set<char> json_open = {'[', '{'};
const std::set<char> json_close = {']', '}'};
const std::unordered_map<char, char> close_open = {{']', '['}, {'}', '{'}};
const std::set<char> json_ends = {',', ']', '}'};

// declared functions
void goto_start(string target);
short find_start(string target, short start = 0);
short find_nth(string target, short n);
std::pair<short, short> find_first_2(string target);
void goto_end(string target, short extra = 0);
short find_end(string target, short extra = 0);
std::pair<int, char> find_first_end();
std::pair<short, char> find_first_of(const std::set<char> targets, bool advance = false);
bool starts_with(string target, bool advance = false);
short find_end_dict(bool advance = false);
short find_last_before(char target, char end, bool advance = false);

#endif