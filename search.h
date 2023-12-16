
#pragma once

#include <iostream>
#include <string>
#include <set>
#include <map>
#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

using namespace std;

//Function remoes the first and last characters if there's a punctuation character. Also lower cases entire string
string cleanToken(string s) {
    
    string cleanStr = "";
    int alphaCount = 0;

    size_t i = 0;
    while(i < s.size() && ispunct(s[i])){
        i++;
    }
    s.erase(0, i); //Removing first trailing punctuations

    size_t j = s.size();
    while(j > 0 && ispunct(s[j - 1])){
        j--;
    }
    s.erase(j); //Removing last trailing punctuations

    
    bool alphaStr = false;
    for(char c:s){
        if(isalpha(c)){
            cleanStr += tolower(c); //Lowercasing string 
            alphaCount++;
            alphaStr = true;
        }
        else if(isspace(c)){ //Middle characters thar are not alphabet characters are fine so appending them to empty string
            cleanStr += c;
        }
        else{
            cleanStr += c;
        }
    }

    if(alphaCount < 1){
        alphaStr = false;
    }

    
    if(alphaStr == false){
        return ""; 
    }
    else{
        return cleanStr;
    }  
}



//Parsing the string by space and calling the clean function for every parsed string
set<string> gatherTokens(string text) {
    set<string> tokens; 
    
    string s;
    stringstream ss(text);

    while (getline(ss, s, ' ')) {
        s = cleanToken(s); 
        tokens.insert(s); //inserting the parsed and cleaned function to a set
    }

    tokens.erase("");
 
    return tokens; 
}


// This function builds the map by setting each word to its associated site
int buildIndex(string filename, map<string, set<string>>& index) {
   
    int count = 0; //Counter for map size
    ifstream inFS;
    inFS.open(filename);
    if (!inFS.is_open()) {
        cout << "Could not find the database file." << endl;
    }
    else{
        string line;
        string site;
        while (getline(inFS, line)) { 
            if(line.substr(0,4) == "www." || line.substr(0,8) == "https://"){
                site = line; //Grabbing the sites from the file
                count++;
            }
            else{
                set<string> words = gatherTokens(line); //grabbing and cleaning the words
                for(const string& word: words){ //Inserting words into map with associated site
                    index[word].insert(site);
                }
            }
        }
    }
    return count; //Returning size
}


set<string> findQueryMatches(map<string, set<string>>& index, string sentence) {
    set<string> keys;
   
    bool space = false;
    for(int i = 0; i < sentence.size(); i++){ //Checking there's one word or multiple by the spaces
        if(sentence[i] == ' '){
            space = true;
        }
    }
    
    string words; 

    if(space == true){ //If multuiple words 
        stringstream ss(sentence);
        while (ss >> words){
            set<string> webSites = index[words]; //First word and its associated sites 
            if(words[0] == '+'){
                if(index.find(words.substr(1)) != index.end()){
                    set<string> currentWebsites = index[words.substr(1)]; //Second word and its associated sites 
                    set<string> intersect;
                    if(keys.empty()){
                        set_intersection( //The intersection of the first two words 
                        webSites.begin(), webSites.end(),
                        currentWebsites.begin(), currentWebsites.end(), 
                        inserter(intersect, intersect.begin())
                        );
                    }
                    else{ //If the first two words have been compared and they need to be compared with another word
                        set_intersection( //The intersection of the first two words with the third word 
                        keys.begin(), keys.end(),
                        currentWebsites.begin(), currentWebsites.end(), 
                        inserter(intersect, intersect.begin())
                        );
                    }
                    keys = intersect;
                }   
            }
            else if(words[0] == '-'){
                if(index.find(words.substr(1)) != index.end()){
                    set<string> currentWebsites = index[words.substr(1)];
                    set<string> intersect;
                    if(keys.empty()){ 
                        set_difference( //The difference of the first two words 
                        webSites.begin(), webSites.end(),
                        currentWebsites.begin(), currentWebsites.end(), 
                        inserter(intersect, intersect.begin())
                        );
                    }
                    else{ //If the first two words have been compared and they need to be compared with another word
                        set_difference( //The difference of the first two words with the third word 
                        keys.begin(), keys.end(), 
                        currentWebsites.begin(), currentWebsites.end(), 
                        inserter(intersect, intersect.begin())
                        );
                    }
                    keys = intersect;
                }   
            }
            else{ //If its just a space then its "or"
                if(index.find(words) != index.end()){
                    keys.insert(index[words].begin(), index[words].end()); 
                }
            }
        }

    }
    else{ //Just returning the sites if its one word
        if(index.find(sentence) != index.end()){
            set<string> links = index[sentence];
            keys = links;
        }
    }

    return keys; 
}



//Output file where we call all the functions 
void searchEngine(string filename) {
    cout << "Stand by while building index..." << endl;
    map<string, set<string>> index;
    int count;
    count = buildIndex(filename, index); //Grabbing the size
    cout << "Indexed " << count << " pages containing "<< index.size() << " unique terms" << endl;

    cout << endl;

    string sentence;
    do{
        cout << "Enter query sentence (press enter to quit): " << endl;
        getline(cin, sentence); //Inputting search terms 
        if(sentence.empty()){ //If the user just types enter then quit
            cout << "Thank you for searching!" << endl;
            break;
        }
        set<string> matches;
        matches = findQueryMatches(index, sentence); //Searching the input terms 
        cout << "Found " << matches.size() << " matching pages" << endl; //Outputting the amount of input terms 

        for(const string&match : matches){ //Outputting the links with the searched items
            cout << match << endl;
        }
    }while(true);
    
}


