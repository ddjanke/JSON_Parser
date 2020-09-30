#include "json_navigator.h"

using std::string;

extern string::iterator json_ptr{};

// Advances string pointer to memory location of start of target string
void goto_start(string target){
    short i = 0;
    short length = target.length();
    while (*(json_ptr + i) != '\0'){
        if (i == length) break;
        if (*(json_ptr + i) == target.at(i)){
            ++i;
        }
        else{
            if (i != 0) i = 0;
            ++json_ptr;
        }
    }
}

// Returns distance (positive) from string pointer to start of target string
short find_start(string target, short start){
    short i = 0;
    short length = target.length();
    while (*(json_ptr + start + i) != '\0'){
        if (i == length) return start;
        if (*(json_ptr + start + i) == target.at(i)){
            ++i;
        }
        else{
            if (i != 0) i = 0;
            ++start;
        }
    }
    return -1;
}

// Returns index of nth instance of target string after string pointer
short find_nth(string target, short n = 1){
    short count = 0;
    short index = 0;
    while (count < n){
        index = find_start(target, index + (count > 0));
        ++count;
    }
    return index;
}

// Returns pair of indexes for the first two occurances of target string after string pointer
std::pair<short, short> find_first_2(string target){
    short count = 0;
    short index[2] = {0, 0};
    while (count < 2){
        index[count] = find_start(target, index[0] + count);
        ++count;
    }
    std::pair<short, short> ends = {index[0], index[1]};
    return ends;
}

// Advances string pointer to first memory location after target string end
void goto_end(string target, short extra){
    short i = 0;
    short length = target.length();
    while (*(json_ptr + i) != '\0'){
        if (i == length){
            json_ptr += (i + extra);
            break;
        }
        if (*(json_ptr + i) == target.at(i)){
            ++i;
        }
        else{
            if (i != 0) i = 0;
            ++json_ptr;
        }
    }
}

// Returns distance from string pointer to memory location after end of the target string
short find_end(string target, short start){
    short i = 0;
    short length = target.length();
    while (*(json_ptr + start + i) != '\0'){
        if (i == length) return start + i;
        if (*(json_ptr + start + i) == target.at(i)){
            ++i;
        }
        else{
            if (i != 0) i = 0;
            ++start;
        }
    }
    return -1;
}

// Return relative index and identity of first 'json_ends' char (',', '}', ']')
std::pair<int, char> find_first_end(){
    int least_index = 0;
    char least_char = '}';
    while (least_index < 1000){
        least_char = *(json_ptr + least_index);
        if (json_ends.find(least_char) != json_ends.end()){
            break;
        }
        least_index++;
    }
    std::pair<int, char> first = {least_index, least_char};
    return first;
}

// Return relative index and identity of first instance of a char in a given set
// If advance == true, string pointer advances to the found character
std::pair<short, char> find_first_of(const std::set<char> targets, bool advance){
    short least_index = 0;
    char least_char;
    while (least_index < 1000){
        least_char = *(json_ptr + least_index);
        if (targets.find(least_char) != targets.end()){
            break;
        }
        least_index++;
    }
    if (advance){
        json_ptr += least_index;
    }
    std::pair<short, char> first = {least_index, least_char};
    return first;
}

// Returns true if the first characters starting from the string pointer are the target string
// If advance == true, string pointer advances to memory location after target
bool starts_with(string target, bool advance) {
    short i = 0;
    short length = target.length();
    while (i < length) {
        if (*(json_ptr + i) == target.at(i)) {
            ++i;
        }
        else {
            return false;
        }
    }
    if (advance) json_ptr += length; 
    return true;
}

// Returns the relative index of the memory location after the end of the current dictionary item
// A full dictionary is found when each opening brace is matched with a corresponding closing brace
// e.g., { [ { } ] }
short find_end_dict(bool advance) {
    int index = 0;
    std::stack<char> open_brackets{};
    std::pair<short,char> next;
    do {
        next = find_first_of(brackets);
        if (next.second == '{' || next.second == '[') open_brackets.push(next.second);
        else open_brackets.pop();
        json_ptr += next.first + 1;
        index += next.first + 1;
    } while (!open_brackets.empty());
    if (!advance) {
        json_ptr -= index;
        return index;
    }
    else return 0;
}

// Returns the relative index of the last instance of the target char before the occurance of
// the end char. If advance == true, string pointer advances to last target char
short find_last_before(char target, char end, bool advance) {
    short i = 1, location = -1;
    while (*(json_ptr + i) != end) {
        if (*(json_ptr + i) == target) location = i;
        ++i;
    }
    if (advance && location >= 0) {
        json_ptr += location;
        location = 0;
    }
    return location;
}