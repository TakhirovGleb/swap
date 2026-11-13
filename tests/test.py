#!/usr/bin/env python3
"""Independent operation simulator, input validation and benchmark checks."""
import itertools
import random
import subprocess
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
RNG = random.Random(42)
MODES = ['--simple', '--medium', '--complex', '--adaptive']


def run(binary, args, data=None):
    return subprocess.run([str(ROOT / binary), *map(str, args)], input=data,
                          text=True, capture_output=True, timeout=20)


def simulate(values, stream):
    a, b = list(values), []
    for instruction in stream.splitlines():
        assert instruction in ('sa', 'sb', 'ss', 'pa', 'pb', 'ra', 'rb',
                               'rr', 'rra', 'rrb', 'rrr'), instruction
        if instruction in ('sa', 'ss') and len(a) > 1:
            a[0], a[1] = a[1], a[0]
        if instruction in ('sb', 'ss') and len(b) > 1:
            b[0], b[1] = b[1], b[0]
        if instruction == 'pa' and b:
            a.insert(0, b.pop(0))
        if instruction == 'pb' and a:
            b.insert(0, a.pop(0))
        if instruction in ('ra', 'rr') and a:
            a.append(a.pop(0))
        if instruction in ('rb', 'rr') and b:
            b.append(b.pop(0))
        if instruction in ('rra', 'rrr') and a:
            a.insert(0, a.pop())
        if instruction in ('rrb', 'rrr') and b:
            b.insert(0, b.pop())
    assert a == sorted(values) and not b, (values, a, b)


def verify(values, mode):
    result = run('push_swap', [mode, *values])
    assert result.returncode == 0 and not result.stderr, result
    simulate(values, result.stdout)
    checked = run('checker', values, result.stdout)
    assert checked.stdout == 'OK\n' and not checked.stderr, checked
    return len(result.stdout.splitlines())


count = 0
for n in range(1, 6):
    for values in itertools.permutations(range(n)):
        for mode in MODES:
            verify(values, mode)
            count += 1
for n in [6, 10, 25, 50, 100, 500]:
    totals = {mode: [] for mode in MODES}
    for _ in range(30):
        values = RNG.sample(range(-100000, 100000), n)
        for mode in MODES:
            totals[mode].append(verify(values, mode))
            count += 1
    if n >= 100:
        print(n, {mode: max(ops) for mode, ops in totals.items()})
        assert max(totals['--adaptive']) < {100: 2000, 500: 12000}[n]
for n in [100, 500]:
    for values in [list(range(n)), list(reversed(range(n))),
                   list(range(1, n)) + [0], [1, 0] + list(range(2, n))]:
        for mode in MODES:
            verify(values, mode)
            count += 1
for args in [[''], [' '], ['1', '1'], ['0', '-0'], ['2147483648'],
             ['-2147483649'], ['+'], ['1x'], ['1.0'], ['--unknown'],
             ['--simple', '--complex'], ['--bench', '--bench'],
             ['9999999999999999999999999999999999999999']]:
    result = run('push_swap', args)
    assert result.returncode != 0 and result.stdout == '' and result.stderr == 'Error\n', args
for binary in ['push_swap', 'checker']:
    result = run(binary, [])
    assert result.returncode == 0 and result.stdout == result.stderr == ''
    result = run(binary, ['2 1'], 'sa\n' if binary == 'checker' else None)
    assert result.returncode == 0
for instruction in ['sa', 'sa\r\n', '\n', 'wat\n', 'rrrr\n', 's\x00a\n']:
    result = run('checker', [2, 1], instruction)
    assert result.returncode != 0 and result.stderr == 'Error\n'
for values, expected in [([1], '0.00'), ([1, 2, 3], '0.00'),
                         ([3, 2, 1], '100.00'), ([2, 1, 3], '33.33')]:
    result = run('push_swap', ['--bench', *values])
    assert f'disorder: {expected}%' in result.stderr
    assert f'total: {len(result.stdout.splitlines())}\n' in result.stderr
    simulate(values, result.stdout)
verify([-2147483648, 2147483647, 0, -1], '--adaptive')
print(f'PASS: {count} sorting cases, parsing, checker and benchmark checks')
