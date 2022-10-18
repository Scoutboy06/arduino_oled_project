# python 3.0

import cv2

filename = 'never_gonna_give_you_up'
filetype = 'mp4'
width = 128
height = 64

thresh = 127


vidcap = cv2.VideoCapture('videos/' + filename + '.' + filetype)
for i in range(100):
  success,image = vidcap.read()

bitmap_arr = []
output_file = open('frameVideos/' + filename + '.h', 'w')
output_file.write('const unsigned char %s [] PROGMEM = {\n' % (filename + '_video'))

while success:   
  success,image = vidcap.read()

  if not success:
    break

  resized = cv2.resize(image, (width, height), interpolation = cv2.INTER_AREA)
  gray_image = cv2.cvtColor(resized, cv2.COLOR_BGR2GRAY)
  # (_, im_bw) = cv2.threshold(gray_image, 128, 255, cv2.THRESH_BINARY | cv2.THRESH_OTSU)
  im_bw = cv2.threshold(gray_image, thresh, 255, cv2.THRESH_BINARY)[1]

  # cv2.imwrite('frameVideos/bw_image.png', im_bw)
  
  base_10_num = 0
  frame = ''
  pixel_count = 0

  for i in range(0, height): # row
    for j in range(0, width): # col
      color = im_bw[i, j] # returns 0 or 255
      base_10_num += int(color / 255) * pow(2, pixel_count)

      pixel_count = (pixel_count + 1) % 16

      if(pixel_count == 0):
        hex_num = hex(base_10_num)
        frame += hex_num + ','

        base_10_num = 0

  output_file.write(frame + '\n')

output_file.write('};')
output_file.close()

print('Done!')