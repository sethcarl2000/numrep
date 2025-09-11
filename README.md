# numrep

## Testing numerical limits in C++ 

My file to test numerical underflows/overflows is 'test_under_over.cpp'. it takes no arguments, and uses a templatized function definition to evaluate the floating-point underflow and overflows of the types 'float' and 'double'. 

It also evaluates the maximum integer representation of the 'short int', 'int', and 'unsigned int' data types, by adding '1' to each type, starting at 0, until the integer type loops around to its minimum value. 

## Bessel function plotting 
I did this with the ROOT macro called 'make_bessel_plots.C'. It can be invoked with:  

> root -l 'make_bessel_plots.C'

If any of the four 'bessel#.dat' files are missing, it will abort execution. 
It parses these files with the function 'parse_bessel_points', and then fills a struct of type 'BesselArray_t' with all the relevant data. 

This macro will also save the result of plotting as 'bessel.png'. It is clear from these plots that the _down_ method is more accurate than the _up_ method, due to subtractive cancellation: The _up_ method adds large numbers together to get a small one, while the _down_ method adds small numbers together to get a large one. 

## Numerical Differentiation 
This script makes use of the <functional> header to pass functions by value. The three numerical differentiation techniques are used to compute the derivatives of the Exp(x) and Cos(x) functions, with the relative error from the analytical result plotted. It's clear that, for each plot, the transition from round-off error to truncation error occurs at the 1e-7, 1e-10, and 1e-13 levels of relative error, for the forward, central and extrapolated methods, respectively. 
