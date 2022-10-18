# python 3.0

import cv2

filename = 'never_gonna_give_you_up'
filetype = 'mp4'
width = 128
height = 64

grayscale_threshold = 127

vidcap = cv2.VideoCapture('videos/' + filename + '.' + filetype)

frame_count = 0
output_file = open('frameVideos/' + filename + '.h', 'w')
output_file.write('const unsigned char %s [] PROGMEM = {\n' % (filename + '_video'))

success,image = vidcap.read()

while success:
  # Resize and grayscale image
  resized = cv2.resize(image, (width, height), interpolation = cv2.INTER_AREA)
  gray_image = cv2.cvtColor(resized, cv2.COLOR_BGR2GRAY)
  im_bw = cv2.threshold(gray_image, grayscale_threshold, 255, cv2.THRESH_BINARY)[1]

  current_frame = []
  current_color = 0
  pixels_in_row = 0

  # Format described in notes.txt step 4

  for i in range(height):
    for j in range(width):
      color = im_bw[i, j] # returns 0 or 255
      if color == current_color:
        pixels_in_row += 1

        if(pixels_in_row >= 255):
          current_frame.append("255")
          current_frame.append("0")
          pixels_in_row = 0

      else:
        current_frame.append(str(pixels_in_row))
        pixels_in_row = 1
        current_color = 255 if current_color == 0 else 0

  current_frame.append(str(pixels_in_row))
  output_file.write("{" + (','.join(current_frame)) + "},\n")

  frame_count += 1
  success,image = vidcap.read()

output_file.write('};')
output_file.close()

print('Done!')