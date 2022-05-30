import cv2
import os

if __name__ == "__main__":
    img = cv2.imread("pic.ppm")
    cv2.imwrite("pic.png", img)
