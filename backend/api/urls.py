from django.urls import path

from . import views

urlpatterns = [
    # path("", views.index, name = "index"),
    path('sendImg/', views.send_img, name="sendImg"),
    path('residues/<int:can>/', views.get_residues_can, name="residuesCan"),
    path('map/', views.map_view, name="Map")
]