#include "BMP.h"

BMP::BMP()
{
    imageinfo = new ImageInfo;
    fileinfo = new FileInfo;
}
void BMP::readImage(string filename)
{
    ifstream file(filename , ios::binary); //opening file

    char filetemp[14];                     //store file informations
    char imagetemp[40];

    file.read(filetemp , sizeof(filetemp)); //read first 14 byte
    fileinfo->setFileHeader(filetemp);      //send allData
    file.read(imagetemp , sizeof(imagetemp));
    imageinfo->setImageHeader(imagetemp);
    fileinfo  ->  setOffSet(54);

    file.seekg(54);    //move cursor offset

    padding = 0;

    while((imageinfo->getWidth()*3+padding)%4 != 0) padding++; //calculate padding

    data = new BYTE[imageinfo -> getBiSize()];  //determine data size
    BYTE * pointerOfData = data;                //point data
    BYTE buffer[imageinfo->getWidth()*3];       //temprature memory

    for(int i=0;i<(int)(imageinfo->getBiSize()/sizeof(buffer));i++)
	{
        file.read((char*)buffer,sizeof(buffer));        //read first row image
        memcpy(pointerOfData,buffer,sizeof(buffer));    //copy buffer adresses
        pointerOfData += sizeof(buffer);                //move pointer
        file.read((char*)buffer,padding);               //move cursor
    }

    file.close();

    red = new BYTE[imageinfo -> getBiSize()/3];
    blue = new BYTE[imageinfo -> getBiSize()/3];
    green = new BYTE[imageinfo -> getBiSize()/3];

    separateRGB();
}

void BMP::saveGrayScale(string filename)
{
    grayData = new BYTE[imageinfo->getBiSize()/3];
    BYTE * iterator = grayData;
    BYTE* pointerOfData = grayData;


    for(int i=0;i<(int)imageinfo->getBiSize();i+=3)
	{
        *iterator = BYTE((data[i]*0.21+data[i+1]*0.72+data[i+2]*0.07));
        iterator++;
    }

    ofstream file(filename + ".bmp" , ios::binary);
    file.write((char *)fileinfo->getAllHeader() , 14);
    file.write((char *)imageinfo->getAllInfo() , 40);


    unsigned int pixelNumber = 0;

    for(int i=0;i<(int)imageinfo->getBiSize()/3;i++)
	{
        file.write((char*)pointerOfData,1);
        file.write((char*)pointerOfData,1);
        file.write((char*)pointerOfData++,1);

        pixelNumber++;

        if(pixelNumber == imageinfo->getWidth())
		{
            BYTE pad = 0;
            for(int i=0;i<padding;i++) file.write((char*)&pad,1);
            pixelNumber = 0;
        }
    }

    file.close();
}
void BMP::saveGrayScale(BYTE * temp , string filename)
{
    BYTE * pointerOfData = temp;

    ofstream file(filename + ".bmp" , ios::binary);
    file.write((char *)fileinfo->getAllHeader() , 14);
    file.write((char *)imageinfo->getAllInfo() , 40);

    unsigned int pixelNumber = 0;

    for(int i=0;i<(int)imageinfo->getBiSize()/3;i++)
	{
        file.write((char*)pointerOfData,1);
        file.write((char*)pointerOfData,1);
        file.write((char*)pointerOfData++,1);

        pixelNumber++;

        if(pixelNumber == imageinfo->getWidth())
		{
            BYTE pad = 0;
            for(int i=0;i<padding;i++) file.write((char*)&pad,1);
            pixelNumber = 0;
        }
    }
    file.close();
}

void BMP::saveImage(string filename)
{
    ofstream file(filename + ".bmp" , ios::binary);
    file.write((char *)fileinfo->getAllHeader() , 14);
    file.write((char *)imageinfo->getAllInfo() , 40);

    BYTE * pointerOfData = data;
    unsigned int pixelNumber = 0;

    for(int i=0;i<(int)imageinfo->getBiSize();i++)
	{
        file.write((char*)pointerOfData++,1);

        pixelNumber++;
        if(pixelNumber >= imageinfo->getWidth() * 3)
		{
            BYTE pad = 0;
            for(int i=0;i<padding;i++) file.write((char*)&pad,1);
            pixelNumber = 0;
            i += padding;
        }
    }

    file.close();
}

void BMP::saveImage(BYTE *temp, string filename)
{
    ofstream file(filename + ".bmp" , ios::binary);
    file.write((char *)fileinfo->getAllHeader() , 14);
    file.write((char *)imageinfo->getAllInfo() , 40);

    BYTE * pointerOfData = temp;
    unsigned int pixelNumber = 0;

    for(int i=0;i<(int)imageinfo->getBiSize();i++)
	{
        file.write((char*)pointerOfData++,1);

        pixelNumber++;
        if(pixelNumber >= imageinfo->getWidth() * 3)
		{
            BYTE pad = 0;
            for(int i=0;i<padding;i++) file.write((char*)&pad,1);
            pixelNumber = 0;
            i += padding;
        }
    }

    file.close();
}

void BMP::separateRGB()
{
	int size = (int)imageinfo->getBiSize()/3;

	for(int i=0; i < size; i++)
	{
		blue[i]=data[i*3]; green[i]=data[i*3+1]; red[i]=data[i*3+2];
	}

}

void BMP::mergeRGB(string image)
{
    int size = (int)imageinfo->getBiSize()/3;
    processedData = new BYTE[imageinfo -> getBiSize()];

    for(int i=0; i < size; i++)
	{
		processedData[i*3]=blue[i]; processedData[i*3+1]=green[i]; processedData[i*3+2]=red[i];
	}

    data = processedData;
    saveImage(image);

}

void BMP::generateKey()
{
    int newitem;
    bool unique;
    srand(time(NULL));
    for(int i=0;i<256;i++)
    {
        do
        {
            unique=true;
            newitem=rand()%256;
            for(int j=0;j<i;j++)
            {
                if(keyMatrix[j]==newitem)
                {
                unique=false;
                break;
                }
            }
        }
        while(!unique);
        keyMatrix[i]=newitem;
    }
    for(int i=0;i<255;i++)
        std::cout<<keyMatrix[i]<<" ";
    std::cout<<keyMatrix[255];
            

}
void BMP::setKey(char** key)
{
    for (size_t i = 2; i < 258; i++)
    {
        /* code */
        keyMatrix[i-2] = atoi(key[i]);
    }
/* 
    for(int i=0;i<255;i++)
        std::cout<<keyMatrix[i]<<" ";
    std::cout<<keyMatrix[255];
 */
    
}
void BMP::encryption(BYTE *toEncrypt)
{
    int size = (int)imageinfo->getBiSize()/3;

    int firstValue,secondValue, fi,fj,si,sj;
    for(int i=0; i<size-1;i+=2)
    {
        firstValue=findIndex(toEncrypt[i]);                 secondValue=findIndex(toEncrypt[i+1]);
        fi=firstValue/16; fj=firstValue%16;                 si=secondValue/16; sj=secondValue%16;

        if(fi == si)
        {
            //aynı satırda
            toEncrypt[i]    = keyMatrix[ fi * 16 + ( (fj+1) % 16)];
            toEncrypt[i+1]  = keyMatrix[ si * 16 + ( (sj+1) % 16)];
        }
        else if(fj == sj)
        {
            //aynı sütün
            toEncrypt[i] = keyMatrix[ ( ( fi+1 ) % 16) * 16 + fj ];
            toEncrypt[i+1] = keyMatrix[ ( ( si+1 ) % 16) * 16 + sj ];

        }
        else
        {
            //ayrık
            toEncrypt[i]    = keyMatrix[ fi * 16 + sj];
            toEncrypt[i+1]  = keyMatrix[ si * 16 + fj];
        }

    }

    if(size%2 != 0) //padding
    {
        firstValue=findIndex(toEncrypt[size]);      secondValue=findIndex(toEncrypt[255]);
        fi=firstValue/16; fj=firstValue%16;         si=secondValue/16; sj=secondValue%16;    

        if(fi == si)
        {
            //aynı satırda
            toEncrypt[size]    = keyMatrix[ fi * 16 + ( (fj+1) % 16)];            
        }
        else if(fj == sj)
        {
            //aynı sütün
            toEncrypt[size] = keyMatrix[ ( ( fi+1 ) % 16) * 16 + fj ];            
        }
        else
        {
            //ayrık
            toEncrypt[size]    = keyMatrix[ fi * 16 + sj];
        }     
        
    }
}
void BMP::decryption(BYTE *toDecrypt)
{
    int size = (int)imageinfo->getBiSize()/3;

    bool padding=false;
    if(size%2!=0)
        padding=true;

    int firstValue,secondValue, fi,fj,si,sj;
    for(int i=0; i<size-1;i+=2)
    {
        firstValue=findIndex(toDecrypt[i]);                 secondValue=findIndex(toDecrypt[i+1]);
        fi=firstValue/16; fj=firstValue%16;                 si=secondValue/16; sj=secondValue%16;

        if(fi == si)
        {
            //aynı satırda
            toDecrypt[i]    = keyMatrix[ fi * 16 + ( (fj-1+16) % 16)];
            toDecrypt[i+1]  = keyMatrix[ si * 16 + ( (sj-1 + 16) % 16)];
        }
        else if(fj == sj)
        {
            //aynı sütün
            toDecrypt[i] = keyMatrix[ ( ( fi-1 + 16 ) % 16) * 16 + fj ];
            toDecrypt[i+1] = keyMatrix[ ( ( si-1 + 16) % 16) * 16 + sj ];

        }
        else
        {
            //ayrık
            toDecrypt[i]    = keyMatrix[ fi * 16 + sj];
            toDecrypt[i+1]  = keyMatrix[ si * 16 + fj];
        }

    }

    if(size%2 != 0)//padding
    {
        firstValue=findIndex(toDecrypt[size]);      secondValue=findIndex(toDecrypt[255]);
        fi=firstValue/16; fj=firstValue%16;         si=secondValue/16; sj=secondValue%16;    

        if(fi == si)
        {
            //aynı satırda
            toDecrypt[size]    = keyMatrix[ fi * 16 + ( (fj-1) % 16)];            
        }
        else if(fj == sj)
        {
            //aynı sütün
            toDecrypt[size] = keyMatrix[ ( ( fi-1 ) % 16) * 16 + fj ];            
        }
        else
        {
            //ayrık
            toDecrypt[size]    = keyMatrix[ fi * 16 + sj];
        }     
        
    } 

}
int BMP::findIndex(BYTE value)
{
    int i=0;
    for(;i<256; i++)
    {
        if(keyMatrix[i] == value)
            return i;
    }
    return -1;
}
