# Proof in Hilbert machine
Logic basics project

The mentioned program that reads a valid statement in the conditional language from the input and provides a proof for it in the Hilbert machine.

This program is written in c++ language and to run it, you just need to build it and run the output.

## Definition
A proof for proposition A in Hilbert's apparatus is a sequence of propositions of length n in the form A1,...,An, whose nth member is the same as A, and for each i less than n, the member Ai:

1. Or is the main point,
2. Or j and k exist less than i such that Aj = (Ak>Ai).

The main topics of the Hilbert apparatus are all instances of each of the following two propositions:

1) (A>(B>A))

2) ((A>(B>C))>(A>B)>(A>C))

We call the principle of the first subject A1 and the second one A2.

## Program input
The input of the program is a well-formed statement in the conditional language, including the characters (, ), p, q, r, s, and >, which are valid in the logic of the statements.

A well-constructed statement in conditional language is defined as follows:

1. The four atoms p, q, r and s are the correct construction proposition in the conditional language.
2. If A and B are two well-formed propositions in the conditional language, then (A>B) is also a well-formed proposition in the conditional language.

## Program output
The program prints a proof for the input proposition in the Hilbert machine in standard form.

An argument in Hilbert's apparatus in standard form is a set of lines, unordered, consisting of three fields. Lines are separated by a newline character and fields are separated by a space character, as shown below.

```
N1 F1 R1
N2 F2 R2
N3 F3 R3
...
Nn Fn Rn
```
In the i-th line, Ni is the order number of the proposition Fi in the argument sequence. Ri is an expression in the following form that shows that Fi applies to one of clauses 1 or 2 in the above definition, in such a way that if Fi is the main subject, Ri is the name of that main subject, i.e. A1 or A2, and if for Fi the propositions Aj and Ak existed as in paragraph 2 of the definition, Ri is equal to j,k, where j and k are the numbers of the jth and kth lines in the proof sequence. Fn is the input statement.

## Example
Input: 
```
(p>p)
```
Output:
```
1 (p>(p>p)) A1
2 (p>((p>p)>p)) A1
3 ((p>((p>p)>p))>((p>(p>p))>(p>p))) A2
4 ((p>(p>p))>(p>p)) 3,2
5 (p>p) 4,1
```
