# Hershey Vector Fonts
## A C library for loading Hershey vector font definitions, and some C functions for drawing the character sets using XLib
### Description
**A C library for loading Hershey vector font definitions, and some C functions for drawing the character sets using XLib**

### Files
- hershey_font.h
- showfont.c
- *.hf

### Example usage
- Compile:
  Within the `src` directory, run
  ```bash
  make showhfont
  ```
- Run:
  ```bash
  ./showhfont simplexroman
  ```

### Example images
![Simplex script](https://github.com/dpcummins/hershey_vector_fonts/blob/main/images/simplexscript.png?raw=true)
![Complex roman](https://github.com/dpcummins/hershey_vector_fonts/blob/main/images/complexroman.png?raw=true)


### Example applications
See this library in use at [XCPlot](https://github.com/dpcummins/XCPlot) and [WatchyHershey](https://github.com/dpcummins/WatchyHershey).


### Notes
The Hershey fonts were originally created by Dr. A. V. Hershey while working at the U. S. National Bureau of Standards.  
The coordinate sets for the Hershey vector font characters were obtained from [http://paulbourke.net/dataformats/hershey/](http://paulbourke.net/dataformats/hershey/).  

The format of the font data provided there was originally created by:  
James Hurt  
Cognition, Inc.  
900 Technology Park Drive  
Billerica, MA 01821  
(mit-eddie!ci-dandelion!hurt)  
See [this](https://web.archive.org/web/20170716165314/https://ghostscript.com/doc/current/Hershey.htm) for further details.
