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
const int max_symbols_size{88 + 2};
wchar_t key_symbols_original_wchar[max_symbols_size]
{
    1, 2, 3, 4, 5, 6, 11, 12, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 28, 29, 30,
    31, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138,
    139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155,
    156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172,
    173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186
}; // 88
wchar_t key_symbols_original_unicode[max_symbols_size]
{
    0x263A, 0x263B, 0x2665, 0x2666, 0x2663, 0x2660, 0x2642, 0x2640, 0x266B, 0x263C, 0x25BA,
    0x25C4, 0x2195, 0x203C, 0xB6, 0xA7, 0x25AC, 0x21A8, 0x2191, 0x2193, 0x221F, 0x2194, 0x25B2,
    0x25BC, 0x007B, 0x007C, 0x007D, 0x007E, 0x2302, 0xC7, 0xFC, 0xE9, 0xE2, 0xE4, 0xE0, 0xE5,
    0xE7, 0xEA, 0xEB, 0xE8, 0xEF, 0xEE, 0xEC, 0xC4, 0xC5, 0xC9, 0xE6, 0xC6, 0xF4, 0xF6, 0xF2,
    0xFB, 0xF9, 0xFF, 0xD6, 0xDC, 0xA2, 0xA3, 0xA5, 0x20A7, 0x192, 0xE1, 0xED, 0xF3, 0xFA,
    0xF1, 0xD1, 0xAA, 0xBA, 0xBF, 0x2310, 0xAC, 0xBD, 0xBC, 0xA1, 0xAB, 0xBB, 0x2591, 0x2592,
    0x2593, 0x2502, 0x2524, 0x2561, 0x2562, 0x2556, 0x2555, 0x2563, 0x2551
}; // 88
wchar_t key_symbols_shuffled_wchar[max_symbols_size]{};
wchar_t key_symbols_shuffled_unicode[max_symbols_size]{};

const int max_msg_size{5000};
wchar_t users_msg_subbed_wchar[max_msg_size]{};
wchar_t users_msg_subbed_unicode[max_msg_size];

void KeyShuffled(int a_plain_size);
void Substitute(string &msg, wchar_t msg_Subbed[], size_t msg_Subbed_size,  wchar_t k_shuffled[], string a_plain);
void BrowseToFile(string usr_msg_plain, string a_plain, wchar_t a_shuff[], bool IO = true);


int main()
{
    string users_selection{};
    string alphabet_plain {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,./?\"\';[]1234567890!@#$%^&*()-=_+ \n\t"};//88
    string alphabet_plain_display {"abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ,./?\"\';[]1234567890!@#$%^&*()-=_+ \\n\\t"};

    size_t users_msg_size;
    string users_msg;
    bool OpnOrSave = true;
    
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
            char users_choice{};
            cout << "Alphabet Plain:\t[ " << alphabet_plain_display << " ]\n";
            KeyShuffled(alphabet_plain.size());
            _setmode(_fileno(stdout), _O_U16TEXT);
            //wcout << "Ky/Sym unicode(org):\t[ " << key_symbols_original_unicode << " ]\n";
            wcout << "Key (symbols):\t[ " << key_symbols_shuffled_unicode << " ]\n\n";
            _setmode(_fileno(stdout), _O_TEXT);
            
            cout << "Enter your message (end it with a tilde `):\n\n";
            getline(cin, users_msg, '`');
            cout << "\n\nYou're subbed message:\n\n";
            Substitute(users_msg, users_msg_subbed_wchar, users_msg_size, key_symbols_shuffled_wchar, alphabet_plain);
            wcout << users_msg_subbed_wchar << "\n\n";
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
                users_msg_size = users_msg.size();
                BrowseToFile(users_msg, alphabet_plain, key_symbols_shuffled_unicode, !OpnOrSave);
                getline(cin, users_msg);
            }
        }
        else if(users_selection == "2")
        {
            BrowseToFile(users_msg, alphabet_plain, key_symbols_shuffled_unicode, OpnOrSave);
            cout << "The original message is:\n\n";
            users_msg_size = sizeof(users_msg_subbed_unicode) / sizeof(users_msg_subbed_unicode[0]);
            Substitute(users_msg, users_msg_subbed_unicode, users_msg_size, key_symbols_shuffled_unicode, alphabet_plain);
            cout << users_msg;
        }
        
        else if(users_selection == "3")
        {
            cout << "Paste in the shuffled/subbed characters:\t";
            wcin.getline(key_symbols_shuffled_wchar, max_msg_size);
            cout << "\nPaste in your message: (max 5000 chars):\n\n";
            wcin.getline(users_msg_subbed_wchar, max_msg_size);
            
            cout << "\n\nThe original message is:\n\n";
            users_msg_size = sizeof(users_msg_subbed_wchar) / sizeof(users_msg_subbed_wchar[0]);
            Substitute(users_msg, users_msg_subbed_wchar, users_msg_size, key_symbols_shuffled_wchar, alphabet_plain);
            cout << users_msg << "\n";
        }
        
        else
            break;
            
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        users_msg = "";
        for(size_t i{}; i < users_msg_size; ++i)
        {
            users_msg_subbed_wchar[i] = 0;
            users_msg_subbed_unicode[i] = 0;
        }
        users_msg_size = 0;
        system("cls");
    }
}


void KeyShuffled(int a_plain_size)
{
    for(int i{}; i < a_plain_size; ++i)
        key_symbols_shuffled_wchar[i] = key_symbols_original_wchar[i];

    srand(time(0));
    random_shuffle(key_symbols_shuffled_wchar, key_symbols_shuffled_wchar + a_plain_size);

    for (int i = 0; i < a_plain_size; ++i)
        for(int j{}; j < a_plain_size; ++j)
            if(key_symbols_shuffled_wchar[j] == key_symbols_original_wchar[i])
                key_symbols_shuffled_unicode[j] = key_symbols_original_unicode[i];
}

void Substitute(string &msg,  wchar_t msg_Subbed[], size_t msg_Subbed_size, wchar_t k_shuffled[], string a_plain)
{
    if(msg.size() == 0)
    {
        //size_t users_msg_size = sizeof(users_msg_subbed_wchar) / sizeof(users_msg_subbed_wchar[0]);
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
                users_msg_subbed_wchar[i] = k_shuffled[a_plain.find(msg.at(i))];
            }
        }
    }

}

void BrowseToFile(string usr_msg_plain, string a_plain, wchar_t a_shuff[], bool IO)
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
                            key_symbols_shuffled_unicode[letters_per_line] = text_content2[i];
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
            outp_obj << "Key (symbols):\t\t[ " << a_shuff << " ]\n\n\n\n";
            outp_obj << "\t\t\t\t\t[\tMESSAGE\t\t]\n\n";
            Substitute(usr_msg_plain, users_msg_subbed_wchar, 0, a_shuff, a_plain);
            for(size_t i{}; i < usr_msg_plain.size(); ++i)
            {
                outp_obj << users_msg_subbed_wchar[i];
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
