from keras.applications.mobilenet import preprocess_input
from keras.preprocessing.image import img_to_array
from keras.preprocessing.image import load_img
from keras.models import load_model

import numpy as np
import imutils
import cv2
import os

# Parameters
TARGET_SIZE = (224, 224)

# load the trained model from disk
print("[INFO] loading model...")
model = load_model('hack_mobilenet.h5')

# load the input image and then clone it so we can draw on it later
waste_types = ['CARTON', 'VIDRIO', 'METAL', 'PAPEL', 'PLASTICO', 'GENERAL']

path = "botellita.jpg"

open_cv_image = cv2.imread(path)
img = load_img(path, target_size = TARGET_SIZE)
img  = img_to_array(img)

output = open_cv_image.copy()
output = imutils.resize(output, width = 400)

# our model was trained on RGB ordered images but OpenCV represents
# images in BGR order, so swap the channels, and then resize to
# 224x224 (the input dimensions for VGG16)
open_cv_image = cv2.cvtColor(open_cv_image, cv2.COLOR_BGR2RGB)
open_cv_image = cv2.resize(open_cv_image, TARGET_SIZE)

# convert the image to a floating point data type and perform mean
# subtraction
open_cv_image = open_cv_image.astype("float32") / 255.
img = np.expand_dims(img, axis = 0)
img = preprocess_input(img)

# pass the image through the network to obtain our predictions
# preds = model.predict(np.expand_dims(image / 255., axis=0))[0]
preds = model.predict(img)[0]
i = np.argmax(preds)
label = waste_types[i]

# draw the prediction on the output image
text = "{} | CONF.: {:.2f}%".format(label, preds[i] * 100)
cv2.putText(output, text, (5, 20), cv2.FONT_HERSHEY_PLAIN, 1.05, (255, 0, 0), 2)

cv2.imwrite('output.jpg', output)