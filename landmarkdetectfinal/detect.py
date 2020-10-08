import subprocess
import os
import cv2
import ctypes
# imgPath=b'land03.bmp'
def detect(imgPath):
    ll=ctypes.cdll.LoadLibrary
    lib = ll("./build/libdetect_final.so")    
    lib.detect_final(imgPath)
    result = cv2.imread("detect_result.jpg")
    cv2.imshow("detect_result",result)
    cv2.waitKey(0)
    cv2.destroyAllWindows()
