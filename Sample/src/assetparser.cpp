// Class automatically generated by Dev-C++ New Class wizard

#include "assetparser.h" // class's header file

vector<string> split(const string& s, const char d) {
    vector<string> result;
    stringstream ss(s);
    string temp;
    
    while (getline(ss, temp, d)) {
        result.push_back(temp);
    }
    return result;
}

string replace_all(const string& s, const string& p, const string& r) {
    string result = s;
    size_t pos = 0;
    size_t offset = 0;
    while ((pos = result.find(p, offset)) != string::npos) {
        result.replace(result.begin() + pos, result.begin() + pos + p.size(), r);
        offset = pos + r.size();
    }
    return result;
}


bool AssetIterator::Next(string& id, string& value) { 
    if (asset && iter != asset->end()) {
        string temp = *iter++;
        vector<string> pair = split(temp, '=');
        if(pair.size() >= 2 && pair[0].length() != 0 && pair[1].length() != 0) {
            id = pair[0];
            value = pair[1];
            return true;
        }
    }
    return false;
}


AssetParser::AssetParser() {
}

AssetParser::~AssetParser() {
    asset_map::iterator iter(assets.begin());
    while(iter != assets.end()) {
        delete iter++->second;
    }
    assets.clear();
}



bool AssetParser::LoadAssetPackFile(string filename) {
    if ( !ValidateAssetPackFile(filename) )
        return false;
        
    ifstream file(filename.c_str());
    string str;
    while (getline(file, str)) {
        // Remove Tab And Space Char
        str = replace_all(str, "\t", "");
        str = replace_all(str, " ", "");
            
        // Check Is Blank line
        if (str.length() == 0)
            continue;
        
        // Check Is Comment line
        if (str.at(0) == '/' && str.at(1) == '/')
            continue;
            
        // Check Is Begin Of Asset Typedef
        if (str.at(0) == '[' && str.at(str.length() - 1) == ']') {
            // Strip string
            str = replace_all(str, "[", "");
            str = replace_all(str, "]", "");
            
            // Insert New Asset Typedef
            if ( !IsTypeExsits(str) )
                InsertNewAssetType(str);
            
            string value;
            vector<string>* assetVec = GetAssetVector(str);
            while (getline(file, value)) {
                value = replace_all(value, "\t", "");
                value = replace_all(value, " ", "");
                if (value.empty() || value.size() < 3)
                    continue;
                else if (value.at(0) == '[' && value.at(1) == '/' && value.at(value.length() - 1) == ']') {
                    // End of Assets Def
                    value = replace_all(value, "[", "");
                    value = replace_all(value, "]", "");
                    value = replace_all(value, "/", "");
                    if (value == str)
                        break;
                }
                assetVec->push_back(value);
            }
        }
    }
    file.close();
    return true;
}


bool AssetParser::IsTypeExsits(string type) {
    asset_map::iterator iter(assets.find(type));
    return iter != assets.end();
}

AssetIterator AssetParser::GetAsset(string type) {
    asset_map::iterator iter(assets.find(type));
    return iter != assets.end() ? AssetIterator(iter->second) : AssetIterator();
}    

bool AssetParser::ValidateFileExists(string filename) {
    ifstream file(filename.c_str());
    if(!file.is_open())
        return false;
    file.close();
    return true;
}


bool AssetParser::ValidateAssetPackFile(string filename) {
    ifstream file(filename.c_str());
    if(!file.is_open())
        return false;
    
    string str;
    string type = "";
    bool defBegin = false;
    while (getline(file, str)) {
        // Remove Tab And Space Char
        str = replace_all(str, "\t", "");
        str = replace_all(str, " ", "");
        
        // Check Is Blank line
        if (str.length() == 0)
            continue;
            
        // Check Is Comment line
        if (str.at(0) == '/' && str.at(1) == '/')
            continue;
        
        if (!defBegin && str.at(0) == '[' && str.at(str.length() - 1) == ']') {  
            // Begin of Asset Typedef 
            str = replace_all(str, "[", "");
            str = replace_all(str, "]", "");
            defBegin = true;
            type = str;
            continue;
        } else if (defBegin && str.at(0) == '[' && str.at(1) == '/' && str.at(str.length() - 1) == ']') {
            // End of Asset Typedef 
            str = replace_all(str, "[", "");
            str = replace_all(str, "]", "");
            str = replace_all(str, "/", "");
            if (str == type) {
                defBegin = false;
                type = "";
                continue;
            }
        }
    }
    file.close();
    return !defBegin;
}


void AssetParser::InsertNewAssetType(string type) {
    vector<string>* vec = new vector<string>();
    assets.insert(make_pair(type, vec));
}


vector<string>* AssetParser::GetAssetVector(string type) {
    asset_map::iterator iter(assets.find(type));
    if (iter != assets.end())
        return iter->second;
    else
        return NULL;
}
