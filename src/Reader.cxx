#include "Reader.h"

template<typename T>
Reader<T>::Reader(std::string fname, bool is_flat)
: _is_flat(is_flat)
{
   this->Open(fname);
   _data = Data<T>();
   this->SetupTree();
   this->GetPOT();
   _entry = 0;
   _nentries = _tree->GetEntries();
}

template<typename T>
Reader<T>::Reader(TTree *tree)
: _tree(tree)
{
   _data = Data<T>();
   this->SetupTree();
   _entry = 0;
   _nentries = _tree->GetEntries();
}

template<typename T>
Reader<T>::~Reader()
{
    if(_file){
        _file->Close();
    }
}

template<typename T>
void Reader<T>::Open(std::string fname)
{
    std::cout << "Opening file: " << fname << std::endl;
    _file = TFile::Open(fname.c_str(), "READ");
    if(_file == nullptr){
        std::cout << "Could not open file: " << fname << std::endl;
        abort();
    }
    TTree *tree;
    std::string tree_name = this->_is_flat ? "cafTree" : "cafTree";
    _file->GetObject(tree_name.c_str(), tree);
    if(tree == nullptr){
        std::cout << "No tree named " << tree_name << " in file" << std::endl;
        abort();
    }
    _tree = tree;


    //Loading the gloabl tree
    TTree *global_tree;
    _file->GetObject("globalTree", global_tree);
    if(global_tree == nullptr){
        std::cout << "WARNING: No tree named 'globalTree' in the input file " << fname << std::endl;
    }
    _global_tree = global_tree;
}

template<typename T>
void Reader<T>::SetupTree(){
    TString start_str = _is_flat ? "rec." : "";
    _tree->SetBranchAddress(start_str + "Ev", &_data.ev);
    _tree->SetBranchAddress(start_str + "Ev_reco_numu", &_data.erec);
    _tree->SetBranchAddress(start_str + "Ev_reco_nue", &_data.erec_nue);
    _tree->SetBranchAddress(start_str + "mode",&_data.mode);
    _tree->SetBranchAddress(start_str + "NuMomX", &_data.NuMomX);
    _tree->SetBranchAddress(start_str + "NuMomY", &_data.NuMomY);
    _tree->SetBranchAddress(start_str + "NuMomZ", &_data.NuMomZ);
    _tree->SetBranchAddress(start_str + "LepMomX", &_data.LepMomX);
    _tree->SetBranchAddress(start_str + "LepMomY", &_data.LepMomY);
    _tree->SetBranchAddress(start_str + "LepMomZ", &_data.LepMomZ);
    _tree->SetBranchAddress(start_str + "cvnnumu", &_data.cvnnumu);
    _tree->SetBranchAddress(start_str + "cvnnue", &_data.cvnnue);
    _tree->SetBranchAddress(start_str + "isCC", &_data.isCC);
    _tree->SetBranchAddress(start_str + "nuPDGunosc", &_data.nuPDGunosc);
    _tree->SetBranchAddress(start_str + "nuPDG", &_data.nuPDG);
    _tree->SetBranchAddress(start_str + "run", &_data.run);
    _tree->SetBranchAddress(start_str + "isFHC", &_data.isFHC);
    _tree->SetBranchAddress(start_str + "BeRPA_A_cvwgt", &_data.BeRPA_cvwgt);
    _tree->SetBranchAddress(start_str + "vtx_x", &_data.vtx_x);
    _tree->SetBranchAddress(start_str + "vtx_y", &_data.vtx_y);
    _tree->SetBranchAddress(start_str + "vtx_z", &_data.vtx_z);  
    _tree->SetBranchAddress(start_str + "LepPDG",&_data.ipnu); 
    _tree->SetBranchAddress(start_str + "LepNuAngle",&_data.LepTheta); 
    _tree->SetBranchAddress(start_str + "Q2",&_data.Q2);
    
    //Current location of systs weights
    _tree->SetBranchAddress("xsSyst_wgt",&_data.weightVec);

    if(_is_flat){
        _tree->SetBranchAddress(start_str + "pot",&_data.weight); //The weight is stored here in a hacky way at the moment.
    }
    else{
        _tree->SetBranchAddress(start_str + "weight",&_data.weight);
        _tree->SetBranchAddress("genie_weight",&_data.genie_weight); //The weight is stored here in a hacky way at the moment.
        _tree->SetBranchAddress("nue_w",&_data.nue_w);
        _tree->SetBranchAddress("numu_w",&_data.numu_w);
        _tree->SetBranchAddress("xsec",&_data.xsec);
    }
    
}

template<typename T>
void Reader<T>::GetPOT(){
    TTree *potTree;
    _file->GetObject("meta", potTree);
    if(potTree == nullptr){
        std::cout << "Could not find the meta tree in file to get POT" << std::endl;
        std::cout << "Setting POT=1" << std::endl;
        // abort();
        return;
    }

    double pot = 0;
    potTree->SetBranchAddress("pot", &pot);
    uint nentries = potTree->GetEntries();
    for(uint i = 0; i < nentries; i++){
        potTree->GetEntry(i);
        _POT += pot;
    } 
}

template<typename T>
bool Reader<T>::GetEntry(int i){
    if(i == -1){
        return _tree->GetEntry(++_entry) != 0;
    }
    else{
        return _tree->GetEntry(i) != 0;
    }
}

template<typename T>
const Data<T>& Reader<T>::GetData(){
    return _data;
}

template<typename T>
TTree* Reader<T>::GetGlobalTree(){
    return _global_tree;
}

template class Reader<float>;
template class Reader<double>;
