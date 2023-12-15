from rest_framework import serializers

from .models import *

class DetectionSerializer(serializers.HyperlinkedModelSerializer):
    class Meta:
        model = Detection
        fields = ['datetime', 'frame', 'latitude', 'longitude', 'most_confident_label', 'confidence']