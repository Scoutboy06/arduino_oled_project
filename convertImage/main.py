# python 3.0

import cv2

filename = 'videos/never_gonna_give_you_up.mp4'
width = 128
height = 64

thresh = 127

vidcap = cv2.VideoCapture(filename)
for i in range(100):
  success,image = vidcap.read()
count = 0
while success:   
  success,image = vidcap.read()
  resized = cv2.resize(image, (width, height), interpolation = cv2.INTER_AREA)
  gray_image = cv2.cvtColor(resized, cv2.COLOR_BGR2GRAY)
  # (_, im_bw) = cv2.threshold(gray_image, 128, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)
  im_bw = cv2.threshold(gray_image, thresh, 255, cv2.THRESH_BINARY)[1]

  # cv2.imwrite('frameVideos/bw_image.png', im_bw)

  for i in range(width):
    for j in range(height):
      color = im_bw[i, j]
  


  count += 1
  break

  
  #(thresh, blackAndWhiteImage) = cv2.threshold(gray_image, 127, 255, cv2.THRESH_BINARY)
  #gör en bitmap

