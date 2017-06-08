#!/usr/bin/env python3
"""A simple script to perfom some basic operations in hashd
"""

import sys
import json
import time
import struct
import argparse
import subprocess
from socket import socket, AF_INET, SOCK_STREAM

OPFIND = 0xff
OPDEL = 0xfd
OPADD = 0xfe


def start_hashd():
    subprocess.Popen(['../../src/hashd'])
    time.sleep(5)


def get_options(args):
    parser = argparse.ArgumentParser(
            description=__doc__,
            formatter_class=argparse.RawDescriptionHelpFormatter
    )
    parser.add_argument('-a',
                        '--add',
                        help="Add records into hash table",
                        action='store_true')
    parser.add_argument('-f',
                        '--find',
                        help="Look up hash table",
                        action='store_true')
    parser.add_argument('-d',
                        '--delete',
                        help="Delete records from hash table",
                        action='store_true')
    return parser.parse_args(args)


def main(args):
    options = get_options(args)

    # Start service
    # start_hashd()

    for i in range(100):
        s = socket(AF_INET, SOCK_STREAM)
        s.connect(('127.0.0.1', 9999))

        key = bytes('secret' + str(i), 'ascii')
        value = bytes(json.dumps({'payload': str(i)}), 'ascii')

        # Add
        if options.add:
            s.send(struct.pack(
                'HHI{0}s{1}s'.format(len(key), len(value)),
                OPADD,
                len(key),
                len(value),
                key,
                value
            ))

        if options.find:
            # Find
            s.send(struct.pack(
                'HHI{0}s'.format(len(key)),
                OPFIND,
                len(key),
                len(value),
                key,
            ))

        if options.delete:
            s.send(struct.pack(
                'HHI{0}s'.format(len(key)),
                OPDEL,
                len(key),
                len(value),
                key,
            ))


if __name__ == '__main__':
    sys.exit(main(sys.argv[1:]))
