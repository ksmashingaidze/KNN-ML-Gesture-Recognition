# KNN-ML-Gesture-Recognition
KNN ML Gesture Recognition in C++

Implementation of a gesture recognition C++ application using image processing techniques and k-nearest neighbors supervised machine learning algorithm. 

Training data stored in the folder "trainingdata", and an image "testimage.bmp" stored in the folder "testimage" are first converted to greyscale and passed through a preliminary edge detection phase in order to extract distinguishing information. The training data is also given an array of known target values, as their classifications are already known. '0' denotes a single finger gesture, and '1' denotes a peace sign gesture. 

Euclidian distances between the test image and each image that constitutes the training data are then calculated. The training target array is then sorted in ascending order of the previously calculated Euclidian distances, from the best match to the least likely match, with the first 'k' members denoting the closest matches to the test image that exist in the training data. The variable 'k' denotes the number of nearest neighbors to the test image that should be considered. 

The target values of the first 'k' members are then averaged by adding them and dividing the result by 'k'. The result is subsequently rounded off to find the target value of the test image ('0' or '1').
