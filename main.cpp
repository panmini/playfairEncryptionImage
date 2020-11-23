#include "BMP.h"

int main(int argc, char** argv)
{
    BMP bmp;

    
    if (*argv[1] == 'e')
    {
        // if argc != 2 then yanlış gönderilmiş
        //std::cout<<"encryption";
        bmp.readImage("manda.bmp");//plain resim okunacak
        bmp.generateKey();
        bmp.encryption(bmp.getRed());
        bmp.encryption(bmp.getGreen());
        bmp.encryption(bmp.getBlue());
        bmp.mergeRGB("cipherImage");

    }
    else if (*argv[1] == 'd')
    {
        // if argc != 258 then yanlış gönderilmiş
        //std::cout<<"decryption";
        bmp.readImage("Recv.bmp"); //şifreli resim okunacak
        bmp.setKey(argv);//argüman olarak key verilecek
        bmp.decryption(bmp.getRed());
        bmp.decryption(bmp.getBlue());
        bmp.decryption(bmp.getGreen());
        bmp.mergeRGB("plainImage");
        
    }
    
/* 
    bmp.readImage("manda.bmp");
    //bmp.saveGrayScale("grayScale");
    // bmp.cropImage(0,0,374,234);
    bmp.generateKey();
    bmp.encryption(bmp.getRed());
    bmp.encryption(bmp.getGreen());
    bmp.encryption(bmp.getBlue());
    bmp.mergeRGB("cipherImage");
    bmp.decryption(bmp.getRed());
    bmp.decryption(bmp.getBlue());
    bmp.decryption(bmp.getGreen());
    bmp.mergeRGB("plainImage");

 */
    return 0;
}
