from django.contrib.auth.models import User, Group
from rest_framework import viewsets
from rest_framework import permissions
from .serializers import *
from .models import *
from rest_framework.permissions import AllowAny
import random as rd
import base64
import json
from django.views.decorators.csrf import csrf_exempt
from django.views.decorators.http import require_POST
from django.http import JsonResponse
from django.shortcuts import render

class ResidueViewSet(viewsets.ModelViewSet):
    """
    API endpoint that allows Residues to be viewed or edited.
    """
    queryset = Residue.objects.all().order_by('-datetime')
    serializer_class = ResidueSerializer
    permission_classes = [AllowAny]


class TrashCanViewSet(viewsets.ModelViewSet):
    """
    API endpoint that allows TrashCans to be viewed or edited.
    """
    queryset = TrashCan.objects.all()
    serializer_class = TrashCanSerializer
    permission_classes = [AllowAny]


@csrf_exempt
@require_POST
def send_img(request):
    if request.method == 'POST':
        # data = request.POST #json
        data = json.loads(request.body)
        print(data)
        imagen_base64 = data.get('frame', None)
        lng, lat = data.get("longitude", '-77.0856462058371'), data.get("latitude", '-77.0856462058371')
        if imagen_base64 is not None:
            try:
                imagen_decodificada = base64.b64decode(imagen_base64)
                imagen_temporal = ContentFile(imagen_decodificada)

                # nuevo_registro = Residue(longitude='-77.0856462058371', latitude='-77.0856462058371')
                nuevo_registro = Residue(longitude=lng, latitude=lat)
                nuevo_registro.frame.save(f'{rd.randint(10, 10000)}.jpg', imagen_temporal)
                
                if nuevo_registro.most_confident_label is None or nuevo_registro.confidence is None:
                    
                    return JsonResponse({'error': "Image not detected"}, status=400)
                
                return JsonResponse({
                    'most_confident_label' : nuevo_registro.most_confident_label,
                    'confidence' : nuevo_registro.confidence,
                })
            except Exception as e:
                return JsonResponse({'error': "Image not detected"}, status=400)

    return JsonResponse({'error': 'Método no permitido'}, status=405)

def get_residues_can(request, can:int):
    if request.method == "GET":
        residues = Residue.objects.filter(trash_can__id=can)
        serializer = ResidueSerializer(residues, many=True)
        return JsonResponse({"data": serializer.data})
    return JsonResponse({"error": "Method not allowed"}, status=405)

def map_view(request):
    return render(request, 'api/map.html')