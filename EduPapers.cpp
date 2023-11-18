#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <ctime>  
#include <cstdlib>  
#include <fstream>
#include <sstream>
#include <iomanip>
using namespace std;

// ANSI escape codes for text colors
const string textColorBlack = "\x1b[30m";
const string textColorRed = "\x1b[31m";
const string textColorGreen = "\x1b[32m";
const string textColorYellow = "\x1b[33m";
const string textColorBlue = "\x1b[34m";
const string textColorMagenta = "\x1b[35m";
const string textColorCyan = "\x1b[36m";
const string textColorWhite = "\x1b[37m";
const string textColorOrange = "\x1b[38;5;202m";

// ANSI escape codes for background colors
const string bgColorBlack = "\x1b[40m";
const string bgColorRed = "\x1b[41m";
const string bgColorGreen = "\x1b[42m";
const string bgColorYellow = "\x1b[43m";
const string bgColorBlue = "\x1b[44m";
const string bgColorMagenta = "\x1b[45m";
const string bgColorCyan = "\x1b[46m";
const string bgColorWhite = "\x1b[47m";

const string resetColor = "\x1b[0m";


// User structure
struct User {
    string username;
    string password;
};
// Comment structure
struct Comment {
    int paperId;
    string text;
};

// Research Paper structure
struct ResearchPaper {
    int id;
    string title;
    string author;
    string date;
    string abstract; 
    vector<string> comments;
};
// Database to store users and research papers
map<string, User> users;
vector<ResearchPaper> papers;

// Function to generate a random unique ID for research papers
int generateUniqueID() {
    static bool seeded = false;
    if (!seeded) {
        srand(time(0));
        seeded = true;
    }
    
    int minID = 10000;  // Minimum 5-digit ID
    int maxID = 9999999;  // Maximum 7-digit ID
    return minID + (rand() % (maxID - minID + 1));
}

// Function to register a new user
void registerUser() {
    string username, password;
    cout << "Enter a username: ";
    cin >> username;
    cout << "Enter a password: ";
    cin >> password;
    cout<<endl;

    // Check if the username already exists
    if (users.find(username) == users.end()) {
        users[username] = {username, password};
        cout << textColorGreen << "Registration successful!\n\n"<<resetColor;
    } else {
        cout << "Username already exists. Registration failed.\n\n";
    }
}


vector<Comment> loadCommentsFromCSV(const string& filename) {
    vector<Comment> comments;
    ifstream file(filename);

    if (!file) {
        cerr << "Error: Unable to open file " << filename << endl;
        return comments;
    }

    string line;
    bool firstLine = true; // Flag to skip the first line (header)
    while (getline(file, line)) {
        if (firstLine) {
            firstLine = false;
            continue; // Skip the header row
        }

        istringstream iss(line);
        string idStr, commentText;

        if (getline(iss, idStr, ',') &&
            getline(iss, commentText, ',')) {
            // Check if idStr can be converted to an integer
            if (isdigit(idStr[0])) {
                int paperId = stoi(idStr);
                comments.push_back({paperId, commentText});
            } else {
                cerr << "Error parsing CSV line - Invalid Paper ID: " << line << endl;
            }
        } else {
            cerr << "Error parsing CSV line: " << line << endl;
        }
    }

    file.close();
    return comments;
}


// Function to log in
bool login() {
    string username, password;
    cout << "Enter your username: ";
    cin >> username;
    cout << "Enter your password: ";
    cin >> password;

    // Load user data from the "users.csv" file
    ifstream file("users.csv");
    if (!file) {
        cerr << "Error: Unable to open user data file." << endl;
        return false;
    }

    bool loginSuccessful = false;
    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string storedUsername, storedPassword;
        if (getline(iss, storedUsername, ',') && getline(iss, storedPassword, ',')) {
            if (storedUsername == username && storedPassword == password) {
                loginSuccessful = true;
                break; // Exit the loop once a match is found
            }
        }
    }

    file.close();

    if (loginSuccessful) {
        cout << textColorGreen << "\nLogin successful!\n\n" << resetColor;
        return true;
    } else {
           cout << textColorRed << "\nLogin failed. Invalid username or password.\n\n";
        return false;
    }
}

// Function to upload a research paper
void uploadPaper() {
    string title, author, date, abstract;
    cout << "Enter the title of the paper: ";
    cin.ignore();  // Clear newline character from previous input
    getline(cin, title);
    cout << "Enter the author: ";
    getline(cin, author);
    cout << "Enter the date: ";
    getline(cin, date);
    cout << "Enter the abstract of the paper: ";
    getline(cin, abstract);

    ResearchPaper paper;
    paper.id = generateUniqueID();
    paper.title = title;
    paper.author = author;
    paper.date = date;
    paper.abstract = abstract; // Assign abstract to the paper
    papers.push_back(paper);

    cout << textColorGreen << "\nPaper uploaded successfully with ID: " << resetColor << paper.id << endl;
}

// Function to save user data to a CSV file (append mode)
void saveUsersToCSV() {
    ofstream file("users.csv", ios::app); // Open in append mode

    if (file.is_open()) {
        for (const auto& entry : users) {
            const User& user = entry.second;
            file << user.username << "," << user.password << "\n";
        }
        file.close();
        cout << "User data appended and saved to 'users.csv' file." << endl;
    } else {
        cout << textColorRed << "\nFailed to save user data." << resetColor << endl;
    }
}


// Function to load papers from a CSV file
// Function to load papers from a CSV file
vector<ResearchPaper> loadPapersFromCSV(const string& filename) {
    vector<ResearchPaper> papers;
    ifstream file(filename);

    if (!file) {
        cerr << "Error: Unable to open file " << filename << endl;
        return papers;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string idStr, title, author, date, content;

        if (getline(iss, idStr, ',') &&
            getline(iss, title, ',') &&
            getline(iss, author, ',') &&
            getline(iss, date, ',')) {
            // Check if idStr can be converted to an integer
            if (isdigit(idStr[0])) {
                int id = stoi(idStr);
                // The remaining part of the line is the content, including any commas
                getline(iss, content);
                papers.push_back({id, title, author, date, content});
            } else {
                cerr << "Error parsing CSV line - Invalid ID: " << line << endl;
            }
        } else {
            cerr << "Error parsing CSV line: " << line << endl;
        }
    }

    file.close();
    return papers;
}


// Function to search papers by keyword
void searchPapersByKeyword() {
    string keyword;
    cout << "Enter a keyword to search for papers: ";
    cin.ignore();
    getline(cin, keyword);

    // Load papers from CSV file
    vector<ResearchPaper> loadedPapers = loadPapersFromCSV("papers.csv");

    cout << "Papers matching the keyword '" << keyword << "':\n";
    bool found = false;
    cout << "+--------+------------------+-----------------+-----------------------+\n";
    cout << "| ID     | Title            | Author          | Date                  |\n";
    cout << "+--------+------------------+-----------------+-----------------------+\n";

    for (const ResearchPaper& paper : loadedPapers) {
        string lowercaseTitle = paper.title;
        string lowercaseAuthor = paper.author;
        string lowercaseKeyword = keyword;
        transform(lowercaseTitle.begin(), lowercaseTitle.end(), lowercaseTitle.begin(), ::tolower);
        transform(lowercaseAuthor.begin(), lowercaseAuthor.end(), lowercaseAuthor.begin(), ::tolower);
        transform(lowercaseKeyword.begin(), lowercaseKeyword.end(), lowercaseKeyword.begin(), ::tolower);
        if (lowercaseTitle.find(lowercaseKeyword) != string::npos || lowercaseAuthor.find(lowercaseKeyword) != string::npos) {
            cout << "| " << std::setw(8) << paper.id << " | " << std::setw(16) << paper.title << " | "
                 << std::setw(15) << paper.author << " | " << std::setw(19) << paper.date << " |\n";

     cout << "+--------+------------------+-----------------+-----------------------+\n";
               if (!paper.abstract.empty()) {
                cout << textColorOrange << "Abstract: " << resetColor << paper.abstract << "\n" ;
                cout << "+--------+------------------+-----------------+-----------------------+\n" << endl;
                cout << "| " << std::setw(70) << " | "<<endl;
            } else {
                cout << "Abstract not found.\n";
            }
            cout << "\n";
            found = true;
        }
    }



    if (!found) {
        cout << textColorRed <<"No papers matching the keyword '" << keyword << "' found.\n"<<resetColor;
    }
}



void savePapersToCSV() {
    // Load existing papers from the file into the 'papers' vector
    vector<ResearchPaper> existingPapers = loadPapersFromCSV("papers.csv");
    
    // Append new papers to the existing ones in memory, but skip duplicates
    for (const ResearchPaper& newPaper : papers) {
        bool isDuplicate = false;
        for (const ResearchPaper& existingPaper : existingPapers) {
            if (newPaper.id == existingPaper.id) {
                isDuplicate = true;
                break; // The paper with the same ID already exists, so skip it
            }
        }
        if (!isDuplicate) {
            existingPapers.push_back(newPaper);
        }
    }
    
    // Open the file for writing (not in append mode)
    ofstream file("papers.csv");
    if (file.is_open()) {
        for (const ResearchPaper& paper : existingPapers) {
            file << paper.id << "," << paper.title << "," << paper.author << "," << paper.date << "," << paper.abstract << "\n";
        }
        file.close();
        cout << "Research paper data saved to 'papers.csv' file." << endl;
    } else {
        cout << textColorRed << "\nFailed to save research paper data.\n" << resetColor << endl;
    }
}
// Function to add suggestions (a simple example)
void addSuggestions() {
    string suggestion;
    cout << "Enter a suggestion: ";
    cin.ignore();
    getline(cin, suggestion);

    // Here, you can implement how to handle suggestions (e.g., store in a file or database)
    // For simplicity, we'll just print the suggestion.
    cout<<"Suggestion received: " << suggestion << endl;
}

#include <iomanip> // Include for std::setw

// Function to view research papers
void viewPapers() {
    string filename = "papers.csv";
    vector<ResearchPaper> loadedPapers = loadPapersFromCSV(filename);
    vector<Comment> comments = loadCommentsFromCSV("comments.csv");
    int choice;
    bool found = false; // Declare and initialize found here
    while (true) {
        cout << textColorCyan << "\nViewPapers Menu:\n\n";
        cout << "1. Display all papers\n";
        cout << "2. Search papers by keyword\n";
        cout << "3. Read a specific paper by ID\n";
        cout << "4. Add suggestions\n";
        cout << "5. Exit\n\n" << resetColor;
        cout << "Enter your choice: \n";
        cin >> choice;

        switch (choice) {
            case 1:
                if (loadedPapers.empty()) {
                    cout << "No papers available.\n";
                } else {
                    cout << textColorGreen<< "+--------+------------------+-----------------+-----------------------+\n";
                    cout << "| ID     | Title              | Author           | Date               |\n";
                    cout << "+--------+------------------+-----------------+-----------------------+\n"<<resetColor<<textColorMagenta;
                    for (const ResearchPaper& paper : loadedPapers) {
                        cout << "| " << std::setw(8) << paper.id << " | " << std::setw(16) << paper.title << " | " << std::setw(15) << paper.author << " | " << std::setw(19) << paper.date << " |\n";
                        cout << "+--------+------------------+-----------------+----------------------+\n";
                    }
                    cout<<resetColor;
                }
                break;
            case 2:
                searchPapersByKeyword();
                break;
            case 3:
                 int paperId;
                cout << "Enter the ID of the paper you want to read: ";
                cin >> paperId;
                found = false;
                for (const ResearchPaper& paper : loadedPapers) {
                    if (paper.id == paperId) {
                       // Display title, author, and date in a box
                        cout << "+---------------------------------------------------------+\n";
                        cout << "| Title: " << std::setw(43) << paper.title << "     |\n";
                        cout << "| Author: " << std::setw(42) << paper.author << "     |\n";
                        cout << "| Date: " << std::setw(44) << paper.date << "     |\n";
                        cout << "+---------------------------------------------------------+\n";
                        
                        // Display the abstract below the box
                        cout << textColorOrange << "Abstract: " << resetColor << paper.abstract << "\n";

                        // Display comments related to this paper
                        cout << textColorGreen << "Comments:\n" << resetColor;
                        cout << textColorMagenta << "+---------------------------------------------------------+\n" << resetColor;
                        for (const Comment& comment : comments) {
                            if (comment.paperId == paperId) {
                                cout << textColorCyan << "Comment: " << resetColor << comment.text << "\n";
                            }
                        }
                        cout << textColorMagenta << "+---------------------------------------------------------+\n" << resetColor;

                        // Ask if the user wants to add a comment
                        cout << "Do you want to add a comment to this paper? (1 for Yes, 0 for No): ";
                        int addCommentChoice;
                        cin >> addCommentChoice;
                        if (addCommentChoice == 1) {
                            cin.ignore();  // Clear newline character
                            string commentText;
                            cout <<textColorCyan<< "\nEnter your comment: "<<resetColor;
                            getline(cin, commentText);
                            comments.push_back({paperId, commentText});

                            // Save the comment to the comments.csv file
                            ofstream commentsFile("comments.csv", ios::app);
                            if (commentsFile.is_open()) {
                                commentsFile << paperId << "," << commentText << "\n";
                                commentsFile.close();
                                cout <<textColorGreen<< "\nComment added successfully!\n"<<resetColor;
                            } else {
                                cout << "Failed to add the comment.\n";
                            }
                        }

                        found = true;
                        break;
                    }
                }
                if (!found) {
                    cout << "Paper with ID " << paperId << " not found.\n";
                }
                break;
            case 4:
                addSuggestions();
                break;
            case 5:
                int returnToMenu;
                cout << "Do you want to return to the main menu? (1 for Yes, 0 for No): ";
                cin >> returnToMenu;
                if (returnToMenu == 1) {
                    return;
                }
                break;
            default:
                cout << textColorRed<< "Invalid choice. Please try again.\n\n"<<resetColor;
                break;
        }
    }
}




int main() {
    
    system("cls"); // clear screen

    const string asciiArt = R"(
  __          __  _                            _          ______    _       _____                            _
  \ \        / / | |                          | |        |  ____|  | |     |  __ \                          | |
   \ \  /\  / /__| | ___ ___  _ __ ___   ___  | |_ ___   | |__   __| |_   _| |__) |_ _ _ __   ___ _ __ ___  | |
    \ \/  \/ / _ \ |/ __/ _ \| '_ ` _ \ / _ \ | __/ _ \  |  __| / _` | | | |  ___/ _` | '_ \ / _ \ '__/ __| | |
     \  /\  /  __/ | (_| (_) | | | | | |  __/ | || (_) | | |___| (_| | |_| | |  | (_| | |_) |  __/ |  \__ \ |_|
      \/  \/ \___|_|\___\___/|_| |_| |_|\___|  \__\___/  |______\__,_|\__,_|_|   \__,_| .__/ \___|_|  |___/ (_)
)";
    
    // Print the ASCII art
    cout << textColorYellow<< asciiArt << resetColor << endl;
 


    int accessMenuChoice;
    cout << "\nDo you want to access the EduPapers menu? (1 for Yes, 0 for No): ";
    cin >> accessMenuChoice;
    cout<<"\n"<<endl;

    if (accessMenuChoice == 0) {
        return 0;  // Exit the program
    }
    else if(accessMenuChoice == 1){
    int choice;
    while (true) {
        cout << textColorYellow << "----------------"<< resetColor;
        cout << textColorOrange <<"\nEduPapers Menu:\n"<<resetColor;
        cout << textColorYellow << "----------------\n\n" << resetColor;
        cout << textColorCyan<<"1. Register\n";
        cout << "2. Login\n";
        cout << "3. Upload a Research Paper\n";
        cout << "4. View Papers\n";
        cout << "5. Exit\n\n"<<resetColor;
        cout << textColorMagenta<<"Enter your choice: "<<resetColor;
        cin >> choice;
        cout<<""<<endl;

        switch (choice) {
            case 1:
                registerUser();
                saveUsersToCSV();
                break;
            case 2:
                login();
                break;
            case 3:
                uploadPaper();
                savePapersToCSV();
                break;
            case 4:
                viewPapers();
                break;
            case 5:
                return 0;
            default:
                cout << textColorRed << "Invalid choice. Please try again.\n" << resetColor;
                break;
        }
    }
    }
    else{
        cout<< textColorRed << "Invalid Input"<< resetColor <<endl;
    }

    return 0;
}
