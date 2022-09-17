#!/bin/bash

echo "Script to start MaryTTS server!"
echo "autorestart arg: $1"

# Runs MaryTTS container (restarts if already running and arg $1 is set to true)
if ! sudo docker ps --format '{{.Image}}' | grep -w synesthesiam/marytts:5.2 &> /dev/null; then
  echo "MaryTTS Server container not running yet."
  echo 'Running container...'
  sudo docker run -d -it -p 59125:59125 synesthesiam/marytts:5.2 --voice cmu-slt-hsmm
else
  echo "MaryTTS Server container already running."
  if $1 == "true"; then
    echo "Stopping container..."
    sudo docker ps -q --filter ancestor=synesthesiam/marytts:5.2 | sudo xargs -r docker stop
    echo 'Stopped container!'
    echo 'Running container...'
    sudo docker run -d -it -p 59125:59125 synesthesiam/marytts:5.2 --voice cmu-slt-hsmm
  fi
fi