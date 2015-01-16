#include "fann.h"
#include "parroto.h"
extern ParrotObserver parroto;

/*
 * encoder.c
 *
 * Created on: Sep 9, 2013
 * 			Author: Amir Yazdanbakhsh <a.yazdanbakhsh@gatech.edu>
 */

#include "datatype.h"
#include "jpegconfig.h"
#include "prototype.h"

#include "rgbimage.h"


UINT8	Lqt [BLOCK_SIZE];
UINT8	Cqt [BLOCK_SIZE];
UINT16	ILqt [BLOCK_SIZE];
UINT16	ICqt [BLOCK_SIZE];

INT16	Y1 [BLOCK_SIZE];
INT16	Y2 [BLOCK_SIZE];
INT16	Y3 [BLOCK_SIZE];
INT16	Y4 [BLOCK_SIZE];
INT16	CB [BLOCK_SIZE];
INT16	CR [BLOCK_SIZE];
INT16	Temp [BLOCK_SIZE];
UINT32 lcode = 0;
UINT16 bitindex = 0;

INT16 global_ldc1;
INT16 global_ldc2;
INT16 global_ldc3;



UINT8* encodeImage(
	RgbImage* srcImage,
	UINT8 *outputBuffer,
	UINT32 qualityFactor,
	UINT32 imageFormat
) {


	global_ldc1 = 0;
	global_ldc2 = 0;
	global_ldc3 = 0;




	/** Quantization Table Initialization */
	initQuantizationTables(qualityFactor);

	srand(time(NULL));


	UINT16 i, j;
	/* Writing Marker Data */
	outputBuffer = writeMarkers(outputBuffer, imageFormat, srcImage->w, srcImage->h);
	for (i = 0; i < srcImage->h; i += 8) {
		for (j = 0; j < srcImage->w; j += 8) {
			readMcuFromRgbImage(srcImage, j, i, Y1);
			/* Encode the data in MCU */
			//outputBuffer = encodeMcu(imageFormat, outputBuffer);
			outputBuffer = encodeMcu(imageFormat, outputBuffer);
		}
	}

	/* Close Routine */
	closeBitstream(outputBuffer);

	return outputBuffer;
}

UINT8* encodeMcu(
	UINT32 imageFormat,
	UINT8 *outputBuffer
) {

	levelShift(Y1);

	double dataIn [BLOCK_SIZE];
	double dataOut[BLOCK_SIZE];

	for (int i = 0; i < BLOCK_SIZE; ++i)
	{
		dataIn[i] = Y1[i] / 256.;
	}

	bool isNN = true;

// #pragma parrot(input, "jpeg", [64]dataIn)
	struct fann *ann;
	fann_type *parrotOut;
	fann_type parrotIn[64];
	parrotIn[0] = dataIn[0];
	parrotIn[1] = dataIn[1];
	parrotIn[2] = dataIn[2];
	parrotIn[3] = dataIn[3];
	parrotIn[4] = dataIn[4];
	parrotIn[5] = dataIn[5];
	parrotIn[6] = dataIn[6];
	parrotIn[7] = dataIn[7];
	parrotIn[8] = dataIn[8];
	parrotIn[9] = dataIn[9];
	parrotIn[10] = dataIn[10];
	parrotIn[11] = dataIn[11];
	parrotIn[12] = dataIn[12];
	parrotIn[13] = dataIn[13];
	parrotIn[14] = dataIn[14];
	parrotIn[15] = dataIn[15];
	parrotIn[16] = dataIn[16];
	parrotIn[17] = dataIn[17];
	parrotIn[18] = dataIn[18];
	parrotIn[19] = dataIn[19];
	parrotIn[20] = dataIn[20];
	parrotIn[21] = dataIn[21];
	parrotIn[22] = dataIn[22];
	parrotIn[23] = dataIn[23];
	parrotIn[24] = dataIn[24];
	parrotIn[25] = dataIn[25];
	parrotIn[26] = dataIn[26];
	parrotIn[27] = dataIn[27];
	parrotIn[28] = dataIn[28];
	parrotIn[29] = dataIn[29];
	parrotIn[30] = dataIn[30];
	parrotIn[31] = dataIn[31];
	parrotIn[32] = dataIn[32];
	parrotIn[33] = dataIn[33];
	parrotIn[34] = dataIn[34];
	parrotIn[35] = dataIn[35];
	parrotIn[36] = dataIn[36];
	parrotIn[37] = dataIn[37];
	parrotIn[38] = dataIn[38];
	parrotIn[39] = dataIn[39];
	parrotIn[40] = dataIn[40];
	parrotIn[41] = dataIn[41];
	parrotIn[42] = dataIn[42];
	parrotIn[43] = dataIn[43];
	parrotIn[44] = dataIn[44];
	parrotIn[45] = dataIn[45];
	parrotIn[46] = dataIn[46];
	parrotIn[47] = dataIn[47];
	parrotIn[48] = dataIn[48];
	parrotIn[49] = dataIn[49];
	parrotIn[50] = dataIn[50];
	parrotIn[51] = dataIn[51];
	parrotIn[52] = dataIn[52];
	parrotIn[53] = dataIn[53];
	parrotIn[54] = dataIn[54];
	parrotIn[55] = dataIn[55];
	parrotIn[56] = dataIn[56];
	parrotIn[57] = dataIn[57];
	parrotIn[58] = dataIn[58];
	parrotIn[59] = dataIn[59];
	parrotIn[60] = dataIn[60];
	parrotIn[61] = dataIn[61];
	parrotIn[62] = dataIn[62];
	parrotIn[63] = dataIn[63];
	ann = fann_create_from_file("fann.config/jpeg.nn");
	parrotOut = fann_run(ann, (fann_type*)parrotIn);
// parroto.write("jpeg", 0, dataIn, 64, 0, 0);
// 
// 	dct(Y1);
// 	quantization(Y1, ILqt);
// 
// 	for (int i = 0; i < BLOCK_SIZE; ++i)
// 	{
// 		dataOut[i] = Temp[i] / 256.;
// 	}
// 	isNN = false;
// 
// parroto.write("jpeg", 1, dataOut, 64, -0.9, 0.9);
// #pragma parrot(output, "jpeg", [64]<-0.9; 0.9>dataOut)
	dataOut[0] = parrotOut[0];
	dataOut[1] = parrotOut[1];
	dataOut[2] = parrotOut[2];
	dataOut[3] = parrotOut[3];
	dataOut[4] = parrotOut[4];
	dataOut[5] = parrotOut[5];
	dataOut[6] = parrotOut[6];
	dataOut[7] = parrotOut[7];
	dataOut[8] = parrotOut[8];
	dataOut[9] = parrotOut[9];
	dataOut[10] = parrotOut[10];
	dataOut[11] = parrotOut[11];
	dataOut[12] = parrotOut[12];
	dataOut[13] = parrotOut[13];
	dataOut[14] = parrotOut[14];
	dataOut[15] = parrotOut[15];
	dataOut[16] = parrotOut[16];
	dataOut[17] = parrotOut[17];
	dataOut[18] = parrotOut[18];
	dataOut[19] = parrotOut[19];
	dataOut[20] = parrotOut[20];
	dataOut[21] = parrotOut[21];
	dataOut[22] = parrotOut[22];
	dataOut[23] = parrotOut[23];
	dataOut[24] = parrotOut[24];
	dataOut[25] = parrotOut[25];
	dataOut[26] = parrotOut[26];
	dataOut[27] = parrotOut[27];
	dataOut[28] = parrotOut[28];
	dataOut[29] = parrotOut[29];
	dataOut[30] = parrotOut[30];
	dataOut[31] = parrotOut[31];
	dataOut[32] = parrotOut[32];
	dataOut[33] = parrotOut[33];
	dataOut[34] = parrotOut[34];
	dataOut[35] = parrotOut[35];
	dataOut[36] = parrotOut[36];
	dataOut[37] = parrotOut[37];
	dataOut[38] = parrotOut[38];
	dataOut[39] = parrotOut[39];
	dataOut[40] = parrotOut[40];
	dataOut[41] = parrotOut[41];
	dataOut[42] = parrotOut[42];
	dataOut[43] = parrotOut[43];
	dataOut[44] = parrotOut[44];
	dataOut[45] = parrotOut[45];
	dataOut[46] = parrotOut[46];
	dataOut[47] = parrotOut[47];
	dataOut[48] = parrotOut[48];
	dataOut[49] = parrotOut[49];
	dataOut[50] = parrotOut[50];
	dataOut[51] = parrotOut[51];
	dataOut[52] = parrotOut[52];
	dataOut[53] = parrotOut[53];
	dataOut[54] = parrotOut[54];
	dataOut[55] = parrotOut[55];
	dataOut[56] = parrotOut[56];
	dataOut[57] = parrotOut[57];
	dataOut[58] = parrotOut[58];
	dataOut[59] = parrotOut[59];
	dataOut[60] = parrotOut[60];
	dataOut[61] = parrotOut[61];
	dataOut[62] = parrotOut[62];
	dataOut[63] = parrotOut[63];

	for(int i = 0; i < BLOCK_SIZE; ++i)
	{
		Temp[i] = dataOut[i] * 256.0;
	}
	if(isNN)
	{
		for(int i = 8; i < BLOCK_SIZE; ++i)
		{
			Temp[i] = 0.0;
		}
	}

	outputBuffer = huffman(1, outputBuffer);

	return outputBuffer;
}
