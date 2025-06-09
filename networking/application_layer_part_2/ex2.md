# To install the Apache server:

## Install Apache2 and PHP

'''bash  
sudo apt update
sudo apt install apache2 php libapache2-mod-php

## Restart Apache

sudo systemctl restart apache2

## Enable PHP module

'''bash
sudo a2enmod php8.3

## Navigate to the correct directory

'''bash
cd /var/www/html

## Remove the default index.html

'''bash
rm index.html

## Create a PHP file

'''bash
subl index.php

'''php
<?php
echo "Hello World!";
?>		

## Set permissions

'''bash
sudo chown www-data:www-data /var/www/html/index.php
sudo chmod 755 /var/www/html/index.php

## Restart Apache again

'''bash
sudo systemctl restart apache2

## Test in browser

http://localhost