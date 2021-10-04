% hello world program
-module(helloworld).
-export([start/0]).

is_prime(X, I) when I * I =< X ->
    (X rem I /= 0) and (is_prime(X, I+1));
is_prime(_, _) -> true.

is_prime(X) when X >= 2 -> is_prime(X, 2);
is_prime(_) -> false.

gen_primes(N, I) when N > 0 ->
    P = is_prime(I),
    if
        P ->
            [I|gen_primes(N-1, I+1)];
        true ->
            gen_primes(N, I+1)
    end;
gen_primes(_, _) -> [].

gen_primes(N) -> gen_primes(N, 2).

start() ->
    L = gen_primes(10),
    io:fwrite("~w~n", [L]),
    
    Seq = [1,2,3,4,5,7,8],
    Seq2 = lists:concat([
        lists:sublist(Seq, 5),
        [6],
        lists:sublist(Seq, 6, 2)
    ]),
    
    io:fwrite("~w~n", [Seq2]).
    fib(10)

    % Seq = lists:seq(0, 100, 1),
    % Primes = [E || E <- Seq, is_prime(E)],
    % io:fwrite("~w~n",[Primes]),
    

fib_int(1, _, B) -> B;
fib_int(N, A, B) -> fib_int(N-1, B, A+B).

fib(0) -> 0;
fib(N) when N < 0 -> err_neg_val;
fib(N) when N < 3 -> 1;
fib(N) -> fib_int(N, 0, 1).

reverse([H|T]) -> reverse(T) ++ [H];
reverse([]) -> [].
