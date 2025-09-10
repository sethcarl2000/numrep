#include <sstream>
#include <fstream>  
#include <string> 
#include <vector> 
#include <stdexcept> 
#include <iostream> 
#include <TGraph.h> 
#include <TCanvas.h>
#include <TPad.h> 
#include <TLegend.h> 

using namespace std; 

struct BesselArray_t { string name=""; std::vector<double> x{}, Jdown{}, Jup{}; }; 

void parse_bessel_points(BesselArray_t& J, const char* path_datfile); 

int make_bessel_plots() 
{
    const char* const here = "make_bessel_plots"; 
    //take the bessel plots given, and draw them. 

    //expected to find the following bessel plots in the same dir: 
    const char* path_J0 = "bessel0.dat"; 
    const char* path_J3 = "bessel3.dat"; 
    const char* path_J5 = "bessel5.dat"; 
    const char* path_J8 = "bessel8.dat"; 
    
    //create the 'BesselArray_t' structs, and give them each the proper name. 
    BesselArray_t J0{"J_{0}"}, J3{"J_{3}"}, J5{"J_{5}"}, J8{"J_{8}"}; 

    //try to parse the points, catch any exceptions which may occur 
    try { 
        parse_bessel_points(J0, path_J0); 
        parse_bessel_points(J3, path_J3); 
        parse_bessel_points(J5, path_J5); 
        parse_bessel_points(J8, path_J8); 
        
    } catch (const std::exception& e) {

        Error(here, "Something went wrong parsing the data points from files.\n what(): %s", e.what()); 
        return -1; 
    }

    cout << "Parsed all points successfully" << endl; 

    //now, we're ready to start drawing them. 
    auto canv = new TCanvas("c", "canvas", 1600, 800); 

    //divide the canvas into 4 plots
    canv->Divide(2,2); 
    int i_canv=1; 

    for (auto& J : {J0, J3, J5, J8}) {

        canv->cd(i_canv++); 

        auto graph_up    = new TGraph( J.x.size(), J.x.data(), J.Jup.data()   );
        auto graph_down  = new TGraph( J.x.size(), J.x.data(), J.Jdown.data() ); 

        string plot_name = J.name; 

        plot_name += Form(";x;%s", J.name.c_str());  

        //set the axis viewing range
        const double x_min = J.x.front(); 
        const double x_max = J.x.back(); 

        const double x_draw_buffer = 0.05 * (x_max - x_min);

        //draw both of the graphs
        graph_up->SetTitle(plot_name.c_str()); 
        graph_up->Draw(); 

        graph_down->SetLineColor(kRed); 
        graph_down->SetLineStyle(kDashed); 
        graph_down->SetLineWidth(2); 
        graph_down->Draw("SAME");

        graph_down->SetTitle("down alg."); 

        //now let's build the legend 
        auto legend = new TLegend(0.6,0.6, 0.9,0.9); 

        legend->SetHeader("Bessel algorithm:"); 
        
        legend->AddEntry(graph_up,   "up alg."); 
        legend->AddEntry(graph_down, "down alg."); 
        
        legend->Draw(); 
    }   

    //now, save the resulant canvas. 
    canv->SaveAs("bessel.png"); 

    return 0; 
}

//this is a basic routine to parse the bessel fcn data from the '.dat' files
void parse_bessel_points(BesselArray_t& J, const char* path_datfile)
{
    const char* const here = "parse_bessel_points"; 

    ifstream file(path_datfile); 

    //check if the file can be opened 
    if (!file.is_open()) {
        ostringstream oss; 
        oss << "in <" << here << ">: unable to open file '" << path_datfile << "'";
        throw invalid_argument(oss.str());  
        return;
    }

    string line; 

    while(getline(file, line)) {

        //skip comment lines & empty lines 
        if (line.empty() || line[0] == '#') continue; 

        //if not, then this line has data         
        istringstream iss(line); 

        double x, Jdown, Jup; 

        iss >> x >> Jdown >> Jup;  
        
        J.x    .push_back(x); 
        J.Jdown.push_back(Jdown); 
        J.Jup  .push_back(Jup); 
    }

    return; 
}

