from keras.applications.mobilenet import preprocess_input
from keras.preprocessing.image import img_to_array
from keras.preprocessing.image import load_img
from keras.models import load_model

from django.db import models
from django.core.validators import FileExtensionValidator
from PIL import Image
import numpy as np
import cv2
from django.utils.safestring import mark_safe
from django.core.files.base import ContentFile
import io

import imutils
import os

class Detection(models.Model):
    datetime = models.DateTimeField("Date and time of detection", auto_now_add=True, null=True)
    
    frame = models.ImageField(upload_to="frames/", null=True)
    frame_image_detected = models.ImageField(upload_to="frames/", null=True, blank=True)
    #video = models.FileField(upload_to="videos/", null=True,
    #validators=[FileExtensionValidator(allowed_extensions=['MOV', 'avi', 'mp4', 'webm', 'mkv'])])
    latitude = models.CharField(max_length=20, default='-12.055747005302624', null=True)
    longitude = models.CharField(max_length=20, default='-77.0856462058371', null=True)
    
    # New fields for detection results
    most_confident_label = models.CharField(max_length=50, blank=True, null=True)
    confidence = models.FloatField(null=True, blank=True)
    
    def __str__(self):
        return str(self.datetime)


    def save(self, *args, **kwargs):
        super().save(*args, **kwargs)
        if self.frame:
            self.process_image_for_detection()

    def process_image_for_detection(self):
        # Model architecture and weights
        #prototxt = "api/MobileNetSSD_deploy.prototxt.txt"
        #model = "api/MobileNetSSD_deploy.caffemodel"
        TARGET_SIZE = (224, 224)
        

        # Load the model
        #net = cv2.dnn.readNetFromCaffe(prototxt, model)
        model = load_model('api/hack_mobilenet.h5')

        # Preprocess the image for object detection
        open_cv_image = cv2.imread(self.frame.path)
        img = load_img(self.frame.path, target_size = TARGET_SIZE)
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

        # Class labels
        waste_types = ['CARTON', 'VIDRIO', 'METAL', 'PAPEL', 'PLASTICO', 'GENERAL']

        # pass the image through the network to obtain our predictions
        # preds = model.predict(np.expand_dims(image / 255., axis=0))[0]
        preds = model.predict(img)[0]

        i = np.argmax(preds)
        label = waste_types[i]

        self.confidence = preds[i] * 100
        self.most_confident_label = label

        # draw the prediction on the output image
        text = "{} | CONF.: {:.2f}%".format(label, preds[i] * 100)
        cv2.putText(output, text, (5, 20), cv2.FONT_HERSHEY_PLAIN, 1.05, (255, 0, 0), 2)

        #cv2.imwrite('output.jpg', output) # NOVA



        # Save the modified image to frame_image_detected
        buffer = io.BytesIO()
        #image_with_box = Image.fromarray(open_cv_image)
        #image_with_box = Image.fromarray((output * 255).astype(np.uint8))
        image_with_box = Image.fromarray(output, mode='RGB')
        image_with_box.save(buffer, format='JPEG')

        

        if self.confidence is not None:
            #self.frame_image_detected.save('detected_{}.jpg'.format(self.pk), ContentFile(buffer.getvalue()), save=False)
            self.frame_image_detected.save('detected_{}.jpg'.format(self.pk), ContentFile(buffer.getvalue()), save=False)


        # Save the updated Detection instance
        if self.confidence is not None and self.frame_image_detected is not None:                    
            super().save(update_fields=['most_confident_label', 'confidence', 'frame_image_detected'])


    def admin_image(self):
        return mark_safe('<a href="{url}"><img src="{url}" width="{width}" height={height} /></a>'.format(
            url=self.frame.url,
            width=100,
            height=100,
            ))
    admin_image.short_description = 'Image'
    admin_image.allow_tags = True

    def admin_image_detected(self):
        return mark_safe('<a href="{url}"><img src="{url}" width="{width}" height={height} /></a>'.format(
            url=self.frame_image_detected.url,
            width=100,
            height=100,
            ))
    admin_image_detected.short_description = 'Image Detected'
    admin_image_detected.allow_tags = True

    class Meta:
        verbose_name = "Predicción"
        verbose_name_plural = "Predicciones"