#!/usr/bin/env python3
"""A simple script to perfom some basic operations in hashd
"""

import sys
import json
import struct
import argparse
from socket import socket, AF_INET, SOCK_STREAM

OPFIND = 0xff
OPDEL = 0xfd
OPADD = 0xfe


class Hashd:
    def __init__(self, ip, port):
        self._connect(ip, int(port))

    def _connect(self, ip, port):
        self._sock = socket(AF_INET, SOCK_STREAM)
        self._sock.connect((ip, port))

    def _pack(self, code, k, v):
        klen, vlen = len(k), len(v)
        fmt = 'HHI{0}s{1}s'.format(klen, vlen)
        packed_str = struct.pack(fmt, code, klen, vlen, k, v)
        return packed_str

    def _get_pack(self, code, k):
        v = ''
        klen, vlen = len(k), len(v)
        fmt = 'HHI{0}s{1}s'.format(klen, vlen)
        packed_str = struct.pack(fmt, code, klen, vlen, k, v)
        return packed_str

    def _send(self, data):
        self._sock.send(data)

    def _recv(self):
        return self._sock.recv(1024)

    def set(self, k, v):
        key = bytes(k, 'ascii')
        value = bytes(v, 'ascii')
        data = self._pack(OPADD, key, value)
        self._send(data)

    def get(self, key):
        key = bytes(key, 'ascii')
        value = bytes('', 'ascii')
        # data = self._get_pack(OPFIND, key, value)
        data = struct.pack(
            'HHI{0}s'.format(len(key)),
            OPFIND,
            len(key),
            len(value),
            key,
        )
        self._send(data)
        print(self._recv())

    # def rm(self, k):
        # pass


def main(args):

    hashd = Hashd('127.0.0.1', '9999')
    hashd.set('a', 'fabio junior bertinatto e meu nome')
    hashd.get('a')

    # for i in range(100):
        # hashd = Hashd('127.0.0.1', 9999)
        # key = 'k' + str(i)
        # value = 'v' + str(i)

        # if hashd.set(key, value) != 1:
            # raise Exception('Error: set {0} -> {1}'.format(key, value))

        # if hashd.get(key) != value:
            # raise Exception('Error: get {0} -> {1}'.format(key, value))

        # if hashd.rm(key) is False:
            # raise Exception('Error: del {0} -> {1}'.format(key, value))

        # if hashd.get(key) is not None:
            # raise Exception('Error: del {0} -> None'.format(key, value))

        # # Add
        # if options.add:
            # s.send(struct.pack(
                # 'HHI{0}s{1}s'.format(len(key), len(value)),
                # OPADD,
                # len(key),
                # len(value),
                # key,
                # value
            # ))

        # if options.find:
            # # Find
            # s.send(struct.pack(
                # 'HHI{0}s'.format(len(key)),
                # OPFIND,
                # len(key),
                # len(value),
                # key,
            # ))

        # if options.delete:
            # s.send(struct.pack(
                # 'HHI{0}s'.format(len(key)),
                # OPDEL,
                # len(key),
                # len(value),
                # key,
            # ))


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
