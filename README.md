# CalcDataProcessor

## Introuction
1. DataProcessor  
	+ A filter which can output useful calc data according to the label data.  
	+ Usage: `g++ DataProcessor.cpp` &nbsp;&nbsp;&nbsp;`a.exe <calc file> <label file>`
   
2. DataConstructor  
	+ Make DataProcessor's output data placed in chronological order.  
	+ Usage: `g++ DataConstructor.cpp` &nbsp;&nbsp;&nbsp;`a.exe <calc file>`  
	+ Output:  `XXX.calcA`  
    
3. Feature Extractor (finished: Bold)
    + Can calculate 13 types of feature, include:
    	1. **Mean, Variance, Standard Deviation**
		4. **Integration**
		5. **Root Mean Square**
		6. **Zero Cross Rate**
		7. **Mean Cross Rate**
		8. **Skewness**
		9. **Kurtosis**
		10. First-three orders of 256-point FFT coefficients
		11. **Entropy**
		12. **Signal Magnitude Area**
		13. **AR coefficients**
	+ Usage: 
		+ Windows: `g++ feature.cpp -lgsl` &nbsp;&nbsp;&nbsp;`a.exe <calcA file>`
		+ Linux: g++ feature.cpp \`pkg-config --cflags --libs gsl\`
	+ Output:  `XXX.calcAF`  
	
## Libraries
+ Need [GSL(GNU Scientific Library)](https://www.gnu.org/software/gsl/) for C++.
