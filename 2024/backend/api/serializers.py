from rest_framework import serializers

from .models import *

class ResidueSerializer(serializers.ModelSerializer):
    class Meta:
        model = Residue
        fields = ['datetime', 'frame', 'latitude', 'longitude', 'most_confident_label', 'confidence', "trash_can"]

class TrashCanSerializer(serializers.ModelSerializer):
    class Meta:
        model = TrashCan
        fields = "__all__"