import random
from django.core.management.base import BaseCommand
from django.contrib.auth.models import User
from api.models import TrashCan  # Cambia "your_app" por el nombre real de tu app
import math

class Command(BaseCommand):
    help = 'Seed database with initial data'

    def handle(self, *args, **kwargs):
        # Limpiar la base de datos
        self.stdout.write('🧹 Limpiando base de datos...')
        TrashCan.objects.all().delete()
        self.stdout.write(self.style.SUCCESS('✔ Todos los TrashCans eliminados.'))

        # Crear superusuario si no existe
        if not User.objects.filter(username='admin').exists():
            User.objects.create_superuser('admin', 'admin@example.com', '1234')
            self.stdout.write(self.style.SUCCESS('👑 Superusuario creado: admin / 1234'))
        else:
            self.stdout.write('⚠️ El superusuario ya existe.')

        # Centro (según coordenadas que diste)
        center_lat = -12.055747005302624
        center_lon = -77.0856462058371
        radius_km = 5  # radio de 5 km

        def generate_random_coords(lat, lon, radius_km):
            radius_deg = radius_km / 111  # aprox. conversión km a grados

            u = random.random()
            v = random.random()
            w = radius_deg * math.sqrt(u)
            t = 2 * math.pi * v
            delta_lat = w * math.cos(t)
            delta_lon = w * math.sin(t) / math.cos(math.radians(lat))

            return round(lat + delta_lat, 6), round(lon + delta_lon, 6)

        self.stdout.write('🚮 Generando TrashCans en un radio de 5km del centro...')
        for i in range(20):
            lat, lon = generate_random_coords(center_lat, center_lon, radius_km)
            temp = round(random.uniform(20.0, 40.0), 2)
            hum = round(random.uniform(30.0, 90.0), 2)

            tc = TrashCan.objects.create(
                temperature=temp,
                humidity=hum,
                latitude=str(lat),
                longitude=str(lon),
            )
            self.stdout.write(self.style.SUCCESS(f'✔ TrashCan #{i+1} → ({lat}, {lon})'))

        self.stdout.write(self.style.SUCCESS('✅ Seed finalizado exitosamente.'))