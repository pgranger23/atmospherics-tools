#include "FluxManager.h"
#include "Reader.h"
#include "Writer.h"
#include "Calculator.h"

enum Sel{
    SelNuE,
    SelNuMu
};

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
    std::string ofolder("/home/pgranger/atmospherics/debug/MaCh3_DUNE/");

    //TODO: NEED TO CHECK THE EXACT MASS
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
    writer.WriteFile();

    // TTree *tree = writer.GetTree();
    Reader<double> new_reader(ofilename, false);

    std::map<std::tuple<Flavour, Flavour, Sel>, Writer*> channels;
    
    for(Flavour ifl : {Flavour::NuE, Flavour::NuMu, Flavour::NuEBar, Flavour::NuMuBar}){
        for(Flavour ofl : {Flavour::NuE, Flavour::NuMu, Flavour::NuTau, Flavour::NuEBar, Flavour::NuMuBar, Flavour::NuTauBar}){
            if(ifl*ofl < 0){//nu -> nubar osc impossible
                continue;
            }
            for(Sel sel : {Sel::SelNuE, Sel::SelNuMu}){
                std::string sel_str;
                if(sel == Sel::SelNuE){
                    sel_str = "nueselec";
                }
                else{
                    sel_str = "numuselec";
                }
                std::string fname = "hd_AV_" + flavours[ifl] + "_x_" + flavours[ofl] + "_" + sel_str + ".root";
                Writer* ch_writer = new Writer(fname);
                channels.insert({{ifl, ofl, sel}, ch_writer});
            } 
        }
    }

    while(new_reader.GetEntry()){
        Data<double> data = new_reader.GetData();
        Sel sel;
        // std::cout << data.ev  << std::endl;
        if(data.cvnnue > 0.5){
            sel = Sel::SelNuE;
        }
        else if(data.cvnnumu > 0.5){
            sel = Sel::SelNuMu;
        }
        else{
            continue;
        }

        Flavour ofl = Flavour(data.nuPDG);
        std::vector<Flavour> ifls;

        if(ofl < 0){
            ifls = {Flavour::NuEBar, Flavour::NuMuBar};
        }
        else{
            ifls = {Flavour::NuE, Flavour::NuMu};
        }

        Writer* channel_writer = channels[{ifls[0], ofl, sel}];
        data.weight = data.nue_w;
        channel_writer->Fill(data);

        channel_writer = channels[{ifls[1], ofl, sel}];
        data.weight = data.numu_w;
        channel_writer->Fill(data);
    }

    for(auto channel : channels){
        std::cout << channel.second->_file->GetList()->GetEntries() << std::endl;
        if(channel.second->_file->GetList()->GetEntries() != 1){
            std::cout << "!= 1" << std::endl;
            channel.second->_file->GetList()->Print();
        }
        channel.second->Write();
        // channel.second->WriteFile();
    }

    return 0;
}
