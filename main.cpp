

#include "search.h"
using namespace std;

bool testCleanToken() {
        string ans = "hello";
        int pass = 0, fail = 0;

        ans == cleanToken(".hello") ? ++pass : ++fail;
        ans == cleanToken("...hello") ? ++pass : ++fail;

        string ans2 = "how";
        ans2 == cleanToken("...how!!!!") ? ++pass : ++fail;

        string ans3 = "how84";
        ans3 == cleanToken("how84!") ? ++pass : ++fail;

        
        return 0 == fail;
}

bool testgatherTokens(){
        int pass = 0, fail = 0;
        set<string> tokens = gatherTokens("to be or not to be");
        set<string> answers = {"to", "be", "or", "not", "to", "be"};

        tokens.size() == answers.size() ? ++pass : ++fail;
        tokens == answers ? ++pass : ++fail;

        set<string> tokens2 = gatherTokens("hello we are doing a test");
        set<string> answers2 = {"hello", "we", "are", "doing", "a", "test"};

        tokens2.size() == answers2.size() ? ++pass : ++fail;
        tokens2 == answers2 ? ++pass : ++fail;

        set<string> tokens3 = gatherTokens("..Hello !we! ..are.. @doing@ !a test::");
        set<string> answers3 = {"hello", "we", "are", "doing", "a", "test"};

        tokens3.size() == answers3.size() ? ++pass : ++fail;
        tokens3 == answers3 ? ++pass : ++fail;

        set<string> tokens4 = gatherTokens("fourth !!!!test case### lol@@");
        set<string> answers4 = {"fourth", "test", "case", "lol"};

        tokens4.size() == answers4.size() ? ++pass : ++fail;
        tokens4 == answers4 ? ++pass : ++fail;  

        return fail == 0;
}

bool testBuildIndex(){
    string filename;
    map<string, set<string>> index;

    int pass = 0, fail = 0;

    int cplusPlusCount = 86;
    int tinyCount = 4;
    int stackOverFlowCount = 13;
    int uicNewsCount = 85;

    cplusPlusCount == buildIndex("cplusplus.txt", index) ? ++pass : ++fail;
    tinyCount == buildIndex("tiny.txt", index) ? ++pass : ++fail;
    stackOverFlowCount == buildIndex("stackoverflow.txt", index) ? ++pass : ++fail;
    uicNewsCount == buildIndex("uiccs-news.txt", index) ? ++pass : ++fail;


    return 0 == fail;
}

bool testQueryMatches(){
    int pass = 0, fail = 0;
    map<string, set<string>> index;

    buildIndex("tiny.txt", index);

    set<string> testCase1 = {"www.rainbow.org"};
    testCase1 == findQueryMatches(index, "red +blue -fish") ? ++pass : ++fail;

    set<string> testCase2 = {"www.dr.seuss.net", "www.rainbow.org", "www.shoppinglist.com"}; 
    testCase2 == findQueryMatches(index, "red fish") ? ++pass : ++fail;

    set<string> testCase3 = {"www.dr.seuss.net"}; 
    testCase3 == findQueryMatches(index, "fish -eggs +blue") ? ++pass : ++fail;

    set<string> testCase4 = {"www.rainbow.org"}; 
    testCase4 == findQueryMatches(index, "red -fish") ? ++pass : ++fail;

    return fail == 0;
}



int main() {
    

    string filename;
    getline(cin, filename);
    searchEngine(filename);

    //cout << testCleanToken() << endl;
    //cout << testgatherTokens() << endl;
    //cout << testBuildIndex() << endl;
    //cout << testQueryMatches() << endl;
    
    return 0;
}

