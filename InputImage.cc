// Manage a text file with a black-white image
// George F. Riley, Georgia Tech, Fall 2009

#include <iostream>
#include <fstream>
#include <sstream>
#include <stdlib.h>

#include "InputImage.h"

using namespace std;

InputImage::InputImage(const char* fileName)
{
  ifstream ifs(fileName);
  if (!ifs)
    {
      cout << "Can't open image file " << fileName << endl;
      exit(1);
    }
  ifs >> width;
  data = new int[width]; // Allocate the data array
  for (int c = 0; c < width; ++c)
    {
      int real;
      ifs >> real;
      data[c] = real;
    }
}

int InputImage::GetWidth() const
{
  return width;
}

int* InputImage::GetImageData() const
{
  return data;
}

void InputImage::SaveImageData(const char* newFileName, int* d, int w)
{
  ofstream ofs(newFileName);
  if (!ofs)
    {
      cout << "Can't create output image " << newFileName << endl;
      return;
    }
  ofs << w << endl;
  for (int c = 0; c < w; ++c)
    { // for each column
      ofs << d[c] << endl;
    }
}

          
      
  
  
