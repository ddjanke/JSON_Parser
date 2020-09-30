// Devon Janke
// Parses a file containing a json string into a json structure
#include "PhoneumJsonParser.h"

using std::cout;
using std::endl;
using std::string;

string UID = "";


int main() {
    const string filename = "ct_data.json";
    string file_data = get_file_contents(&filename);
    std::cout << "data read" << std::endl;
    std::cout << time(0) << " " << file_data.length() << endl;
    PhoneumJsonParser pjp(file_data); 
    pjp.set_app("CT");
    pjp.parse(); 
    //json_ptr = file_data.begin();
    //user_parser();
    std::cout << "data parsed" << std::endl;
    std::cout << time(0) << " " << pjp.output_json.length() << endl;
    return 0;
}

string user_parser() {
    string new_json = "{", uid, player_data;
    short uid_end = 0;
    //print_next_n(100);
    while (*json_ptr != '}'){
        ++json_ptr;
        uid_end = find_end(":");
        uid = string(json_ptr, json_ptr + uid_end);
        UID = uid;
        json_ptr += uid_end;
        //cout << uid << " " << endl; 
        player_data = player_data_parser();
        //cout << uid << " " << player_data << endl;
        new_json.append(uid).append(player_data);
        new_json.append(string({*json_ptr}));
        //cout << new_json << endl;
        //std::cin.ignore();
    }
    //cout << string(new_json.end() - 1000, new_json.end()) << endl;
    //std::cin.ignore();
    return new_json;
}

string player_data_parser() {
    string dict = "{", key, value;
    short key_end, key_start;
    char quest_type;
    bool skip = false;

    while (*json_ptr != '}'){
        key_start = find_start("\"");
        key_end = find_end(":");
        key = string(json_ptr + key_start, json_ptr + key_end);
        //if (UID == "\"LVF4DeFgNO\":") {
        //cout << key << endl;
        //print_next_n(2000);
        //}
        json_ptr += key_end;
        skip = false;
        if (db_keys.find(key) == db_keys.end()) skip = true;
        else if (key == "\"t\":") value = transactions();
        else if (key == "\"i\":") { key = ""; value = items(); }
        else if (key == "\"c\":") { key = "";  value = collectibles(); }
        else if (key == "\"w\":") value = dict_flatten_key("\"a\":");
        else if (key == "\"s\":") { key = ""; value = stats(); }
        else if (key == "\"p\":") { key = ""; value = private_stats(); }
        else if (key == "\"l\":") value = ledger(); 
        else if (key == "\"r\":") { key = ""; value = referrals(); }
        else if (key == "\"st\":") value = staking();
        else if (key == "\"dr\":") {
            key_end = find_first_of(json_ends).first;
            json_ptr += key_end;
            continue; }
        else if (key == "\"q\":") {
            key = "\"quests\":";
            quest_type = quest_check();
            //print_next_n(100);
            switch(quest_type){
                case('p') :
                    value = dict_flatten_key("\"c\":");
                    ++json_ptr;
                    break;
                case('}') : continue;
            }
            if (!value.empty()) key = quest_rewards(value, "quest", 10, 100).append(key);

        }
        else if (key == "\"d\":") { key = ""; value = details(); }
        else value = "";

        if (!skip) {
            if (dict.length() > 1 && value != "") dict.append(",");
            if (!value.empty()) dict.append(key).append(value);
        }
        else find_end_dict(true);
        
    }
    dict.append("}");
    ++json_ptr;
    return dict;
}

// Print next n characters from string pointer
void print_next_n(int n) {
    for (int i = 0; i < n; ++i){
        std::cout << *(json_ptr + i);
    }
    std::cout << std::endl;
}

// Read contents of a file (used for testing/debuggins)
string get_file_contents(const string *filename) {
    std::ifstream in (*filename, std::ios::in | std::ios::binary);
    if (in){
        string contents;
        in.seekg(0, std::ios::end);
        contents.resize(in.tellg());
        in.seekg(0, std::ios::beg);
        in.read(&contents[0], contents.size());
        in.close();
        return(contents);
    }
    else return "File not found.";
}

// Check if string is numeric or a string (starts with ")
inline bool check_is_num(string &num) {
    return num[0] != '\"';
}

// Returns the string between the next two quotation marks
// If advance == true, string pointer advances to memory loacation after second "
inline string str_btwn_quotes(bool advance) {
    std::pair<short, short> ends = find_first_2("\"");
    string quote_string = string(json_ptr + ends.first + 1, json_ptr + ends.second);
    if (advance) json_ptr += (ends.second + 1);
    return quote_string;
}

// Returns the string between the next two quotation marks, including the start and end "
// If advance == true, string pointer advances to memory loacation after second "
inline string str_with_quotes(bool advance) {
    std::pair<short, short> ends = find_first_2("\"");
    string quote_string = string(json_ptr + ends.first, json_ptr + ends.second + 1);
    if (advance) json_ptr += (ends.second + 1);
    return quote_string;
}

// Returns true if string str contains the given char c
bool str_contains(string str, char c) {
    return (str.find(c) != string::npos);
}

// Returns a string that is a flattened nested dictionary, returning only the value of interest
// instead of the entire sub-dictionary
string dict_flatten_key(string keyword) {
    string dict_string = "", key, value;
    short end;
    while (*json_ptr != '}') {
        //if (UID == "\"LVF4DeFgNO\":") {
        //print_next_n(100);
        //}
        ++json_ptr;
        end = find_end(":");
        key = string(json_ptr + find_start("\""), json_ptr + end);
        json_ptr += end;
        end = find_end(keyword);
        if (end < find_end("}")) {
            json_ptr += end;
            end = find_first_of(json_ends).first;
            value = string(json_ptr, json_ptr + end);
            if (!check_is_num(value)) {
                cout << "Error: " << UID << " " << key << endl;
                value = "0";
            }
            goto_end("}");
            if (!dict_string.empty()) dict_string.append(",");
            dict_string.append(key).append(value);
        }
        else goto_end("}");
    }
    ++json_ptr;
    if (!dict_string.empty()) dict_string =
        string("{").append(dict_string).append("}");
    return dict_string;
}

// Returns count of total collectibles and "ruby" item
string collectibles() {
    string key, amt_str;
    string dict_string = "";
    short collectibles = 0, ruby = 0, val_end, amt;
    do{
        key = str_btwn_quotes();
        goto_end("\"a\":");
        val_end = find_first_of(json_ends).first;
        amt_str = string(json_ptr, json_ptr + val_end);
        if (check_is_num(amt_str)) amt = stoi(amt_str);
        else {
            cout << "Error: " << UID << " c " << key << endl;
            amt = 0;
        }
        json_ptr += val_end + 1;
        if (key == "r") ruby = amt;
        else collectibles += amt;
    }while(*json_ptr != '}');
    dict_string.append("\"ruby\":").append(std::to_string(ruby));
    dict_string.append(",\"collectibles\":").append(std::to_string(collectibles));
    ++json_ptr;
    return dict_string;
}

// Returns a string with key/value entries for all of the user's items
// Keys and magnets/chests are counted separately
string items() {
    string key, amt_str;
    string dict_string = "";
    short items = 0, magnet = 0, phoneumKey = 0, val_end, amt;
    std::set<char> ikey = {'a', '}'};
    std::pair<short, char> first;
    do{
        key = str_btwn_quotes();
        json_ptr += 2;
        first = find_first_of(ikey);
        if (first.second == '}') json_ptr += first.first + 1;
        else {
            switch(key[0]) {
                case('m'):
                    goto_end("\"a\":");
                    val_end = find_first_of(json_ends).first;
                    amt_str = string(json_ptr, json_ptr + val_end);
                    if (check_is_num(amt_str)) amt = stoi(amt_str);
                    else {
                        cout << "Error: " << UID << " items m" << endl;
                        amt = 0;
                    }
                    json_ptr += val_end + 1;
                    magnet = amt;
                    break;
                case('k'):
                    goto_end("\"a\":");
                    val_end = find_first_of(json_ends).first;
                    amt_str = string(json_ptr, json_ptr + val_end);
                    if (check_is_num(amt_str)) amt = stoi(amt_str);
                    else {
                        cout << "Error: " << UID << " items k" << endl;
                        amt = 0;
                    }
                    json_ptr += val_end + 1;
                    phoneumKey = amt;
                    break;
                case('q'):
                    goto_end("\"a\":");
                    val_end = find_first_of(json_ends).first;
                    amt_str = string(json_ptr, json_ptr + val_end);
                    if (check_is_num(amt_str)) amt = stoi(amt_str);
                    else {
                        cout << "Error: " << UID << " items m" << endl;
                        amt = 0;
                    }
                    json_ptr += val_end + 1;
                    items += amt;
                    break;
                default:
                    goto_end("\"a\":");
                    val_end = find_first_of(json_ends).first;
                    amt_str = string(json_ptr, json_ptr + val_end);
                    if (check_is_num(amt_str)) amt = stoi(amt_str);
                    else {
                        cout << "Error: " << UID << " items m" << endl;
                        amt = 0;
                    }
                    goto_end("}");
                    items += amt;

            }
        }
    }while(*json_ptr != '}');
    ++json_ptr;

    if (items != 0)
        dict_string = string("\"items\":").append(std::to_string(items));

    if (magnet != 0) {
        if (dict_string.length() > 0) dict_string.append(",");
        dict_string.append("\"magnet\":").append(std::to_string(magnet));
    }
    if (phoneumKey != 0) {
        if (dict_string.length() > 0) dict_string.append(",");
        dict_string.append("\"keys\":").append(std::to_string(phoneumKey));
    }

    return dict_string;
}

// Checks if the user has any progressive quests in progress, returns 'p' if so, '}' if not
char quest_check() {
    string first_key = str_btwn_quotes();
    if (first_key == "k") find_end_dict(true);
    if (first_key == "p") return 'p';

    const std::set<char> q_char = {'p', '}'};
    std::pair<short, char> start_char = find_first_of(q_char);
    json_ptr += start_char.first + 2;
    return start_char.second;
}

// Returns string of flattened private stats
string private_stats() {
    string stat_str = "", key;
    short value_end;
    while (*json_ptr != '}'){
        ++json_ptr;
        key = str_btwn_quotes(false);
        if (key == "q"){
            goto_end("\":");
            value_end = find_end("}");
            string qr = string(json_ptr, json_ptr + value_end);
            stat_str.append(string("\"questReset\":").append(qr));
            key = quest_rewards(qr, "reset", progress_rewards(20, 10),
                                progress_rewards(10, 100));
            stat_str = key.append(stat_str);
            json_ptr += value_end;
			if (*json_ptr != '}') stat_str.append(",");
        }
        else if (key == "r") {
            key = "\"rubySold\":";
            goto_end("\":");
            value_end = find_first_of(json_ends).first;
            stat_str.append(key).append(string(json_ptr, json_ptr + value_end));
            json_ptr += value_end;
            if (*json_ptr != '}') stat_str.append(",");
        }
        else if (key == "s") {
            key = "\"collectibleSold\":";
            goto_end("\":");
            value_end = find_first_of(json_ends).first;
            stat_str.append(key).append(string(json_ptr, json_ptr + value_end));
            json_ptr += value_end;
            if (*json_ptr != '}') stat_str.append(",");
        }
    }
    ++json_ptr;

    return stat_str;
}

// Returns key/value pairs for desired stats
string stats() {
    string stat_str = "", key, value, lr = "";
    short value_end;
    while (*json_ptr != '}'){
        ++json_ptr;
        key = str_btwn_quotes(false);
        if (statlist.find(key) != statlist.end()){
            value_end = find_end(":");
            key = string(json_ptr, json_ptr + value_end);
            json_ptr += value_end;
            switch(*json_ptr) {
                case('\"'):
                    if (key == "\"q\":") stat_str.append(key).append("-1");
                    else stat_str.append(key).append(str_with_quotes());
                    break;
                default:
                    value_end = find_first_of(json_ends).first;
                    value = string(json_ptr, json_ptr + value_end);
                    if ( key == "\"xp\":") {
                        if (stoi(value) >= 1000) {
                            lr = level_rewards(stoi(value));
                            stat_str = lr.append(stat_str);
                        }
                    }
                    stat_str.append(key).append(value);
                    json_ptr += value_end;
            }
            
            if (*json_ptr != '}') stat_str.append(",");
        }
        else{
            value_end = find_first_of(json_ends).first;
            json_ptr += value_end;
        }
    }
    if (stat_str.back() == ',') stat_str.pop_back();
    ++json_ptr;

    return stat_str;
}

// Returns user's e-mail address from details
string details() {
    
    goto_start("\"e\"");
    short end = find_nth("\"", 4);
    string email = string(json_ptr, json_ptr + end + 1);
    goto_start("},");
    ++json_ptr;
    if (email.find('@') == string::npos) email = "";
    return email;
}

// Returns calculated rewards earned from progressive tasks
long progress_rewards(short level, short amt) {
    long rewards = 0;
    for (short i = 1; i < level + 1; ++i) {
        rewards += i * amt;
    }
    return rewards;
}

// Returns rewards (Gold and PHT) earned from leveling up
string level_rewards(double xp) {
    short level = 1;
    double xp_dbl = (double) xp / 1000;
    for (double l : levels) {
        if (xp_dbl < l) break;
        ++level;
    }
    string rewards = string("\"levelPHT\":")
                    .append(std::to_string(progress_rewards(level, 50)))
                    .append(string(",\"levelGold\":"))
                    .append(std::to_string(progress_rewards(level, 150)))
                    .append(",");

    return rewards;
}

// Returns string of rewards (Gold and chests) earned from completing progressive quests
string quest_rewards(string quests, string type, short flip, short other) {
    string::iterator q_ptr = quests.begin();
    char q;
    short end;
    int val;
    long gold = 0, chests = 0;
    //if (UID == "\"LVF4DeFgNO\":") {
    //    cout << quests << endl;
    //}
    while(*q_ptr != '}') {
        end = 1;
        q_ptr += 2;
        q = *q_ptr;
        while(*q_ptr != ':') ++q_ptr;
        ++q_ptr;
        while ( *(q_ptr + end) != ',' && *(q_ptr + end) != '}') ++end;
        val = stoi(string(q_ptr, q_ptr + end));
        if (type == "quest") {
            if (q == 'f') gold += progress_rewards(val, flip);
            else {
                gold += progress_rewards(val, other);
                chests += val;
            }
        }
        else{
            if (q == 'f') gold += val * flip;
            else {
                gold += val * other;
                chests += val * 10;
            }
        }
        q_ptr += end;
    }
    string rewards = string("\"").append(type).append("Gold\":")
                    .append(std::to_string(gold))
                    .append(string(",\"")).append(type).append("Chest\":")
                    .append(std::to_string(chests))
                    .append(",");
    return rewards;
}

// Returns string from calculated rewards from progress of referred players
string referrals() {
    string key, value;
    short count;
    int total_gold = 0, total_pht = 0;
    std::set<char> claimed = {'c', '}'};
    goto_end("\"a\":");
    //if (UID == "\"IRdcAlskPy\":") print_next_n(1000);
    while (*json_ptr != '}') {
        key = str_btwn_quotes();
        //cout << key << endl;
        if (find_first_of(claimed, true).second == 'c'){
            json_ptr += 3;
            value = string(json_ptr, json_ptr + find_first_end().first);
            if (key[0] == 'e') {
                total_pht += ref_rewards.at(key) * stoi(value);
            }
            else {
                total_gold += ref_rewards.at(key) * stoi(value);
            }
        }
        goto_end("}");
        json_ptr += find_first_end().first;
    }
    ++json_ptr;
    goto_end("}");
    if (total_gold + total_pht > 0) {
        key = "";
        if (total_gold > 0) key.append("\"referralGold\":")
                                .append(std::to_string(total_gold));
        if (total_pht > 0) {
            if (key.length() > 1) key.append(",");
            key.append("\"referralPHT\":").append(std::to_string(total_pht));
        }
        //cout << key << endl;
        //std::cin.ignore();
        return key;
    }
    else return "";
}

// Checks if map already contains a given key
// If false, add entry with amt as value. If true, add amt to value related to key
inline void check_map_add(std::unordered_map<string, double> &txs,
                            std::set<string> &txs_keys, string key, double amt) {
    if (txs_keys.find(key) != txs_keys.end()) {
            txs[key] += amt;
    }
    else {
        txs[key] = amt;
        txs_keys.insert(key);
    }
}

// Returns string with k:v pair for staking amount
string staking() {
    short end = find_first_of(json_ends).first;
    short start = find_end("a\":");
    string value;
    if (start < end) {
        value =  string(json_ptr + start, json_ptr + end);
        //if (std::stoi(value) > 0) cout << value << endl;
    }
    else value = "";

    goto_end("}");
    return value;
}

// Flatten and group all transactions to a dictionary containing only symbol:value pairs
string transactions() {
    string key, symbol, to;
    short val_end, deposit = 0;
    double amount = 0;
    
    std::unordered_map<string, double> txs;
    std::unordered_map<string, short> decimals;
    std::set<string> txs_keys;
    while (*json_ptr != '}') {
        goto_end("\":{");
        while (*json_ptr != '}') {
            key = str_btwn_quotes();
            ++json_ptr;
            val_end = find_first_of(json_ends).first;
            if (key == "a") {
                amount = std::stod(string(json_ptr, json_ptr + val_end));
            }
            else if (key == "f") {
                if (starts_with("\"Clo")) {
                    symbol = "\"cloud\"";
                    break;
                }
                if (starts_with("\"PHT")) {
                    symbol = "\"deposit\"";
                    ++deposit;
                    break;
                }
                if (starts_with("\"Stak")) {
                    symbol = "\"stakingReward\"";
                    break;
                }
                short space = find_last_before(' ', '\"', true);
                if (space > -1) {
                    if (starts_with(" Col") || starts_with(" Rew")) {
                        symbol = "\"collection\"";
                        break;
                    }
                }
                json_ptr += find_first_of(json_ends).first;
                continue;
                
            }
            else if (key == "h") {
                if (starts_with("\"fail") || starts_with("\"Fail") ||
                    starts_with("\"FAIL")) break;
            }
            else if (key == "s") {
                symbol = string(json_ptr, json_ptr + val_end);
            }
            else if (key == "to") {
                to = string(json_ptr + 1, json_ptr + val_end - 1);
                if (to == "Convert") {
                    symbol = "\"convert\"";
                    break;
                }
            }
            json_ptr += val_end;
        }
        if (key != "h") check_map_add(txs, txs_keys, symbol, amount);
        goto_end("}");
    }
    ++json_ptr;
    
    //cout << map_to_string(txs, txs_keys) << endl;
    //std::cin.ignore();
    key = map_to_string(txs, txs_keys);
    if (deposit > 0) key.append(",\"numDeposits\":").append(std::to_string(deposit));
    return key;
}

// Returns a string dictionary of item:amount pairs of all purchased items
string ledger() {
    string key, item;
    short val_end;
	int amount;
    //print_next_n(1000);
    
    std::unordered_map<string, double> txs;
    std::set<string> txs_keys;
    while (*json_ptr != '}') {
        goto_end("\":{");
        amount = -1;
        item = "";
        while (*json_ptr != '}') {
            key = str_btwn_quotes();
            ++json_ptr;
            val_end = find_first_of(json_ends).first;
            if (key == "a" || key == "amount") {
                amount = std::stod(string(json_ptr, json_ptr + val_end));
                if (item != "") {
                    check_map_add(txs, txs_keys, item, amount);
                }
            }
            else if (key == "n" || key == "title") {
                short space = find_last_before(' ', '\"', true);
                if (space > -1) {
                    *json_ptr = '\"';
                }
                item = str_with_quotes();
                if (amount != -1) {
                    check_map_add(txs, txs_keys, item, amount);
                }
            }

            json_ptr += find_first_of(json_ends).first;
        }
        if (item == "\"Gold\"" && amount >= 48000){
            check_map_add(txs, txs_keys, string("\"Ruby\""), 1);
        }
        ++json_ptr;
        //cout << UID << map_to_string(txs, txs_keys) << endl;
    }
    ++json_ptr;
    //cout << UID << map_to_string(txs, txs_keys) << endl;
    //std::cin.ignore();
    return map_to_string(txs, txs_keys);
}

// Return a string version of a number, preserving all decimal places
string number_string(double num) {
    unsigned int num2 = (int) num;
    if (num2 < num) {
        std::ostringstream streamObj;
        streamObj << num;
        return streamObj.str();
    }
    else return std::to_string(num2);
}

// Returns a string version of an unordered map
string map_to_string(std::unordered_map<string, double> &txs,
                        std::set<string> &txs_keys) {
    string tx_string = "{";
    for (string s : txs_keys) {
        if (tx_string.length() > 1) tx_string.append(",");
        tx_string.append(s).append(":");
        tx_string.append(number_string(txs[s]));
    }
    if (tx_string.length() != 0) tx_string.append("}");
    return tx_string;
}



