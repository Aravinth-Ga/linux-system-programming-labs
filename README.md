# Linux System Programming Labs

This repository contains hands-on lab exercises created while studying
**Linux System Programming** by Robert Love.

The goal is to build deep, long-lasting understanding of Linux system calls
by isolating concepts and experimenting with kernel behavior.

## Philosophy

- One concept per lab
- Minimal code, maximum clarity
- Focus on *why* something works or fails
- Learn by observing kernel behavior, not memorizing APIs

## Current Progress

- Chapter 2 – File I/O
  - `open()` basics: read-only, missing file without `O_CREAT`
  - `O_CREAT` flows: create, `O_EXCL`, `O_TRUNC`
  - `O_APPEND` behavior
  - Permission failure case (`EACCES`)

## Environment

- Language: C
- Platform: Linux (tested on modern distributions)
- Focus: User-space system programming, POSIX interfaces

## Notes

I keep the labs small on purpose so each file explains a single idea. The
compiled outputs and quick screenshots live under `build/` and are only here
to make it easier to sanity-check behavior while iterating.
