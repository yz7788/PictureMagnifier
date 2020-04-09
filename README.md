# Picture Magnifier
## Description
This repository provides code for resampling picture and an interactive picture magnifier. Solution/ Project on Visual Studio 2019 is provided.

## Input
Input to the program will take four parameters where
- The first parameter is the name of the image, which should be in an 8 bit per channel RGB format (Total 24 bits per pixel).
- The second parameter is a mode. It will have an input of 1 or 2. For mode 1 you will be rescaling/resampling your image. Mode 2 represents an interactive picture magnifier application.
- The third parameter is a floating-point value suggesting by how much the image has to be scaled, such as 0.5 or 1.2 This single number will scale both width and height, resulting in re-sampling your image.
- The fourth parameter will be a boolean value (0 or 1) suggesting whether you want to deal with aliasing. A 0 signifies do nothing (aliasing will remain in your output) and a 1 signifies that anti-aliasing should be performed

## Compile
To invoke the program, run in command line as:
	
	YourProgram.exe C:/myDir/myImage.rgb M S A

where M S A are the parameters as described above.

## Result
Here is a screenshot example of the magnifier demo.
<p align="center">
 <img src="magnifier.png" height="600"/>
</p >
