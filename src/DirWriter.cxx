
#include <iostream>
#include "DirWriter.h"

DirWriter::DirWriter(std::string name, TFile *cur_file)
{
    this->Create(name);
    this->SetupTree();
}

DirWriter::~DirWriter()
{
}

void DirWriter::Create(std::string name)
{
    std::cout << "Create a directory in the same file" << std::endl;
    TDirectory *direc = new TDirectory(name.c_str(), name.c_str());

    _tree = new TTree("weights", "weights");
    _tree->SetDirectory(direc);
}

void DirWriter::SetupTree(){
    _tree->Branch("run", &_data.run);
    _tree->Branch("weight",&_data.weight);
    _tree->Branch("flux_nue",&_data.flux_nue);
    _tree->Branch("flux_numu",&_data.flux_numu);
    _tree->Branch("xsec",&_data.xsec);
    _tree->Branch("nue_w",&_data.nue_w);
    _tree->Branch("numu_w",&_data.numu_w);
    _tree->Branch("osc_from_e_w",&_data.osc_from_e_w);
    _tree->Branch("osc_from_mu_w",&_data.osc_from_mu_w);
    _tree->Branch("final_oscillated_w",&_data.final_oscillated_w);
    _tree->Branch("genie_weight",&_data.genie_weight);
}