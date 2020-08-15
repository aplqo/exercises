fib::Integer->Integer->Int->Integer
fib a b 0=b
fib a b n=fib b (a+b) (n-1)

main::IO()
main=do
    t<-getLine
    let n=read t
    let r=fib 1 3 (n - 2)
    putStrLn (show (if n `mod` 2 == 0 then r * r - 4 else r * r))