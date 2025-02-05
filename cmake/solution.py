#!/usr/bin/env python3

import sys
import os

script_path = os.path.dirname(os.path.abspath(__file__))
os.chdir(script_path)
sys.path.append(script_path + "/tools/solution")

import solution_main

solution_main.main(solution_main.parser.parse_args())
