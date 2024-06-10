#pragma once
#include "TFile.h"
#include "TTree.h"
#include <iostream>
#include <memory>
#include "Data.h"

enum CAFtype{
    OLD_CAF,
    FLAT_CAF,
    NEW_CAF,
    UNDEFINED
};

template <typename T>
class Reader
{
private:
    CAFtype _caftype = CAFtype::UNDEFINED;
    std::string _fname = "";
    TFile *_file = nullptr;
    TTree *_tree = nullptr;
    TTree *_global_tree = nullptr;
    double _POT = 0;
    Data<T> _data;
    int _nentries;
    int _entry = -1;
    

    void Open(std::string fname);
    void SetupTree();
    void SetupTreeHierarchical();
    void GetPOT();
    CAFtype DetectCAFType();

public:
    Reader(std::string fname);
    Reader(TTree *tree);
    ~Reader();
    double POT(){return _POT;};
    bool GetEntry(int i = -1);
    const Data<T>& GetData();
    TTree* GetGlobalTree();
    TFile* GetFile();
};