# Generated by Django 5.0 on 2024-07-09 05:07

import django.db.models.deletion
from django.db import migrations, models


class Migration(migrations.Migration):

    dependencies = [
        ('api', '0006_alter_detection_trash_can'),
    ]

    operations = [
        migrations.AlterField(
            model_name='detection',
            name='trash_can',
            field=models.ForeignKey(blank=True, default=None, null=True, on_delete=django.db.models.deletion.CASCADE, to='api.trashcan'),
        ),
    ]
