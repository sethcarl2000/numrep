#include <functional> 
#include <iostream> 
#include <cmath> 
#include <vector> 
#include <TCanvas.h> 
#include <TPad.h> 
#include <TGraph.h> 

using namespace std; 

double forward_deriv(std::function<double(double)> fcn, double x, double h) { return (fcn(x + h ) - fcn(x)) / h; }
double central_deriv(std::function<double(double)> fcn, double x, double h) { return (fcn(x + h/2.) - fcn(x - h/2.))/ h; } 
double extrapolated_deriv(std::function<double(double)> fcn, double x, double h) 
{ 
    return ( 4.*central_deriv(fcn, x, h/2.) - central_deriv(fcn, x, h) ) / 3.; 
} 

void Plot_deriv_errors(std::function<double(double)> fcn, std::function<double(double)> deriv, double x, std::vector<double> h_vals)
{
    
    const double log10 = log(10.); 

    //h-vals and erorrs are drawn in a log-log plot. 
    vector<double> error_fwd{}, error_cent{}, error_extrap{};  

    for (double h : h_vals) {

        double deriv_analytical = deriv(x); 

        error_fwd   .push_back( fabs((forward_deriv(fcn, x, h) - deriv_analytical) / deriv_analytical) ); 
        error_cent  .push_back( fabs((central_deriv(fcn, x, h) - deriv_analytical) / deriv_analytical) ); 
        error_extrap.push_back( fabs((extrapolated_deriv(fcn, x, h) - deriv_analytical) / deriv_analytical) ); 
    }   

    //find the minimum and maximum 
    double y_min{1e30}, y_max{-1e30}; 

    auto find_min_max = [&y_min,&y_max](double y) { 
        y_min = min<double>(y_min, y); 
        y_max = max<double>(y_max, y); 
    }; 

    for (auto y : error_fwd)    find_min_max(y); 
    for (auto y : error_cent)   find_min_max(y); 
    for (auto y : error_extrap) find_min_max(y); 

    //now, create the graphs
    auto graph_fwd    = new TGraph( h_vals.size(), h_vals.data(), error_fwd.data() );
    auto graph_cent   = new TGraph( h_vals.size(), h_vals.data(), error_cent.data() );
    auto graph_extrap = new TGraph( h_vals.size(), h_vals.data(), error_extrap.data() );

    //set the log scale for x & y 
    gPad->SetLogx(1); gPad->SetLogy(1); 

    //set margin so we can see the histo titles
    gPad->SetLeftMargin(0.12); 
    gPad->SetRightMargin(0.05); 

    //draw both of the graphs
    graph_fwd->SetTitle(Form("x = %.1f;h;relative error",x));
    graph_fwd->GetYaxis()->SetRangeUser( y_min, y_max );  
    graph_fwd->Draw(); 

    graph_cent->SetLineColor(kRed); 
    graph_cent->SetLineStyle(kDashed); 
    graph_cent->SetLineWidth(2); 
    graph_cent->Draw("SAME");
    
    graph_extrap->SetLineColor(kBlue); 
    graph_extrap->SetLineStyle(kDotted); 
    graph_extrap->SetLineWidth(2); 
    graph_extrap->Draw("SAME");

    //now let's build the legend 
    auto legend = new TLegend(0.12,0.1, 0.5,0.3); 

    legend->SetHeader("Differentiation Method"); 
    
    legend->AddEntry(graph_fwd,    "forward"); 
    legend->AddEntry(graph_cent,   "central");
    legend->AddEntry(graph_extrap, "extrapolated");
    
    legend->Draw();     
}

double fcn_exp(double x) { return exp(x); };

double fcn_cos(double x) { return cos(x); };
double fcn_d_cos(double x) { return -1.*sin(x); }

int numerical_differentiation(const double h_min = 1e-12, const double h_max = 1e-1)
{   
    
    //produce the log-spaced set of values
    const size_t n_points = 200; 
    const double dx_spacing = exp((log(h_max) - log(h_min))/((double)n_points-1) ); 

    vector<double> h_vals{}; 

    double h = h_min; 
    for (size_t i=0; i<n_points; i++) { h_vals.push_back( h ); h *= dx_spacing; }


    //make a canvas, and draw the results of the numerical differentiation for Exp(x)
    auto canv = new TCanvas("c", "differentiation - Exp(x)", 1600, 600);

    canv->Divide(3,1); 

    canv->cd(1); Plot_deriv_errors(fcn_exp, fcn_exp, 0.1, h_vals); 
    canv->cd(2); Plot_deriv_errors(fcn_exp, fcn_exp, 1., h_vals); 
    canv->cd(3); Plot_deriv_errors(fcn_exp, fcn_exp, 100., h_vals); 
    
    canv->cd(0); canv->SaveAs("diff_test_ExpX.png"); 

    //make a new canvas, repeat the same process for Cos(x)
    canv = new TCanvas("c2", "differentiation - Cos(x)", 1600, 600);

    canv->Divide(3,1); 

    canv->cd(1); Plot_deriv_errors(fcn_cos, fcn_d_cos, 0.1, h_vals); 
    canv->cd(2); Plot_deriv_errors(fcn_cos, fcn_d_cos, 1., h_vals); 
    canv->cd(3); Plot_deriv_errors(fcn_cos, fcn_d_cos, 100., h_vals); 
    
    canv->cd(0); canv->SaveAs("diff_test_CosX.png"); 
    
    return 0; 
}

