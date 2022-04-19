#include <iostream>
#include <string>
#include <cstdlib>      // srand
#include <ctime>        // time
#include <algorithm>    // std::random_shuffle
#include <fstream>
//#define NOMINMAX    // For VISUAL STUDIOS compiler: To use the MAX keyword from cin.ignore(numeric_limits..) in VisualStudios

#include <windows.h>
using std::cout;
using std::cin;
using std::string;
using std::numeric_limits;
using std::streamsize;
using std::fstream;
using std::ios;

string AlphabetShuffled(string alpha);
void ConvertToSub(string &msg, string a_plain, string a_shuffled);
void ConvertToPlain(string &msg, string a_plain, string a_shuffled);
void BrowseToFile(string &usr_msg, string a_plain, string a_shuff, bool IO = true);

int main()
{
    string users_selection{""};
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
        while(getline(cin, users_selection, '\n') && users_selection != "1" && users_selection != "2" && users_selection != "3" && users_selection != "4")
        {
            cout << "Invalid entry. Try again: ";
        }
        
        cout << "\n";
        if(users_selection == "1")
        {
            alphabet_shuffled = AlphabetShuffled(alphabet_plain);
            char users_choice{};
            cout << alphabet_plain << "\n";
            cout << alphabet_shuffled << "\n\n";
            
            cout << "Enter your message:\t\t";
            cin >> users_msg;
            
            cout << "You're subbed message:\t\t";
            ConvertToSub(users_msg, alphabet_plain, alphabet_shuffled);
            cout << users_msg << "\n\n";
            cout << "Would you like to save the the message to a txt file (y/n): ";
            users_choice = cin.get();
            while(cin.get(users_choice) && users_choice != 'y' && users_choice != 'Y' && users_choice != 'n' && users_choice != 'N')
            {
                cout << "Invalid entry, try again (y/n): ";
                if(users_choice != '\n') // cin.get has no delimiter, so this step is nessessary
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            
            if(users_choice == 'y' || users_choice == 'Y')
            {
                BrowseToFile(users_msg, alphabet_plain, alphabet_shuffled, !OpnOrSave);
            }
        }
        else if(users_selection == "2")
        {
            BrowseToFile(users_msg, alphabet_plain, alphabet_shuffled);
            cout << "The original message is:\n\n";
            ConvertToPlain(users_msg, alphabet_plain, alphabet_shuffled);
            cout << users_msg;
        }
        
        else if(users_selection == "3")
        {
            cout << "Paste in the shuffled/subbed characters:\t";
            getline(cin, alphabet_shuffled);
            cout << "Paste in your message:\t\t\t\t";
            getline(cin, users_msg);
            
            cout << "The original message is:\t\t\t";
            ConvertToPlain(users_msg, alphabet_plain, alphabet_shuffled);
            cout << users_msg;
        }
        
        else
            break;
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
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
