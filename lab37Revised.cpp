/*
    Lab 37: Hash table - Revised
        1. Part One: write a function sum_ascii() that receives a single string and returns the sum of that string's character's ASCII values.
        2. Part Two:
            - Your code contains a large dataset: over 100,000 12-character strings of hexadecimal characters. These might represent serial numbers, part numbers, etc.
            - Read the file. Send each string to the function, receiving that string's ASCII sum, and totaling the return value into a grand total.
            - Output the grand total (the sum of all ASCII values in the entire file); it should be 69893419.
        3. Part Three:
            - Change the function sum_ascii() to gen_hash_index(). Adapt the code appropriately.
            - Create the std::map named hash_table. The key in the map is an int, the hash index. The value in a map is a std::list, which will contain the 12-character codes from the file that all map to that hash index.
            - When codes are read from the file, send the code to your function. Receive its hash index that's returned from the function. Input that pair into the map (the hash index and the code string). Remember that the code string is going into a std::list, so use the appropriate method to manipulate that std::list.
            - Display just the first 100 map entries to the console to test your data structure. Remember how to access map elements with .first and .second as necessary.

    Lab 38: Hash Tables II
        1. Add an interactive menu to your Lab 37 project with these new features:
            - Print the first 100 entries; search for a key; add a key; remove a key; modify a key; and exit.
        2. In your finished Lab 37, create a new Lab38 branch, and code this assignment on that branch.
            - Do not collapse/merge/pull/squash anything - just leave that branch active so I can see it.

    Notes:
        1. Added functions addKey, searchKey, removeKey, and modifyKey are VERY verbose and could be refactored for simplicity, but have been left as is
            for time convenience.
        2. Hash table creation in main could be refactored into it's own function to return a map and clean up main, but again, was left as is for time convenience.
        3. Functions modify Keys ONLY. removeKey and modifyKey will affect values as well, but functions provide no access to user to interact with Values directly.
*/

#include <iostream>
#include <iomanip>
#include <fstream>
#include <map>
#include <list>
#include <climits> // INT_MIN and MAX
using namespace std;

int gen_hash_index(const string &);                               // Receives a single string and returns the sum of that string's character's ASCII values
bool ContainsKey(const map<int, list<string>> &, const string &); // Helper function checks if key is in hash table
int mainMenu();                                                   // Outputs prompt and collects user selection
bool isValidOption(string, const int, const int);                 // Helper function to validate user input
void PrintEntries(const map<int, list<string>> &, const int);     // Print number of entries specified
void addKey(map<int, list<string>> &);                            // Adds key to table
void searchKey(const map<int, list<string>> &);                   // Returns whether key exists and related values
void removeKey(map<int, list<string>> &);                         // Removes key and related values
void modifyKey(map<int, list<string>> &);                         // Modifies a key and transfers list values to new key

const string INPUT_FILENAME = "lab37Data.txt"; // Filename for data input

int main()
{
    ifstream inputFile;                // For ascii codes
    string fileLine = "";              // A line from the input file
    int sum = 0;                       // Sum of ascii codes
    map<int, list<string>> hash_table; // Table holding values from file
    int hashIndex = 0;                 // Hash index of line within loop
    const int NUM_LINES_OUTPUT = 100;  // Number of lines to output
    int userSelectedOption = 0;        // Holds user selection

    // Try opening file and output error to console if file is no good
    inputFile.open(INPUT_FILENAME);
    if (!inputFile)
    {
        throw runtime_error("Error opening file: " + INPUT_FILENAME);
    }
    else
    {
        while (getline(inputFile, fileLine))
        {
            hashIndex = gen_hash_index(fileLine); // Get hash code for line
            auto it = hash_table.find(hashIndex); // Iterator to hash bucket in map
            if (it == hash_table.end())           // Key does NOT exist
            {
                hash_table.insert({hashIndex, {fileLine}}); // Create new entry
            }
            else
            {
                it->second.push_back(fileLine); // Add to current entry
            }
        }
        // CLOSE THE F-ING FILE
        inputFile.close();
    }

    // Prompt user for selection and input
    // User selection 0 is the program exit code
    do
    {
        userSelectedOption = mainMenu();
        switch (userSelectedOption)
        {
        case 1:
            // Outputs given number of items to console
            PrintEntries(hash_table, NUM_LINES_OUTPUT);
            break;
        case 2:
            // Takes user to function to delete a given text string within tree
            addKey(hash_table);
            break;
        case 3:
            // Deletes key and associated data
            removeKey(hash_table);
            break;
        case 4:
            // Tells user whether a key exists
            searchKey(hash_table);
            break;
        case 5:
            // Allows user to modify a key
            modifyKey(hash_table);
            break;
        default:
            break;
        }
    } while (userSelectedOption != 0);

    cout << "Thank you for using the Hash Table Management Tool" << endl;

    return 0;
}

// Receives a single string and returns the sum of that string's character's ASCII values
int gen_hash_index(const string &s)
{
    int sum = 0;
    for (char letter : s)
    {
        sum += (int)letter;
    }
    return sum;
}

// Helper function checks if key is in hash table
bool ContainsKey(const map<int, list<string>> &hashMap, const string &s)
{
    auto it = hashMap.find(gen_hash_index(s));
    if (it == hashMap.end())
        return false; // Index doesn't exist
    // Iterate to check if string is in list bucket
    for (string code : it->second)
    {
        if (code == s)
            return true;
    }
    return false; // List bucket iterated but code not found
}

// Main menu for application
int mainMenu()
{
    string userInput = "";

    do
    {
        // Output prompt
        cout << "\nHash Table Menu" << "\n"
             << "[1] Print first 100 entries" << "\n"
             << "[2] Add key" << "\n"
             << "[3] Remove key" << "\n"
             << "[4] Search key" << "\n"
             << "[5] Modify key" << "\n"
             << "[0] Quit" << "\n"
             << "Choice --> ";
        getline(cin, userInput); // Get user input as string and test
        cout << endl;
    } while (!isValidOption(userInput, 0, 5));

    // If isValidOption passed, stoi(userInput) has already been tested and is safe
    return stoi(userInput);
}

// Return t/f if userInput is a valid int between min and max
// WARNING: stoi() will convert a double to an int or any string following an int.
// Ex: stoi("2.9") will return 2 and so will stoi("2tGznso"), etc.
bool isValidOption(string userInput, const int minOption, const int maxOption)
{
    int selectedOption = 0;
    try
    {
        selectedOption = stoi(userInput);
    }
    catch (const std::exception &e)
    {
        cout << "Invalid input: Please enter a valid integer." << "\n\n";
        return false;
    }

    // if userInput is an int but outside expected range
    if (selectedOption < minOption || selectedOption > maxOption)
    {
        cout << "Invalid input: Please enter an integer between " << minOption << " and " << maxOption << "." << "\n\n";
        return false;
    }

    return true;
}

// Print number of entries specified
void PrintEntries(const map<int, list<string>> &mapContainer, const int numToPrint)
{
    int counter = 0;                                                                                      // Counter for lines of output
    for (auto itMap = mapContainer.begin(); itMap != mapContainer.end() && counter < numToPrint; itMap++) // For each map object
    {
        for (auto itList = itMap->second.begin(); itList != itMap->second.end() && counter < numToPrint; itList++) // For each list inside map
        {
            cout << fixed << setw(3) << right << ++counter << ". " << *itList << endl; // Counter incremented in statement
        }
    }
}

// Adds key to table
void addKey(map<int, list<string>> &mapContainer)
{
    string userInput = "";

    // Get user input to add to table
    cout << "Please enter a Key to add to the hash table (Leave empty to cancel operation): ";
    getline(cin, userInput);
    // Guard against empty string and return
    if (userInput == "")
    {
        cout << "Operation Cancelled." << endl;
        return;
    }

    // Add if entry doesn't already exist
    if (ContainsKey(mapContainer, userInput)) // Key already exists
    {
        cout << "Key: \"" << userInput << "\" already exists." << endl;
        return;
    }
    else
    {
        auto it = mapContainer.find(gen_hash_index(userInput));

        // Add key to bucket or create bucket if doesn't exist
        if (it == mapContainer.end()) // Bucket does not exist, so create and add value
        {
            mapContainer.insert({gen_hash_index(userInput), {userInput}});
        }
        else // Bucket exists, but key needs to be added
        {
            it->second.push_back(userInput);
        }

        cout << "Key: \"" << userInput << "\" inserted into table." << endl;
        return;
    }
}

// Returns whether key exists and related values
void searchKey(const map<int, list<string>> &mapContainer)
{
    string userInput = "";

    cout << "Please enter a Key to search for in the hash table (Leave empty to cancel operation): ";
    getline(cin, userInput);
    // Guard against empty string and return
    if (userInput == "")
    {
        cout << "Operation Cancelled." << endl;
        return;
    }

    // Search for key in container
    if (!ContainsKey(mapContainer, userInput)) // Key does NOT exist
    {
        cout << "Key: \"" << userInput << "\" does not exist." << endl;
        return;
    }
    else
    {
        cout << "Key: \"" << userInput << "\" exists." << endl;
        return;
    }
}

// Removes key and related values
void removeKey(map<int, list<string>> &mapContainer)
{
    string userInput = "";

    cout << "Please enter a Key to remove from the hash table (Leave empty to cancel operation): ";
    getline(cin, userInput);
    // Guard against empty string and return
    if (userInput == "")
    {
        cout << "Operation Cancelled." << endl;
        return;
    }

    // Search for key in container
    if (!ContainsKey(mapContainer, userInput)) // Key does NOT exist
    {
        cout << "Key: \"" << userInput << "\" does not exist." << endl;
        return;
    }
    else
    {
        list<string> tempList = mapContainer.at(gen_hash_index(userInput));

        // Remove user value from bucket, and delete bucket if empty
        tempList.remove(userInput);
        if (tempList.size() == 0)
        {
            mapContainer.erase(gen_hash_index(userInput));
        }

        cout << "Key: \"" << userInput << "\" has been removed." << endl;
        return;
    }
}

// Modifies a key and transfers list values to new key
void modifyKey(map<int, list<string>> &mapContainer)
{
    string userInput = "";

    // Get key to modify from user
    cout << "Please enter a Key to modify from the hash table (Leave empty to cancel operation): ";
    getline(cin, userInput);
    // Guard against empty string and return
    if (userInput == "")
    {
        cout << "Operation Cancelled." << endl;
        return;
    }

    // Verify Key is in container
    if (!ContainsKey(mapContainer, userInput)) // Key does NOT exist
    {
        cout << "Key: \"" << userInput << "\" does not exist." << endl;
        return;
    }
    else // Key exists, get new Key from user and replace old Key
    {
        string newUserInput = "";

        cout << "Please enter a new string to replace Key (Leave empty to cancel operation): ";
        getline(cin, newUserInput);
        // Guard against empty string and return
        if (newUserInput == "")
        {
            cout << "Operation Cancelled." << endl;
            return;
        }

        // Remove old Key
        int indexOld = gen_hash_index(userInput); // Get hash of old key, delete from bucket.
        auto it = mapContainer.find(indexOld);    // Hash index
        list<string> tempList = it->second;       // Bucket
        tempList.remove(userInput);               // Delete Key from bucket
        if (tempList.size() == 0)
            mapContainer.erase(indexOld); // If bucket becomes empty, delete from hash table

        // Add new Key
        int indexNew = gen_hash_index(newUserInput); // Get hash of new key, add to bucket.
        it = mapContainer.find(indexNew);            // Hash index
        if (it == mapContainer.end())
            mapContainer.insert({indexNew, {newUserInput}}); // Bucket does not exist, so add bucket and key

        
        {
            // Insert new entry into map
            mapContainer.insert({newUserInt, it->second}); // Create new entry with current list
            cout << "Key: " << userInt << " modified to " << newUserInt << "." << endl;
        }
        else // New key already exists and current values must be inserted into existing key
        {
            // Append values from old key to end of new key value
            itNew->second.insert(itNew->second.end(), it->second.begin(), it->second.end());
            cout << "Key: " << newUserInt << " already exists and values have been appended." << endl;
        }
        // Remove old key
        mapContainer.erase(userInt);
        return; // Not necessary, but good habit
    }
}