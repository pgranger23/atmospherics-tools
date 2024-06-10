#include "Reader.h"

template<typename T>
Reader<T>::Reader(std::string fname)
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
CAFtype Reader<T>::DetectCAFType(){
    if(_tree->GetBranch("rec.common") != nullptr){
        std::cout << "Detected a CAF file with the new hierarchical CAF format." << std::endl;
        return CAFtype::NEW_CAF;
    }

    if(_tree->GetBranch("rec") != nullptr){
        std::cout << "Detected a CAF file with the legacy CAF format." << std::endl;
        return CAFtype::FLAT_CAF;
    }

    //TODO: Actually ibvestigate the difference between flatcaf and caf

    // if(_tree->GetBranch("Ev") != nullptr){
    //     std::cout << "Detected a CAF file with the legacy FlatCAF format." << std::endl;
    //     return CAFtype::FLAT_CAF;
    // }

    std::cout << "No legible CAF format detected from the input. Aborting!" << std::endl;
    abort();
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
    std::string tree_name = "cafmaker/cafTree";
    _file->GetObject(tree_name.c_str(), tree);
    if(tree == nullptr){
        std::cout << "No tree named " << tree_name << " in file" << std::endl;
        abort();
    }
    _tree = tree;
    _caftype = this->DetectCAFType();

    //Loading the gloabl tree
    TTree *global_tree;

    tree_name = "meta";

    _file->GetObject(tree_name.c_str(), global_tree);
    if(global_tree == nullptr){
        std::cout << "WARNING: No tree named '" << tree_name << "' in the input file " << fname << std::endl;
    }
    _global_tree = global_tree;
}

template<typename T>
void Reader<T>::SetupTreeHierarchical(){
    _tree->SetBranchAddress("mc.nu.E", &_data.ev);

    _tree->SetBranchAddress("Ev_reco_numu", &_data.erec);
    _tree->SetBranchAddress("Ev_reco_nue", &_data.erec_nue);

    _tree->SetBranchAddress("mc.nu.mode",&_data.mode);
    _tree->SetBranchAddress("mc.nu.momentum.x", &_data.NuMomX);
    _tree->SetBranchAddress("mc.nu.momentum.y", &_data.NuMomY);
    _tree->SetBranchAddress("mc.nu.momentum.z", &_data.NuMomZ);
    _tree->SetBranchAddress("mc.nu.genweight", &_data.weight);
    _tree->SetBranchAddress("common.ixn.pandora.dir.lngtrk.x", &_data.LepMomX);
    _tree->SetBranchAddress("common.ixn.pandora.dir.lngtrk.y", &_data.LepMomY);
    _tree->SetBranchAddress("common.ixn.pandora.dir.lngtrk.z", &_data.LepMomZ);
    _tree->SetBranchAddress("common.ixn.pandora.nuhyp.cvn.numu", &_data.cvnnumu);
    _tree->SetBranchAddress("common.ixn.pandora.nuhyp.cvn.nue", &_data.cvnnue);
    _tree->SetBranchAddress("mc.nu.iscc", &_data.isCC);

    _tree->SetBranchAddress("mc.nu.pdgorig", &_data.nuPDGunosc);
    _tree->SetBranchAddress("mc.nu.pdg", &_data.nuPDG);
    _tree->SetBranchAddress("meta.fd_hd.run", &_data.run); //TODO: Probably also add the event number and such things
    _tree->SetBranchAddress("beam.ismc", &_data.isFHC); //TODO: Change this to an actual isFHC variable
    // _tree->SetBranchAddress("BeRPA_A_cvwgt", &_data.BeRPA_cvwgt); //TODO: No idea what to do with this
    _tree->SetBranchAddress("mc.nu.vtx.x", &_data.vtx_x);
    _tree->SetBranchAddress("mc.nu.vtx.y", &_data.vtx_y);
    _tree->SetBranchAddress("mc.nu.vtx.z", &_data.vtx_z); 

    _tree->SetBranchAddress("LepPDG",&_data.ipnu); 
    _tree->SetBranchAddress("LepNuAngle",&_data.LepTheta); //TODO: Find the right variable
    _tree->SetBranchAddress("mc.nu.Q2",&_data.Q2);
    
    //Current location of systs weights
    _tree->SetBranchAddress("xsSyst_wgt",&_data.weightVec);

    if(_caftype == CAFtype::FLAT_CAF){
        _tree->SetBranchAddress("pot",&_data.weight); //The weight is stored here in a hacky way at the moment.
    }
    else{
        _tree->SetBranchAddress("weight",&_data.weight);
        _tree->SetBranchAddress("genie_weight",&_data.genie_weight); //The weight is stored here in a hacky way at the moment.
        _tree->SetBranchAddress("nue_w",&_data.nue_w);
        _tree->SetBranchAddress("numu_w",&_data.numu_w);
        _tree->SetBranchAddress("xsec",&_data.xsec);
    }
    
}

template<typename T>
TFile* Reader<T>::GetFile(){
    return _file;
}

template<typename T>
void Reader<T>::SetupTree(){
    TString start_str = (_caftype == CAFtype::FLAT_CAF) ? "rec." : "";
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

    if(_caftype == CAFtype::FLAT_CAF){
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
    double pot = 0;
    _global_tree->SetBranchAddress("pot", &pot);
    uint nentries = _global_tree->GetEntries();
    for(uint i = 0; i < nentries; i++){
        _global_tree->GetEntry(i);
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
