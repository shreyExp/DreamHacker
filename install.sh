#!/bin/bash
make
sudo cp UI /var/www/html
chromium-browser --disable-features=AudioServiceSandbox localhost
