#pragma once
#include "Data.h"
#include <string>
#include "TFile.h"
#include "TTree.h"

class Writer
{
private:
    
    TTree *_tree;
    Data<double> _data;

    void Create(std::string fname);
    void SetupTree();
public:
    TFile *_file;
    Writer(std::string fname);
    ~Writer();
    void Fill(const Data<double>& data);
    void Write();
    void WriteFile();
    TTree* GetTree();
};
