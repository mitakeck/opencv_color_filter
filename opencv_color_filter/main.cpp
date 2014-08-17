#include <opencv\cv.h>
#include <opencv\highgui.h>

int main(void){

	// “ü—Í‰æ‘œ
	char fileName[] = "lena.jpg";
	IplImage* input = cvLoadImage(fileName, 1);
	IplImage* output = cvCreateImage(cvGetSize(input), IPL_DEPTH_8U, 3);

	for(int y=0; y<input->height; y++){
		for(int x=0; x<input->width; x++){
			uchar b = input->imageData[input->widthStep * y + x*3];
			uchar g = input->imageData[input->widthStep * y + x*3 + 1];
			uchar r = input->imageData[input->widthStep * y + x*3 + 2];
			if(r>20 && r>b*2 && r>g*2 ){
				output->imageData[input->widthStep * y + x*3] = b;
				output->imageData[input->widthStep * y + x*3 + 1] = g;
				output->imageData[input->widthStep * y + x*3 + 2] = r;
			}else{
				output->imageData[input->widthStep * y + x*3] = 255;
				output->imageData[input->widthStep * y + x*3 + 1] = 255;
				output->imageData[input->widthStep * y + x*3 + 2] = 255;
			}
		}
	}
	cvNamedWindow("input", CV_WINDOW_AUTOSIZE);
	cvShowImage("input", input);

	cvNamedWindow("output", CV_WINDOW_AUTOSIZE);
	cvShowImage("output", output);

	cvWaitKey(0);

	cvReleaseImage(&input);
	cvReleaseImage(&output);
	cvDestroyWindow("input");
	cvDestroyWindow("output");

	return 0;
}