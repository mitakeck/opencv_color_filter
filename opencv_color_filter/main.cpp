#include <opencv\cv.h>
#include <opencv\highgui.h>


// http://imagingsolution.blog107.fc2.com/blog-entry-248.html
void cv_ColorExtraction(IplImage* src_img, IplImage* dst_img, 
                            int code,
                            int ch1_lower, int ch1_upper, 
                            int ch2_lower, int ch2_upper, 
                            int ch3_lower, int ch3_upper
                        ){

    int i, k;    

    IplImage *Color_img;
    IplImage *ch1_img, *ch2_img, *ch3_img;
    IplImage *Mask_img;

    int lower[3];
    int upper[3];
    int val[3];

    CvMat *lut;    

    //codeに基づいたカラー変換
    Color_img = cvCreateImage(cvGetSize(src_img), src_img->depth, src_img->nChannels);
    cvCvtColor(src_img, Color_img, code);
        
    //3ChのLUT作成
    lut    = cvCreateMat(256, 1, CV_8UC3);

    lower[0] = ch1_lower;
    lower[1] = ch2_lower;
    lower[2] = ch3_lower;

    upper[0] = ch1_upper;
    upper[1] = ch2_upper;
    upper[2] = ch3_upper;

    for (i = 0; i < 256; i++){
        for (k = 0; k < 3; k++){
            if (lower[k] <= upper[k]){
                if ((lower[k] <= i) && (i <= upper[k])){
                    val[k] = 255;
                }else{
                    val[k] = 0;
                }
            }else{
                if ((i <= upper[k]) || (lower[k] <= i)){
                    val[k] = 255;
                }else{
                    val[k] = 0;
                }
            }
        }
        //LUTの設定
        cvSet1D(lut, i, cvScalar(val[0], val[1], val[2]));
    }

    //3ChごとのLUT変換（各チャンネルごとに２値化処理）
    cvLUT(Color_img, Color_img, lut);
    cvReleaseMat(&lut);

    //各チャンネルごとのIplImageを確保する
    ch1_img = cvCreateImage(cvGetSize(Color_img), Color_img->depth, 1);
    ch2_img = cvCreateImage(cvGetSize(Color_img), Color_img->depth, 1);
    ch3_img = cvCreateImage(cvGetSize(Color_img), Color_img->depth, 1);

    //チャンネルごとに二値化された画像をそれぞれのチャンネルに分解する
    cvSplit(Color_img, ch1_img, ch2_img, ch3_img, NULL);

    //3Ch全てのANDを取り、マスク画像を作成する。
    Mask_img = cvCreateImage(cvGetSize(Color_img), Color_img->depth, 1);
    cvAnd(ch1_img, ch2_img, Mask_img);
    cvAnd(Mask_img, ch3_img, Mask_img);

    //入力画像(src_img)のマスク領域を出力画像(dst_img)へコピーする
    cvZero(dst_img);
    cvCopy(src_img, dst_img, Mask_img);

    //解放
    cvReleaseImage(&Color_img);
    cvReleaseImage(&ch1_img);
    cvReleaseImage(&ch2_img);
    cvReleaseImage(&ch3_img);
    cvReleaseImage(&Mask_img);

}


int main(void){

	// 入力画像
	char fileName[] = "lena.jpg";
	IplImage* input = cvLoadImage(fileName, 1);
	IplImage* output = cvCreateImage(cvGetSize(input), IPL_DEPTH_8U, 3);

	for(int y=0; y<input->height; y++){
		for(int x=0; x<input->width; x++){
			uchar b = input->imageData[input->widthStep * y + x*3];
			uchar g = input->imageData[input->widthStep * y + x*3 + 1];
			uchar r = input->imageData[input->widthStep * y + x*3 + 2];
			if(r>20 && r>b*2 && r>g*2 ){
				output->imageData[input->widthStep * y + x*3] = 255;
				output->imageData[input->widthStep * y + x*3 + 1] = 255;
				output->imageData[input->widthStep * y + x*3 + 2] = 255;
			}else{
				output->imageData[input->widthStep * y + x*3] = 0;
				output->imageData[input->widthStep * y + x*3 + 1] = 0;
				output->imageData[input->widthStep * y + x*3 + 2] = 0;
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