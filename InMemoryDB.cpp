#include <iostream>
#include <string>
#include <map>
#include <optional>
#include <stdexcept>
using namespace std;

class InMemoryDB{
public:
    map<string, int> transaction;  
    map<string, int> temp;
    bool check = false; 
    optional<int> get(string key);
    void put(string key, int val);
    void begin_transaction();
    void commit();
    void rollback();
    void print();
};



optional<int> InMemoryDB::get(string key)
{
    if(transaction.find(key) == transaction.end()){
        return nullopt;
    }
    return transaction[key];
}

void InMemoryDB::put(string key, int val)
{
    if(check == false){
       throw runtime_error("Transaction not in progress");
    }
    temp[key] = val;
}

void InMemoryDB::begin_transaction()
{
    transaction.clear();
    temp.clear();
    check = true;
}

void InMemoryDB::commit()
{
    if(temp.empty()){
        throw runtime_error("No open transaction");
    }
    transaction.insert(temp.begin(), temp.end());
    temp.clear();
}

void InMemoryDB::rollback()
{
    if(temp.empty()){
        throw runtime_error("No ongoing transaction");
    }
    
    temp.clear();
    transaction.clear();
}

void InMemoryDB::print()
{
    cout << temp["A"] << endl;
}

int main(){
    InMemoryDB test;
    cout << "Test 1: Testing get(A), which should return null since A does not exist in the DB yet" << endl;
    if(test.get("A") == nullopt){
        cout << "Expected null, returned null" << endl;
    }
    cout << endl;

    cout << "Test 2: Testing put(A, 5), which should return an exception since a transaction is not in progress" << endl;
    try{
        test.put("A", 5);
    }
    catch(const std:: exception& e){
        cout << "Error caught: Transaction not in progress" << endl;;
    }

    cout << endl;

    test.begin_transaction();
    cout << "New transaction started" << endl << endl;

    // set’s value of A to 5, but its not committed yet
    test.put("A", 5);
    cout << "Test 3: Testing put(A, 5), which should work since a transaction is in progress" << endl;
    try{
        test.put("A", 5);
    }
    catch(const std:: exception& e){
        cout << "Error caught: Transaction not in progress" << endl;;
    }
    cout << "No error was caught A was successfully added to DB" << endl << endl;




cout << "Test 4: Testing get(A), which should return null since A does not exist in the DB yet" << endl;
if(test.get("A") == nullopt){
    cout << "Test successful, returned null" << endl;
};

cout << endl << "Test 5: Testing put(A, 6), which should update A's value to 6" << endl;
test.put("A", 6);
cout << "Expected 6 and got " << test.temp["A"] << endl << endl;;

test.commit();

cout << "Test 6: Testing get(A), which should return 6" << endl;
if(test.get("A") == 6){
    cout << "Expect 6 and got 6" << endl << endl;
}

cout << "Test 7: Testing commit(), which should return an error" << endl;
try{
    test.commit();
    }
    catch(const std:: exception& e){
        cout << "Error caught: No open transaction" << endl << endl;
    }

cout << "Test 8: Testing rollback(), which should return an error" << endl;
try{
    test.rollback();
    }
    catch(const std:: exception& e){
        cout << "Error caught: No ongoing transaction" << endl << endl;
    }

    cout << "Test 9: Testing get(B), which should return null since B does not exist in the DB yet" << endl;
    if(test.get("B") == nullopt){
        cout << "Expected null, returned null" << endl;
    }
    cout << endl;

cout << "Starting new transaction" << endl << endl;
test.begin_transaction();

    test.put("B", 10);
    cout << "Test 10: Testing put(B, 10), which should work since a transaction is in progress" << endl;
    try{
        test.put("B", 10);
    }
    catch(const std:: exception& e){
        cout << "Error caught: Transaction not in progress" << endl;;
    }
    cout << "No error was caught B was successfully added to DB" << endl << endl;

cout << "Rolling out changes" << endl << endl;
test.rollback();
cout << "Test 11: Testing get(B), which should return null since changes were rolled back" << endl;
if(test.get("B") == nullopt){
    cout << "Test successful, returned null" << endl;
};

    return 0;
}