from django.contrib import admin

from .models import Residue, TrashCan

class TrashCanAdmin(admin.ModelAdmin):

    list_display = [field.name for field in TrashCan._meta.fields]

class ResidueAdmin(admin.ModelAdmin):

    list_display = ("id", 'admin_image', 'datetime', 'most_confident_label', 'confidence', 'latitude', 'longitude')

    ordering = ("id",)
    

    ordering = ('-datetime',)

admin.site.register(TrashCan, TrashCanAdmin)
admin.site.register(Residue, ResidueAdmin)