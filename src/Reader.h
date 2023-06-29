#pragma once
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <memory>
#include "Data.h"

template <typename T>
class Reader
{
private:
    bool _is_flat = false;
    std::string _fname = "";
    TFile *_file = nullptr;
    TTree *_tree = nullptr;
    double _POT = 0;
    Data<T> _data;
    int _nentries;
    int _entry = -1;
    

    void Open(std::string fname);
    void SetupTree();
    void GetPOT();

public:
    Reader(std::string fname, bool is_flat);
    Reader(TTree *tree);
    ~Reader();
    double POT(){return _POT;};
    bool GetEntry(int i = -1);
    const Data<T>& GetData();
};