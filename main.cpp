#include <iostream>
#include <string>
#include <cstdlib>      // srand
#include <ctime>        // time
#include <algorithm>    // std::random_shuffle
#include <fstream>
//#define NOMINMAX    // For VISUAL STUDIOS compiler: To use the MAX keyword from cin.ignore(numeric_limits..) in VisualStudios

#include <windows.h>

//using namespace std;  // "best practice" is to avoid using namespace std because it can (but not likely), cause certain problems.
                        // Sources: https://www.reddit.com/r/learnprogramming/comments/19f3cr/c_why_do_people_say_not_to_use_using_namespace_std/           https://www.geeksforgeeks.org/using-namespace-std-considered-bad-practice/
                        // BUT: It doesn't effect anything in terms of compile time, or program speed.
                        // If not using own namespaces with ambiguous names and if only using
                        // namespace std in each .cpp file and not header file, than it's absolutely fine.
                        // For this program, using namespace std is perfectly fine, but for demonstration purposes, I'm using what's needed manually.
using std::cout;
using std::cin;
using std::string;
using std::numeric_limits;
using std::streamsize;
using std::fstream;
using std::ios;

// TODO:
// Remake sub cifer using find function instead of nested looping.
// Create 3 functions:
// 1 to generate random key and optionally, only if existing key is empty(for future loading in .txt file with a spcific key),
// 1 to encrypt message,
// and 1 to decrypt message
// Display generated random key once to user so they can use it to decrypt the message
// Optional: Add the ability to open and save to file now to. No need to do later. Learn now.

// Rand string sources:
// https://stackoverflow.com/questions/8364597/shuffling-string
// https://www.cs.cmu.edu/~pattis/15-1XX/common/handouts/ascii.html
// http://www.cplusplus.com/reference/algorithm/random_shuffle/

// Fstream sources:
// https://stackoverflow.com/questions/15667530/fstream-wont-create-a-file

// int selectionion condition source:
// https://www.daniweb.com/programming/software-development/threads/279409/cin-to-allow-numeric-only

// cin >> user_selection then getline(cin, users_msg) issue that caused program to skip the users message
// prompt. Answer source: Discord user Death#7963
/*My Question: "is something left in the buffer that needs to be ignored?"
Response: " yes, this happens when u do cin >> into a number followed by a getline
From the cin >>, there is a number and a newline character in the buffer
but it only takes the number the first thing the getline sees is the '\n' so it's like
"Ok, I guess I'm done" "
*/

/*
- GetOpenFileName function error occurs in codelite and codeblocks which doesn't occur in
Visual studios because codelite uses GCC compiler, codeblocks uses minGW (VS uses microsoft compiler). 
Error displayed is "undefined reference GetOpenFileName"
Solution for codelite: Add "comdlg32.dll" to the linker library in the project.
Source: https://stackoverflow.com/questions/40751685/using-getopenfilename-in-dev-c-vs      https://i.stack.imgur.com/Ewi8Q.png
Solution for codeblocks:    Rightclick project->Build options, Under Linker settings->Add, navigate
or paste the path: "C:\Windows\System32\comdlg32.dll" and OK
Source for linker location: https://c-for-dummies.com/blog/?p=2159
Source for path:    Common sense

- Lot's of room for inprovement here. I barely knew/know what I am doing here during the
open and save dialogue boxes. I just got help from discord user µBrain#6100 who googled
up the template used from lines 191 to 207 in the Window dialogue function found from:  http://www.cplusplus.com/forum/windows/169960/
*/

string AlphabetShuffled(string alpha);
void ConvertToSub(string &msg, string a_plain, string a_shuffled);
void ConvertToPlain(string &msg, string a_plain, string a_shuffled);
void BrowseToFile(string &usr_msg, string a_plain, string a_shuff, bool IO = true);


int main()
{
    int users_selection{};
    string alphabet_plain {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ"};
    string alphabet_shuffled{};
    string users_msg;
    bool OpnOrSave = true;
    
    
    while(true)
    {
        cout << "------------ Sub cifer ------------ \n\n";
        cout << "1) Create a sub cifer message with option to save it\n";
        cout << "2) Load a saved .txt file with a subbed message\n";
        cout << "3) Load a sub cifer message from clipboard\n";
        cout << "4) Exit\n\n";
        cout << "Select a number from 1 to 4 from the options above: ";
        while(!(cin >> users_selection) || users_selection < 1 || users_selection > 4)
        {
            cout << "Invalid entry. Try again: ";
            cin.clear();    //clear bad input flag
            cin.ignore(numeric_limits<streamsize>::max(), '\n');   //discard input
        }

        
        if(users_selection == 1)
        {
            alphabet_shuffled = AlphabetShuffled(alphabet_plain);
            char users_choice;
            cout << alphabet_plain << "\n";
            cout << alphabet_shuffled << "\n\n";
            
            cout << "Enter your message:\t\t";
            cin.ignore(numeric_limits<streamsize>::max(), '\n'); //or cin.get() would work too, to discard input from previous cin(?) or this will be skipped
            getline(cin, users_msg);
            
            cout << "You're subbed message:\t\t";
            ConvertToSub(users_msg, alphabet_plain, alphabet_shuffled);
            cout << users_msg << "\n\n";
            cout << "Would you like to save the the message to a txt file (y/n): ";

            while(cin >> users_choice && users_choice != 'y' && users_choice != 'Y' && users_choice != 'n' && users_choice != 'N')
            {
                cout << "Invalid entry, try again (y/n): ";
                cin >> users_choice;
                cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            
            if(users_choice == 'y' || users_choice == 'Y')
            {
                BrowseToFile(users_msg, alphabet_plain, alphabet_shuffled, !OpnOrSave);
            }
            cin.clear();
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        }
        else if(users_selection == 2)
        {
            BrowseToFile(users_msg, alphabet_plain, alphabet_shuffled);
            cout << "The original message is:\n\n";
            ConvertToPlain(users_msg, alphabet_plain, alphabet_shuffled);
            cout << users_msg;
        }
        
        else if(users_selection == 3)
        {
            cout << "Paste in the shuffled/subbed characters:\t";
            cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');  // ignore to not skip prompt. numeric limit stream size to not store anything in the buffer or multiple char's other than index 0 will be stored into "users_selection"(?).
                                                                            // "numeric_limits" requires the <limits> header file, but either one of the includes already tagged it or codelite ide includes it by default
            getline(cin, alphabet_shuffled);
            cout << "Paste in your message:\t\t\t\t";
            getline(cin, users_msg);
            
            cout << "The original message is:\t\t\t";
            ConvertToPlain(users_msg, alphabet_plain, alphabet_shuffled);
            cout << users_msg;
        }
        
        else
            return 0;
        users_msg = "";
        alphabet_shuffled = "";
        cout << "\n\n\n\n";
    }
}


string AlphabetShuffled(string alpha)
{
    srand(time(0));
    random_shuffle(alpha.begin(), alpha.end());
    return alpha;
}

void ConvertToSub(string &msg, string a_plain, string a_shuffled)
{
    for(size_t i{}; i < msg.size(); ++i)
    {
        if(a_plain.find(msg.at(i)) != string::npos)
        {
            msg.at(i) = a_shuffled.at(a_plain.find(msg.at(i)));
        }
    }
}

void ConvertToPlain(string &msg, string a_plain, string a_shuffled)
{
    for(size_t i{}; i < msg.size(); ++i)
    {
        if(a_shuffled.find(msg.at(i)) != string::npos)
        {
            msg.at(i) = a_plain.at(a_shuffled.find(msg.at(i)));
        }
    }
}

void BrowseToFile(string &usr_msg, string a_plain, string a_shuff, bool IO)
{
    OPENFILENAME ofn;           // common dialog box structure
    TCHAR szFile[260] = { 0 };  // if using TCHAR macros

    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = HWND_DESKTOP;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = TEXT("All\0*.*\0Text\0*.TXT\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

    // Everything above is for setting up the dialogu box.
    // At this time I lack a bit of knowledge on this and need to learn it fully.
    // The rest below was history/easy.
    if (IO == true)// Open it.     parameter was if(GetOpenFileName(&ofn) == TRUE)  <- Which would mean while the dialogue box is open..
    {
        GetOpenFileName(&ofn);      // This function opens the dialogue box
        fstream inp_obj;
        inp_obj.open(ofn.lpstrFile);
        if(inp_obj.is_open())
        {
            string  text_content{};
            int line{1};
            while(getline(inp_obj, text_content, '\n'))
            {
                if(line == 2)
                    a_shuff = text_content.substr(21,52); // starting index 21, length 52
                if(line >= 6)
                    usr_msg += text_content + "\n";
                ++line;
            };
            ConvertToPlain(usr_msg, a_plain, a_shuff);  // Msg converted to plain
        }
        else
            cout << "Error. Failed to open file from path.\nFile or dir inaccessible. Read/write permissions may be denied\n";
    }
    else    // Save it
    {
        GetSaveFileName(&ofn);     // This function saves the dialogue box
        fstream outp_obj;
        outp_obj.open(ofn.lpstrFile, ios::out | ios::app);
        if(outp_obj.is_open())
        {
            outp_obj << "Alphabet plain:\t\t[ " << a_plain << " ]\n";
            outp_obj << "Alphabet shuffled:\t[ " << a_shuff << " ]\n\n\n";
            outp_obj << "Message: \n";
            outp_obj << usr_msg << "\n\n";
            outp_obj.close();
            cout << "Message saved to: " << ofn.lpstrFile << "\n";
        }
        else
            cout << "Error. Failed to save file to that path.\nFile or dir inaccessible. Read/write permissions may be denied\n";
    }
}
