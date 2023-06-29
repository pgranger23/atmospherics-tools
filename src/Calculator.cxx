#include "Calculator.h"

template<typename T>
Calculator<T>::Calculator(const FluxManager& mgr, Reader<T>& rdr, Writer& wrt, float exposure_scaling)
: _mgr(mgr), _rdr(rdr), _wrt(wrt), _exposure_scaling(exposure_scaling)
{
}

template<typename T>
Calculator<T>::~Calculator()
{
}

template<>
Data<double> Calculator<float>::FetchData(){
    return float2double(_rdr.GetData());
}

template<>
Data<double> Calculator<double>::FetchData(){
    return _rdr.GetData();
}

template<typename T>
void Calculator<T>::Process(){
    double POT = _rdr.POT();
    std::cout << "Got POT of: " << POT << std::endl;
    while(_rdr.GetEntry()){
        Data<double> data = FetchData();
        double Enu = data.ev;
        double costh = data.NuMomY/sqrt(pow(data.NuMomX, 2) + pow(data.NuMomY, 2) + pow(data.NuMomZ, 2));
        double phi = 60; //To be modified in the case of 3D flux. I would need to check.

        Flavour nuE = (data.nuPDG > 0) ? Flavour::NuE : Flavour::NuEBar;
        Flavour nuMu = (data.nuPDG > 0) ? Flavour::NuMu : Flavour::NuMuBar;

        double nuE_flux = _mgr.GetFlux(nuE, Enu, costh, phi);
        double nuMu_flux = _mgr.GetFlux(nuMu, Enu, costh, phi);

        //We assume here that the numu flux was used for the generation
        
        double xsec_w = data.weight/nuMu_flux/POT;
        double nuE_w = xsec_w*nuE_flux*_exposure_scaling;
        double nuMu_w = xsec_w*nuMu_flux*_exposure_scaling;
        // std::cout << nuE_w << " " << nuMu_w << " " << xsec_w << std::endl;

        data.flux_nue = nuE_flux;
        data.flux_numu = nuMu_flux;
        data.xsec = xsec_w;
        data.nue_w = nuE_w;
        data.numu_w = nuMu_w;

        _wrt.Fill(data);
    }
}

template class Calculator<float>;