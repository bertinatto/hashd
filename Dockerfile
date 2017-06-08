FROM fedora:25
ADD . /chroot
WORKDIR /chroot
RUN dnf install -y clang libev-devel glib-devel make
