## 9ccs

9ccs is 9cc-syakyou edition. Syakyou means copy for hands-on in Japanese.

See https://www.sigbus.info/compilerbook

### Points

#### Debugging with gdb


```
gcc -g3 9ccs.c
gdb a.out
```

ref: 478b796bc2854ce3d1e6737d2bdd7e18943254c3
remove tokenize function call and compile, then run result `SIGSEGV` with Address boundary error!
https://qiita.com/arene-calix/items/a08363db88f21c81d351

#### Increment and assign

ref: e45cec0fc2283111391bc78810b114d26e9912d6

This cause bug token->str has no multiple operator. Ex, '5==6' -> token->str is '6'

```c
            cur = new_token(TK_RESERVED, cur, p+=2, 2);
            cur = new_token(TK_RESERVED, cur, p++, 1);
```

```c
x=0;
y = x++;
->
x = 1, y = 0
```

```c
x=0;
y= x+=1
->
x=1, y=1
```

#### gcc withc -Wl option

Incorrect
```
gcc --static -Wl, -N -o test test.o
/usr/bin/ld: cannot find : No such file or directory
collect2: error: ld returned 1 exit status
```

correct
```
gcc --static -Wl,-N -o test test.o
```