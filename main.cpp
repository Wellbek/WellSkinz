#include <iostream>
#include <fstream>
#include <string>

#include <thread>
#include <chrono>
#include <ctime>

#include <windows.h>

#include "Functions.h"
#include "Globals.h"
#include "WeaponIDs.h"

#include "json/json.h"
#include "color.hpp"

#define stringify( name ) # name
#define underlineStart "\033[4m"
#define underlineEnd "\033[0m"
//#define _WIN32_WINNT 0x0500

using namespace std;

int GetModelIndex(const char* modelName)
{
    clientstate = rpm<DWORD>(engine + csgo::dwClientState);
    int m_pModelPrecacheTable = rpm<int>(clientstate + 0x52A4);
    int nsd = rpm<int>(m_pModelPrecacheTable + 0x40);
    int m_pItems = rpm<int>(nsd + 0xC);
    int xxxxx = m_pItems + 0xC;

    for (int i = 0; i < 1024; i++)
    {
        int nsdi_i = rpm<int>(xxxxx + (i * 0x34));
        char str[128] = { 0 };
        if (ReadProcessMemory(pHandle, (LPCVOID)nsdi_i, str, sizeof(str), NULL))
        {
            if (_stricmp(str, modelName) == 0)
            {
                return i;
            }
        }
    }
    return 0;
}

int GetModelIndexByID(const short knifeID)
{
    int modelIndex = 0;
    switch (knifeID)
    {
    case WEAPON_KNIFE:
        modelIndex = GetModelIndex("models/weapons/v_knife_default_ct.mdl");
        break;
    case WEAPON_KNIFE_T:
        modelIndex = GetModelIndex("models/weapons/v_knife_default_t.mdl");
        break;
    case WEAPON_KNIFE_BAYONET:
        modelIndex = GetModelIndex("models/weapons/v_knife_bayonet.mdl");
        break;
    case WEAPON_KNIFE_BUTTERFLY:
        modelIndex = GetModelIndex("models/weapons/v_knife_butterfly.mdl");
        break;
    case WEAPON_KNIFE_FALCHION:
        modelIndex = GetModelIndex("models/weapons/v_knife_falchion_advanced.mdl");
        break;
    case WEAPON_KNIFE_FLIP:
        modelIndex = GetModelIndex("models/weapons/v_knife_flip.mdl");
        break;
    case WEAPON_KNIFE_GUT:
        modelIndex = GetModelIndex("models/weapons/v_knife_gut.mdl");
        break;
    case WEAPON_KNIFE_GYPSY_JACKKNIFE:
        modelIndex = GetModelIndex("models/weapons/v_knife_gypsy_jackknife.mdl");
        break;
    case WEAPON_KNIFE_KARAMBIT:
        modelIndex = GetModelIndex("models/weapons/v_knife_karam.mdl");
        break;
    case WEAPON_KNIFE_M9_BAYONET:
        modelIndex = GetModelIndex("models/weapons/v_knife_m9_bay.mdl");
        break;
    case WEAPON_KNIFE_PUSH:
        modelIndex = GetModelIndex("models/weapons/v_knife_push.mdl");
        break;
    case WEAPON_KNIFE_STILETTO:
        modelIndex = GetModelIndex("models/weapons/v_knife_stiletto.mdl");
        break;
    case WEAPON_KNIFE_SURVIVAL_BOWIE:
        modelIndex = GetModelIndex("models/weapons/v_knife_survival_bowie.mdl");
        break;
    case WEAPON_KNIFE_TACTICAL:
        modelIndex = GetModelIndex("models/weapons/v_knife_tactical.mdl");
        break;
    case WEAPON_KNIFE_URSUS:
        modelIndex = GetModelIndex("models/weapons/v_knife_ursus.mdl");
        break;
    case WEAPON_KNIFE_WIDOWMAKER:
        modelIndex = GetModelIndex("models/weapons/v_knife_widowmaker.mdl");
        break;
    default:
        break;
    }
    return modelIndex;
}

void Skinchanger(Json::Value _skindata){
    for (int i = 0; i < 8; i++)
    {
        int weapon = rpm<int>(localPlayer + csgo::m_hMyWeapons + i * 0x4) & 0xFFF;
        weapon = rpm<int>(client + csgo::dwEntityList + (weapon - 1) * 0x10);
        short weaponID = rpm<short>(weapon + csgo::m_iItemDefinitionIndex);
        if (weaponID == 0 || (weaponID > 49 && weaponID != weapon_m4a1_silencer && weaponID != weapon_usp_silencer && weaponID != WEAPON_KNIFE_T && !(weaponID >= 500 && weaponID <= 523))) { continue; } 
        
        //temp 
        int paintKit = 0;
        int entityQuality = 0;
        float wear = 0;
        int seed = 0;
        int statTrack = 0;

        //temp knife abfrage
        if ((weaponID == WEAPON_KNIFE || weaponID == WEAPON_KNIFE_T || (weaponID >= 500 && weaponID <= 523))){
            if (_skindata["weapon_knife"].isNull()) { continue; }
            
            paintKit = _skindata["weapon_knife"]["paintKit"].asInt();
            entityQuality = _skindata["weapon_knife"]["entityQuality"].asInt();
            wear = _skindata["weapon_knife"]["wear"].asFloat();
            seed = _skindata["weapon_knife"]["seed"].asInt();
            statTrack = _skindata["weapon_knife"]["statTrack"].asInt(); 

            wpm<short>(weapon + csgo::m_iItemDefinitionIndex, _skindata["weapon_knife"]["knife"].asInt());
            wpm<int>(weapon + csgo::m_nModelIndex, knifeIndex);
        }else {
            if (_skindata[CSGO_WEAPON_NAMES[weaponID]].empty()) { continue; }
            
            paintKit = _skindata[CSGO_WEAPON_NAMES[weaponID]]["paintKit"].asInt();
            entityQuality = _skindata[CSGO_WEAPON_NAMES[weaponID]]["entityQuality"].asInt();
            wear = _skindata[CSGO_WEAPON_NAMES[weaponID]]["wear"].asFloat();
            seed = _skindata[CSGO_WEAPON_NAMES[weaponID]]["seed"].asInt();
            statTrack = _skindata[CSGO_WEAPON_NAMES[weaponID]]["statTrack"].asInt(); 
        }

        if (paintKit != 0)
        {
            bool requiresUpdate = rpm<int>(weapon + csgo::m_nFallbackPaintKit) != paintKit;

            if (rpm<int>(weapon + csgo::m_iItemIDHigh) != -1)
                wpm<int>(weapon + csgo::m_iItemIDHigh, -1);

            wpm<int>(weapon + csgo::m_OriginalOwnerXuidLow, 0);
            wpm<int>(weapon + csgo::m_OriginalOwnerXuidHigh, 0);
            wpm<int>(weapon + csgo::m_nFallbackPaintKit, paintKit);
            wpm<int>(weapon + csgo::m_nFallbackSeed, seed);
            wpm<int>(weapon + csgo::m_nFallbackStatTrak, statTrack);
            wpm<float>(weapon + csgo::m_flFallbackWear, wear);

            if (statTrack >= 0)
                wpm<int>(weapon + csgo::m_iEntityQuality, 9);
            else
                wpm<int>(weapon + csgo::m_iEntityQuality, entityQuality);
            
            if (requiresUpdate)
                wpm<int>(clientstate + csgo::clientstate_delta_ticks, -1); //force update             
        }
    }
}

void Knifechanger(){
    if (knifeIndex == 0) { return; }

    int curWeapon = rpm<int>(localPlayer + csgo::m_hActiveWeapon) & 0xFFF;
    curWeapon = rpm<int>(client + csgo::dwEntityList + (curWeapon - 1) * 0x10); 
    short curWeaponID = rpm<short>(curWeapon + csgo::m_iItemDefinitionIndex);

    if (curWeaponID == WEAPON_KNIFE || curWeaponID == WEAPON_KNIFE_T || (curWeaponID >= 500 && curWeaponID <= 523)){
        DWORD knifeViewModel = rpm<DWORD>(localPlayer + csgo::m_hViewModel) & 0xfff;
        knifeViewModel = rpm<DWORD>(client + csgo::dwEntityList + (knifeViewModel - 1) * 0x10);

        if (knifeViewModel != 0)
            wpm<DWORD>(knifeViewModel + csgo::m_nModelIndex, knifeIndex);
    }
}

void ClearConsole(){
    system("cls");
}

void PrintWelcome(){
    std::cout << R"(
 __          __  _ _    _____ _    _           
 \ \        / / | | |  / ____| |  (_)          
  \ \  /\  / /__| | | | (___ | | ___ _ __  ____
   \ \/  \/ / _ \ | |  \___ \| |/ / | '_ \|_  /
    \  /\  /  __/ | |  ____) |   <| | | | |/ / 
     \/  \/ \___|_|_| |_____/|_|\_\_|_| |_/___|  

)" << '\n';
}

void PrintStatus(bool skinchanger, bool autodisable){
    ClearConsole();
    PrintWelcome();

    cout << underlineStart << "Controls:" << underlineEnd << endl;
    cout << dye::yellow("[") << "F2" << dye::yellow("]") << "-> Apply Skins" << endl;
    cout << dye::yellow("[") << "F3" << dye::yellow("]") << "-> Toggle Auto-Disable" << endl;
    cout << dye::yellow("[") << "F4" << dye::yellow("]") << "-> Change Skin" << endl;
    cout << dye::yellow("[") << "F1" << dye::yellow("]") << "-> Change Knife" << endl;

    cout << '\n';

    cout << underlineStart << "Status:" << underlineEnd << endl;
    if (autodisable) cout << "Auto disable after 2 sec: " << dye::green("ON") << endl;
    else cout << "Auto disable after 2 sec: " << dye::red("OFF") << endl;

    if (skinchanger) cout << "Skinchanger: " << dye::green("ON") << endl;
    else cout << "Skinchanger: " << dye::red("OFF") << endl;
}

int SelectWeaponToChangeSkin(){
    ClearConsole();
    cout << "Which weapon do you wish to change the skin of?" << endl;
    cout << "===============================================" << endl;
    cout << dye::red("0") << " - Cancel request" << endl;
    cout << "===============================================" << endl;
    cout << dye::yellow("1") << "   | Deagle" << endl;
    cout << dye::yellow("2") << "   | Dual Berettas" << endl;
    cout << dye::yellow("3") << "   | Fiveseven" << endl;
    cout << dye::yellow("4") << "   | Glock" << endl;
    cout << dye::yellow("7") << "   | AK47" << endl;
    cout << dye::yellow("8") << "   | AUG" << endl;
    cout << dye::yellow("9") << "   | AWP" << endl;
    cout << dye::yellow("10") << "  | Famas" << endl;
    cout << dye::yellow("11") << "  | G3SG1" << endl;
    cout << dye::yellow("13") << "  | Galil" << endl;
    cout << dye::yellow("14") << "  | M249" << endl;
    cout << dye::yellow("16") << "  | M4A4" << endl;
    cout << dye::yellow("17") << "  | MAC10" << endl;
    cout << dye::yellow("19") << "  | P90" << endl;
    cout << dye::yellow("24") << "  | UMP" << endl;
    cout << dye::yellow("25") << "  | XM1014" << endl;
    cout << dye::yellow("26") << "  | Bizon" << endl;
    cout << dye::yellow("27") << "  | MAG7" << endl;
    cout << dye::yellow("28") << "  | Negev" << endl;
    cout << dye::yellow("29") << "  | Sawedoff" << endl;
    cout << dye::yellow("30") << "  | TEC9" << endl;
    cout << dye::yellow("32") << "  | P2000" << endl;
    cout << dye::yellow("33") << "  | MP7" << endl;
    cout << dye::yellow("34") << "  | MP9" << endl;
    cout << dye::yellow("35") << "  | Nova" << endl;
    cout << dye::yellow("36") << "  | P250" << endl;
    cout << dye::yellow("38") << "  | Scar20" << endl;
    cout << dye::yellow("39") << "  | SG556" << endl;
    cout << dye::yellow("40") << "  | SSG08" << endl;
    cout << dye::yellow("60") << "  | M4A1 Silencer" << endl;
    cout << dye::yellow("61") << "  | USP Silencer" << endl;
    cout << dye::yellow("63") << "  | CZ75A" << endl;
    cout << dye::yellow("64") << "  | Revolver" << endl;
    cout << "===============================================" << endl;
    cout << "Please type-in the desired WeaponID" << endl;
    int weaponToChange = 0; 
    cin >> weaponToChange;
    return weaponToChange;
}

int SelectKnifeToChangeSkin(){
    ClearConsole();
    cout << "Which Knife do you wish to change the skin of?" << endl;
    cout << "Note: Some knives may currently have bugged animations" << endl;
    cout << "===============================================" << endl;
    cout << dye::red("0") << " - Cancel request" << endl;
    cout << "===============================================" << endl;
    cout << dye::yellow("500") << " | Bayonet" << endl;
    cout << dye::yellow("505") << " | Flip" << endl;
    cout << dye::yellow("506") << " | Gut" << endl;
    cout << dye::yellow("507") << " | Karambit" << endl;
    cout << dye::yellow("508") << " | M9 Bayonet" << endl;
    cout << dye::yellow("509") << " | Huntsman" << endl;
    cout << dye::yellow("512") << " | Falchion" << endl;
    cout << dye::yellow("514") << " | Survival Bowie" << endl;
    cout << dye::yellow("515") << " | Butterfly" << endl;
    cout << dye::yellow("516") << " | Shadowdaggers" << endl;
    cout << dye::yellow("519") << " | Ursus" << endl;
    cout << dye::yellow("520") << " | Navaja" << endl;
    cout << dye::yellow("522") << " | Stiletto" << endl;
    cout << dye::yellow("523") << " | Talon" << endl;
    cout << "===============================================" << endl;
    cout << "Please type-in the desired KnifeID" << endl;
    int knifeToChange = 0; 
    cin >> knifeToChange;
    return knifeToChange;
}

void SelectNewSkin(int forWeapon, Json::Value _skindata){
    int paintKit = 0;
    int entityQuality = 0;
    float wear = 0.0001;
    int seed = 0;
    int statTrack = 0;
    bool knife = false;

    if (forWeapon == 0) return;
    else if(forWeapon >= 500) knife = true;
    
    for (int i = 0; i < 5; i++){
        ClearConsole();
        if (knife) cout << "New Skin for " << dye::yellow("weapon_knife") << ":" << endl;
        else cout << "New Skin for " << dye::yellow(CSGO_WEAPON_NAMES[forWeapon]) << ":" << endl;
        cout << "Skin IDs can be checked on sites like https://totalcsgo.com/skin-ids" << endl;  
        cout << "===============================================" << endl;
        cout << dye::red("BACK") << " - Cancel request" << endl;
        cout << dye::yellow("0") << " - System uses default value " << endl;
        cout << "===============================================" << endl;
        
        string input;
        switch (i){
            case 0:   
                cout << "Enter new skin ID (integer)" << endl;
                cin >> input;
                if (input == "BACK") return;
                paintKit = stoi(input);
                break;
            case 1:
                cout << "Enter new entity quality (integer)" << endl;
                cin >> input;
                if (input == "BACK") return;
                entityQuality = stoi(input);
                break;
            case 2:{
                    float newValue = 0;
                    cout << "Enter wear (float, e.g. 0.0001)" << endl; 
                    cin >> input;
                    if (input == "BACK") return;
                    newValue = stof(input);
                    //if (newValue > 0) wear = newValue; //Conversion ist not right or smth we get values like 9.9999997473787516e-005 so this feature disabled for now
                    break;
                }
            case 3:
                cout << "Enter seed (integer)" << endl;
                cin >> input;
                if (input == "BACK") return;
                seed = stoi(input);
                break;
            case 4:
                cout << "Enter StatTrack amount (integer)" << endl;
                cin >> input;
                if (input == "BACK") return;
                statTrack = stoi(input);
                break;
        }
    }

    if (knife){
        _skindata["weapon_knife"]["knife"] = forWeapon;
        _skindata["weapon_knife"]["paintKit"] = paintKit;
        _skindata["weapon_knife"]["entityQuality"] = entityQuality;
        _skindata["weapon_knife"]["wear"] = wear;
        _skindata["weapon_knife"]["seed"] = seed;
        _skindata["weapon_knife"]["statTrack"] = statTrack;
    } else {
        _skindata[CSGO_WEAPON_NAMES[forWeapon]]["paintKit"] = paintKit;
        _skindata[CSGO_WEAPON_NAMES[forWeapon]]["entityQuality"] = entityQuality;
        _skindata[CSGO_WEAPON_NAMES[forWeapon]]["wear"] = wear;
        _skindata[CSGO_WEAPON_NAMES[forWeapon]]["seed"] = seed;
        _skindata[CSGO_WEAPON_NAMES[forWeapon]]["statTrack"] = statTrack;
    }

    Json::StreamWriterBuilder builder;
    builder["commentStyle"] = "None";
    builder["indentation"] = "   ";

    std::unique_ptr<Json::StreamWriter> writer(builder.newStreamWriter());
    std::ofstream outputFileStream("skindata.json");
    writer -> write(_skindata, &outputFileStream);

    if (knife) knifeIndex = GetModelIndexByID(_skindata["weapon_knife"]["knife"].asInt());
}

//https://stackoverflow.com/questions/27068650/c-console-application-always-on-top/27076663#27076663
void ConsoleOnTop(){
    HWND consoleWindowHandle = GetConsoleWindow();

    if( consoleWindowHandle ){
        cout << endl << "Setting up associated console window ON TOP !";
        SetWindowPos(
            consoleWindowHandle,
            HWND_TOPMOST, 
            0, 0, 
            0, 0, 
            SWP_DRAWFRAME | SWP_NOMOVE | SWP_NOSIZE | SWP_SHOWWINDOW 
        );
        ShowWindow(
            consoleWindowHandle,
            SW_NORMAL
        );
        cout << endl << "Done.";
    } else {
        cout << endl << "There is no console window associated with this app :(";
    }   
}
    
int main() {   
    if (AttachProcess("csgo.exe")) {
	    client = GetModule("client.dll"); 
	    engine = GetModule("engine.dll");
	    clientstate = rpm<DWORD>(engine + csgo::dwClientState);

        bool skinTrigger = false;
        bool autoDisableTrigger = false;
        bool trigger = false;
        bool autoDisable = false;
        bool skinChangeTrigger = false;
        bool knifeChangeTrigger = false;

        auto autoStart = chrono::system_clock::now();
        auto autoEnd = chrono::system_clock::now();

        cout << boolalpha; 

        Json::Reader jsonReader;
        Json::Value skindata;
        ifstream file("skindata.json");
        file >> skindata;
        jsonReader.parse(file, skindata);

        knifeIndex = GetModelIndexByID(skindata["weapon_knife"]["knife"].asInt());
        //cout << knifeIndex << endl;

        ConsoleOnTop();
        PrintStatus(trigger, autoDisable);

	    while (!GetAsyncKeyState(VK_DELETE)) 
	    {
		    localPlayer = rpm<DWORD>(client + csgo::dwLocalPlayer); 

            if (GetAsyncKeyState(VK_F2) && skinTrigger == false)
            {
                skinTrigger = true;
                trigger = !trigger;
                if (trigger) {
                    //cout << "SKIN CHANGER: " << trigger << endl;
                    ifstream file("skindata.json");
                    file >> skindata;
                    jsonReader.parse(file, skindata);
                    if (autoDisable){
                        autoStart = chrono::system_clock::now();
                    }
                }
                //else cout << "SKIN CHANGER: " << trigger << endl;
                PrintStatus(trigger, autoDisable);
            }
            if (!GetAsyncKeyState(VK_F2) && skinTrigger == true)
            {
                skinTrigger = false;
            }
            if (trigger == true)
            {
 				Skinchanger(skindata);
            }

            if (GetAsyncKeyState(VK_F3) && autoDisableTrigger == false)
            {
                autoDisableTrigger = true;
                autoDisable = !autoDisable;
                //cout << "AUTO DISABLE AFTER 2 SEC: " << autoDisable << endl;
                PrintStatus(trigger, autoDisable);
            }
            if (!GetAsyncKeyState(VK_F3) && autoDisableTrigger == true)
            {
                autoDisableTrigger = false;
            }
            if (autoDisable == true && trigger == true){
                autoEnd = chrono::system_clock::now();
                auto elapsedSeconds = chrono::duration_cast<chrono::seconds>(autoEnd - autoStart).count();
                //cout << "elapsedSeconds: " << elapsedSeconds << endl;
                if(elapsedSeconds >= 2){
                    trigger = false;
                    //cout << "SKIN CHANGER: " << trigger << endl;
                    PrintStatus(trigger, autoDisable);
                }
            }

            if (GetAsyncKeyState(VK_F4) && skinChangeTrigger == false)
            {
                skinChangeTrigger = true;
                trigger = false;
                SelectNewSkin(SelectWeaponToChangeSkin(), skindata);
                PrintStatus(trigger, autoDisable);
            }
            if (!GetAsyncKeyState(VK_F4) && skinChangeTrigger == true)
            {
                skinChangeTrigger = false;
            }

            if (GetAsyncKeyState(VK_F1) && knifeChangeTrigger == false)
            {
                knifeChangeTrigger = true;
                trigger = false;
                SelectNewSkin(SelectKnifeToChangeSkin(), skindata);
                PrintStatus(trigger, autoDisable);
            }
            if (!GetAsyncKeyState(VK_F1) && knifeChangeTrigger == true)
            {
                knifeChangeTrigger = false;
            }

            Knifechanger();
	    }
    }
	CloseHandle(pHandle);
}
