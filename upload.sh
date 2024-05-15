#!/bin/bash

# Hacer git pull
git pull

# Añadir todos los archivos al staging area
git add -A

# Pedir al usuario que ingrese el mensaje del commit
echo "Ingrese el mensaje del commit:"
read commit_message

# Hacer commit con el mensaje ingresado por el usuario
git commit -m "$commit_message"

# Hacer push al repositorio remoto
git push
