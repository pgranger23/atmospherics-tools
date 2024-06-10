#pragma once
#include "Writer.h"

class DirWriter : public Writer
{
private:
    void SetupTree();
    void Create(std::string name);
public:
    DirWriter(std::string name, TFile *cur_file);
    ~DirWriter();
};
