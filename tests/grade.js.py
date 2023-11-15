import os
import subprocess
from pathlib import Path

import difflib

def asset_lines_equal(output, exp):
    output_lines = output.split("\n")
    exp_lines = exp.split("\n")

    diff = list(difflib.unified_diff(exp_lines, output_lines, lineterm='', fromfile='expected', tofile='output'))
    if diff:
        for line in diff:
            print(line)
        return False
    return True

def run_binary(binary, test_file):
    test_file = Path(__file__).parent / "kevin" / binary / f"{test_file}.inn"
    env = os.environ.copy()
    env["LOGNAME"] = "student"
    with open(test_file, "r") as file:
        input_data = file.read()
    result = subprocess.run([Path(__file__).parent.parent / f"{binary}.out"],
                            input=input_data, text=True, stdout=subprocess.PIPE,
                            stderr=subprocess.PIPE, env=env)
    return result.stdout, result.stderr, result.returncode

def run_test(binary, test_file):
    output_file = Path(__file__).parent / "kevin" / binary / f"{test_file}.outt"
    errput_file = Path(__file__).parent / "kevin" / binary / f"{test_file}.errr"
    stdout, stderr, code = run_binary(binary, test_file)
    print(f"Checking stdout for {binary}/{test_file}")
    with open(output_file, "r") as file:
        ok = asset_lines_equal(stdout, file.read())
    print(f"Checking stderr for {binary}/{test_file}")
    with open(errput_file, "r") as file:
        ok = ok and asset_lines_equal(stderr, file.read())
    retcode_file = output_file.with_suffix('.rett')
    with open(retcode_file, "r") as file:
        ok = ok and code == int(file.read().strip())
    if not ok:
        print("Test failed")
        exit(1)
    return ok

calc_tests = [
    'array-assignment', 'array-equality', 'array-literals', 'array-lookup',
    'builtin-errors', 'invalid-assignee', 'len', 'not-a-number', 'not-an-array',
    'not-an-integer', 'old-errors', 'old-operations', 'out-of-bounds', 'pop', 'push'
]

format_tests = [
    'array-example', 'array-literals', 'array-lookup', 'function-call',
    'function-definition', 'function-example', 'lex-error', 'old-statements', 'parse-error'
]

lex_tests = [
    'array-tokens', 'function-tokens', 'lex-error', 'old-tokens', 'support-tokens'
]

for test in calc_tests:
    run_test("calc", test)

for test in format_tests:
    run_test("format", test)

for test in lex_tests:
    run_test("lex", test)