import os
import sys

_TIMEOUT=60

_EXPECTED_TO_LEAK = [
    "dudect_aes32_-O2",
    "dudect_donnabad_-O2",
    "dudect_simple_O0",
    ]

_EXPECTED_NOT_TO_LEAK = [
    "dudect_donna_-O2",
    "dudect_aesbitsliced_-O2",
    "dudect_simple_O2",
    ]

def check(name, expected_to_leak):
    cmd = "timeout %d ./%s" % (_TIMEOUT, name)
    ret = os.system(cmd) >> 8   # os.system returns the exit code in the top bits
    actual_leaks = (ret != 124) # if command timeouts, ret will be 124
    if actual_leaks != expected_to_leak:
        print("FAIL %s expected? %s actual: %s"%(name, expected_to_leak, actual_leaks))
        sys.exit(1)

for name in _EXPECTED_TO_LEAK:
    check(name, expected_to_leak=True)

for name in _EXPECTED_NOT_TO_LEAK:
    check(name, expected_to_leak=False)

print("PASS")
