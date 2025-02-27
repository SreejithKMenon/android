#
# Copyright (C) 2018 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
"""Generates a intermediate file for using in making a compilation database.

The intermediate file is also a JSON file, but only encodes the data for a
single output.
"""
from __future__ import print_function

import argparse
import json
import os


def parse_args():
    """Parses and returns command line arguments."""
    parser = argparse.ArgumentParser()

    parser.add_argument(
        '-o', '--output', type=os.path.realpath, help='Path to output file')

    parser.add_argument(
        '-d',
        '--directory',
        type=os.path.realpath,
        help='Working directory for the compile command.')

    parser.add_argument('-f', '--file', help='Source file.')
    parser.add_argument('--object-file', help='Object file.')
    parser.add_argument(
        '--command-file', type=os.path.realpath, help='Compilation command.')

    return parser.parse_args()


def main():
    """Program entry point."""
    args = parse_args()

    with open(args.command_file) as command_file:
        command = command_file.read().strip()

    with open(args.output, 'w') as out_file:
        json.dump({
            'directory': args.directory,
            'file': args.file,
            'output': args.object_file,
            'command': command,
        }, out_file)


if __name__ == '__main__':
    main()
