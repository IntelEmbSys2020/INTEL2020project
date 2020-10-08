#include "final.h"
#include "detection.h"
#include "estimate.h"

extern "C"
{
    void detect_final(char* str);
}

void detect_final(char* str){
    Mat image = imread(str);
    Mat detect_result;
    flag_detection_t td;
    Eular_t current_att;
    current_att.pitch = 0;
    current_att.yaw = 0;
    current_att.roll = 0;
    detect_result = DetectMark(image,td);
    
    if(td.is_found == 0){
        putText(detect_result,"Can't find landmark!",Point(100,50),FONT_HERSHEY_PLAIN,2,Scalar(0,0,255),3);
    }
    else{
        Estimate(&td, current_att);
        char ch[64];
        sprintf(ch,"%g %g %g",td.Pw.x,td.Pw.y,td.Pw.z);
        cout << td.Pw.x << " " << td.Pw.y << " " << td.Pw.z << endl;
        putText(detect_result,"Find landmark!",Point(100,50),FONT_HERSHEY_PLAIN,2,Scalar(0,0,255),3);
        putText(detect_result,"Relative location:",Point(100,100),FONT_HERSHEY_PLAIN,2,Scalar(0,0,255),3);
        putText(detect_result,ch,Point(100,150),FONT_HERSHEY_PLAIN,2,Scalar(0,0,255),3);
    }
    //namedWindow("test",0);
    //imshow("test",detect_result);
    //return detect_result;
    imwrite("detect_result.jpg",detect_result);
    //waitKey(1000);
    //destroyAllWindows();
}