//phoneum_json_parser.h
#ifndef phoneum_json_parser_h
#define phoneum_json_parser_h

#include <time.h>

#include <string>
#include <vector>
#include <unordered_map>
#include <stack>
#include <sstream>
#include <fstream>
#include <iostream>
#include <utility>
#include <set>
#include <algorithm>

#include "json_navigator.h"

using std::string;

// constants

const double levels[] = { 1., 3.5, 8., 15., 25., 39., 57., 79., 106., // 1 - 10
                        139., 177., 221., 271., 330, 393., 470., 550., 640., 750., // 11 - 20
                        870., 1010., 1165., 1335., 1525., 1735., 1965., 2215., 2500., 2800., // 21 - 30
                        3130., 3480., 3860., 4260., 4700., 5170., 5670., 6220., 6800., 7400.};

const std::set<std::string> statlist = {"cp", "gm", "v", "co", "c", "d", "b",
                                        "xp", "ts", "p", "q", "te"};

const std::unordered_map<string,short> ref_rewards= {{"c", 1000}, {"l3", 200}, {"lx", 1500},
                                                {"e2", 200}, {"e5", 500}, {"e10", 1000}};

// declared functions
string user_parser();
string player_data_parser();
void print_next_n(int n);
string get_file_contents(const string *filename);
bool check_is_num(string &num);
string str_btwn_quotes(bool advance = true);
string str_with_quotes(bool advance = true);
bool str_contains(string str, char c);
string dict_flatten_key(std::string keyword);
string collectibles();
string items();
char quest_check();
string private_stats();
string stats();
string details();
long progress_rewards(short level, short amt);
string level_rewards(double xp);
string quest_rewards(string quests, string type, short flip, short other);
string referrals();
inline void check_map_add(std::unordered_map<string, double> &txs,
                            std::set<string> &txs_keys, string key, double amt);
string staking();
string transactions();
string ledger();
string number_string(double num);
string map_to_string(std::unordered_map<string, double> &txs,
                        std::set<string> &txs_keys);



#endif