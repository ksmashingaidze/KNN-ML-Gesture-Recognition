#include <iostream>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <cstring>
#include <string>
#include <strstream>
#include <sstream>
#include <math.h>

using namespace std;

unsigned char header[54];       //Store image header. Note that in BMP format, image header is typically 54 bytes long.
unsigned char colorTable[1024]; //Store color table, if it is present. In BMP format, color table is typically 1024 bytes long.
int i;                          //Initialise general index
int j;                          //Initialise file looping index
int knn = 5;                    //Initialise k nearest neighbors
float y;                        //Represents output classification
int filecounter;
string filestring;
int trainingdatalength = 20;
float euclidiandistance[20];    //Variable to store euclidian distances
float temp;                     //Temporary variable for sorting
int temp2;                      //Temporary variable for storing target

int main()
{
    //OPEN TEST IMAGE
    FILE *streamIn;                      //Stores input image
    streamIn = fopen("testdata/testimage.bmp","rb"); //Open the input image file
    for (i=0; i<54; i++) {
        header[i] = getc(streamIn);  //Assign image header values to array, byte-wise
    }

    int width = *(int*)&header[18];    //18th byte of the header. Value is width of the image.
    int height = *(int*)&header[22];   //22nd byte of the header. Value is height of the image.
    int bitDepth = *(int*)&header[28]; //28th byte of the header. Value is the number of bits per pixel.

    if (bitDepth <= 8) {               //Note in color image, bitDepth is usually 24.
            for (i=0; i<1024; i++) {
                colorTable[i] = getc(streamIn);
            }
        //fread(colorTable, sizeof(unsigned char), 1024, streamIn);
    }

    unsigned char testbuf[height*width][3]; //Variable stores image data. Second dimension represents RGB components
    unsigned char testgreyscale[height*width][3]; //Variable to store output greyscale pixels

    for (i=0; i<(width*height); i++) {  //i represents the current pixel in this case
        testbuf[i][0] = getc(streamIn);     //Blue data. 0 if no blue present in the pixel in frame, 255 if full blue.
        testbuf[i][1] = getc(streamIn);     //Green data. 0 if no green present in the pixel in frame, 255 if full green.
        testbuf[i][2] = getc(streamIn);     //Red data. 0 if no red present in the pixel in frame, 255 if full red.

        testgreyscale[i][0] = (testbuf[i][2]*0.3) + (testbuf[i][1]*0.59) + (testbuf[i][0]*0.11); //Conversion from RGB to greyscale
        testgreyscale[i][1] = (testbuf[i][2]*0.3) + (testbuf[i][1]*0.59) + (testbuf[i][0]*0.11); //Conversion from RGB to greyscale
        testgreyscale[i][2] = (testbuf[i][2]*0.3) + (testbuf[i][1]*0.59) + (testbuf[i][0]*0.11); //Conversion from RGB to greyscale
    }

    fclose(streamIn);


    //OPEN TRAINING DATA AND COLLECT EUCLIDIAN DISTANCES
    int targetdata[20] = {0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,1,1,1,1,1}; //Array storing targets. 0 is a single finger, 1 is a peace sign. From training data
    for (j=0; j<trainingdatalength; j++) {
        filestring = "trainingdata/";
        FILE *streamIn;                     //Stores input image
        filecounter = filecounter + 1;
        stringstream ss;
        ss << filecounter;
        string fileindex = ss.str();
        filestring.append(fileindex);
        filestring.append(".bmp");
        const char* filename = filestring.c_str();
        streamIn = fopen(filename,"rb");   //Open the input image file

        for (i=0; i<54; i++) {
            header[i] = getc(streamIn);    //Assign image header values to array, byte-wise
        }

        //EXTRACTION OF IMAGE SPECIFICATIONS
        int width = *(int*)&header[18];    //18th byte of the header. Value is width of the image.
        int height = *(int*)&header[22];   //22nd byte of the header. Value is height of the image.
        int bitDepth = *(int*)&header[28]; //28th byte of the header. Value is the number of bits per pixel.

        if (bitDepth <= 8) {               //Note in color image, bitDepth is usually 24.
            for (i=0; i<1024; i++) {
                colorTable[i] = getc(streamIn);
            }
        }

        unsigned char buf[height*width][3]; //Variable stores image data. Second dimension represents RGB components
        unsigned char greyscale[height*width][3]; //Variable to store output greyscale pixels

        //EXTRACTION OF RGB VALUES FROM BMP FILE
        for (i=0; i<(width*height); i++) {  //i represents the current pixel in this case
            buf[i][0] = getc(streamIn);     //Blue data. 0 if no blue present in the pixel in frame, 255 if full blue.
            buf[i][1] = getc(streamIn);     //Green data. 0 if no green present in the pixel in frame, 255 if full green.
            buf[i][2] = getc(streamIn);     //Red data. 0 if no red present in the pixel in frame, 255 if full red.

            //CONVERSION OF COLOR IMAGE TO GREYSCALE
            greyscale[i][0] = (buf[i][2]*0.3) + (buf[i][1]*0.59) + (buf[i][0]*0.11); //Conversion from RGB to greyscale
            greyscale[i][1] = (buf[i][2]*0.3) + (buf[i][1]*0.59) + (buf[i][0]*0.11); //Conversion from RGB to greyscale
            greyscale[i][2] = (buf[i][2]*0.3) + (buf[i][1]*0.59) + (buf[i][0]*0.11); //Conversion from RGB to greyscale
        }

        //DETERMINE EUCLIDIAN DISTANCE
        for (i=0; i<(width*height); i++) {
            euclidiandistance[j] = euclidiandistance[j] + (((*(int*)&testgreyscale[i][0]) - (*(int*)&greyscale[i][0]))^2) + (((*(int*)&testgreyscale[i][1]) - (*(int*)&greyscale[i][1]))^2) + (((*(int*)&testgreyscale[i][2]) - (*(int*)&greyscale[i][2]))^2);
        }

        euclidiandistance[j] = sqrt(fabs(euclidiandistance[j]/20));

        fclose(streamIn);

    }

    //SORT TRAINING DATA IN ASCENDING ORDER OF EUCLIDIAN DISTANCE. SORT CORRESPONDING TARGETS AS WELL.
    for (i=0; i<=trainingdatalength; i++) {
        for (j=0; j<=trainingdatalength-i; j++) {
            if (euclidiandistance[j] > euclidiandistance[j+1]) {
                temp = euclidiandistance[j];
                temp2 = targetdata[j];
                euclidiandistance[j] = euclidiandistance[j+1];
                targetdata[j] = targetdata[j+1];
                euclidiandistance[j+1] = temp;
                targetdata[j+1] = temp2;
            }
        }
    }

    //IMPLEMENTATION OF KNN AVERAGING FOR CLASSIFICATION
    for (i=0; i<knn; i++) {
        y = y + targetdata[i];
    }

    y = round(y/knn);

    if (y == 0) {
        cout << "The test data is a single finger." << endl;
    }
    else if (y == 1) {
        cout << "The test data is a peace sign." << endl;
    }

    return 0;
}
