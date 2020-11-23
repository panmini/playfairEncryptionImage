#include<iostream>
#include<string>
#include<fstream>
#include <cstdint>
#include <cstring>
#include <math.h>
#include <algorithm>
#include <time.h>
#include "FileInfo.h"
#include "ImageInfo.h"

typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef uint32_t DWORD;

class BMP
{
    public:
        BMP();

        FileInfo * fileinfo;
        ImageInfo * imageinfo;

        void readImage(string filename); //Reads Img from BMP File
        void saveGrayScale(string filename); //Convert bmp to intensity
        void saveGrayScale(BYTE * ,string filename);//Saves Intensity to same path as bmp.h (filename is the name of the file)
        void saveImage(string filename);//Save the BMP file
        void saveImage(BYTE *temp, string filename);

        void separateRGB();
        void mergeRGB(string);
        void generateKey();
        void setKey(char**);
        void encryption(BYTE *toEncrypt);
        void decryption(BYTE *toDecrypt);
        int findIndex(BYTE value);
        BYTE *getRed()  {return red;};
        BYTE *getGreen(){return green;};
        BYTE *getBlue() {return blue;};

        
    private:


        BYTE * data, *processedData; //Points RGB Image
        BYTE * grayData;//Points Intensity Image
        BYTE *red, *blue,*green;

        int keyMatrix[256];


        //BYTE * tempMatrix;

        int padding; //Width or Height must be dividible by 4 while operating img file conversions.
};
