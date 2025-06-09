# Password List Generation Using Crunch

## Overview
This document outlines how to use Crunch to generate password lists combining personal information patterns such as names, dates of birth, and pet names. This documentation is for educational and security research purposes only.

## What is Crunch?
Crunch is a wordlist generator where you can specify a standard character set or a character set you specify. It can generate all possible combinations and permutations based on given criteria.

## Basic Syntax
```bash
crunch <min-len> <max-len> [charset] -t <pattern> -o <outputfile>
```

## Character Set Symbols
- `@` = lowercase letters
- `,` = uppercase letters
- `%` = numbers
- `^` = special characters

## Common Patterns

### Name + Date of Birth
```bash
# Pattern for pet name + 4-digit date
crunch 11 11 -t Buddy%%%%! >> name_dob.txt
```

