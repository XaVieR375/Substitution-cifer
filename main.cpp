#include <iostream>
#include <string>
#include <cstdlib>      // srand
#include <ctime>        // time
#include <algorithm>    // std::random_shuffle
#include <fstream>
#include <io.h>         // for _fileno. For unicode conversion (ie _setmode(_fileno(stdout), _O_U16TEXT))
#include <fcntl.h>      // for _setmode. For unicode as well
#include <locale>       // for std::consume_header  https://en.cppreference.com/w/cpp/locale/codecvt_mode
#include <codecvt>      // for std::consume_header

//#define NOMINMAX    // For VISUAL STUDIOS compiler: To use the MAX keyword from cin.ignore(numeric_limits..) in VisualStudios
#include <windows.h>

using std::cout;
using std::cin;
using std::string;
using std::numeric_limits;
using std::streamsize;
using std::ios;
using std::wcout;
using std::wcin;
using std::wstring;
using std::random_shuffle;
using std::wfstream;

// Globals variables (bad practice, but alternative options like reference_wrapper seams like would create even more overhead)
wchar_t key_symbols_original_unicode[]
{
    0x263A, 0x263B, 0x2665, 0x2666, 0x2663, 0x2660, 0x2642, 0x2640, 0x266B, 0x263C, 0x25BA,
    0x25C4, 0x2195, 0x203C, 0xB6, 0xA7, 0x25AC, 0x21A8, 0x2191, 0x2193, 0x221F, 0x2194, 0x25B2,
    0x25BC, 0x007B, 0x007C, 0x007D, 0x007E, 0x2302, 0xC7, 0xFC, 0xE9, 0xE2, 0xE4, 0xE0, 0xE5,
    0xE7, 0xEA, 0xEB, 0xE8, 0xEF, 0xEE, 0xEC, 0xC4, 0xC5, 0xC9, 0xE6, 0xC6, 0xF4, 0xF6, 0xF2,
    0xFB, 0xF9, 0xFF, 0xD6, 0xDC, 0xA2, 0xA3, 0xA5, 0x20A7, 0x192, 0xE1, 0xED, 0xF3, 0xFA,
    0xF1, 0xD1, 0xAA, 0xBA, 0xBF, 0x2310, 0xAC, 0xBD, 0xBC, 0xA1, 0xAB, 0xBB, 0x2591, 0x2592,
    0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551, 0x2557, 0x255D,
    0x255C, 0x255B, 0x2510, 0x2514, 0x2534,  0x252C, 0x251C, 0x2500, 0x253C, 0x255E, 0x255F,
    0x255A, 0x2554, 0x2569, 0x2566, 0x2560, 0x2550, 0x256C, 0x2567, 0x2568, 0x2564, 0x2565,
    0x2559, 0x2558, 0x2552, 0x2553, 0x256B, 0x256A, 0x2518, 0x250C, 0x3B1, 0xDF, 0x393, 0x3C0,
    0x3A3, 0x3C3, 0xB5, 0x3C4, 0x3A6, 0x398, 0x3A9, 0x3B4, 0x221E, 0x3C6, 0x3B5, 0x2229, 0x2261,
    0xB1, 0x2265, 0x2264, 0x2320, 0x2321, 0xF7, 0x2248, 0xB0, 0x207F, 0xB2
}; // C syle string with 88 chars(symbols).    Unicode conversion source / Code table values:    https://r12a.github.io/app-conversion/
const int max_msg_size{5000};
wchar_t users_msg_subbed_unicode[max_msg_size];

void KeyShuffled();     // https://www.geeksforgeeks.org/shuffle-an-array-using-stl-in-c
void Substitute(string &msg, wchar_t msg_Subbed[], size_t msg_Subbed_size,  wchar_t k_shuffled[], string a_plain);
void BrowseToFile(string usr_msg_plain, string a_plain, bool IO = true);


int main()
{
    string alphabet_plain {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,./?\"\';[]1234567890!@#$%^&*()-=_+ \n\t"}; //88
    string alphabet_plain_display {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,./?\"\';[]1234567890!@#$%^&*()-=_+ \\n\\t"};
    string users_selection{};
    string users_msg;
    
    while(true)
    {
        cout << "------------ Sub Cipher ------------ \n\n";
        cout << "1) Create a sub cipher message with option to save it\n";
        cout << "2) Load a saved .txt file with a subbed message\n";
        cout << "3) Load a sub cipher message from clipboard\n";
        cout << "4) Exit\n\n";
        cout << "Select a number from 1 to 4 from the options above: ";
        while(getline(cin, users_selection, '\n') && users_selection != "1" && users_selection != "2" && users_selection != "3" && users_selection != "4")
            cout << "Invalid entry. Try again: ";
        
        cout << "\n";
        if(users_selection == "1")
        {
            cout << "Alphabet Plain:\t[ " << alphabet_plain_display << " ]\n";
            KeyShuffled();
            wcout << "Key (symbols):\t[ " ;
            _setmode(_fileno(stdout), _O_U16TEXT);
            for(size_t i{}; i < alphabet_plain.size(); ++i)
                wcout << key_symbols_original_unicode[i];
            wcout << " ]\n\n";
            _setmode(_fileno(stdout), _O_TEXT);
            
            cout << "Enter your message (end it with a tilde `):\n\n";
            getline(cin, users_msg, '`');
            cout << "\n\nYou're subbed message:\n\n";
            Substitute(users_msg, users_msg_subbed_unicode, 0, key_symbols_original_unicode, alphabet_plain);
            _setmode(_fileno(stdout), _O_U16TEXT);
            wcout << users_msg_subbed_unicode << "\n\n";
            _setmode(_fileno(stdout), _O_TEXT);
            cout << "Would you like to save the the message to a txt file (y/n): ";
            char users_choice{};
            users_choice = cin.get();
            while(cin.get(users_choice) && users_choice != 'y' && users_choice != 'Y' && users_choice != 'n' && users_choice != 'N')
            {
                cout << "Invalid entry, try again (y/n): ";
                if(users_choice != '\n')    // cin.get has no delimiter, so this step is nessessary
                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
            }
            
            if(users_choice == 'y' || users_choice == 'Y')
            {
                BrowseToFile(users_msg, alphabet_plain, false);
                getline(cin, users_msg);
            }
        }
        else if(users_selection == "2")
        {
            BrowseToFile(users_msg, alphabet_plain);
            cout << "The original message is:\n\n";
            Substitute(users_msg, users_msg_subbed_unicode, max_msg_size, key_symbols_original_unicode, alphabet_plain);
            cout << users_msg;
        }
        
        else if(users_selection == "3")
        {
            // Console mode inputs wchar values in decimal format so a new set of wchar variables
            // are used to avoid overwritting unicode values with decimal values
            wchar_t key_symbols_shuffled_wchar[max_msg_size]{};
            wchar_t users_msg_subbed_wchar[max_msg_size]{};
            
            cout << "Paste in the shuffled/subbed characters:\t";
            wcin.getline(key_symbols_shuffled_wchar, max_msg_size);
            cout << "\nPaste in your message: (max 5000 chars):\n\n";
            wcin.getline(users_msg_subbed_wchar, max_msg_size);
            
            cout << "\n\nThe original message is:\n\n";
            Substitute(users_msg, users_msg_subbed_wchar, max_msg_size, key_symbols_shuffled_wchar, alphabet_plain);
            cout << users_msg << "\n";
        }
        
        else
            break;
            
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        users_msg = "";
        for(size_t i{}; i < max_msg_size; ++i)
            users_msg_subbed_unicode[i] = 0;
        system("cls");
    }
}


void KeyShuffled()
{
    size_t key_symbols_original_unicode_size = sizeof(key_symbols_original_unicode) / sizeof(key_symbols_original_unicode[0]);
    srand(time(0));
    random_shuffle(key_symbols_original_unicode, key_symbols_original_unicode + key_symbols_original_unicode_size);
}

void Substitute(string &msg,  wchar_t msg_Subbed[], size_t msg_Subbed_size, wchar_t k_shuffled[], string a_plain)
{
    if(msg.size() == 0)
    {
        for(size_t i{}; i < msg_Subbed_size; ++i)
        {
            for(size_t j{}; j < a_plain.size(); ++j)
            {
                if(msg_Subbed[i] == k_shuffled[j])
                {
                    msg += a_plain.at(j);
                }
            }
        } 
    }
    else
    {
        for(size_t i{}; i < msg.size(); ++i)
        {
            if(a_plain.find(msg.at(i)) != string::npos)
            {
                msg_Subbed[i] = k_shuffled[a_plain.find(msg.at(i))];
            }
        }
    }

}

void BrowseToFile(string usr_msg_plain, string a_plain, bool IO)
{
    OPENFILENAME ofn;           // common dialog box structure
    TCHAR szFile[260] = { 0 };  // if using TCHAR macros
    
    // Initialize OPENFILENAME
    ZeroMemory(&ofn, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.hwndOwner = HWND_DESKTOP;
    ofn.lpstrFile = szFile;
    ofn.nMaxFile = sizeof(szFile);
    ofn.lpstrFilter = TEXT("Text Files (*.txt)\0*.txt\0All Files (*.*)\0*.*\0");
    ofn.nFilterIndex = 1;
    ofn.lpstrFileTitle = NULL;
    ofn.nMaxFileTitle = 0;
    ofn.lpstrInitialDir = NULL;
    ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_OVERWRITEPROMPT;
    ofn.lpstrDefExt = ofn.lpstrFilter;

    // Everything above is for setting up the dialogu box.
    // At this time I lack a bit of knowledge on this and need to learn it fully.
    // The rest below was history/easy.
    if (IO == true)// Open it.     parameter was if(GetOpenFileName(&ofn) == TRUE)  <- Which would mean while the dialogue box is open..
    {
        GetOpenFileName(&ofn);      // This function opens the dialogue box
        wfstream inp_obj;
        inp_obj.open(ofn.lpstrFile, ios::in);
        inp_obj.imbue(std::locale(inp_obj.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::codecvt_mode::little_endian>));
        if(inp_obj.is_open())
        {
            wchar_t text_content2[max_msg_size + 245]{};
            wstring users_msg_wstring{};
            int line{1};

            while(!inp_obj.eof())
            {
                inp_obj.getline(text_content2, max_msg_size);
                if(line == 4)
                    for(int i {}, letters_per_line{}; i < 109; ++i)
                        if(i > 17 && i < 106)
                        {
                            key_symbols_original_unicode[letters_per_line] = text_content2[i];
                            ++letters_per_line;
                        }
                
                if(line > 9)
                    users_msg_wstring += text_content2;
                ++line;
            };
            for(size_t i{}; i < users_msg_wstring.size(); ++i)
                users_msg_subbed_unicode[i] = users_msg_wstring[i];
        }
        else
            cout << "Error. Failed to open file from path.\nFile or dir inaccessible. Read/write permissions may be denied\n";
    }
    //else    // Save it
    else
    {
        GetSaveFileName(&ofn);     // This function saves the dialogue box
        wfstream outp_obj;
        outp_obj.open(ofn.lpstrFile, ios::out| ios::binary);
        outp_obj.imbue(std::locale(outp_obj.getloc(), new std::codecvt_utf16<wchar_t, 0x10ffff, std::codecvt_mode::little_endian>));

        if(outp_obj.is_open())
        {
            outp_obj << "\nAlphabet plain:\t\t[ ";
            for(size_t i{}; i < a_plain.size()-2; ++i)
                outp_obj << (char)a_plain.at(i);
            outp_obj << "\\t\\n ]\n\n";
            outp_obj << "Key (symbols):\t\t[ ";
            for(size_t i{}; i < a_plain.size(); ++i)
                outp_obj << key_symbols_original_unicode[i];
            outp_obj << " ]\n\n\n\n";
            outp_obj << "\t\t\t\t\t[\tMESSAGE\t\t]\n\n";
            for(size_t i{}; i < usr_msg_plain.size(); ++i)
            {
                outp_obj << users_msg_subbed_unicode[i];
                if(i != 0 && i % 100 == 0)
                    outp_obj << "\n";
            }                
            outp_obj << "\n\n\n\n";
            outp_obj.close();
            cout << "Message saved to: " << ofn.lpstrFile << "\n";
        }
        else
            cout << "Error. Failed to save file to that path.\nFile or dir inaccessible. Read/write permissions may be denied\n";
    }
}
