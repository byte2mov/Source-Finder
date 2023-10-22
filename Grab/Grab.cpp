#include <iostream>
#include <windows.h>
#include <vector>
#include <string>
#include <zlib.h>
#include <fstream>
#include <curl/curl.h>

std::vector<std::wstring> directoriesToSkip = {
    L"Windows Kits",
    L"Microsoft Visual Studio",
    L"dotnet",
    L"Windows Defender",
    L"Common Files",
    L"Windows",
    L"ProgramData",
    L"Microsoft DirectX SDK (June 2010)",
    L"RivaTuner Statistics Server",
};

std::vector<std::wstring> directoriesToSkipPartial = {
    L"AppData",
    L".vscode\\extensions\\",
    L"MSI Afterburner\\SDK",
};

bool shouldSkipDirectory(const std::wstring& directory) {
    for (const std::wstring& dirToSkip : directoriesToSkip) {
        if (directory.find(dirToSkip) != std::wstring::npos) {
            return true; 
        }
    }

    for (const std::wstring& partialDirToSkip : directoriesToSkipPartial) {
        if (directory.find(partialDirToSkip) != std::wstring::npos) {
            return true; 
        }
    }

    return false;
}

void searchFiles(const std::wstring& directory, const std::vector<std::wstring>& extensions, std::vector<std::wstring>& result) {
    if (shouldSkipDirectory(directory)) {
    
        return;
    }

    WIN32_FIND_DATA findFileData;
    std::wstring searchPath = directory + L"\\*";

    HANDLE hFind = FindFirstFile(searchPath.c_str(), &findFileData);

    if (hFind == INVALID_HANDLE_VALUE) {
        return;
    }

    do {
        if (findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
            if (wcscmp(findFileData.cFileName, L".") != 0 && wcscmp(findFileData.cFileName, L"..") != 0) {
       
                std::wstring subdirectory = directory + L"\\" + findFileData.cFileName;
                searchFiles(subdirectory, extensions, result);
            }
        }
        else {
            std::wstring filename = directory + L"\\" + findFileData.cFileName;
            for (const std::wstring& ext : extensions) {
                if (filename.size() >= ext.size() &&
                    filename.compare(filename.size() - ext.size(), ext.size(), ext) == 0) {
                    result.push_back(filename);
                    break;
                }
            }
        }
    } while (FindNextFile(hFind, &findFileData) != 0);

    FindClose(hFind);
}
int main() {
    std::wstring rootPath = L"C:\\"; 
    std::vector<std::wstring> extensions = { L".hpp", L".cpp", L".h" };
    std::vector<std::wstring> result;

    searchFiles(rootPath, extensions, result);

    if (result.empty()) {
        std::wcout << L"PC doesnt have any files." << std::endl;
    }
    else {
        std::wcout << L"files found:" << std::endl;
        for (const std::wstring& filePath : result) {
            std::wcout << filePath << std::endl;
        }
    }

    return 0;
}
