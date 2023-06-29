#include "FluxManager.h"
#include "Reader.h"
#include "Writer.h"
#include "Calculator.h"

int main(int argc, char const *argv[])
{
    std::map<Flavour, std::string> fluxes = {
        {Flavour::NuE, "/home/pgranger/atmospherics/honda_2d_homestake_2015_nue.root"},
        {Flavour::NuMu, "/home/pgranger/atmospherics/honda_2d_homestake_2015_numu.root"},
        {Flavour::NuEBar, "/home/pgranger/atmospherics/honda_2d_homestake_2015_nuebar.root"},
        {Flavour::NuMuBar, "/home/pgranger/atmospherics/honda_2d_homestake_2015_numubar.root"},
    };

    std::string ifilename("/home/pgranger/atmospherics/debug/MaCh3_DUNE/sum.root");
    std::string ofilename("/home/pgranger/atmospherics/debug/MaCh3_DUNE/output.root");

    float det_mass = 2.5; //kton
    int year = 3600*24*365;
    float target_exposure = 400; //kton.yr

    float exposure_scaling = target_exposure*year/det_mass;

    FluxManager manager(fluxes);
    Reader<float> reader(ifilename, true);

    Writer writer(ofilename);

    Calculator<float> calc(manager, reader, writer, exposure_scaling);
    calc.Process();
    writer.Write();
    

    return 0;
}
