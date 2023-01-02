//
// Created by j3280 on 2023/1/1.
//
#include <iostream>
#include <string>
#include <map>
#include <dirent.h>
#include <string>
#include <windows.h>
#include <vector>
#include <iostream>
#include <sys/stat.h>
#include <io.h>
#include "argparse.hpp"

#define MAXPATH 250
using std::ifstream;

void Color(short x)    //自定义函根据参数改变颜色
{
    if (x >= 0 && x <= 15)//参数在0-15的范围颜色
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), x);    //只有一个参数，改变字体颜色
    else//默认的颜色白色
        SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 7);
}

void Read_Folder(std::string fileDir, std::vector<std::string> &files) {
    struct dirent *ent = NULL;
    DIR *dir = opendir(fileDir.c_str());
    if (dir != NULL) {
        while ((ent = readdir(dir)) != NULL) {
            if (strcmp(ent->d_name, ".") != 0 && strcmp(ent->d_name, "..") != 0) {
                //std::cout << ent->d_name << std::endl;
                files.push_back(ent->d_name);
            }
        }
        closedir(dir);

    } else {
        std::cout << "failed to open directory" << std::endl;
    }
}

void SetColorAndBackground(int ForgC, int BackC) {
    WORD wColor = ((BackC & 0x0F) << 4) + (ForgC & 0x0F);
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), wColor);
}

int CheckPath(const std::string strPath) {
    struct stat infos{};

    if (stat(strPath.c_str(), &infos) != 0) {
        return -1;    //无效
    } else if (infos.st_mode & S_IFDIR) {
        return 0;    //目录
    } else if (infos.st_mode & S_IFREG) {
        return 1;    //文件
    } else {
        return -1;
    }
}

long getFileSize(const char *strFileName) {
    FILE *fp = fopen(strFileName, "r");
    fseek(fp, 0L, SEEK_END);
    long size = ftell(fp);
    fclose(fp);
    return size;
}

struct stat sb{};
long file_size;
std::vector<int> files_Sort;

std::vector<int> files_Size;

void Bubble_Sort(std::vector<int> vec) {
    for (int i = 0; i < vec.size() - 1; i++) {
        for (int j = 0; j < vec.size() - 1 - i; j++) {
            if (vec[j] > vec[j + 1]) {
                int tmpe = vec[j];
                vec[j] = vec[j + 1];
                vec[j + 1] = tmpe;
            }
        }
    }
}

void ll_main() {
    std::vector<std::string> files;
    char buffer[MAXPATH];
    getcwd(buffer, MAXPATH);
    Read_Folder(buffer, files);
    std::string Slash = "\\";
    for (int i = 0; i < files.size(); i++) {
        std::string File_Folder = buffer + Slash + files[i];
        if (CheckPath(File_Folder) == 0) {
            Color(3);
            std::cout << files[i] << "\\" << std::endl;
            Color(16);
//            std::cout << "   ";
        } else if (CheckPath(File_Folder) == 1) {
            if (!stat(File_Folder.c_str(), &sb)) {
                file_size = sb.st_size;
                files_Size.push_back(file_size);
            } else {
                perror("stat");
            }
            Color(2);
//            file_size = std::filesystem::file_size(File_Folder)
            std::cout << files[i] << "\tsize: " << file_size << " kib" << std::endl;
            Color(16);
        }
        Color(16);
    }
}

void ls_main() {
    std::vector<std::string> files;
    char buffer[MAXPATH];
    getcwd(buffer, MAXPATH);
    Read_Folder(buffer, files);
    std::string Slash = "\\";
    for (int i = 0; i < files.size(); i++) {
        std::string File_Folder = buffer + Slash + files[i];
        if (CheckPath(File_Folder) == 0) {
            Color(3);
            std::cout << files[i] << "\\   ";
            Color(16);
        } else if (CheckPath(File_Folder) == 1) {
            Color(2);
            std::cout << files[i];
            Color(16);
            std::cout << "   ";
        }
        Color(16);
    }
}

int main(int argc, char const *argv[]) {
    auto args = util::argparser("A quantum physics calculation program.");
    args.set_program_name("test")
            .add_help_option()
            .add_sc_option("-v", "--version", "查看版本号", []() {
                std::cout << "version " << 1.0 << std::endl;
            })
            .add_option("-y", "--yes", "一行一个文件", false)
            .add_option("-n", "--no", "一行多个文件", true).parse(argc, argv);

    if (args.has_option("--yes")) {
        ll_main();
    } else if(args.has_option("--no")) {
        ls_main();
    }
}