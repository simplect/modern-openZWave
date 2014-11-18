tar -xzf rpi.tgz
cp rpi/ModernOZW .
mv rpi/open-zwave .
echo "Installed successfully, now going into sudo and copying the library to /usr/local/lib"
echo "Please edit this file if your libraries are stored elsewhere"
sudo cp open-zwave/libopenzwave.so.1.3 /usr/lib/
sudo rm /usr/lib/libopenzwave.so
sudo ln -s /usr/lib/libopenzwave.so.1.3 /usr/lib/libopenzwave.so
sudo ldconfig
