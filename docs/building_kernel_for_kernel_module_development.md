this document is a prescription for building the linux kernel from scratch for kernel module development.
Linux kernels check the certificates of kernel modules before loading them into the kernel, cause we do not have a trusty certificate we cannot load our kernel modules. for more information check this: https://www.kernel.org/doc/html/v4.18/admin-guide/module-signing.html

### Compiling kernel 
## 1- Download kernel
```
wget https://cdn.kernel.org/pub/linux/kernel/v6.x/linux-6.6.22.tar.xz
```
or 
```
$ git clone git://git.kernel.org/pub/scm/linux/kernel/git/stable/linux-stable.git
```
## 2- Install packages
```
sudo apt-get install git fakeroot build-essential ncurses-dev xz-utils libssl-dev bc flex libelf-dev bison
```
## 3- Config
this step is the most important part, I use my ubuntu config file to config the new kernel.
copy the existing config file.
```
cd linux-6.6.22
cp -v /boot/config-$(uname -r) .config
```
run:
```
make menuconfig
```
save and exit.
open the *.config* and replace corresponding lines with:
```
#
# Certificates for signature checking
#
CONFIG_MODULE_SIG_KEY="certs/signing_key.pem"
CONFIG_SYSTEM_TRUSTED_KEYRING=y
CONFIG_SYSTEM_TRUSTED_KEYS=""
CONFIG_SYSTEM_EXTRA_CERTIFICATE=y
CONFIG_SYSTEM_EXTRA_CERTIFICATE_SIZE=4096
CONFIG_SECONDARY_TRUSTED_KEYRING=y
CONFIG_SYSTEM_BLACKLIST_KEYRING=y
CONFIG_SYSTEM_BLACKLIST_HASH_LIST=""
CONFIG_SYSTEM_REVOCATION_LIST=y
CONFIG_SYSTEM_REVOCATION_KEYS=""
# end of Certificates for signature checking
```
save it and exit.
we disable the *canonical-certs* and *canonical-revoced-cert*, the **CONFIG_MODULE_SIG_KEY** part points to the certificates which the kernel is used to sign all modules during the build process.
If you want to have canonical certs which contains trusty canonical certs you reach them with:
```
sudo mkdir -p /debian
sudo apt install linux-source
sudo cp -v /usr/src/linux-source-*/debian/canonical-*.pem debian/
sudo apt purge linux-source*
```
## 4-Generating our key
**you can exit this step and jump to building, during the building phase it generates a key for you.** 
```
openssl req -new -nodes -utf8 -sha256 -days 36500 -batch -x509 \
   -config certs/default_x509.genkey -outform PEM -out certs/signing_key.pem \
   -keyout certs/signing_key.pem
```
you can replace *default_x509.genkey* with your self defined file.  
## 5-Compiling
```
sudo make
```
## 6- Installing
```
sudo make modules_install
sudo make install
sudo reboot
```
## 7-Signing our modules
at the end you can sign your kernel modules with:
```
scripts/sign-file sha256 /certs/signing_key.pem /certs/signing_key.509 /path/to/yourmodule.ko
```
finaly you are able to load it.

