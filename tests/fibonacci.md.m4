# The Fibonacci sequence

The [Fibonacci sequence](https://en.wikipedia.org/wiki/Fibonacci_number)\ $f_n$ defined as

\begin{align*}
f_1 &= 1 \\
f_2 &= 1 \\
f_n &= f_{n-2} + f_{n-1} \quad\quad \text{for $n>2$} \\
\end{align*}

can be solved in many different ways. The following input computes a vector of size\ 15 whose components are the elements\ $f_n$ of the sequence.

## Input file

~~~wasora
include(fibonacci.was)
~~~

## Execution

~~~
$ wasora fibonacci.was
include(fibonacci.txt)
$
~~~


## Explanation

A vector `f` of size 15 is defined. Then, $f(i)$ for\ $i=1,2$ is set to 1 using the `<1:2>` construction that expands the special variable `i` to the given range. The next line sets\ $f(i)$ for\ $i=3,\dots,15$ to\ $f(i-2)+f(i-1)$. Note that when defining the range for\ $i$, expressions and functions such as `vecsize(f)` can appear. Finally, the vector is printed with `PRINT_VECTOR` using the [printf](http://man7.org/linux/man-pages/man3/printf.3.html) format `%g`. Note that all numbers in wasora are treated as double-precision floating-point representations.
