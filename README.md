# CalcDataProcessor

## Introuction
1. DataProcessor  
	+ A filter which can output useful calc data according to the label data.  
	+ Usage: `g++ DataProcessor.cpp` &nbsp;&nbsp;&nbsp;`a.exe <calc file> <label file>`
   
2. DataConstructor  
	+ Make DataProcessor's output data placed in chronological order.  
	+ Usage: `g++ DataConstructor.cpp` &nbsp;&nbsp;&nbsp;`a.exe <calc file>`  
	+ Output:  `XXX.calcA`  
    
3. Feature Extractor (not finish)
    + Can calculate 13 types of feature, include: Mean,  Variance, Standard Deviation, Integration, Root Mean Square, Zero Cross Rate, Mean Cross Rate, Skewness, Kurtosis, First-three orders of 256-point FFT coefficients, Entropy, Signal Magnitude Area, AR coefficients.  
	
## Libraries
+ Need [GSL(GNU Scientific Library)](https://www.gnu.org/software/gsl/) for C++.
