# Quadtree
Program for image processing

  I used four data structures: the first one is a pixel in RGB format, the second is a vector structure, the third is a structure used for file description of the image and the fourth is a quadtree used for retains the pixels of the image.
  # We can also do four operations on an image:
  - Image compression - MakeTree function calculate the average value of the colors in the nodes and decide if it is necessary to divide the node in four nodes.
  - Image decompression - MakeTreeVector calculate the compression tree and then the matrix of pixels for the compress image
  - Image reflection - This is about interchange the nodes in the quadtree
  - Overlaying images - Traverse the two compression trees and form the third compression tree that have the average values of the colors from pixels
